--任务的接受条件
function Task_Accept_00000618()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(618) or task:HasCompletedTask(618) or task:HasSubmitedTask(618) then
		return false;
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000618()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(618) or task:HasCompletedTask(618) or task:HasSubmitedTask(618) then
		return false;
	end
	return true;
end


--任务完成条件
function Task_Submit_00000618()
	if GetPlayer():GetTaskMgr():HasCompletedTask(618) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000618(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(618) == npcId and Task_Accept_00000618 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 618
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_001113;
	elseif task:GetTaskSubmitNpc(618) == npcId then
		if Task_Submit_00000618() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 618
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_001114;
		elseif task:HasAcceptedTask(618) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 618
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_001115;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000618_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_001116..GetPlayerName(GetPlayer())..task_msg_001117;
	action.m_ActionMsg = task_msg_001118;
	return action;
end

function Task_00000618_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000618_step_table = {
		[1] = Task_00000618_step_01,
		[10] = Task_00000618_step_10,
		};

function Task_00000618_step(step)
	if Task_00000618_step_table[step] ~= nil then
		return Task_00000618_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000618_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000618() then
		return false;
	end
	if not task:AcceptTask(618) then
		return false;
	end
	return true;
end



--提交任务
function Task_00000618_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(618) then
		return false;
	end


	player:AddExp(3000);
	return true;
end

--放弃任务
function Task_00000618_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(618);
end
