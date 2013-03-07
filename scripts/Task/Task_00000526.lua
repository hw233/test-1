--任务的接受条件
function Task_Accept_00000526()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(526) or task:HasCompletedTask(526) or task:HasSubmitedTask(526) then
		return false;
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000526()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(526) or task:HasCompletedTask(526) or task:HasSubmitedTask(526) then
		return false;
	end
	return true;
end


--任务完成条件
function Task_Submit_00000526()
	if GetPlayer():GetTaskMgr():HasCompletedTask(526) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000526(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(526) == npcId and Task_Accept_00000526 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 526
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_003772;
	elseif task:GetTaskSubmitNpc(526) == npcId then
		if Task_Submit_00000526() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 526
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_003773;
		elseif task:HasAcceptedTask(526) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 526
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_003774;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000526_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer())..task_msg_003775;
	action.m_ActionMsg = task_msg_003776;
	return action;
end

function Task_00000526_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_003777;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000526_step_table = {
		[1] = Task_00000526_step_01,
		[10] = Task_00000526_step_10,
		};

function Task_00000526_step(step)
	if Task_00000526_step_table[step] ~= nil then
		return Task_00000526_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000526_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000526() then
		return false;
	end
	if not task:AcceptTask(526) then
		return false;
	end
	task:AddTaskStep(526);
	return true;
end



--提交任务
function Task_00000526_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(526) then
		return false;
	end


	return true;
end

--放弃任务
function Task_00000526_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(526);
end
