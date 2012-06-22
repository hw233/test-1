--任务的接受条件
function Task_Accept_00000002()
	local player = GetPlayer();
	if player:GetLev() < 2 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(2) or task:HasCompletedTask(2) or task:HasSubmitedTask(2) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(1) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(1) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(1) then
			return false;
		end
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000002()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 2 then
		return false;
	end
	if task:HasAcceptedTask(2) or task:HasCompletedTask(2) or task:HasSubmitedTask(2) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(1) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(1) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(1) then
			return false;
		end
	end
	return true;
end


--任务完成条件
function Task_Submit_00000002()
	if GetPlayer():GetTaskMgr():HasCompletedTask(2) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000002(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(2) == npcId and Task_Accept_00000002 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 2
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_59;
	elseif task:GetTaskSubmitNpc(2) == npcId then
		if Task_Submit_00000002() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 2
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_60;
		elseif task:HasAcceptedTask(2) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 2
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_61;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000002_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_62..GetPlayerName(GetPlayer())..task_msg_63;
	action.m_ActionMsg = task_msg_64;
	return action;
end

function Task_00000002_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_65;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000002_step_table = {
		[1] = Task_00000002_step_01,
		[10] = Task_00000002_step_10,
		};

function Task_00000002_step(step)
	if Task_00000002_step_table[step] ~= nil then
		return Task_00000002_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000002_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000002() then
		return false;
	end
	local package = player:GetPackage();
	local reqGrids = 0;
	reqGrids = reqGrids + package:GetItemUsedGrids(2000, 1, 1);
	if reqGrids > player:GetFreePackageSize() then
		player:sendMsgCode(2, 1012, 0);
		return false;
	end
	if not task:AcceptTask(2) then
		return false;
	end
	package:AddItem(2000, 1, 1);
	return true;
end



--提交任务
function Task_00000002_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(2001,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 1013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(2) then
		return false;
	end

	if IsEquipTypeId(2001) then
		for k = 1, 1 do
			package:AddEquip(2001, 1);
		end
	else 
		package:AddItem(2001,1,1);
	end

	player:AddExp(750);
	player:getTael(100);
	return true;
end

--放弃任务
function Task_00000002_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItem(2000, 1, 1);
	return GetPlayer():GetTaskMgr():AbandonTask(2);
end
