require("global")

local DayCopyTask = { {80101, 80102, 80103}, {80104, 80105, 80106}, {80108, 80109, 80110}, {80114, 80115, 80116}, {80117, 80118, 80119} };

local DayCopyTask_Inn = {};
local action = ActionTable:Instance();
action.m_ActionType = 0x60;
action.m_ActionToken = 1;
action.m_ActionID = 1;
action.m_ActionStep = 0;
action.m_ActionMsg = "焚骨窟副本";
table.insert(DayCopyTask_Inn, action);

action = ActionTable:Instance();
action.m_ActionType = 0x60;
action.m_ActionToken = 1;
action.m_ActionID = 2;
action.m_ActionStep = 0;
action.m_ActionMsg = "天人府邸副本";
table.insert(DayCopyTask_Inn, action);

action = ActionTable:Instance();
action.m_ActionType = 0x60;
action.m_ActionToken = 1;
action.m_ActionID = 3;
action.m_ActionStep = 0;
action.m_ActionMsg = "困仙副本";
table.insert(DayCopyTask_Inn, action);

action = ActionTable:Instance();
action.m_ActionType = 0x60;
action.m_ActionToken = 1;
action.m_ActionID = 4;
action.m_ActionStep = 0;
action.m_ActionMsg = "无间炼狱副本";
table.insert(DayCopyTask_Inn, action);

action = ActionTable:Instance();
action.m_ActionType = 0x60;
action.m_ActionToken = 1;
action.m_ActionID = 5;
action.m_ActionStep = 0;
action.m_ActionMsg = "海底幻境副本";
table.insert(DayCopyTask_Inn, action);


function RunDayCopyTaskStep(npcId, index)
	local taskActionTable = {};
	local taskAction = null;
	if index == 1 then
		taskAction = Task_00080101(npcId);
		if taskAction.m_ActionType ~= 0 then
			table.insert(taskActionTable, taskAction);
		end
		taskAction = Task_00080102(npcId);
		if taskAction.m_ActionType ~= 0 then
			table.insert(taskActionTable, taskAction);
		end
		taskAction = Task_00080103(npcId);
		if taskAction.m_ActionType ~= 0 then
			table.insert(taskActionTable, taskAction);
		end
	elseif index == 2 then
		taskAction = Task_00080104(npcId);
		if taskAction.m_ActionType ~= 0 then
			table.insert(taskActionTable, taskAction);
		end
		taskAction = Task_00080105(npcId);
		if taskAction.m_ActionType ~= 0 then
			table.insert(taskActionTable, taskAction);
		end
		taskAction = Task_00080106(npcId);
		if taskAction.m_ActionType ~= 0 then
			table.insert(taskActionTable, taskAction);
		end		
	elseif index == 3 then
		taskAction = Task_00080108(npcId);
		if taskAction.m_ActionType ~= 0 then
			table.insert(taskActionTable, taskAction);
		end
		taskAction = Task_00080109(npcId);
		if taskAction.m_ActionType ~= 0 then
			table.insert(taskActionTable, taskAction);
		end
		taskAction = Task_00080110(npcId);
		if taskAction.m_ActionType ~= 0 then
			table.insert(taskActionTable, taskAction);
		end
	elseif index == 4 then
		taskAction = Task_00080114(npcId);
		if taskAction.m_ActionType ~= 0 then
			table.insert(taskActionTable, taskAction);
		end
		taskAction = Task_00080115(npcId);
		if taskAction.m_ActionType ~= 0 then
			table.insert(taskActionTable, taskAction);
		end
		taskAction = Task_00080116(npcId);
		if taskAction.m_ActionType ~= 0 then
			table.insert(taskActionTable, taskAction);
		end	
	elseif index == 5 then
		taskAction = Task_00080117(npcId);
		if taskAction.m_ActionType ~= 0 then
			table.insert(taskActionTable, taskAction);
		end
		taskAction = Task_00080118(npcId);
		if taskAction.m_ActionType ~= 0 then
			table.insert(taskActionTable, taskAction);
		end
		taskAction = Task_00080119(npcId);
		if taskAction.m_ActionType ~= 0 then
			table.insert(taskActionTable, taskAction);
		end		
	end
	return taskActionTable;
end

-----------------------------------------------

function RunDayCopyTask(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local thisDay = GetSharpDay(os.time());
	
	local i = 1;
	local j = 1;
	local skip = false;
	local taskId = 0;
	while i <= 5 do
		j = 1;
		skip = false;
		while j <= 3 do
			taskId = DayCopyTask[i][j];
			if task:HasAcceptedTask(taskId) or task:HasCompletedTask(taskId) or (task:HasSubmitedTask(taskId) and GetSharpDay(task:GetTaskEndTime(taskId)) == thisDay) then
				skip = true;
				break;
			end
			j = j + 1;
		end
		if skip then break; end
		i = i + 1;
	end
	if skip then
		return RunDayCopyTaskStep(npcId,i);
	else
		return DayCopyTask_Inn;
	end
end
