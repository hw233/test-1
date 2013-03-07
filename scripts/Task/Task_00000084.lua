--任务的接受条件
function Task_Accept_00000084()
	local player = GetPlayer();
	if player:GetLev() < 45 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(84) or task:HasCompletedTask(84) or task:HasSubmitedTask(84) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(82) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(82) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(82) then
			return false;
		end
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000084()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 45 then
		return false;
	end
	if task:HasAcceptedTask(84) or task:HasCompletedTask(84) or task:HasSubmitedTask(84) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(82) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(82) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(82) then
			return false;
		end
	end
	return true;
end


--任务完成条件
function Task_Submit_00000084()
	if GetPlayer():GetTaskMgr():HasCompletedTask(84) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000084(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(84) == npcId and Task_Accept_00000084 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 84
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_002859;
	elseif task:GetTaskSubmitNpc(84) == npcId then
		if Task_Submit_00000084() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 84
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_002860;
		elseif task:HasAcceptedTask(84) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 84
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_002861;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000084_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = task_msg_002862;
	action.m_ActionMsg = task_msg_002863;
	return action;
end

function Task_00000084_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_002864;
	action.m_ActionMsg = task_msg_002865;
	return action;
end

function Task_00000084_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_002866;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000084_step_table = {
		[1] = Task_00000084_step_01,
		[2] = Task_00000084_step_02,
		[10] = Task_00000084_step_10,
		};

function Task_00000084_step(step)
	if Task_00000084_step_table[step] ~= nil then
		return Task_00000084_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000084_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000084() then
		return false;
	end
	if not task:AcceptTask(84) then
		return false;
	end
	task:AddTaskStep(84);
	return true;
end



--提交任务
function Task_00000084_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(84) then
		return false;
	end


	player:AddExp(25000);
	return true;
end

--放弃任务
function Task_00000084_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(84);
end
