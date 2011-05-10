require("global")
require("DayTask/DayTaskTable")
require("DayTask/DayTaskAward_00001003")
require("DayTask/DayTaskAward_00001004")
require("DayTask/DayTaskAward_00001005")
require("DayTask/DayTaskAward_00001006")

function RunDayTask(npcId)
	local player = GetPlayer();
	local playerLev = player:GetLev();
	local loopTaskId = LoopTasksNpc[npcId];
	if loopTaskId == nil then
		return ActionTable:Instance();
	end
	local loopTask = LoopTasksDesc[loopTaskId];
	if playerLev < loopTask[4] or playerLev > loopTask[5] then
		return ActionTable:Instance();
	end
	local taskMgr = player:GetTaskMgr();
	local taskCount = taskMgr:GetDayTaskCompletedCnt(loopTaskId);
	if taskCount >= loopTask[6] then
		return ActionTable:Instance();
	end	
	local player = GetPlayer();
	local taskId = player:GetTaskMgr():GetDayTaskCurrTaskId(loopTaskId);
	if taskId == 0 then
		return ActionTable:Instance();
	end
	return RunTask(player, taskId, npcId);
end

function GetTaskLoopTaskId(taskId)
	
end

--循环任务奖励
function DayTaskAward(taskId)
	return GetPlayer():GetTaskMgr():DayTaskAward(taskId);
end

function GetAutoCompletedTask(dayTaskId)
	local auto = 0;
	if GetPlayer():getCountry() == 0 then
		if LoopTasksAutoTaskId[dayTaskId] ~= nil then
			auto = LoopTasksAutoTaskId[dayTaskId][1];
		end
	else
		if LoopTasksAutoTaskId[dayTaskId] ~= nil then
			auto = LoopTasksAutoTaskId[dayTaskId][2];
		end
	end
	return auto;
end


function GetAutoCompletedDayTaskId(taskId)
	
end

function GetAutoCompletedConsume()
	return 20;
end

function GetRandLoopTask(dayTaskId)
	local player = GetPlayer();
	local playerLev = player:GetLev();
	local state = player:getCountry();
	local taskSet;
	
	if dayTaskId == 1003 then
		if playerLev >= 70 then
			taskSet = {70301, 70302, 70303, 70304, 70305, 70306, 70307, 70308, 70309, 70310};
		elseif playerLev >= 60 then
			taskSet = {70301, 70302, 70303, 70304, 70305, 70306, 70307, 70308, 70309};
		elseif playerLev >= 50 then
			taskSet = {70301, 70302, 70303, 70304, 70305, 70306, 70307};
		elseif playerLev >= 40 then
			taskSet = {70301, 70302, 70303, 70304, 70305, 70306};
		else
			return 0;
		end	
	elseif dayTaskId == 1004 then
		return 0;  --FIXME later
	else
		return 0
	end
	
	local tableSize = table.getn(taskSet);
	local index = math.random(1, tableSize);
	return taskSet[index];
end

function GetRandLoopTaskQuality()
	local index = math.random(1, Task_Quality_Faces);
  local i
  local count = #Task_Quality_Bound;
  for i = 1, count do
    if index <= Task_Quality_Bound[i] then
      return i;
    end
  end
	return Task_Quality_Table[index];
end

function GetRandLoopTaskManualQuality(preColor, useGold, oldCount)
  if oldCount == 0 and preColor == 4 and not useGold then
    return preColor;
  end

  local bound
  local faces
			
	if preColor == 1 then
    bound = Task_Quality_White_Bound;
    faces = Task_Quality_White_Faces;
	elseif preColor == 2 then
    bound = Task_Quality_Green_Bound;
    faces = Task_Quality_Green_Faces;
	elseif preColor == 3 then
    bound = Task_Quality_Blue_Bound;
    faces = Task_Quality_Blue_Faces;
	elseif preColor == 4 then
    bound = Task_Quality_Purple_Bound;
    faces = Task_Quality_Purple_Faces;
	elseif preColor == 5 then
		return 5;
	end
	
  oldCount = oldCount + 1;
  if oldCount > #bound then
    oldCount = #bound;
  end
  local dice = math.random(1, faces[oldCount]);
  if dice <= bound[oldCount] then
    return preColor + 1;
  end
  return preColor;
end

function GetLoopTaskMaxCount(dayTaskId)
	return 	LoopTasksDesc[dayTaskId][6];
end

function GetLoopTaskMaxQualityCount(dayTaskId)
	return 	LoopTasksDesc[dayTaskId][7];
end

function FlushLoopTask(preColor, color, count, oldCount)
	local result = {flush_color = preColor, total_count = 0, flush_count = oldCount};
	while result.total_count < count do
		local newColor = GetRandLoopTaskManualQuality(result.flush_color, true, result.flush_count);
		result.total_count = result.total_count + 1;
    if newColor ~= result.flush_color then
      result.flush_count = 0;
      result.flush_color = newColor;
      if newColor >= color then
        break;
      end
    else
      result.flush_count = result.flush_count + 1;
    end
	end
	return result;
end

function GetLoopTaskTasks(dayTaskId)
	local player = GetPlayer();
	local taskSet;
	if player:getCountry() == 0 then
		--天族
		taskSet = LoopTasksSet[dayTaskId][1];
	else
		--魔族
		taskSet = LoopTasksSet[dayTaskId][2];
	end
	return taskSet;
end

function GetLoopTaskIdByNpc(npcId)
	return LoopTasksNpc[npcId];
end


function GetLoopTaskFlushGold(dayTaskId)
	return LoopTasksDesc[dayTaskId][8];	
end

function IsAutoTask(task)
	for i, v in pairs(TasksAutoTaskIdSet) do
		if v == task then
			return true;
		end
	end
	return false;
end

function IsFlushQualityDayTask(dayTaskId)
	if dayTaskId == 1003 then
		return true;
	end
	return false;
end
