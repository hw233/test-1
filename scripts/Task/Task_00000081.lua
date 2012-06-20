--任务的接受条件
function Task_Accept_00000081()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(81) or task:HasCompletedTask(81) or task:HasSubmitedTask(81) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(80) then
			return false;
		end
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000081()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(81) or task:HasCompletedTask(81) or task:HasSubmitedTask(81) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(80) then
			return false;
		end
	end
	return true;
end


--任务完成条件
function Task_Submit_00000081()
	if GetPlayer():GetTaskMgr():HasCompletedTask(81) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000081(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(81) == npcId and Task_Accept_00000081 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 81
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_1776;
	elseif task:GetTaskSubmitNpc(81) == npcId then
		if Task_Submit_00000081() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 81
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_1777;
		elseif task:HasAcceptedTask(81) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 81
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_1778;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000081_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = task_msg_1779;
	action.m_ActionMsg = task_msg_1780;
	return action;
end

function Task_00000081_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_1781;
	action.m_ActionMsg = task_msg_1782;
	return action;
end

function Task_00000081_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_1783;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000081_step_table = {
		[1] = Task_00000081_step_01,
		[2] = Task_00000081_step_02,
		[10] = Task_00000081_step_10,
		};

function Task_00000081_step(step)
	if Task_00000081_step_table[step] ~= nil then
		return Task_00000081_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000081_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000081() then
		return false;
	end
	if not task:AcceptTask(81) then
		return false;
	end
	return true;
end



--提交任务
function Task_00000081_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(81) then
		return false;
	end


	player:AddExp(20000);
	return true;
end

--放弃任务
function Task_00000081_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(81);
end
