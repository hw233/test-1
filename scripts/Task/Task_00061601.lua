--����Ľ�������
function Task_Accept_00061601()
	local player = GetPlayer();
	if player:GetLev() < 61 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61601) or task:HasCompletedTask(61601) or task:HasSubmitedTask(61601) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061601()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 61 then
		return false;
	end
	if task:HasAcceptedTask(61601) or task:HasCompletedTask(61601) or task:HasSubmitedTask(61601) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00061601()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61601) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061601(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61601) == npcId and Task_Accept_00061601 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61601
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "食尸小册子";
	elseif task:GetTaskSubmitNpc(61601) == npcId then
		if Task_Submit_00061601() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61601
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "食尸小册子";
		elseif task:HasAcceptedTask(61601) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61601
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "食尸小册子";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061601_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "《摩睺教你食尸》\n1.抢门板\n2.把人拍死\n3.吃尸体";
	action.m_ActionMsg = "说实话，我有点反胃了，这个，有点让人觉得恶心。";
	return action;
end

function Task_00061601_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你怎么会拿到这本小册子的。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061601_step_table = {
		[1] = Task_00061601_step_01,
		[10] = Task_00061601_step_10,
		};

function Task_00061601_step(step)
	if Task_00061601_step_table[step] ~= nil then
		return Task_00061601_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061601_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061601() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61601) then
		return false;
	end
	task:AddTaskStep(61601);
	return true;
end



--�ύ����
function Task_00061601_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15015,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(61601) then
		return false;
	end

	package:DelItemAll(15015,1);

	player:AddExp(135000);
	player:getCoin(115000);
	return true;
end

--��������
function Task_00061601_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15015,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61601);
end
