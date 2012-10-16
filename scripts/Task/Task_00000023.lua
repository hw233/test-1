--任务的接受条件
function Task_Accept_00000023()
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(23) or task:HasCompletedTask(23) or task:HasSubmitedTask(23) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(22) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(22) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(22) then
			return false;
		end
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000023()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(23) or task:HasCompletedTask(23) or task:HasSubmitedTask(23) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(22) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(22) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(22) then
			return false;
		end
	end
	return true;
end


--任务完成条件
function Task_Submit_00000023()
	if GetPlayer():GetTaskMgr():HasCompletedTask(23) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000023(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(23) == npcId and Task_Accept_00000023 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 23
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_000189;
	elseif task:GetTaskSubmitNpc(23) == npcId then
		if Task_Submit_00000023() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 23
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_000190;
		elseif task:HasAcceptedTask(23) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 23
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_000191;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000023_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_000192;
	action.m_ActionMsg = task_msg_000193;
	return action;
end

function Task_00000023_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_000194;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000023_step_table = {
		[1] = Task_00000023_step_01,
		[10] = Task_00000023_step_10,
		};

function Task_00000023_step(step)
	if Task_00000023_step_table[step] ~= nil then
		return Task_00000023_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000023_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000023() then
		return false;
	end
	if not task:AcceptTask(23) then
		return false;
	end
	task:AddTaskStep(23);
	return true;
end



--提交任务
function Task_00000023_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(23) then
		return false;
	end


	player:AddExp(1800);
	return true;
end

--放弃任务
function Task_00000023_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(23);
end
