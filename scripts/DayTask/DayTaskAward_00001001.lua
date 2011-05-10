local Award_multiple_00001001 ={
		[1] = 1,
		[2] = 1.2,
		[3] = 1.5,
		[4] = 2.0,
		[5] = 3.0,
};
local LoopTasks_Box_Table_000010001 = {
		[2] = 8949,
		[3] = 8950,
		[4] = 8951,
		[5] = 8952,
};

function ExtraAward_00001001(count)
	if(count == 10 or 
	   count == 30 or
	   count == 50 or
	   count == 70 or
	   count == 90 or
	   count == 100) then
	   
		return true;
	else
		return false;	
	end
end

function IsPackageEnough_00001001(count, color)
	local player = GetPlayer();
	local package = player:GetPackage();
	local reqGrids1 = 0;
	local reqGrids2 = 0;
	local reqGrids3 = 0;

	if color >= 2 and color <= 5 and count == 10 then	
		reqGrids1 = package:GetItemUsedGrids(LoopTasks_Box_Table_000010001[color], 1, 1);
	end
	
	if ExtraAward_00001001(count) then
		reqGrids2 = package:GetItemUsedGrids(8935, 2, 1);
		reqGrids3 = package:GetItemUsedGrids(8900, 2, 1);		
	end
	
	local reqGrids = reqGrids3 + reqGrids2 + reqGrids1;
	
	if reqGrids > player:GetFreePackageSize() then
		return false;
	else
		return true;
	end
end

function DayTaskAward_Exp_00001001(count, color)
	local player = GetPlayer();
	local playerLev = player:GetLev();
	
	-- 次数大于10时 每次经验 加30(20级-30级)、50(30级到40级)
	if count > 10 then	
		return 200;
	else
		return 100 * count * 2 * Award_multiple_00001001[color];
	end	
end

--经验循环任务
--count:任务完成的次数 color:当前任务颜色，为0则表示非刷新任务颜色任务
function DayTaskAward_Item_00001001(count, color)   
	local player = GetPlayer();	
	local package = player:GetPackage();
	local playerLev = player:GetLev();
	
	--判断背包是否有足够的空间
	if not IsPackageEnough_00001001(count, color) then
		player:sendMsgCode(2, 2013, 0);
		return false
	end
	
	--10、30、50、70、90、100次额外奖励
	if ExtraAward_00001001(count) then
		package:AddItem(8935, 2, 1);
		package:AddItem(8900, 2, 1);
	end	

	if color >= 2 and color <= 5 and count == 10 then					-- 第10次任务如果有颜色，奖励霹雳宝箱
		package:AddItem(LoopTasks_Box_Table_000010001[color], 1, 1);	
	end

	return true;
end


function DayTaskAward_00001001(count, color)
	if DayTaskAward_Item_00001001(count, color) then
		GetPlayer():AddExp(DayTaskAward_Exp_00001001(count, color));
		return true;
	end
	return false;
end