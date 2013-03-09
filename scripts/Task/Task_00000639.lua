--任务的接受条件
function Task_Accept_00000639()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(639) or task:HasCompletedTask(639) or task:HasSubmitedTask(639) then
		return false;
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000639()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(639) or task:HasCompletedTask(639) or task:HasSubmitedTask(639) then
		return false;
	end
	return true;
end


--任务完成条件
function Task_Submit_00000639()
	if GetPlayer():GetTaskMgr():HasCompletedTask(639) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000639(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(639) == npcId and Task_Accept_00000639 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 639
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_001251;
	elseif task:GetTaskSubmitNpc(639) == npcId then
		if Task_Submit_00000639() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 639
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_001252;
		elseif task:HasAcceptedTask(639) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 639
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_001253;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000639_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_001254..GetPlayerName(GetPlayer())..task_msg_001255;
	action.m_ActionMsg = task_msg_001256;
	return action;
end

function Task_00000639_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer())..task_msg_001257;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000639_step_table = {
		[1] = Task_00000639_step_01,
		[10] = Task_00000639_step_10,
		};

function Task_00000639_step(step)
	if Task_00000639_step_table[step] ~= nil then
		return Task_00000639_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000639_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000639() then
		return false;
	end
	if not task:AcceptTask(639) then
		return false;
	end
	return true;
end



--提交任务
function Task_00000639_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(639) then
		return false;
	end


	player:AddExp(3000);
	return true;
end

--放弃任务
function Task_00000639_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(639);
end
