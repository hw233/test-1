--任务的接受条件
function Task_Accept_00000629()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(629) or task:HasCompletedTask(629) or task:HasSubmitedTask(629) then
		return false;
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000629()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(629) or task:HasCompletedTask(629) or task:HasSubmitedTask(629) then
		return false;
	end
	return true;
end


--任务完成条件
function Task_Submit_00000629()
	if GetPlayer():GetTaskMgr():HasCompletedTask(629) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000629(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(629) == npcId and Task_Accept_00000629 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 629
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_001188;
	elseif task:GetTaskSubmitNpc(629) == npcId then
		if Task_Submit_00000629() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 629
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_001189;
		elseif task:HasAcceptedTask(629) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 629
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_001190;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000629_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_001191..GetPlayerName(GetPlayer())..task_msg_001192;
	action.m_ActionMsg = task_msg_001193;
	return action;
end

function Task_00000629_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer())..task_msg_001194;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000629_step_table = {
		[1] = Task_00000629_step_01,
		[10] = Task_00000629_step_10,
		};

function Task_00000629_step(step)
	if Task_00000629_step_table[step] ~= nil then
		return Task_00000629_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000629_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000629() then
		return false;
	end
	if not task:AcceptTask(629) then
		return false;
	end
	return true;
end



--提交任务
function Task_00000629_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(629) then
		return false;
	end


	player:AddExp(3000);
	return true;
end

--放弃任务
function Task_00000629_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(629);
end
