--����Ľ�������
function Task_Accept_00090101()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90101) or task:HasCompletedTask(90101) or task:HasSubmitedTask(90101) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090101()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90101) or task:HasCompletedTask(90101) or task:HasSubmitedTask(90101) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00090101()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90101) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090101(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90101) == npcId and Task_Accept_00090101 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90101
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "祈福";
	elseif task:GetTaskSubmitNpc(90101) == npcId then
		if Task_Submit_00090101() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90101
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "祈福";
		elseif task:HasAcceptedTask(90101) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90101
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "祈福";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090101_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我们的将士都是远离家乡来到战场拼杀，很多人都牺牲了。你能帮我去找活佛吗？让他为我们的将士亡魂祈福。";
	action.m_ActionMsg = "战争是可怕的。";
	return action;
end

function Task_00090101_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "战争何时休！";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090101_step_table = {
		[1] = Task_00090101_step_01,
		[10] = Task_00090101_step_10,
		};

function Task_00090101_step(step)
	if Task_00090101_step_table[step] ~= nil then
		return Task_00090101_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090101_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090101() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90101) then
		return false;
	end
	task:AddTaskStep(90101);
	return true;
end



--�ύ����
function Task_00090101_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90101) then
		return false;
	end


	player:AddExp(500000);
	player:getCoin(300000);
	return true;
end

--��������
function Task_00090101_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90101);
end
