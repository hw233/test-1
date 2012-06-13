--任务的接受条件
function Task_Accept_00000011()
	local player = GetPlayer();
	if player:GetLev() < 8 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(11) or task:HasCompletedTask(11) or task:HasSubmitedTask(11) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(10) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(10) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(10) then
			return false;
		end
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000011()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 8 then
		return false;
	end
	if task:HasAcceptedTask(11) or task:HasCompletedTask(11) or task:HasSubmitedTask(11) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(10) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(10) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(10) then
			return false;
		end
	end
	return true;
end


--任务完成条件
function Task_Submit_00000011()
	if GetPlayer():GetTaskMgr():HasCompletedTask(11) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000011(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(11) == npcId and Task_Accept_00000011 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 11
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_117;
	elseif task:GetTaskSubmitNpc(11) == npcId then
		if Task_Submit_00000011() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 11
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_118;
		elseif task:HasAcceptedTask(11) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 11
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_119;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000011_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = task_msg_120;
	action.m_ActionMsg = task_msg_121;
	return action;
end

function Task_00000011_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 3;
	action.m_NpcMsg = task_msg_122;
	action.m_ActionMsg = task_msg_123;
	return action;
end

function Task_00000011_step_03()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 4;
	action.m_NpcMsg = task_msg_124;
	action.m_ActionMsg = task_msg_125;
	return action;
end

function Task_00000011_step_04()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_126;
	action.m_ActionMsg = task_msg_127;
	return action;
end

function Task_00000011_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_128;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000011_step_table = {
		[1] = Task_00000011_step_01,
		[2] = Task_00000011_step_02,
		[3] = Task_00000011_step_03,
		[4] = Task_00000011_step_04,
		[10] = Task_00000011_step_10,
		};

function Task_00000011_step(step)
	if Task_00000011_step_table[step] ~= nil then
		return Task_00000011_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000011_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000011() then
		return false;
	end
	if not task:AcceptTask(11) then
		return false;
	end
	task:AddTaskStep(11);
	return true;
end



--提交任务
function Task_00000011_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(11) then
		return false;
	end


	player:AddExp(1440);
	return true;
end

--放弃任务
function Task_00000011_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(11);
end
