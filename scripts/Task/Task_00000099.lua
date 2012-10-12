--任务的接受条件
function Task_Accept_00000099()
	local player = GetPlayer();
	if player:GetLev() < 50 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(99) or task:HasCompletedTask(99) or task:HasSubmitedTask(99) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(98) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(98) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(98) then
			return false;
		end
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000099()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 50 then
		return false;
	end
	if task:HasAcceptedTask(99) or task:HasCompletedTask(99) or task:HasSubmitedTask(99) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(98) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(98) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(98) then
			return false;
		end
	end
	return true;
end


--任务完成条件
function Task_Submit_00000099()
	if GetPlayer():GetTaskMgr():HasCompletedTask(99) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000099(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(99) == npcId and Task_Accept_00000099 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 99
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_000454;
	elseif task:GetTaskSubmitNpc(99) == npcId then
		if Task_Submit_00000099() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 99
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_000455;
		elseif task:HasAcceptedTask(99) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 99
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_000456;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000099_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_000457..GetPlayerName(GetPlayer())..task_msg_000458;
	action.m_ActionMsg = task_msg_000459;
	return action;
end

function Task_00000099_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_000460;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000099_step_table = {
		[1] = Task_00000099_step_01,
		[10] = Task_00000099_step_10,
		};

function Task_00000099_step(step)
	if Task_00000099_step_table[step] ~= nil then
		return Task_00000099_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000099_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000099() then
		return false;
	end
	if not task:AcceptTask(99) then
		return false;
	end
	task:AddTaskStep(99);
	return true;
end



--提交任务
function Task_00000099_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(99) then
		return false;
	end


	player:AddExp(31000);
	return true;
end

--放弃任务
function Task_00000099_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(99);
end
