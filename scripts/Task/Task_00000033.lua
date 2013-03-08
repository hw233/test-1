--任务的接受条件
function Task_Accept_00000033()
	local player = GetPlayer();
	if player:GetLev() < 20 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(33) or task:HasCompletedTask(33) or task:HasSubmitedTask(33) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(32) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(32) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(32) then
			return false;
		end
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000033()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 20 then
		return false;
	end
	if task:HasAcceptedTask(33) or task:HasCompletedTask(33) or task:HasSubmitedTask(33) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(32) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(32) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(32) then
			return false;
		end
	end
	return true;
end


--任务完成条件
function Task_Submit_00000033()
	if GetPlayer():GetTaskMgr():HasCompletedTask(33) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000033(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(33) == npcId and Task_Accept_00000033 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 33
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_000251;
	elseif task:GetTaskSubmitNpc(33) == npcId then
		if Task_Submit_00000033() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 33
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_000252;
		elseif task:HasAcceptedTask(33) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 33
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_000253;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000033_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_000254;
	action.m_ActionMsg = task_msg_000255;
	return action;
end

function Task_00000033_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_000256;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000033_step_table = {
		[1] = Task_00000033_step_01,
		[10] = Task_00000033_step_10,
		};

function Task_00000033_step(step)
	if Task_00000033_step_table[step] ~= nil then
		return Task_00000033_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000033_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000033() then
		return false;
	end
	if not task:AcceptTask(33) then
		return false;
	end
	task:AddTaskStep(33);
	return true;
end



--提交任务
function Task_00000033_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(33) then
		return false;
	end


	player:AddExp(4900);
	return true;
end

--放弃任务
function Task_00000033_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(33);
end
