--任务的接受条件
function Task_Accept_00000200()
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(200) or task:HasCompletedTask(200) or task:HasSubmitedTask(200) then
		return false;
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000200()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(200) or task:HasCompletedTask(200) or task:HasSubmitedTask(200) then
		return false;
	end
	return true;
end


--任务完成条件
function Task_Submit_00000200()
	if GetPlayer():GetTaskMgr():HasCompletedTask(200) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000200(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(200) == npcId and Task_Accept_00000200 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 200
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_2187;
	elseif task:GetTaskSubmitNpc(200) == npcId then
		if Task_Submit_00000200() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 200
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_2188;
		elseif task:HasAcceptedTask(200) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 200
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_2189;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000200_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_2190..GetPlayerName(GetPlayer())..task_msg_2191;
	action.m_ActionMsg = task_msg_2192;
	return action;
end

function Task_00000200_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_2193;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000200_step_table = {
		[1] = Task_00000200_step_01,
		[10] = Task_00000200_step_10,
		};

function Task_00000200_step(step)
	if Task_00000200_step_table[step] ~= nil then
		return Task_00000200_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000200_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000200() then
		return false;
	end
	if not task:AcceptTask(200) then
		return false;
	end
	return true;
end



--提交任务
function Task_00000200_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(200) then
		return false;
	end


	player:AddExp(30000);
	return true;
end

--放弃任务
function Task_00000200_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(200);
end
