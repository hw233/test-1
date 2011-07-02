--����Ľ�������
function Task_Accept_00000031()
	local player = GetPlayer();
	if player:GetLev() < 25 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31) or task:HasCompletedTask(31) or task:HasSubmitedTask(31) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000031()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 25 then
		return false;
	end
	if task:HasAcceptedTask(31) or task:HasCompletedTask(31) or task:HasSubmitedTask(31) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000031()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000031(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31) == npcId and Task_Accept_00000031 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "太乙真金";
	elseif task:GetTaskSubmitNpc(31) == npcId then
		if Task_Submit_00000031() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "太乙真金";
		elseif task:HasAcceptedTask(31) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "太乙真金";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000031_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "看起来，<name>你经过多次历练，已经成长了不少。我这里有一块在东海偶得的太乙真金，是强化神兵的最佳材料，你拿去找武器店的沈浪，他是一位隐居多年的剑仙，看到这块太乙真金自会告诉你用法。";
	action.m_ActionMsg = "多谢前辈指点，我这就去。";
	return action;
end

function Task_00000031_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你手上发光的是什么东西？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000031_step_table = {
		[1] = Task_00000031_step_01,
		[10] = Task_00000031_step_10,
		};

function Task_00000031_step(step)
	if Task_00000031_step_table[step] ~= nil then
		return Task_00000031_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000031_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000031() then
		return false;
	end
	if not task:AcceptTask(31) then
		return false;
	end
	task:AddTaskStep(31);
	return true;
end



--�ύ����
function Task_00000031_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(31) then
		return false;
	end


	player:AddExp(2222);
	return true;
end

--��������
function Task_00000031_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(31);
end
