--任务的接受条件
function Task_Accept_00000201()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(201) or task:HasCompletedTask(201) or task:HasSubmitedTask(201) then
		return false;
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000201()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(201) or task:HasCompletedTask(201) or task:HasSubmitedTask(201) then
		return false;
	end
	return true;
end


--任务完成条件
function Task_Submit_00000201()
	if GetPlayer():GetTaskMgr():HasCompletedTask(201) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000201(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(201) == npcId and Task_Accept_00000201 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 201
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = task_msg_2169;
	elseif task:GetTaskSubmitNpc(201) == npcId then
		if Task_Submit_00000201() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 201
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = task_msg_2170;
		elseif task:HasAcceptedTask(201) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 201
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = task_msg_2171;
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000201_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_2172;
	action.m_ActionMsg = task_msg_2173;
	return action;
end

function Task_00000201_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = task_msg_2174;
	action.m_ActionMsg = "";
	return action;
end

local Task_00000201_step_table = {
		[1] = Task_00000201_step_01,
		[10] = Task_00000201_step_10,
		};

function Task_00000201_step(step)
	if Task_00000201_step_table[step] ~= nil then
		return Task_00000201_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000201_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000201() then
		return false;
	end
	if not task:AcceptTask(201) then
		return false;
	end
	return true;
end



--提交任务
function Task_00000201_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(201) then
		return false;
	end


	player:AddExp(30000);
	return true;
end

--放弃任务
function Task_00000201_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(201);
end
