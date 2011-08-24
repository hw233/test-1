--low lev, high lev, green id, blue id, violet id, orange id
local WealthBox_Table_000010004 = {
	[1] = {[0] = 40, [1] = 49, [2] = 8973,[3] = 8978,[4] = 8983,[5] = 8988}, -- 40-49 level
	[2] = {[0] = 50, [1] = 59, [2] = 8974,[3] = 8979,[4] = 8984,[5] = 8989}, -- 50-59 level
	[3] = {[0] = 60, [1] = 69, [2] = 8975,[3] = 8980,[4] = 8985,[5] = 8990}, -- 60-69 level
	[4] = {[0] = 70, [1] = 79, [2] = 8976,[3] = 8981,[4] = 8986,[5] = 8991}, -- 70-79 level
	[5] = {[0] = 80, [1] = 100, [2] = 8977,[3] = 8982,[4] = 8987,[5] = 8992}, -- 80-90 level
}
-- low lev, high lev, coin kcard id, count
local Extra_Box_Table_00001004 = {
	[1] = {40, 49, 8939, 1},
	[2] = {50, 59, 8939, 2},
	[3] = {60, 69, 8939, 2},
	[4] = {70, 100, 8939, 3},
};

function ExtraAward_00001004(count)
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

local Award_multiple_00001004 ={
		[1] = 1,
		[2] = 1.2,
		[3] = 1.5,
		[4] = 2.0,
		[5] = 3.0,
};



function IsPackageEnough_00001004(count, color)
	local player = GetPlayer();	
	local package = player:GetPackage();
	local playerLev = player:GetLev();
	local reqGrids1 = 0;
	local reqGrids2 = 0;
	
	--获得财富宝箱资格？
	if color >= 2 and color <= 5 and count == 10 then
		for i = 1, 5 do
			if playerLev >= WealthBox_Table_000010004[i][0] and playerLev <= WealthBox_Table_000010004[i][1] then
				reqGrids1 = package:GetItemUsedGrids(WealthBox_Table_000010004[i][color], 1);
				break;
			end
		end
	end
	
	--获得铜币卡资格？
	if ExtraAward_00001004(count) then
		for i = 1, 4 do
			if playerLev >= Extra_Box_Table_00001004[i][1] and playerLev <= Extra_Box_Table_00001004[i][2] then
				reqGrids2 = package:GetItemUsedGrids(Extra_Box_Table_00001004[i][3], Extra_Box_Table_00001004[i][4], 1);
				break;
			end
		end
	end
	
	local reqGrids = reqGrids2 + reqGrids1;
	if reqGrids > player:GetFreePackageSize() then
		return false;
	else
		return true;
	end
end

function DayTaskAward_Exp_00001004(count, color)
	local player = GetPlayer();
	local playerLev = player:GetLev();
	
	-- 次数大于10时 每次经验 加30(20级-30级)、50(30级到40级)
	if count > 10 then	
		if playerLev >= 70 then
			return 4000;
		elseif playerLev >= 60 then
			return 2000;
		elseif playerLev >= 50 then
			return 1500;
		else
			return 1000;
		end
	else 
		if playerLev >= 70 then
			return 4000 * count * 5 * Award_multiple_00001004[color];
		elseif playerLev >= 60 then
			return 3000 * count * 3 * Award_multiple_00001004[color]; 
		elseif playerLev >= 50 then
			return 2500 * count * 2 * Award_multiple_00001004[color];
		else
			return 1500 * count * 2 * Award_multiple_00001004[color];
		end
	end
	
	return 0;
end

function DayTaskAward_Item_00001004(count, color)
	local player = GetPlayer();	
	local package = player:GetPackage();
	local player_lev = player:GetLev();
	
	--判断背包空间足够？
	if not IsPackageEnough_00001004(count, color) then
		player:sendMsgCode(2, 1013, 0);
		return false
	end
	
	--10、30、50、70、90、100 奖励铜币卡
	if ExtraAward_00001004(count) then
		for i = 1, 4 do
			if player_lev >= Extra_Box_Table_00001004[i][1] and player_lev <= Extra_Box_Table_00001004[i][2] then
				package:AddItem(Extra_Box_Table_00001004[i][3], Extra_Box_Table_00001004[i][4], 1);	
				break;
			end
		end
	end
	
	--第10次奖励对应颜色的财富宝箱
	if count == 10 and color >= 2 and color <= 5 then
		if player_lev >= 40 and player_lev <= 49 then
			package:AddItem(WealthBox_Table_000010004[1][color], 1, 1);
		elseif player_lev >= 50 and player_lev <= 59 then
			package:AddItem(WealthBox_Table_000010004[2][color], 1, 1);
		elseif player_lev >= 60 and player_lev <= 69 then
			package:AddItem(WealthBox_Table_000010004[3][color], 1, 1);
		elseif player_lev >= 70 and player_lev <= 79 then
			package:AddItem(WealthBox_Table_000010004[4][color], 1, 1);
		elseif player_lev >= 80 then
			package:AddItem(WealthBox_Table_000010004[5][color], 1, 1);
		end	
	end
	
	return true;	
end



--铜币循环任务
--count:任务完成的次数 color:当前任务颜色，为0则表示非刷新任务颜色任务
function DayTaskAward_00001004(count, color)
	if DayTaskAward_Item_00001004(count, color) then
		GetPlayer():getCoin(DayTaskAward_Exp_00001004(count, color));
		return true;
	end
	return false;
end
