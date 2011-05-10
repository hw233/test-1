--银币循环任务
function DayTaskAward_00001005(count, color)   --count:任务完成的次数 color:当前任务颜色，为0则表示非刷新任务颜色任务
	local player = GetPlayer();	
	player:getTael(50);
	return true;
end