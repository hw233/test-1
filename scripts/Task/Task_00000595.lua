--任务的接受条件
function Task_Accept_00000595()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(595) or task:HasCompletedTask(595) or task:HasSubmitedTask(595) then
		return false;
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000595()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(595) or task:HasCompletedTask(595) or task:HasSubmitedTask(595) then
		return false;
	end
	return true;
end


--任务完成条件
function Task_Submit_00000595()
	if GetPlayer():GetTaskMgr():HasCompletedTask(595) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000595(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(595) == npcId and Task_Accept_00000595 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 595
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_1624;
	elseif task:GetTaskSubmitNpc(595) == npcId then
		if Task_Submit_00000595() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 595
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_1625;
		elseif task:HasAcceptedTask(595) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 595
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_1626;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000595_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_1627..GetPlayerName(GetPlayer())..task_msg_1628;
	action.m_ActionMsg = task_msg_1629;
	return action;
end

function Task_00000595_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer())..task_msg_1630;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000595_step_table = {
		[1] = Task_00000595_step_01,
		[10] = Task_00000595_step_10,
		};

function Task_00000595_step(step)
	if Task_00000595_step_table[step] ~= nil then
		return Task_00000595_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000595_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000595() then
		return false;
	end
	if not task:AcceptTask(595) then
		return false;
	end
	return true;
end



--提交任务
function Task_00000595_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(595) then
		return false;
	end


	return true;
end

--放弃任务
function Task_00000595_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(595);
end
