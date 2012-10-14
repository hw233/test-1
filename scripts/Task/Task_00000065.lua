--任务的接受条件
function Task_Accept_00000065()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(65) or task:HasCompletedTask(65) or task:HasSubmitedTask(65) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(64) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(64) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(64) then
			return false;
		end
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000065()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(65) or task:HasCompletedTask(65) or task:HasSubmitedTask(65) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(64) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(64) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(64) then
			return false;
		end
	end
	return true;
end


--任务完成条件
function Task_Submit_00000065()
	if GetPlayer():GetTaskMgr():HasCompletedTask(65) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000065(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(65) == npcId and Task_Accept_00000065 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 65
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_002077;
	elseif task:GetTaskSubmitNpc(65) == npcId then
		if Task_Submit_00000065() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 65
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_002078;
		elseif task:HasAcceptedTask(65) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 65
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_002079;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000065_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = task_msg_002080;
	action.m_ActionMsg = task_msg_002081;
	return action;
end

function Task_00000065_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_002082;
	action.m_ActionMsg = task_msg_002083;
	return action;
end

function Task_00000065_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_002084;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000065_step_table = {
		[1] = Task_00000065_step_01,
		[2] = Task_00000065_step_02,
		[10] = Task_00000065_step_10,
		};

function Task_00000065_step(step)
	if Task_00000065_step_table[step] ~= nil then
		return Task_00000065_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000065_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000065() then
		return false;
	end
	local package = player:GetPackage();
	local reqGrids = 0;
	reqGrids = reqGrids + package:GetItemUsedGrids(807, 1, 1);
	if reqGrids > player:GetFreePackageSize() then
		player:sendMsgCode(2, 1012, 0);
		return false;
	end
	if not task:AcceptTask(65) then
		return false;
	end
	package:AddItem(807, 1, 1);
	task:AddTaskStep(65);
	return true;
end



--提交任务
function Task_00000065_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(807,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(65) then
		return false;
	end

	package:DelItem(807,1,1);

	player:AddExp(80000);
	return true;
end

--放弃任务
function Task_00000065_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItem(807, 1, 1);
	local itemNum = 0;
	return GetPlayer():GetTaskMgr():AbandonTask(65);
end
