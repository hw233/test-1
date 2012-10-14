--任务的接受条件
function Task_Accept_00000029()
	local player = GetPlayer();
	if player:GetLev() < 18 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(29) or task:HasCompletedTask(29) or task:HasSubmitedTask(29) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(28) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(28) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(28) then
			return false;
		end
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000029()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 18 then
		return false;
	end
	if task:HasAcceptedTask(29) or task:HasCompletedTask(29) or task:HasSubmitedTask(29) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(28) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(28) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(28) then
			return false;
		end
	end
	return true;
end


--任务完成条件
function Task_Submit_00000029()
	if GetPlayer():GetTaskMgr():HasCompletedTask(29) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000029(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(29) == npcId and Task_Accept_00000029 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 29
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_000220;
	elseif task:GetTaskSubmitNpc(29) == npcId then
		if Task_Submit_00000029() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 29
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_000221;
		elseif task:HasAcceptedTask(29) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 29
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_000222;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000029_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = task_msg_000223..GetPlayerName(GetPlayer())..task_msg_000224;
	action.m_ActionMsg = task_msg_000225;
	return action;
end

function Task_00000029_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_000226..GetPlayerName(GetPlayer())..task_msg_000227;
	action.m_ActionMsg = task_msg_000228;
	return action;
end

function Task_00000029_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_000229;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000029_step_table = {
		[1] = Task_00000029_step_01,
		[2] = Task_00000029_step_02,
		[10] = Task_00000029_step_10,
		};

function Task_00000029_step(step)
	if Task_00000029_step_table[step] ~= nil then
		return Task_00000029_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000029_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000029() then
		return false;
	end
	if not task:AcceptTask(29) then
		return false;
	end
	return true;
end



--提交任务
function Task_00000029_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(29) then
		return false;
	end


	player:AddExp(4600);
	player:getTael(300);
	return true;
end

--放弃任务
function Task_00000029_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(29);
end
