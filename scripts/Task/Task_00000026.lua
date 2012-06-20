--任务的接受条件
function Task_Accept_00000026()
	local player = GetPlayer();
	if player:GetLev() < 16 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(26) or task:HasCompletedTask(26) or task:HasSubmitedTask(26) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000026()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 16 then
		return false;
	end
	if task:HasAcceptedTask(26) or task:HasCompletedTask(26) or task:HasSubmitedTask(26) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(25) then
			return false;
		end
	end
	return true;
end


--任务完成条件
function Task_Submit_00000026()
	if GetPlayer():GetTaskMgr():HasCompletedTask(26) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000026(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(26) == npcId and Task_Accept_00000026 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 26
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_201;
	elseif task:GetTaskSubmitNpc(26) == npcId then
		if Task_Submit_00000026() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 26
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_202;
		elseif task:HasAcceptedTask(26) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 26
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_203;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000026_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_204;
	action.m_ActionMsg = task_msg_205;
	return action;
end

function Task_00000026_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_206..GetPlayerName(GetPlayer())..task_msg_207;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000026_step_table = {
		[1] = Task_00000026_step_01,
		[10] = Task_00000026_step_10,
		};

function Task_00000026_step(step)
	if Task_00000026_step_table[step] ~= nil then
		return Task_00000026_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000026_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000026() then
		return false;
	end
	local package = player:GetPackage();
	local reqGrids = 0;
	reqGrids = reqGrids + package:GetItemUsedGrids(800, 1, 1);
	if reqGrids > player:GetFreePackageSize() then
		player:sendMsgCode(2, 1012, 0);
		return false;
	end
	if not task:AcceptTask(26) then
		return false;
	end
	package:AddItem(800, 1, 1);
	task:AddTaskStep(26);
	return true;
end



--提交任务
function Task_00000026_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(800,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(26) then
		return false;
	end

	package:DelItem(800,1,1);

	player:AddExp(6000);
	player:getTael(500);
	return true;
end

--放弃任务
function Task_00000026_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItem(800, 1, 1);
	local itemNum = 0;
	return GetPlayer():GetTaskMgr():AbandonTask(26);
end
