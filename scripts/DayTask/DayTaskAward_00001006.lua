--战功循环任务
function DayTaskAward_00001006(count, color)   --count:任务完成的次数 color:当前任务颜色，为0则表示非刷新任务颜色任务
	local player = GetPlayer();	
	local playerLev = player:GetLev();
	if playerLev >= 40 and playerLev <= 59 then
		player:getAchievement(5);
	else
		if playerLev >= 60 and playerLev <= 100 then
			player:getAchievement(10);
		end
	end
	
	return true;
end
