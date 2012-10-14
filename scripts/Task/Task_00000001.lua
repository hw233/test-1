--任务的接受条件
function Task_Accept_00000001()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(1) or task:HasCompletedTask(1) or task:HasSubmitedTask(1) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(123) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(123) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(123) then
			return false;
		end
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000001()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(1) or task:HasCompletedTask(1) or task:HasSubmitedTask(1) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(123) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(123) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(123) then
			return false;
		end
	end
	return true;
end


--任务完成条件
function Task_Submit_00000001()
	if GetPlayer():GetTaskMgr():HasCompletedTask(1) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000001(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(1) == npcId and Task_Accept_00000001 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 1
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_000031;
	elseif task:GetTaskSubmitNpc(1) == npcId then
		if Task_Submit_00000001() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 1
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_000032;
		elseif task:HasAcceptedTask(1) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 1
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_000033;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000001_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_000034;
	action.m_ActionMsg = task_msg_000035;
	return action;
end

function Task_00000001_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_000036;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000001_step_table = {
		[1] = Task_00000001_step_01,
		[10] = Task_00000001_step_10,
		};

function Task_00000001_step(step)
	if Task_00000001_step_table[step] ~= nil then
		return Task_00000001_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000001_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000001() then
		return false;
	end
	if not task:AcceptTask(1) then
		return false;
	end
	return true;
end



--提交任务
function Task_00000001_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(2000,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 1013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(1) then
		return false;
	end

	if IsEquipTypeId(2000) then
		for k = 1, 1 do
			package:AddEquip(2000, 1);
		end
	else 
		package:AddItem(2000,1,1);
	end

	player:AddExp(460);
	player:getTael(200);
	return true;
end

--放弃任务
function Task_00000001_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(1);
end
