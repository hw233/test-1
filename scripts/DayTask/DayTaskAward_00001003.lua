--low lev, high lev, green id, blue id, violet id, orange id
local LoopTasks_Box_Table_00001003 = {
		[1] = {[0] = 40, [1] = 49, [2] = 8953, [3] = 8958, [4] = 8963, [5] = 8968}, -- 40-49
		[2] = {[0] = 50, [1] = 59, [2] = 8954, [3] = 8959, [4] = 8964, [5] = 8969}, -- 50-69
		[3] = {[0] = 60, [1] = 69, [2] = 8955, [3] = 8960, [4] = 8965, [5] = 8970}, -- 60-69
		[4] = {[0] = 70, [1] = 79, [2] = 8956, [3] = 8961, [4] = 8966, [5] = 8971}, -- 70-79
		[5] = {[0] = 80, [1] = 89, [2] = 8957, [3] = 8962, [4] = 8967, [5] = 8972}, -- 80-89
		[6] = {[0] = 90, [1] = 99, [2] = 8957, [3] = 8962, [4] = 9212, [5] = 9213}, -- 90-100	
		[6] = {[0] = 100, [1] = 120, [2] = 8957, [3] = 8962, [4] = 9212, [5] = 9216}, -- 90-100	
};


local Award_multiple_00001003 ={
		[1] = 1,
		[2] = 1.2,
		[3] = 1.5,
		[4] = 2.0,
		[5] = 3.0,
};

-- low lev, high lev, coin kcard id, count
local Extra_Box_Table_00001003 = {
	[1] = {40, 49, 8936, 1, 8901, 2},
	[2] = {50, 59, 8936, 2, 8901, 2},
	[3] = {60, 69, 8936, 2, 8901, 3},
	[4] = {70, 100, 8936, 3, 8901, 3},
};

function ExtraAward_00001003(count)
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

function IsPackageEnough_00001003(count, color)
	local player = GetPlayer();	
	local package = player:GetPackage();
	local playerLev = player:GetLev();
	local reqGrids1 = 0;
	local reqGrids2 = 0;
	local reqGrids3 = 0;
	local reqGrids4 = 0;
	
	if color >= 2 and color <= 5 and count == 10 then	
			for i = 1, 5 do
				if playerLev >= LoopTasks_Box_Table_00001003[i][0] and playerLev <= LoopTasks_Box_Table_00001003[i][1] then
					reqGrids1 = package:GetItemUsedGrids(LoopTasks_Box_Table_00001003[i][color], 1, 1);
					break;
				end
		end
	end
	
	if ExtraAward_00001003(count) then
		reqGrids4 = package:GetItemUsedGrids(8935, 2, 1) + package:GetItemUsedGrids(8900, 2, 1);
		for i = 1, 4 do
			if playerLev >= Extra_Box_Table_00001003[i][1] and playerLev <= Extra_Box_Table_00001003[i][2] then
				reqGrids2 = package:GetItemUsedGrids(Extra_Box_Table_00001003[i][3], Extra_Box_Table_00001003[i][4], 1);
				reqGrids3 = package:GetItemUsedGrids(Extra_Box_Table_00001003[i][5], Extra_Box_Table_00001003[i][6], 1);
				break;
			end
		end
	end
	
	local reqGrids = reqGrids4 + reqGrids3 + reqGrids2 + reqGrids1;
	if reqGrids > player:GetFreePackageSize() then
		return false;
	else
		return true;
	end
end


function DayTaskAward_Exp_00001003(count, color)
	local player = GetPlayer();
	local playerLev = player:GetLev();
	
	--参照经验虚幻任务奖励.excel 大于40级 次数大于10
	if count > 10 then
		local factor1 = 2 * math.floor((count-11) / 10);
		local factor2 = math.mod((count-11), 10);
		if playerLev >= 110 then
			return 20000 + 5000 * factor1 + 8000 * factor2;
		elseif playerLev >= 100 then
			return 10000 + 2500 * factor1 + 2000 * factor2;
		elseif playerLev >= 90 then
			return 5000 + 1000 * (factor1 + factor2);
		elseif playerLev >= 80 then
			return 4500 + 500 * factor1 + 800 * factor2;
		elseif playerLev >= 70 then
			return 4000 + 500 * (factor1 + factor2);
		elseif playerLev >= 60 then
			return 3000 + 200 * factor1 + 100 * factor2;
		elseif playerLev >= 50 then
			return 1800 + 100 * factor1 + 50 * factor2;
		else
			return 1000 + 50 * (factor1 + factor2);
		end
	else
		if count <= 10 then
			if playerLev >= 110 then
				return 4000 * count * 30 * Award_multiple_00001003[color];
			elseif playerLev >= 100 then
				return 2500 * count * 20 * Award_multiple_00001003[color];
			elseif playerLev >= 90 then
				return 1000 * count * 20 * Award_multiple_00001003[color];
			elseif playerLev >= 80 then
				return 1000 * count * 13 * Award_multiple_00001003[color];
			elseif playerLev >= 70 then
				return 1000 * count * 10 * Award_multiple_00001003[color];
			elseif playerLev >= 60 then
				return 600 * count * 8 * Award_multiple_00001003[color];
			elseif playerLev >= 50 then
				return 500 * count * 6 * Award_multiple_00001003[color];
			else
				return 400 * count * 5 * Award_multiple_00001003[color];
			end
		end
	end	
end

--经验循环任务
--count:任务完成的次数 color:当前任务颜色，为0则表示非刷新任务颜色任务
function DayTaskAward_Item_00001003(count, color)   
	local player = GetPlayer();	
	local package = player:GetPackage();
	local playerLev = player:GetLev();
	
	if not IsPackageEnough_00001003(count, color) then
		player:sendMsgCode(2, 2013, 0);
		return false
	end
	
	--10、30、50、70、90、100次获得额外奖励
	if ExtraAward_00001003(count) then
		for i = 1, 4 do
			if playerLev >= Extra_Box_Table_00001003[i][1] and playerLev <= Extra_Box_Table_00001003[i][2] then
				package:AddItem(Extra_Box_Table_00001003[i][3], Extra_Box_Table_00001003[i][4], 1);
				package:AddItem(Extra_Box_Table_00001003[i][5], Extra_Box_Table_00001003[i][6], 1);
				break;
			end
		end
	end
		
	--第10次获得相应颜色宝箱
	if playerLev >= 40 and playerLev <= 49 then
		if color >= 2 and color <= 5 and count == 10 then
			package:AddItem(LoopTasks_Box_Table_00001003[1][color], 1, 1);	
		end			 
	elseif playerLev >= 50 and playerLev <= 59 then
		if color >= 2 and color <= 5 and count == 10 then
			package:AddItem(LoopTasks_Box_Table_00001003[2][color], 1, 1);
		end
	elseif playerLev >= 60 and playerLev <= 69 then
		if color >= 2 and color <= 5 and count == 10 then
			package:AddItem(LoopTasks_Box_Table_00001003[3][color], 1, 1);
		end
	elseif playerLev >= 70 and playerLev <= 79 then
		if color >= 2 and color <= 5 and count == 10 then
			package:AddItem(LoopTasks_Box_Table_00001003[4][color], 1, 1);
		end
	elseif playerLev >= 80 and playerLev <= 89 then
		if color >= 2 and color <= 5 and count == 10 then
			package:AddItem(LoopTasks_Box_Table_00001003[5][color], 1, 1);
		end
	elseif playerLev >= 90 then
		if color >= 2 and color <= 5 and count == 10 then
			package:AddItem(LoopTasks_Box_Table_00001003[6][color], 1, 1);
		end
	end		
	
	return true;
end

function DayTaskAward_00001003(count, color)
	if DayTaskAward_Item_00001003(count, color) then
		local player = GetPlayer()
		local expGot = DayTaskAward_Exp_00001003(count, color);
--		local stage = getActivityStage();
		local weekday = getWeekDay();
		if weekday == 2 then
			expGot = expGot * 2;
		end
--		if stage == 7 then
--			expGot = expGot * 2;
--			SendMsg(player, 0x35, "活动期间，经验已加倍");
--		end
		player:AddExp(expGot);
		return true;
	end
	return false;
end   
