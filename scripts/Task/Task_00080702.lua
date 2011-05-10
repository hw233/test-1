--����Ľ�������
function Task_Accept_00080702()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80702) or task:HasCompletedTask(80702) or task:HasSubmitedTask(80702) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80701) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80701) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080702()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80702) or task:HasCompletedTask(80702) or task:HasSubmitedTask(80702) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80701) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80701) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00080702()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80702) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080702(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80702) == npcId and Task_Accept_00080702 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80702
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "研究幻术";
	elseif task:GetTaskSubmitNpc(80702) == npcId then
		if Task_Submit_00080702() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80702
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "研究幻术";
		elseif task:HasAcceptedTask(80702) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80702
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "研究幻术";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080702_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这就是传说中神奇的玄幻草吗？";
	action.m_ActionMsg = "那它有什么效果呢？";
	return action;
end

function Task_00080702_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我也不清楚，这应该跟幻术有关系，你拿去给巫师，我想她会知道。";
	action.m_ActionMsg = "就这么一株小草，我看作用不大。";
	return action;
end

function Task_00080702_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这可是极难得的玄幻草呀。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080702_step_table = {
		[1] = Task_00080702_step_01,
		[2] = Task_00080702_step_02,
		[10] = Task_00080702_step_10,
		};

function Task_00080702_step(step)
	if Task_00080702_step_table[step] ~= nil then
		return Task_00080702_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080702_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080702() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15103, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(80702) then
		return false;
	end
	task:AddTaskStep(80702);
	return true;
end



--�ύ����
function Task_00080702_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15103,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(80702) then
		return false;
	end

	package:DelItemAll(15103,1);

	player:AddExp(675000);
	player:getCoin(314000);
	return true;
end

--��������
function Task_00080702_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15103,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80702);
end
