--任务的接受条件
function Task_Accept_00000082()
	local player = GetPlayer();
	if player:GetLev() < 45 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(82) or task:HasCompletedTask(82) or task:HasSubmitedTask(82) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(81) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(81) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(81) then
			return false;
		end
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000082()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 45 then
		return false;
	end
	if task:HasAcceptedTask(82) or task:HasCompletedTask(82) or task:HasSubmitedTask(82) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(81) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(81) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(81) then
			return false;
		end
	end
	return true;
end


--任务完成条件
function Task_Submit_00000082()
	if GetPlayer():GetTaskMgr():HasCompletedTask(82) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000082(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(82) == npcId and Task_Accept_00000082 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 82
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_002106;
	elseif task:GetTaskSubmitNpc(82) == npcId then
		if Task_Submit_00000082() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 82
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_002107;
		elseif task:HasAcceptedTask(82) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 82
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_002108;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000082_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = task_msg_002109;
	action.m_ActionMsg = task_msg_002110;
	return action;
end

function Task_00000082_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_002111;
	action.m_ActionMsg = task_msg_002112;
	return action;
end

function Task_00000082_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_002113;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000082_step_table = {
		[1] = Task_00000082_step_01,
		[2] = Task_00000082_step_02,
		[10] = Task_00000082_step_10,
		};

function Task_00000082_step(step)
	if Task_00000082_step_table[step] ~= nil then
		return Task_00000082_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000082_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000082() then
		return false;
	end
	if not task:AcceptTask(82) then
		return false;
	end
	task:AddTaskStep(82);
	return true;
end



--提交任务
function Task_00000082_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(82) then
		return false;
	end


	player:AddExp(20000);
	return true;
end

--放弃任务
function Task_00000082_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(82);
end
