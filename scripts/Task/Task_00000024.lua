--任务的接受条件
function Task_Accept_00000024()
	local player = GetPlayer();
	if player:GetLev() < 15 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(24) or task:HasCompletedTask(24) or task:HasSubmitedTask(24) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(23) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(23) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(23) then
			return false;
		end
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000024()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 15 then
		return false;
	end
	if task:HasAcceptedTask(24) or task:HasCompletedTask(24) or task:HasSubmitedTask(24) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(23) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(23) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(23) then
			return false;
		end
	end
	return true;
end


--任务完成条件
function Task_Submit_00000024()
	if GetPlayer():GetTaskMgr():HasCompletedTask(24) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000024(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(24) == npcId and Task_Accept_00000024 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 24
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_2145;
	elseif task:GetTaskSubmitNpc(24) == npcId then
		if Task_Submit_00000024() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 24
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_2146;
		elseif task:HasAcceptedTask(24) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 24
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_2147;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000024_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = GetPlayerName(GetPlayer())..task_msg_2148;
	action.m_ActionMsg = task_msg_2149;
	return action;
end

function Task_00000024_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_2150;
	action.m_ActionMsg = task_msg_2151;
	return action;
end

function Task_00000024_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_2152;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000024_step_table = {
		[1] = Task_00000024_step_01,
		[2] = Task_00000024_step_02,
		[10] = Task_00000024_step_10,
		};

function Task_00000024_step(step)
	if Task_00000024_step_table[step] ~= nil then
		return Task_00000024_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000024_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000024() then
		return false;
	end
	if not task:AcceptTask(24) then
		return false;
	end
	return true;
end



--提交任务
function Task_00000024_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(509,1,1) + package:GetItemUsedGrids(516,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 1013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(24) then
		return false;
	end

	if IsEquipTypeId(509) then
		for k = 1, 1 do
			package:AddEquip(509, 1);
		end
	else 
		package:AddItem(509,1,1);
	end
	if IsEquipTypeId(516) then
		for k = 1, 1 do
			package:AddEquip(516, 1);
		end
	else 
		package:AddItem(516,1,1);
	end

	player:AddExp(3800);
	return true;
end

--放弃任务
function Task_00000024_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(24);
end
