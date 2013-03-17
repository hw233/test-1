--任务的接受条件
function Task_Accept_00000035()
	local player = GetPlayer();
	if player:GetLev() < 21 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(35) or task:HasCompletedTask(35) or task:HasSubmitedTask(35) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(34) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(34) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(34) then
			return false;
		end
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000035()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 21 then
		return false;
	end
	if task:HasAcceptedTask(35) or task:HasCompletedTask(35) or task:HasSubmitedTask(35) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(34) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(34) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(34) then
			return false;
		end
	end
	return true;
end


--任务完成条件
function Task_Submit_00000035()
	if GetPlayer():GetTaskMgr():HasCompletedTask(35) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000035(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(35) == npcId and Task_Accept_00000035 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 35
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_000263;
	elseif task:GetTaskSubmitNpc(35) == npcId then
		if Task_Submit_00000035() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 35
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_000264;
		elseif task:HasAcceptedTask(35) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 35
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_000265;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000035_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer())..task_msg_000266;
	action.m_ActionMsg = task_msg_000267;
	return action;
end

function Task_00000035_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer())..task_msg_000268;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000035_step_table = {
		[1] = Task_00000035_step_01,
		[10] = Task_00000035_step_10,
		};

function Task_00000035_step(step)
	if Task_00000035_step_table[step] ~= nil then
		return Task_00000035_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000035_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000035() then
		return false;
	end
	if not task:AcceptTask(35) then
		return false;
	end
	return true;
end



--提交任务
function Task_00000035_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(514,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 1013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(35) then
		return false;
	end

	if IsEquipTypeId(514) then
		for k = 1, 1 do
			package:AddEquip(514, 1);
		end
	else 
		package:AddItem(514,1,1);
	end

	player:AddExp(6200);
	return true;
end

--放弃任务
function Task_00000035_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(35);
end
