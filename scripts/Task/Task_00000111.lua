--任务的接受条件
function Task_Accept_00000111()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(111) or task:HasCompletedTask(111) or task:HasSubmitedTask(111) then
		return false;
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000111()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(111) or task:HasCompletedTask(111) or task:HasSubmitedTask(111) then
		return false;
	end
	return true;
end


--任务完成条件
function Task_Submit_00000111()
	if GetPlayer():GetTaskMgr():HasCompletedTask(111) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000111(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(111) == npcId and Task_Accept_00000111 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 111
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "111";
	elseif task:GetTaskSubmitNpc(111) == npcId then
		if Task_Submit_00000111() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 111
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "111";
		elseif task:HasAcceptedTask(111) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 111
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "111";
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000111_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "榻愭急婧熻浣犲幓瀵硅瘽";
	action.m_ActionMsg = "榻愭急婧熻浣犲幓瀵硅瘽";
	return action;
end

function Task_00000111_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "榻愭急婧熻浣犲幓瀵硅瘽";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000111_step_table = {
		[1] = Task_00000111_step_01,
		[10] = Task_00000111_step_10,
		};

function Task_00000111_step(step)
	if Task_00000111_step_table[step] ~= nil then
		return Task_00000111_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000111_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000111() then
		return false;
	end
	if not task:AcceptTask(111) then
		return false;
	end
	task:AddTaskStep(111);
	return true;
end



--提交任务
function Task_00000111_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(111) then
		if DayTaskAward(0) then
			task:DayTaskSubmit(111);
			return true;
		end
	end

	return false;
end

--放弃任务
function Task_00000111_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(111);
end
