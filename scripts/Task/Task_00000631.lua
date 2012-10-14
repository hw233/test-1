--任务的接受条件
function Task_Accept_00000631()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(631) or task:HasCompletedTask(631) or task:HasSubmitedTask(631) then
		return false;
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000631()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(631) or task:HasCompletedTask(631) or task:HasSubmitedTask(631) then
		return false;
	end
	return true;
end


--任务完成条件
function Task_Submit_00000631()
	if GetPlayer():GetTaskMgr():HasCompletedTask(631) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000631(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(631) == npcId and Task_Accept_00000631 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 631
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_001269;
	elseif task:GetTaskSubmitNpc(631) == npcId then
		if Task_Submit_00000631() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 631
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_001270;
		elseif task:HasAcceptedTask(631) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 631
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_001271;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000631_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_001272;
	action.m_ActionMsg = task_msg_001273;
	return action;
end

function Task_00000631_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer())..task_msg_001274;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000631_step_table = {
		[1] = Task_00000631_step_01,
		[10] = Task_00000631_step_10,
		};

function Task_00000631_step(step)
	if Task_00000631_step_table[step] ~= nil then
		return Task_00000631_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000631_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000631() then
		return false;
	end
	if not task:AcceptTask(631) then
		return false;
	end
	return true;
end



--提交任务
function Task_00000631_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(631) then
		return false;
	end


	player:AddExp(3000);
	return true;
end

--放弃任务
function Task_00000631_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(631);
end
