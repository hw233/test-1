--任务的接受条件
function Task_Accept_00000115()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(115) or task:HasCompletedTask(115) or task:HasSubmitedTask(115) then
		return false;
	end
	return true;
end




-----可接任务条件
function Task_Can_Accept_00000115()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(115) or task:HasCompletedTask(115) or task:HasSubmitedTask(115) then
		return false;
	end
	return true;
end


--任务完成条件
function Task_Submit_00000115()
	if GetPlayer():GetTaskMgr():HasCompletedTask(115) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC交互的任务脚本
---------------------------------------
function Task_00000115(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(115) == npcId and Task_Accept_00000115 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 115
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "115";
	elseif task:GetTaskSubmitNpc(115) == npcId then
		if Task_Submit_00000115() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 115
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "115";
		elseif task:HasAcceptedTask(115) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 115
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "115";
		end
	end
	return action;
end

-------------------------------------------------
--------任务交互步骤
-------------------------------------------------
function Task_00000115_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "榻愭急婧熻浣犲幓瀵硅瘽";
	action.m_ActionMsg = "榻愭急婧熻浣犲幓瀵硅瘽";
	return action;
end

function Task_00000115_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "榻愭急婧熻浣犲幓瀵硅瘽";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000115_step_table = {
		[1] = Task_00000115_step_01,
		[10] = Task_00000115_step_10,
		};

function Task_00000115_step(step)
	if Task_00000115_step_table[step] ~= nil then
		return Task_00000115_step_table[step]();
	end
	return ActionTable:Instance();
end

--接受任务
function Task_00000115_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000115() then
		return false;
	end
	if not task:AcceptTask(115) then
		return false;
	end
	task:AddTaskStep(115);
	return true;
end



--提交任务
function Task_00000115_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(115) then
		if DayTaskAward(0) then
			task:DayTaskSubmit(115);
			return true;
		end
	end

	return false;
end

--放弃任务
function Task_00000115_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(115);
end
