--任务的接受条件
function Task_Accept_00000094()
	local player = GetPlayer();
	if player:GetLev() < 47 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(94) or task:HasCompletedTask(94) or task:HasSubmitedTask(94) then
		return false;
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000094()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 47 then
		return false;
	end
	if task:HasAcceptedTask(94) or task:HasCompletedTask(94) or task:HasSubmitedTask(94) then
		return false;
	end
	return true;
end


--任务完成条件
function Task_Submit_00000094()
	if GetPlayer():GetTaskMgr():HasCompletedTask(94) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000094(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(94) == npcId and Task_Accept_00000094 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 94
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_001008;
	elseif task:GetTaskSubmitNpc(94) == npcId then
		if Task_Submit_00000094() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 94
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_001009;
		elseif task:HasAcceptedTask(94) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 94
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_001010;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000094_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = task_msg_001011;
	action.m_ActionMsg = task_msg_001012;
	return action;
end

function Task_00000094_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_001013..GetPlayerName(GetPlayer())..task_msg_001014;
	action.m_ActionMsg = task_msg_001015;
	return action;
end

function Task_00000094_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer())..task_msg_001016;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000094_step_table = {
		[1] = Task_00000094_step_01,
		[2] = Task_00000094_step_02,
		[10] = Task_00000094_step_10,
		};

function Task_00000094_step(step)
	if Task_00000094_step_table[step] ~= nil then
		return Task_00000094_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000094_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000094() then
		return false;
	end
	if not task:AcceptTask(94) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(809,1);
	if itemNum ~= 0 then
		if itemNum > 2 then
			itemNum = 2;
			package:SetItem(809, itemNum, 1);
		end
		task:AddTaskStep2(94, 1, itemNum);
	end
	return true;
end



--提交任务
function Task_00000094_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(809,1) < 2 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(94) then
		return false;
	end

	package:DelItemAll(809,1);

	player:AddExp(40000);
	return true;
end

--放弃任务
function Task_00000094_abandon()
	local package = GetPlayer():GetPackage();
	local itemNum = 0;
	package:DelItemAll(809,1);
	return GetPlayer():GetTaskMgr():AbandonTask(94);
end
