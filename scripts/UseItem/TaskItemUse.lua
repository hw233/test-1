--任务物品使用
require("UseItem/TaskItemUseTable")

function IsTaskItemItem(itemId)
	if TaskItemUse_Table[0][itemId] == nil then
		if TaskItemUse_Table[1][itemId] == nil then
			return false;
		end
	end
	return true;
end

function RunTaskItemUse(itemId)
	local player = GetPlayer();
	local TaskItemUse = TaskItemUse_Table[GetPlayerData(6)][itemId];
	if TaskItemUse == nil then
		return false;
	end
	if player:getLocation() ~= TaskItemUse[2] then
		return false;
	end
	--玩家是否有此任务
	local taskId = TaskItemUse[1];
	if player:GetTaskMgr():HasAcceptedTask(taskId) then
		--更新此任务进度
		if player:GetTaskMgr():AddTaskStep(taskId) then
			--player:DelItem(itemId);
			return true;
		end
	end
	return false;
end
