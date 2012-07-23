--任务的接受条件
function Task_Accept_00000158()
	local player = GetPlayer();
	if player:GetLev() < 75 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(158) or task:HasCompletedTask(158) or task:HasSubmitedTask(158) then
		return false;
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000158()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 75 then
		return false;
	end
	if task:HasAcceptedTask(158) or task:HasCompletedTask(158) or task:HasSubmitedTask(158) then
		return false;
	end
	return true;
end


--任务完成条件
function Task_Submit_00000158()
	if GetPlayer():GetTaskMgr():HasCompletedTask(158) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000158(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(158) == npcId and Task_Accept_00000158 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 158
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_001856;
	elseif task:GetTaskSubmitNpc(158) == npcId then
		if Task_Submit_00000158() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 158
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_001857;
		elseif task:HasAcceptedTask(158) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 158
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_001858;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000158_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_001859;
	action.m_ActionMsg = task_msg_001860;
	return action;
end

function Task_00000158_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_001861;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000158_step_table = {
		[1] = Task_00000158_step_01,
		[10] = Task_00000158_step_10,
		};

function Task_00000158_step(step)
	if Task_00000158_step_table[step] ~= nil then
		return Task_00000158_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000158_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000158() then
		return false;
	end
	if not task:AcceptTask(158) then
		return false;
	end
	task:AddTaskStep(158);
	return true;
end



--提交任务
function Task_00000158_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(158) then
		return false;
	end


	player:AddExp(80000);
	return true;
end

--放弃任务
function Task_00000158_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(158);
end
