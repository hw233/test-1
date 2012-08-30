function checkExpire(player)
  local tm = os.time()
  local nextReward = player:getNextExtraReward()
  if tm >= nextReward then
    player:removeStatus(0xFFF8FF00)
    local thisDay = GetSharpDay(tm)
    nextReward = GetSharpDay(tm) + 86400
    player:setNextExtraReward(nextReward)
  end
end

function checkExpire2(player, stage)
  checkExpire(player)
end

function sendRewardMail(player, title, content, itemid, count)
  player:GetMailBox():newItemMail(0x21, title, content, itemid, count)
end

function sendItemPackageMail(player, title, content, table_item)
  player:GetMailBox():newItemPackageMail(title, content, table_item);
end

local MailPackage_Table = {
	[1] = {[8922] = 1},
	[2] = {[8922] = 2},
	[3] = {[8922] = 3},
	[4] = {[8922] = 4, [8923] = 1},
	[5] = {[8922] = 5, [8923] = 2, [8924] = 1},
	[6] = {[8922] = 6, [8923] = 3, [8924] = 2, [8927] = 1},
	[11] = {[8934] = 1},
	[12] = {[8934] = 2},
	[13] = {[8934] = 3},
	[14] = {[8934] = 4, [8924] = 1},
	[15] = {[8934] = 5, [8924] = 2, [8923] = 1},
	[16] = {[8934] = 6, [8924] = 3, [8923] = 2, [8927] = 1},
	[1001] = {[8934] = 2},
	[1003] = {[8934] = 10, [8940] = 5},
	[1004] = {[8934] = 6},
	[10002] = {[8923] = 2},
	[10003] = {[8922] = 2},
}
function onGetMailItems(pkgId)--client read mail 
	local ItemTable = {};
	if pkgId == 10000 then
		table.insert(ItemTable, 0xA000);--itemId:Coin = 0x8000, Tael = 0x9000, Coupon = 0xA000, Gold = 0xB000, Achievement = 0xC000
		table.insert(ItemTable, 888);--itemCount
		return ItemTable;
	elseif pkgId == 10001 then
		table.insert(ItemTable, 0xA000);
		table.insert(ItemTable, 88)
		return ItemTable;
	end
	local pkgFunc = MailPackage_Table[pkgId]
	if pkgFunc ~= nil then    
		for k, v in pairs(pkgFunc) do
			table.insert(ItemTable, k);
			table.insert(ItemTable, v);
		end
	end
	if pkgId == 1001 then
		table.insert(ItemTable, 0xA000);   
		table.insert(ItemTable, 100);    	
	elseif pkgId == 1002 then
		table.insert(ItemTable, 0xA000);
		table.insert(ItemTable, 88);        
	elseif pkgId == 1003 then
		table.insert(ItemTable, 0xA000);
		table.insert(ItemTable, 1000);        
	elseif pkgId == 1004 then
		table.insert(ItemTable, 0xA000); 
		table.insert(ItemTable, 300);    	
	end
	return ItemTable;
end

function onTakeMailPackage(player, pkgId)
  if pkgId == 10000 then
    player:getCoupon(888)
    return true
  elseif pkgId == 10001 then
    player:getCoupon(88)
    return true
  end
  local pkgFunc = MailPackage_Table[pkgId]
  if pkgFunc ~= nil then
    local package = player:GetPackage()
    local reqGrids = 0
    for k, v in pairs(pkgFunc) do
      reqGrids = reqGrids + package:GetItemUsedGrids(k, v, 1)
    end
    if reqGrids > package:GetRestPackageSize() then
      return false
    end
    for k, v in pairs(pkgFunc) do
      package:AddItem(k, v, 1)
    end
  end
  if pkgId == 10002 then
  elseif pkgId == 10003 then
  elseif pkgId == 1001 then
    player:getCoupon(100)
  elseif pkgId == 1002 then
    player:getCoupon(88)
  elseif pkgId == 1003 then
    player:getCoupon(1000)
  elseif pkgId == 1004 then
    player:getCoupon(300)
  end
  return true
end

function onLogin(player)
	local stage = getActivityStage();
    checkExpire2(player, stage);
    if not player:isOffical() then
        onLoginPF(player)
    end
    if getNetValentineDay() then
        onNVDLogin(player)
    end

    if getJune() then
        onJune(player)
    end
    if getJune1() then
        onJune1(player)
    end
    if getDuanWu() then
        onDuanWu(player)
    end
    if getJuly() then
        onJuly(player)
    end
end

function onNVDLogin(player)
    if player:GetVar(106) == 0 then
        sendItemPackageMail(player, msg_1, msg_2, {9027,1,1})
        player:SetVar(106, 1)
    end
end

function onLevelup(player, olev, nlev)
    if getChristmas() then
        onChristmas(player)
    end
    if getNewYear() then
        onNewYear(player)
    end
    if getValentineDay() then
        onValentineDay(player)
    end
    if getFoolsDay() then
        onFoolsDay(player)
    end
    if getMayDay() then
        onMayDay(player)
    end
    if getMayDay1() then
        onMayDay1(player)
    end
    if getJune() then
        onJune(player)
    end
    if getJune1() then
        onJune1(player)
    end
    if getJuly() then
        onJuly(player)
    end
end

function onDungeonWin(player, id, count, free)
    June(player, 0);
    Qixi(player, 0);
    if free == true then
        FallActivity(player, 1)
    else
        FallActivity(player, 2)
    end
end

function onClanBattleAttend(player)
	local stage = getActivityStage()
  	if stage == 1 then
		checkExpire(player)
		if player:hasStatus(0x400) then
			return
		end
		player:addStatus(0x400)
	end
end

function onCountryBattleAttend(player)
	local stage = getActivityStage();
	if stage == 1 then	
	end
end

function onCountryBattleWinStreak(player, count)
end

function onAttakerAward(attacker, defender, award)
	local isMale1 = attacker:IsMale();
	local isMale2 = defender:IsMale();
	if (isMale1 and not isMale2) or (not isMale1 and isMale2) then
		return (award * 3 / 2);		
	else
		return award;
	end	
end

function getActivateAttrResult(lastActivateCount, quality)
  if lastActivateCount > 3 then
    return true;
  end
  local rand = math.random(100);
  return rand < 40;
end

function onAthleticWin(player)
	local stage = getActivityStage()
	if stage == 1 then	
		checkExpire(player)
		local cnt = player:getStatusBit(20, 3);
		if(cnt >= 5) then
			return;
		end	
		cnt = cnt + 1;
		player:setStatusBit(20, 3, cnt);
		if cnt == 5 then
		end
	end
end

function onDayTaskComplete(player, count)
	local stage = getActivityStage()
	if stage == 1 then
		checkExpire(player)
		local cnt = player:getStatusBit(24, 2)
		if cnt >= 2 then
			return
		end
		local cnt2 = player:getStatusBit(26, 4)
		cnt2 = cnt2 + count
		if cnt2 >= 10 then
			cnt2 = cnt2 - 10
			cnt = cnt + 1
			player:setStatusBit(24, 2, cnt)
		end
		player:setStatusBit(26, 4, cnt2)
	end
end

function onExchange(player)
end

function onMergeGem(player, lev, num)
    if getGemMergeAct() then
        for n = 1, num do
            local r1 = {1416,1417,1418,1419,1420,}
            local r2 = {1421,1422,1423,1424,1425,1426,}
            local items = {
                [6] = {507,1,1, 503,1,1},
                [7] = {30,1,1, r1[math.random(1,#r1)],1,1, r2[math.random(1,#r2)],1,1},
                [8] = {30,4,1, r1[math.random(1,#r1)],1,1, r1[math.random(1,#r1)],1,1, r1[math.random(1,#r1)],1,1, r2[math.random(1,#r2)],1,1, r2[math.random(1,#r2)],1,1, r2[math.random(1,#r2)],1,1, getRandOEquip(player:GetLev()),1,1},
                [9] = {30,20,1, 1239,1,1, 1232,1,1, getRandOEquip(player:GetLev()),1,1, getRandOEquip(player:GetLev()),1,1, getRandOEquip(player:GetLev()),1,1},
                [10] = {30,50,1, getRandOEquip(player:GetLev()),1,1, getRandOEquip(player:GetLev()),1,1, getRandOEquip(player:GetLev()),1,1, getRandOEquip(player:GetLev()),1,1, getRandOEquip(player:GetLev()),1,1, getRandOEquip(player:GetLev()),1,1,},
            }

            local item = items[lev]
            if item == nil then
                return
            end

            sendItemPackageMail(player, msg_3, msg_3, item);
        end
	end
end

function onOnlineAward(player, itemId, count)
	local stage = getActivityStage();
	local rand = math.random(1, 100);
	local package = player:GetPackage();
	if rand < 16 then
			if package:Add(itemId, count * 2, true, false, 12) == nil then
				return false
			end
		SendMsg(player, 0x35, msg_4);
		else
			if package:Add(itemId, count, true, false, 12) == nil then
				return false
			end
	end
	return true
end

function onEnchant(player, level)
  local stage = getActivityStage();
  if stage == 2 then
	if level == 7 then
	elseif level == 8 then
	elseif level == 9 then
	elseif level == 10 then
		local favor = {5820, 5821, 5822, 5823, 5824};
		local rand_favor = math.random(1, #favor);
		local table_items = {favor[rand_favor], 10, 0};
	end
  end
end

function onAttackBoss(player)
	local stage = getActivityStage()
	if stage == 1 then
		checkExpire(player)
		local cnt = player:getStatusBit(30, 2)
		if cnt >= 3 then
			return
		end
		cnt = cnt + 1
		player:setStatusBit(30, 2, cnt)
		if cnt == 3 then
		end
	end
end

function onPurchase(player, id, count)
  return true
end

function onTopup(player, oldGold, newGold)
  local stage = getActivityStage();
  if stage == 2 then
	if player:hasStatus(0x200) then
		return;
	end
	player:addStatus(0x200)
  end
  if stage == 5 then
	local rand = math.random(1, 16);
	local gold = newGold - oldGold;
  end
end

function onTavernFlush(color)
end

function onLuckyDrawItemRoll(t)
  local stage = getActivityStage();
  if stage > 0 and stage < 4 then
	if t == 1 then
		if math.random(1,250) < 4 then
			return 9208
		end
	elseif t == 2 or t == 3 or t == 4 or t == 7 then
		if math.random(1,40) == 1 then
			return 9208
		end
	end	
  end
  return 0
end

extra_dungeon_loot_item = {9032, 9032, 9032, 9033, 9033, 9033, 9034, 9034, 9034, 9035}
function onDungeonLootItemRoll(player, id, l, b)
	local stage = getActivityStage();
	if stage ~= 3 then
		return 0;
	end

	local highr = 50;
	if (id == 1 and level == 15) or (id == 2 and level == 22) or (id == 3 and level == 44) or (id == 4 and level == 20) or (id == 5 and level == 20)then
        highr = 5;
	elseif not b then
        highr = 25;
	else
        highr = 10;
	end	
	if math.random(1, highr) == 1 then
		local pic = {9218, 9219, 9220, 9221, 9222};
		local rand = math.random(1, #pic);
		local table_items = {pic[rand], 1, 0};
	end
  return 0;
end

function exchangeRoll(player, itemId)
  local roll = math.random(1,1000) - 1;
  if roll < 10 then
    local c
    if roll == 0 then
      c = 888;
    else
      c = 100;
    end
    player:getCoupon(c);
    return;
  end
  local itemId = 8996;
  local itemCount = 1;
  if roll < 160 then
    itemId = 8923;
  elseif roll < 310 then
    itemId = 5002 + (math.random(1,4) - 1) * 10;
    itemCount = 3;
  elseif roll < 460 then
    itemId = 8919;
  elseif roll < 510 then
    itemId = 8999;
    itemCount = 3;
  elseif roll < 610 then
    itemId = 8927;
  elseif roll < 710 then
   itemId = 8934;
  elseif roll < 910 then
   itemId = 8941;
   itemCount = 3;
  end
  player:GetPackage():AddItem(itemId, itemCount, true);
end

function exchangeExtraReward(player, id)
	if id == 8 then
		local package = player:GetPackage()
		if package:IsFull() then
            player:sendMsgCode(2, 1011, 0)
			return;
		end
		if not package:DelItem(9224, 10, true) then
			--player:sendMsgCode(2, 2112, 0)
			return;
		end
		package:AddItem(9225, 1, true);
	elseif id == 9 then
		checkExpire(player)
		local cnt = player:getStatusBit(12, 2)
		if cnt >= 2 then
			--player:sendMsgCode(2, 2113, 0)
			return;
		end
		local package = player:GetPackage()
		if package:IsFull() then
			player:sendMsgCode(2, 1011, 0)
			return;
		end
		if not package:ExistItem(9218) or not package:ExistItem(9219) or not package:ExistItem(9220) or not package:ExistItem(9221) or not package:ExistItem(9222) then
			--player:sendMsgCode(2, 2113, 0)
			return
		end
		cnt = cnt + 1
		player:setStatusBit(12, 2, cnt)
		package:DelItem(9218, 1, false);
		package:DelItem(9219, 1, false);
		package:DelItem(9220, 1, false);
		package:DelItem(9221, 1, false);
		package:DelItem(9222, 1, false);
		package:AddItem(9223, 1, true);  
	else 
		return;  
	end
  if id == 1 then
    local package = player:GetPackage()
    if package:IsFull() then
      player:sendMsgCode(2, 1011, 0)
      return
    end
    if not package:DelItem(9001, 10, true) then
      --player:sendMsgCode(2, 2100, 0)
      return
    end
    exchangeRoll(player, 9001)
  elseif id == 2 then
    local package = player:GetPackage()
    if package:IsFull() then
      player:sendMsgCode(2, 1011, 0)
      return
    end
    if not package:DelItem(9004, 5, true) then
      --player:sendMsgCode(2, 2101, 0)
      return
    end
    exchangeRoll(player, 9004)
  elseif id == 3 then
    checkExpire(player)
    local package = player:GetPackage()
    if package:IsFull() then
      player:sendMsgCode(2, 1011, 0)
      return
    end
    if not package:DelItem(9038, 10, true) then
      --player:sendMsgCode(2, 2104, 0)
      return
    end
    package:AddItem(9039, 1, true);
  elseif id == 4 then
    checkExpire(player)
    local cnt = player:getStatusBit(12, 2)
    if cnt >= 2 then
      --player:sendMsgCode(2, 2102, 0)
      return
    end
    local package = player:GetPackage()
    if package:IsFull() then
      player:sendMsgCode(2, 1011, 0)
      return
    end
    if package:GetItemNum(9032, false) < 3 or package:GetItemNum(9033, false) < 3 or package:GetItemNum(9034, false) < 3 then
      player:sendMsgCode(2, 2102, 0)
      return
    end
    cnt = cnt + 1
    player:setStatusBit(12, 2, cnt)
    package:DelItem(9032, 3, false);
    package:DelItem(9033, 3, false);
    package:DelItem(9034, 3, false);
    package:AddItem(9036, 1, true);
  elseif id == 5 then
    checkExpire(player)
    local cnt = player:getStatusBit(14, 2)
    if cnt >= 2 then
      --player:sendMsgCode(2, 2103, 0)
      return
    end
    local package = player:GetPackage()
    if package:IsFull() then
      player:sendMsgCode(2, 1011, 0)
      return
    end
    if package:GetItemNum(9032, false) < 3 or package:GetItemNum(9033, false) < 3 or package:GetItemNum(9034, false) < 3 or package:GetItemNum(9035, false) < 2 then
      --player:sendMsgCode(2, 2103, 0)
      return
    end
    cnt = cnt + 1
    player:setStatusBit(14, 2, cnt)
    package:DelItem(9032, 3, false);
    package:DelItem(9033, 3, false);
    package:DelItem(9034, 3, false);
    package:DelItem(9035, 2, false);
    package:AddItem(9037, 1, true);
  elseif id == 6 then
    checkExpire(player)
    local cnt = player:getStatusBit(24, 2)
	if cnt >= 2 then
		--player:sendMsgCode(2, 2111, 0)
		return;
     end	
	local package = player:GetPackage();
	if package:GetItemNum(9205, false) < 3 or package:GetItemNum(9206, false) < 3 or package:GetItemNum(9207, false) < 3 then
      --player:sendMsgCode(2, 2111, 0)
      return;
    end
	cnt = cnt + 1;
	player:setStatusBit(24, 2, cnt);
	package:DelItem(9205, 3, false);
	package:DelItem(9206, 3, false);
	package:DelItem(9207, 3, false);
	package:AddItem(9209, 1, true);	
  elseif id == 7 then
	local package = player:GetPackage()
	if package:GetItemNum(9208, false) < 3 then
		--player:sendMsgCode(2, 2110, 0);
		return;
	end
	local exchangeCount = player:getTicketCount();
	exchangeCount = exchangeCount + 1;
	player:setTicketCount(exchangeCount, true);
	local table_count = {5, 10, 50, 100, 200, 500, 1000};
	for i, v in ipairs(table_count) do
		if exchangeCount == v then
		end
	end
	package:DelItem(9208, 3, false);
	package:AddItem(9210, 1, true);	
  end
end



function PrepareTask(player)
    local ActiveTask = {}
    local date = os.date("%Y%m%d", os.time())
    if date >=  "20110909" and date <= "20110912" then
        local num = player:getBuffData(21)
        if num <= os.time() or num == 0 then
            local action = ActionTable:Instance();
            action.m_ActionType = 0x70;
            action.m_ActionToken = 1;
            action.m_ActionID = 1; -- 领取月饼
            action.m_ActionStep = 0;
            action.m_ActionMsg = msg_5;
            table.insert(ActiveTask, action);
        else
            local action = ActionTable:Instance();
            action.m_ActionType = 0x70;
            action.m_ActionToken = 1;
            action.m_ActionID = 0;
            action.m_ActionStep = 0;
            action.m_ActionMsg = msg_6;
            table.insert(ActiveTask, action);
        end
    end

    return ActiveTask
end

function RunActiveTask(player, id)
    return PrepareTask(player)
end

function RunActiveTaskStep(player, id, actionId)
    local ActiveTask = {}
    return ActiveTask
end

function onRecruitAward(player)
    local date = os.date("%Y%m%d", os.time())
    if date >=  "20110915" then
		local table_items = {0xA000, 1000, 1};
        sendItemPackageMail(player, msg_7, msg_8, table_items)
    end
end

function onCopyFloorWin(player, id, floor, spot, lootlvl)
    if getSingleDay() then
        if id == 1 and floor == 2 and spot == 5 then -- 杨花
            local package = player:GetPackage();
            local shengyi = player:GetVar(2);
            if shengyi == 0 then
                package:AddItem(70, 1, true)
                player:SetVar(2, 70)
                SendMsg(player, 0x35, msg_9);
                SendMsg(player, 1, msg_9);
            end
        end

        if id == 1 and floor == 3 and spot == 5 then -- 智通
            local package = player:GetPackage();
            local apron = player:GetVar(3);
            if apron == 0 then
                package:AddItem(71, 1, true)
                player:SetVar(3, 71)
                SendMsg(player, 0x35, msg_10);
                SendMsg(player, 1, msg_10);
            end
        end
    end
end

function SingleDayReward(player, lootlvl)
    if getSingleDay() then
        if lootlvl > 3 then
            return;
        end

        local itemNum = {
            [0] = 1,
            [1] = 2,
            [2] = 4,
            [3] = 6,
        };

        local package = player:GetPackage();
        package:AddItem(69, itemNum[lootlvl], false);    
    end
end

function Christmas(player, lootlvl, where)
    if getChristmas() then
        if lootlvl > 3 then
            return;
        end

        local itemNum = {
            [0] = 1,
            [1] = 2,
            [2] = 4,
            [3] = 6,
        };

        local package = player:GetPackage();
        package:AddItem(401, itemNum[lootlvl], false);
    end
end

function ValentineDay(player, lootlvl, where)
    if getValentineDay() then
        if lootlvl > 3 then
            return;
        end

        local itemNum = {
            [0] = 1,
            [1] = 2,
            [2] = 4,
            [3] = 6,
        };

        local package = player:GetPackage();
        package:AddItem(441, itemNum[lootlvl], false);
    end
end

function NetValentineDay(player, lootlvl)
    if getNetValentineDay() then
        if lootlvl > 3 then
            return;
        end

        local itemNum = {
            [0] = 1,
            [1] = 2,
            [2] = 4,
            [3] = 6,
        };

        local package = player:GetPackage();
        local items = {9023,9024,9025,9026}
        for n = 1, itemNum[lootlvl] do
            package:AddItem(items[math.random(1,#items)], 1, false);
        end
    end
end

function GirlDay(player, lootlvl)
    if getGirlDay() then
        local item = {471, 472, 473, 474}

        if lootlvl > 3 then
            lootlvl = 0
        end

        local itemNum = {
            [0] = 1,
            [1] = 2,
            [2] = 4,
            [3] = 6,
        };

        local package = player:GetPackage();
        for m = 1,itemNum[lootlvl] do
            package:AddItem(item[math.random(1, #item)], 1, false);
        end
    end
end

function WhiteLoveDay(player, lootlvl, where)
    if getWhiteLoveDay() and ((getWeekDay() == 4 and where == 0) or (getWeekDay() == 5 and where == 1)) then
        if lootlvl > 3 then
            lootlvl = 0
        end

        local itemNum = {
            [0] = 2,
            [1] = 3,
            [2] = 4,
            [3] = 5,
        };

        local package = player:GetPackage();
        package:AddItem(476, itemNum[lootlvl], true);
    end
end

function MayDay(player, lootlvl)
    if getMayDay() then
        if lootlvl > 3 then
            lootlvl = 0
        end

        local itemNum = {
            [0] = 2,
            [1] = 4,
            [2] = 8,
            [3] = 12,
        };

        local item = {496,497}
        local package = player:GetPackage();
        package:AddItem(item[math.random(1,#item)], itemNum[lootlvl], true);
    end
end

function LuckyDrawBox(player, id)
    local items = {
        [2] = 9012,
        [3] = 9013,
        [4] = 9014,
        [5] = 9015,
        [6] = 9016,
        [7] = 9035,
    }

    local item = items[id]
    if item == nil then
        return
    end

    local package = player:GetPackage()
    package:Add(item, 1, true)
end

function onCopyWin(player, id, floor, spot, lootlvl)
    SingleDayReward(player, lootlvl);
    Christmas(player, lootlvl, 0);
    ValentineDay(player, lootlvl)
    NetValentineDay(player, lootlvl)
    GirlDay(player, lootlvl)
    WhiteLoveDay(player, lootlvl, 0)
    ChingMingDay(player, lootlvl)
    MayDay(player, lootlvl)
    June(player, lootlvl);
    Qixi(player, lootlvl);
    LuckyDrawBox(player, id)
    if player:getQQVipPrivilege() == true then
        player:setQQVipPrivilege(false)
        FallActivity(player, 1)
    else
        if lootlvl == 0 then
            FallActivity(player, 1)
        else
            FallActivity(player, lootlvl)
        end
    end
end


function onFrontMapFloorWin(player, id, spot, lootlvl)
end

function onFrontMapWin(player, id, spot, lootlvl)
    SingleDayReward(player, lootlvl);
    Christmas(player, lootlvl, 1);
    ValentineDay(player, lootlvl)
    NetValentineDay(player, lootlvl)
    GirlDay(player, lootlvl)
    WhiteLoveDay(player, lootlvl, 1)
    ChingMingDay(player, lootlvl)
    MayDay(player, lootlvl)
    June(player, lootlvl);
    Qixi(player, lootlvl);
    if lootlvl == 0 then
        FallActivity(player, 1)
    else
        FallActivity(player, lootlvl)
    end
end

local vippack = {
    [1] = {{511,2},{505,2},{500,2},{15,1},{502,5},{47,1},{48,2},{49,1},{50,1},{51,1},{513,2},{5033,3},{9,5},{56,10}},
    [2] = {{56,2},{51,1},{15,1},{511,1}},
    [3] = {{15,1},{5101,1},{510,1},{502,1}},
    [4] = {{511,1},{512,1},{505,2},{500,1},{15,1},{503,3},{47,1},{48,2},{49,1},{50,1},{51,1},{513,1},{5034,1},{9,5},{56,10},{57,10}},
    [5] = {{511,4},{512,2},{505,2},{500,5},{15,5},{503,5},{47,1},{48,2},{49,1},{50,1},{51,2},{513,1},{5034,1},{9,5},{56,12},{57,12},{514,3}},
    [6] = {{511,4},{512,2},{505,2},{500,5},{15,6},{503,5},{47,1},{48,2},{49,1},{50,1},{51,2},{513,1},{5034,1},{9,5},{56,12},{57,12},{514,3},{506,2},{508,2}},
    [7] = {{511,4},{512,2},{505,2},{500,5},{15,6},{503,5},{47,1},{48,2},{49,1},{50,1},{51,2},{513,1},{5034,1},{9,5},{56,12},{57,12},{514,3},{506,5},{508,5}},
    [8] = {{511,4},{512,4},{505,2},{500,8},{15,10},{503,5},{47,2},{49,2},{50,2},{513,1},{5034,1},{514,5},{2554,1},{2579,1},{2601,1}},
};

-- XXX: 必须修改这里
local packsize = {
    14,4,16,17,4,19,19,15
};

-- 每个包每个玩家可领取的次数
local packFrequency = {
    1,1,7,1,1,1,1,1
};

function testTakePackSize(player, _type)
	local package = player:GetPackage();
    local needsize = packsize[_type];
    if needsize == nil then
        return false
    end

    if package:GetRestPackageSize() < needsize then
        player:sendMsgCode(2, 1011, 0)
        return false
    end
    return true
end

function testTakePack(player, _type, _freq)
    local freq = packFrequency[_type];
    if freq == nil then
        return false 
    end

    if freq > _freq then
        return true
    end

    return false
end

function onGetVipPack(player, _type)
    if _type == 0 then
        return
    end

	local package = player:GetPackage();
    local needsize = packsize[_type];
    if needsize == nil then
        return
    end

    if package:GetRestPackageSize() < needsize then
        player:sendMsgCode(2, 1011, 0)
        return
    end

    if vippack[_type] == nil then
        return
    end

    for k, v in pairs(vippack[_type]) do
        package:Add(v[1], v[2], true)
    end
end

function onThanksgivingDay(player)
    if not getThanksgiving() then
        return
    end

    sendItemPackageMail(player, msg_11, msg_12, {8,3,1});

    player:SetVar(4, -1)
    local cnt = player:GetVar(5)
    player:SetVar(5, cnt+1);

    if cnt == 6 then
        sendItemPackageMail(player, msg_13, msg_14, {1525,5,1});
    end
end

function onChristmas(player)
    if not getChristmas() then
        return
    end

    local lvl = player:GetLev()
    if lvl < 30 then
        return
    end

    if lvl >= 30 and player:GetVar(7) == 0 then
        sendItemPackageMail(player, msg_15, msg_16, {1637,1,1});
        player:SetVar(7,1)
    end

    if lvl >= 40 and player:GetVar(8) == 0 then
        sendItemPackageMail(player, msg_17, msg_18, {1752,1,1});
        player:SetVar(8,1)
    end
end

function onNewYear(player)
    if not getNewYear() then
        return
    end

    local lvl = player:GetLev()
    if lvl < 30 then
        return
    end

    if lvl >= 30 and player:GetVar(42) == 0 then
        sendItemPackageMail(player, msg_20, msg_19, {417,1,1});
        sendItemPackageMail(player, msg_21, msg_19, {418,1,1});
        sendItemPackageMail(player, msg_22, msg_19, {419,1,1});
        sendItemPackageMail(player, msg_23, msg_19, {420,1,1});
        sendItemPackageMail(player, msg_24, msg_19, {421,1,1});
        sendItemPackageMail(player, msg_25, msg_19, {422,1,1});
        sendItemPackageMail(player, msg_26, msg_19, {423,1,1});
        sendItemPackageMail(player, msg_27, msg_19, {424,1,1});
        sendItemPackageMail(player, msg_28, msg_19, {425,1,1});
        sendItemPackageMail(player, msg_29, msg_19, {426,1,1});
        sendItemPackageMail(player, msg_30, msg_19, {427,1,1});
        sendItemPackageMail(player, msg_31, msg_19, {428,1,1});
        sendItemPackageMail(player, msg_32, msg_33, {1753,1,1});
        player:SetVar(42,1)
    end
end

function onBlueactiveday(player)
    if not getBlueactiveday() then
        return
    end

    if player:GetVar(45) >= 1 then
        return
    end

    sendItemPackageMail(player, msg_34, msg_35, {9,1,1, 502,1,1, 510,1,1, 55,1,1, 29,1,1, 51,2,1});
    player:SetVar(45, 1);
end

function onValentineDay(player)
    if not getValentineDay() then
        return
    end

    local lvl = player:GetLev()
    if lvl < 30 then
        return
    end

    if lvl >= 30 and player:GetVar(47) == 0 then
        sendItemPackageMail(player, msg_36, msg_37, {1754,1,1});
        player:SetVar(47, 1)
    end
end

function onGirlDay(player)
    if not getGirlDay() then
        return
    end
end

function onFoolsDay(player)
    if not getFoolsDay() then
        return
    end

    local lvl = player:GetLev()
    if lvl < 40 then
        return
    end

    if lvl >= 40 and player:GetVar(58) == 0 then
        sendItemPackageMail(player, msg_38, msg_39, {479,1,1});
        local date = os.date("%m%d", os.time())
        if date == "0401" then
            sendItemPackageMail(player, msg_40, msg_41, {480,5,1});
        end
        player:SetVar(58, 1)
    end
end

function onMayDay(player)
    if not getMayDay() then
        return
    end

    local lvl = player:GetLev()
    if lvl < 40 then
        return
    end

    if lvl >= 40 and player:GetVar(94) == 0 then
        sendItemPackageMail(player, msg_42, msg_43, {1755,1,1});
        player:SetVar(94, 1)
    end
end

function onMayDay1(player)
    if not getMayDay1() then
        return
    end

    local lvl = player:GetLev()
    if lvl < 40 then
        return
    end

    if lvl >= 40 and player:GetVar(95) == 0 then
        sendItemPackageMail(player, msg_44, msg_45, {9001,1,1});
        sendItemPackageMail(player, msg_44, msg_45, {9002,1,1});
        sendItemPackageMail(player, msg_44, msg_45, {9003,1,1});
        sendItemPackageMail(player, msg_44, msg_45, {9004,1,1});
        sendItemPackageMail(player, msg_44, msg_45, {9005,1,1});
        sendItemPackageMail(player, msg_44, msg_45, {9006,1,1});
        player:SetVar(95, 1)
    end
end

function ChingMingDay(player, lootlvl)
    if not getChingMing() then
        return
    end

    if lootlvl > 3 then
        lootlvl = 0
    end

    local itemNum = {
            [0] = 1,
            [1] = 2,
            [2] = 4,
            [3] = 6,
        };
    local package = player:GetPackage();
    package:AddItem(481, itemNum[lootlvl], true);
end

function FallActivity(player, count)
    if not getFallAct() then
        return
    end

    if count > 3 then
        count = 3
    end

    local package = player:GetPackage();
    package:AddItem(9119, count, true);
end

function onLoginPF(player)
    if player:GetVar(49) == 0 then
        local pfnames = {
            [1] = "QQ空间",
            [2] = "朋友网",
            [4] = "Q+",
            [10] = "QQ游戏大厅",
            -- [11] = "3366",
            -- [12] = "官网",
        }
        local titles = {
            [1] = pfnames[1].."登陆奖励",
            [2] = pfnames[2].."登陆奖励",
            [4] = pfnames[4].."登陆奖励",
            [10] = pfnames[10].."登陆奖励",
            --[11] = pfnames[11].."登陆奖励",
            -- [12] = pfnames[12].."登陆奖励",
        }
        local ctxs = {
            [1] = "每日通过"..pfnames[1].."登陆《蜀山传奇》都可以获得【"..pfnames[1].."登陆礼包】 \n登陆礼包每日只能领取一次",
            [2] = "每日通过"..pfnames[2].."登陆《蜀山传奇》都可以获得【"..pfnames[2].."登陆礼包】 \n登陆礼包每日只能领取一次",
            [4] = "每日通过"..pfnames[4].."登陆《蜀山传奇》都可以获得【"..pfnames[4].."登陆礼包】 登陆礼包每日只能领取一次",
            [10] = "每日通过"..pfnames[10].."登陆《蜀山传奇》都可以获得【"..pfnames[10].."登陆礼包】 \n登陆礼包每日只能领取一次",
            --[11] = "每日通过"..pfnames[11].."登陆《蜀山传奇》都可以获得【"..pfnames[11].."登陆礼包】 登陆礼包每日只能领取一次",
            -- [12] = "每日通过"..pfnames[12].."登陆《蜀山传奇》都可以获得【"..pfnames[12].."登陆礼包】 登陆礼包每日只能领取一次",
        }

        local pkgs = {
            [1] = {469,1,1},
            [2] = {470,1,1},
            [4] = {448,1,1},
            [10] = {468,1,1},
            --[11] = {448,1,1},
            -- [12] = {448,1,1},
        }

        local pf = player:getPlatform()

        if pf == 11 then
            return
        end

        if pfnames[pf] == nil then
            return
        end

        sendItemPackageMail(player, titles[pf], ctxs[pf], pkgs[pf]);
        player:SetVar(49, 1)
    end
end

function onInvitedBy(player)
    -- sendItemPackageMail(player, "好友邀请奖励", "您的好友邀请您一同游玩蜀山传奇，系统赠送您一份大礼包", {503,5,1, 56,10,1, 57,10,1, 5035,1,1});
end

function onCLLoginReward(player, cts)
    if isFBVersion() then
        if cts == 0 then
            player:getCoupon(20)
            return
        end

        local coupon = {20,30,40,50,60,70,80,}
        if cts > 7 then
            return
        end
        player:getCoupon(coupon[cts]);
    else
        local coupon = {10,15,20,25,30,35,40,}
        if cts > 7 then
            return
        end
        player:getCoupon(coupon[cts]);
    end
end

function onCLLoginRewardRF(player, cts, _type)
    if isFBVersion() then
        if cts == 0 then
            player:getCoupon(10)
            return 0
        end
    else
        if _type == 1 then -- XXX: 走0x13了
            local items = {515,503,507,56,57,509}
            local prob = {379,1895,2400,5768,9558,10000}
            local p = math.random(1,10000)
            local i = 1
            for n = 1,#prob do
                if p <= prob[n] then
                    i = n
                    break
                end
            end
            return items[i]
        elseif _type == 2 then
            local coupon = {10,10,10,10,10,10,10}
            if cts > 7 then
                return 0
            end
            player:getCoupon(coupon[cts]);
        elseif _type == 3 then
            if cts >= 3 then
                local package = player:GetPackage()
                if package:IsFull() then
                    return 0
                end

                package:Add(9036, 1, 1)
                return 1
            end
        elseif _type == 4 then
        end
    end
    return 0
end

function onCL3DayReward(player)
    local package = player:GetPackage()
    if package:IsFull() then
        player:sendMsgCode(2, 1011, 0)
        return;
    end
    package:AddItem(9011, 1, 1)
end

function onRC7DayWill(player, idx)
    if idx == 1 then
        player:getPrestige(5000, true)
        return true
    end

    if idx == 2 then
        player:getAchievement(3000)
        return true
    end

    if idx == 3 then
        player:AddPExp(100000)
        return true
    end

    if idx == 4 then
        player:AddExp(1000000)
        return true
    end
end

function onUseMDSoul(player, _type)
    if _type == 0 or _type > 3 then
        return 0
    end

    local items = {
        {9000,47,509,507,515,509,507,515,},
        {503,514,506,508,517,512,501,513,},
        {497,496,15,56,57,511,500,518,},
    }

    if _type == 3 then
        local prob = {500,1000,2500,4000,5500,7000,8500,10000}
        local p = math.random(1,10000)
        local i = 1
        for n = 1,#prob do
            if p <= prob[n] then
                i = n
                break
            end
        end

        return items[_type][i]
    end

    return items[_type][math.random(1,#items[_type])]
end

function onTurnOnRC7Day(player, total, offset)
    local items = {
        {{15,2},{502,1},{57,1},{56,1},{506,1},{508,1},{510,2},{503,1}},
        {{15,2},{502,2},{57,1},{56,1},{506,1},{508,1},{510,5},{503,1}},
        {{15,2},{502,3},{57,2},{56,2},{506,2},{508,2},{511,2},{503,2}},
        {{15,2},{502,4},{57,2},{56,2},{506,2},{508,2},{511,2},{503,2}},
        {{15,2},{502,5},{57,2},{56,2},{506,2},{508,2},{511,2},{503,2}},
        {{15,2},{502,6},{57,2},{56,2},{506,2},{508,2},{511,2},{503,2}},
        {{15,5},{502,10},{57,5},{56,5},{507,2},{509,2},{512,2},{503,2}},
    }

    if offset + 1 > #items then
        return false
    end

    local totals = {10,50,100,200,300,400,500}

    local max = 0
    for n = 1, #totals do
        if total >= totals[n] then
            max = n
        else
            break
        end
    end

    if max == 0 then
        return false
    end

    if offset >= max then
        return false
    end

    local package = player:GetPackage()
    if package:GetRestPackageSize() < 8 then
        player:sendMsgCode(2, 1011, 0)
        return false
    end

    local item = items[offset + 1]
    for k,v in pairs(item) do
        package:AddItem(v[1], v[2], 1)
    end

    return true
end

function onTurnOnRF7Day(player, total, offset)
    local items = {
        {{56,1},{57,1}},
        {{56,2},{57,2},{15,1}},
        {{56,3},{57,3},{15,2}},
        {{56,4},{57,4},{15,3},{503,1}},
        {{56,5},{57,5},{15,4},{503,2}},
        {{56,6},{57,6},{15,5},{503,3},{516,1}},
        {{56,7},{57,7},{15,6},{503,4},{516,2}},
    }

    if offset + 1 > #items then
        return false
    end

    local totals = {10,50,100,300,500,700,1000}

    local max = 0
    for n = 1, #totals do
        if total >= totals[n] then
            max = n
        else
            break
        end
    end

    if max == 0 then
        return false
    end

    if offset >= max then
        return false
    end

    local item = items[offset + 1]
    local package = player:GetPackage()
    if package:GetRestPackageSize() < #item then
        player:sendMsgCode(2, 1011, 0)
        return false
    end

    for k,v in pairs(item) do
        package:Add(v[1], v[2], 1)
    end

    return true
end

function onEnchantAct(player, level, quality, _type)
    if isFBVersion() then
        local pic = {1416, 1417, 1418, 1419, 1420};
        local rand = math.random(1, #pic);
        local rand2 = math.random(1, #pic);
        local rand3 = math.random(1, #pic);
        local items = {
            [4] = {502,1,1},
            [6] = {509,2,1},
            [8] = {509,5,1, pic[rand],1,1, 30,1,1},
            [10] = {509,15,1, pic[rand],1,1, pic[rand2],1,1, pic[rand3],1,1, 30,3,1},
        };
        sendItemPackageMail(player, msg_46, msg_46, items[level]);
    else
        local items = {
            [2] = {
                [8] = {509,1,1, 507,1,1, },
            },
            [3] = {
                [7] = {509,1,1, 507,1,1, },
                [8] = {509,2,1, 507,2,1, },
            },
            [4] = {
                [6] = {515,1,1, 509,1,1, 507,1,1, },
                [7] = {515,2,1, 514,3,1, },
                [8] = {515,3,1, },
            },
            [5] = {
                [6] = {515,2,1, 514,5,1, },
                [7] = {515,2,1, 509,1,1, 507,1,1, },
                [8] = {515,5,1, 509,1,1, 507,1,1, },
            },
        };
        print('level: ' .. level .. ' quality: ' .. quality .. ' _type: ' .. _type)
        if items[quality] == nil then
            return
        end
        if items[quality][level] == nil then
            return
        end

        sendItemPackageMail(player, msg_93, msg_94, items[quality][level]);
    end
end

function onEnchantGt11(player, id, level, _type)
    local items = {
        [1] = {
            [11] = {9022,2,1, 9021,2,1},
            [12] = {9022,3,1, 9021,3,1},
        },
        [2] = {
            [11] = {9022,1,1, 9021,1,1},
            [12] = {9022,1,1, 9021,2,1},
        },
    };
    if items[_type][level] == nil then
        return
    end
    sendItemPackageMail(player, msg_47, msg_48 .. "[4:"..id.."] "..level..msg_49, items[_type][level]);
end

function onSoulEnchantMaxSoul(player, oms, yams)
    local items = {
        {9069,2,1, 9017,2,1, 9072,1,1},
        {9021,2,1, 9075,2,1, 9019,2,1},
        {9022,1,1, 9073,1,1, 9070,1,1, 9071,1,1},
        {9022,2,1, 9074,2,1, 9068,3,1},
    }

    if oms >= yams then
        return
    end

    local ms = {100,120,160,210}

    if yams < ms[1] then
        return
    end

    local s = 0
    local e = 0
    for n=1,#ms do
        if oms >= ms[n] then
            s = n
        end
        if yams >= ms[n] then
            e = n
        end
    end

    if s >= e then
        return
    end

    for j=s+1,e do
        sendItemPackageMail(player, msg_104, msg_104, items[j])
    end
end

function onTrainFighterAct(player, fgt)
    local table_items = {30, 6, 1};
    sendItemPackageMail(player, msg_50, msg_50, table_items)
end

function onJune(player)
    if not getJune() then
        return
    end

    local lvl = player:GetLev()
    if lvl < 40 then
        return
    end

    if lvl >= 40 and player:GetVar(111) == 0 then
        sendItemPackageMail(player, msg_51, msg_52, {1756,1,1});
        player:SetVar(111, 1)
    end
end

function onJune1(player)
    if not getJune1() then
        return
    end

    local lvl = player:GetLev()
    if lvl < 40 then
        return
    end

    if lvl >= 40 and player:GetVar(110) == 0 then
        sendItemPackageMail(player, msg_53, msg_56, {9031,1,1});
        sendItemPackageMail(player, msg_54, msg_57, {9032,1,1});
        sendItemPackageMail(player, msg_55, msg_58, {9033,1,1});
        player:SetVar(110, 1)
    end
end

function onJuly(player)
    if not getJuly() then
        return
    end

    local lvl = player:GetLev()
    if lvl < 40 then
        return
    end

    if lvl >= 40 and player:GetVar(136) == 0 then
        sendItemPackageMail(player, msg_102, msg_102, {9077,1,1});
        sendItemPackageMail(player, msg_102, msg_102, {9078,1,1});
        sendItemPackageMail(player, msg_102, msg_102, {9079,1,1});
        player:SetVar(136, 1)
    end
end

function June(player, lootlvl)
    if getJune() then
        -- 棒棒糖
        local package = player:GetPackage();
        package:AddItem(9028, 1, true, 0, 40);
    end
end

function onRechargeAct(player, total)
    local title = msg_96
    local content = msg_97
    if total >= 1000 and total <= 1999 then
        sendItemPackageMail(player, title, content, {0xB000,100,0});
    elseif total >= 2000 and total <= 3999 then
        sendItemPackageMail(player, title, content, {0xB000,300,0});
    elseif total >= 4000 then
        sendItemPackageMail(player, title, content, {0xB000,0.2*total,0});
    end
end

function sendRNR(player, off, date, total)
    local rm = os.date("%m", date)
    local rd = os.date("%d", date)
    local title = string.format(msg_91, rm, rd, off+1)

    local rorate = {[0]=50,[1]=30,[2]=20,}
    local ror = rorate[off]
    if ror == nil then
        return
    end

    local cal = {99,199,399,699,1099,1599,2199,2899,3699,4599,5599,6799,8199,9799,11599,13599,15899,18499,21399,24599,28199,32199,36699,41699,47299,53499,60399,67999,76399,85899,99999,}
    local rate = {5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,36,}

    local i = 0
    local sz = #cal
    for n=1,sz do
        if total >= cal[n] then
            i = n
        end
    end

    if i == 0 then
        return
    end

    local r = rate[i]
    if r == nil then
        return
    end

    local f = math.floor(total * (r/100) * (rorate[0]/100))
    local s = math.floor(total * (r/100) * (rorate[1]/100))
    local t = math.ceil(total * (r/100)) - (f + s)
    local ms = {f, s, t}

    local m = ms[off+1]
    if m == nil then
        return
    end
    local ctx = string.format(msg_92, rm, rd, total, off+1, m)

    sendItemPackageMail(player, title, ctx, {0xB000,m,1});
end

function onDuanWu(player)
    if not getDuanWu() then
        return
    end

    local lvl = player:GetLev()
    if lvl < 40 then
        return
    end

    if lvl >= 40 and player:GetVar(135) == 0 then
        sendItemPackageMail(player, msg_95, msg_95, {1527,2,1});
        player:SetVar(135, 1)
    end
end

function calcRechargeLevel(lvls, total)
    if lvls == nil then
        return 0
    end

    local sz = #lvls
    if sz == 0 then
        return 0
    end

    for j = 1,sz do
        if total < lvls[j] then
            return (j-1)
        end
    end
    return sz
end

function sendRechargeMails1(player, ototal, ntotal)
    local lvls = {
        {99,199,399,699,1099,1599,2299,3699,5799,9999,},
        {99,199,399,699,1099,1599,2299,3699,5799,9999,},
        {99,199,399,699,1099,1599,2299},
    }
    local items = {
        {
            {56,3,1,},
            {57,4,1,},
            {508,6,1,},
            {506,6,1},
            {515,5,1},
            {9021,1,1},
            {516,5,1, 47,3,1},
            {503,8,1},
            {509,5,1, 507,1,1},
            {509,7,1, 507,3,1, 9022,1,1},
        },
        {
            {500,3,1},
            {503,2,1},
            {56,3,1, 57,3,1},
            {516,2,1},
            {509,3,1, 507,2,1},
            {5035,1,1},
            {515,5,1, 47,3,1},
            {503,4,1, 0xA000,200,1},
            {509,5,1, 507,1,1},
            {509,7,1, 507,3,1, 9022,1,1},
        },
        {
            {500,2,1},
            {514,2,1},
            {503,2,1},
            {516,2,1},
            {515,1,1},
            {515,2,1},
            {515,3,1},
        },
    }

    local start = { ['year'] = 2012, ['month'] = 7, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(start);
    local n = os.time();

    local i = 0
    if n >= s and n < s + 3 * 86400 then
        i = 1
    elseif n >= s + 3 * 86400 and n < s + 6 * 86400 then
        i = 2
    end

    start = { ['year'] = 2012, ['month'] = 7, ['day'] = 9, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    s = os.time(start)
    if n >= s and n < s + 5 * 86400 then
        i = 3
    end

    if i == 0 then
        return
    end

    local lvl = lvls[i]
    if lvl == nil then
        return
    end
    local olvl = calcRechargeLevel(lvl, ototal)
    local nlvl = calcRechargeLevel(lvl, ntotal)

    if nlvl == 0 or olvl == nlvl then
        return
    end

    for k = olvl+1, nlvl do
        local title = string.format(msg_100, lvls[i][k])
        local ctx = string.format(msg_101, lvls[i][k])
        sendItemPackageMail(player, title, ctx, items[i][k]);
    end
end

function sendRechargeMails2(player, ototal, ntotal)
    local lvls = {
        99,199,399,699,1099,1599,2199,2899,3699,4599,5599,8999,15999,
    }
    local items = {
        {503,2,1, 514,1,1},
        {500,3,1, 56,6,1, 57,2,1},
        {508,2,1, 56,6,1, 57,5,1},
        {511,6,1, 466,6,1},
        {516,3,1, 512,2,1},
        {5065,1,1, 56,5,1},
        {503,5,1, 56,6,1, 57,2,1},
        {515,2,1, 56,6,1, 57,2,1},
        {515,2,1, 56,6,1, 57,5,1},
        {515,2,1, 56,6,1, 57,6,1},
        {549,2,1, 56,6,1, 57,6,1},
        {515,5,1, 30,10,1, 56,5,1, 57,5,1},
        {507,5,1, 509,5,1, 515,5,1, 547,5,1},
    }

    local olvl = calcRechargeLevel(lvls, ototal)
    local nlvl = calcRechargeLevel(lvls, ntotal)

    if nlvl == 0 or olvl == nlvl then
        return
    end

    for k = olvl+1, nlvl do
        local title = string.format(msg_100, lvls[k])
        local ctx = string.format(msg_101, lvls[k])
        sendItemPackageMail(player, title, ctx, items[k]);
    end
end

function sendRechargeMails3(player, ototal, ntotal)
    local lvls = {
        99,199,399,699,1099,1599,2199,2899,3699,4599,5599,8999,15999,26999,42999,64999,99999,
    }
    local items = {
        {503,2,1, 516,2,1},
        {514,1,1, 57,2,1},
        {500,3,1, 56,3,1},
        {505,2,1, 15,6,1},
        {512,1,1, 513,1,1},
        {508,2,1, 506,3,1},
        {33,2,1, 515,3,1, 507,2,1},
        {503,2,1, 514,3,1},
        {509,1,1, 507,1,1},
        {8000,3,1, 549,3,1, 509,2,1},
        {1528,1,1},
        {509,1,1, 507,1,1},
        {509,2,1, 507,1,1, 515,1,1},
        {509,2,1, 507,2,1, 515,1,1},
        {509,2,1, 507,2,1, 515,2,1},
        {509,3,1, 507,3,1, 515,2,1},
        {509,3,1, 507,3,1, 515,3,1},
    }

    local olvl = calcRechargeLevel(lvls, ototal)
    local nlvl = calcRechargeLevel(lvls, ntotal)

    if nlvl == 0 or olvl == nlvl then
        return
    end

    for k = olvl+1, nlvl do
        local title = string.format(msg_100, lvls[k])
        local ctx = string.format(msg_101, lvls[k])
        sendItemPackageMail(player, title, ctx, items[k]);
    end
end

function sendRechargeMails4(player, ototal, ntotal)
    local lvls = {
        99,199,399,699,1099,1599,2199,2899,3699,4599,5599,8999,15999,26999,42999,64999,99999,
    }
    local items = {
        {503,2,1, 514,1,1},
        {500,3,1, 56,6,1, 57,2,1},
        {503,2,1, 56,4,1, 57,5,1},
        {512,6,1, 8000,4,1},
        {516,3,1, 512,2,1},
        {5025,1,1, 56,5,1},
        {503,5,1, 56,6,1, 57,2,1},
        {515,2,1, 56,6,1, 57,2,1},
        {515,2,1, 56,6,1, 57,5,1},
        {515,2,1, 56,6,1, 57,6,1},
        {549,2,1, 56,6,1, 57,6,1},
        {515,5,1, 30,10,1, 1528,2,1},
        {507,5,1, 509,5,1, 515,5,1, 547,5,1},
        --{509,2,1, 507,2,1, 515,1,1},
        --{509,2,1, 507,2,1, 515,2,1},
        --{509,3,1, 507,3,1, 515,2,1},
        --{509,3,1, 507,3,1, 515,3,1},
    }

    local olvl = calcRechargeLevel(lvls, ototal)
    if ntotal > 15999 then
        ntotal = 15999
    end
    local nlvl = calcRechargeLevel(lvls, ntotal)

    if nlvl == 0 or olvl == nlvl then
        return
    end

    for k = olvl+1, nlvl do
        local title = string.format(msg_107, lvls[k])
        local ctx = string.format(msg_108, lvls[k])
        sendItemPackageMail(player, title, ctx, items[k]);
    end
end

function sendRechargeMails5(player, ototal, ntotal)
    local lvls = {
        99,199,399,699,1099,1599,2199,2899,3699,4599,5599,8999,15999,26999,42999,64999,99999,
    }
    local items = {
        {500,2,1},
        {514,2,1},
        {503,2,1},
        {516,2,1},
        {515,1,1},
        {515,2,1},
        {515,3,1},
    }

    local olvl = calcRechargeLevel(lvls, ototal)
    if ntotal > 2199 then
        ntotal = 2199
    end
    local nlvl = calcRechargeLevel(lvls, ntotal)

    if nlvl == 0 or olvl == nlvl then
        return
    end

    for k = olvl+1, nlvl do
        local title = string.format(msg_109, lvls[k])
        local ctx = string.format(msg_110, lvls[k])
        sendItemPackageMail(player, title, ctx, items[k]);
    end
end

function sendRechargeMails6(player, ototal, ntotal)
    local lvls = {
        10,99,199,399,699,1099,1599,2199,2899,3699,4599,5599,8999,15999,26999,42999,64999,
    }
    local items = {
        {1325,1,1, 551,2,1},
        {503,2,1, 516,2,1},
        {8000,1,1, 551,2,1},
        {517,2,1, 1325,1,1},
        {505,2,1, 15,6,1},
        {512,1,1, 513,1,1},
        {1528,1,1, 1325,1,1},
        {515,3,1, 509,2,1},
        {503,2,1, 516,2,1},
        {1325,2,1, 515,1,1},
        {517,3,1, 549,2,1, 509,2,1},
        {1528,2,1},
        {1325,5,1, 516,3,1},
        {1528,2,1, 549,2,1, 515,2,1},
        {509,2,1, 507,2,1, 515,3,1, 1325,2,1},
        {509,2,1, 507,2,1, 515,3,1, 1528,5,1},
        {509,3,1, 507,3,1, 515,5,1, 503,10,1},
    }

    local olvl = calcRechargeLevel(lvls, ototal)
    local nlvl = calcRechargeLevel(lvls, ntotal)

    if nlvl == 0 or olvl == nlvl then
        return
    end

    for k = olvl+1, nlvl do
        local title = string.format(msg_100, lvls[k])
        local ctx = string.format(msg_101, lvls[k])
        sendItemPackageMail(player, title, ctx, items[k]);
    end
end

function sendRechargeMails(player, ototal, ntotal)
    --sendRechargeMails1(player, ototal, ntotal)
    --[[local start = { ['year'] = 2012, ['month'] = 7, ['day'] = 25, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(start)
    if os.time() >= s then
        sendRechargeMails3(player, ototal, ntotal)
    else
        sendRechargeMails2(player, ototal, ntotal)
    end--]]
    --if getRechargeActive() then
    --    sendRechargeMails4(player, ototal, ntotal)
    --end
    --if getRechargeActive3366() then
    --    sendRechargeMails5(player, ototal, ntotal)
    --end
    sendRechargeMails6(player, ototal, ntotal)
end

function onEquipForge(player, id, onums)
    local start = { ['year'] = 2012, ['month'] = 7, ['day'] = 5, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(start)
    local e = s + 7 * 86400
    local n = os.time()

    if n < s or n >= e then
        return
    end

    local items = {
        {500,3,1},
        {501,3,1, 549,1,1, 30,1,1, 9076,1,1},
        {30,3,1, 9076,2,1, getRandOEquip(player:GetLev()),1,1},
    }

    local item = items[onums]
    if item == nil then
        return
    end

    sendItemPackageMail(player, msg_103, msg_103, item);
end

function onSoSoMapAward(player, off)
    if off >= 7 then
        return 0
    end

    local items = {
        {503, 1},
        {514, 1},
        {56, 1},
        {57, 1},
        {11, 2},
        {518, 5},
        {509, 1},
    }

    local item = items[off+1]
    if item == nil then
        return 0 
    end

    print("item[1]" .. item[1])
    local package = player:GetPackage()
    package:Add(item[1], item[2], true)
    return 1
end


function sendConsumeMails1(player, ototal, ntotal)
    local lvls = {
        100,300,700,1300,2100,3100,4300,5700,7300,9100,11100,17900,31900,53900,85900,129900,199900,
    }

    local items = {
        {500,2,1, 514,2,1, 57,2,1, 56,2,1, 15,2,1},
        {514,1,1, 57,1,1},
        {500,1,1, 56,1,1},
        {512,1,1, 513,1,1, 503,1,1, 516,1,1, 515,1,1},
        {508,2,1, 506,2,1},
        {8000,2,1, 33,3,1, 515,1,1, 509,2,1},
        {505,2,1, 15,3,1},
        {503,2,1, 514,3,1},
        {509,1,1, 507,1,1},
        {9076,1,1, 509,1,1, 507,1,1, 515,1,1},
        {9076,2,1, 509,1,1, 507,1,1, 515,1,1},
        {9076,3,1, 509,1,1, 507,1,1, 515,1,1},
        {9076,5,1, 509,1,1, 507,1,1, 515,1,1},
        {9076,6,1, 509,2,1, 507,2,1, 515,2,1},
        {9076,8,1, 509,2,1, 507,2,1, 515,2,1},
        {9076,9,1, 509,2,1, 507,2,1, 515,2,1},
        {9076,10,1, 509,2,1, 507,2,1, 515,2,1},
    }

    local olvl = calcRechargeLevel(lvls, ototal)
    local nlvl = calcRechargeLevel(lvls, ntotal)

    if nlvl == 0 or olvl == nlvl then
        return
    end

    for k = olvl+1, nlvl do
        if lvls[k] == nil or items[k] == nil then
            return
        end
        local title = string.format(msg_105, lvls[k])
        local ctx = string.format(msg_106, lvls[k])
        sendItemPackageMail(player, title, ctx, items[k]);
    end
end

function sendConsumeMails2(player, ototal, ntotal)
    local lvls = {
        100,300,700,1300,2100,3100,4300,5700,7300,9100,11100,17900,25900,35900,55900,
    }

    local items = {
        {516,2,1, 503,2,1},
        {500,1,1, 517,1,1},
        {15,2,1},
        {547,2,1, 515,1,1, 1325,2,1},
        {505,2,1, 512,2,1, 1528,1,1},
        {8000,2,1, 516,1,1, 515,1,1, 509,2,1},
        {513,2,1, 9082,6,1},
        {503,2,1, 1325,1,1},
        {509,1,1, 507,1,1},
        {1325,2,1, 1528,2,1, 515,2,1},
        {9076,3,1, 515,1,1},
        {9022,5,1, 549,2,1, 515,2,1},
        {9022,5,1, 515,3,1, 1325,2,1, 1528,2,1},
        {9076,5,1, 515,5,1, 1325,1,1, 1528,1,1},
        {9076,15,1},
    }

    local olvl = calcRechargeLevel(lvls, ototal)
    local nlvl = calcRechargeLevel(lvls, ntotal)

    if nlvl == 0 or olvl == nlvl then
        return
    end

    for k = olvl+1, nlvl do
        if lvls[k] == nil or items[k] == nil then
            return
        end
        local title = string.format(msg_105, lvls[k])
        local ctx = string.format(msg_106, lvls[k])
        sendItemPackageMail(player, title, ctx, items[k]);
    end
end

function sendConsumeMails(player, ototal, ntotal)
    --sendConsumeMails1(player, ototal, ntotal);
    sendConsumeMails2(player, ototal, ntotal);
end

local awardPool = {
    {{29, 1}, {29, 2}, {29, 3}, {55, 1}, {502, 1}, {510, 1}, {499, 5}},
    {{56, 1}, {500, 1}, {499, 10}},
    {{511, 3}, {505, 1}},
    {{503, 1}, {514, 2}},
    {{515, 1}, {509, 1}, {507, 1}}
};

function onGetAthlRandomMaxValue(diffculty)
    if diffculty == 0 or diffculty > 5 then
        return 0
    end
    return #awardPool[diffculty]
end

function onGetAthlRandomAward(diffculty, opt)
    if diffculty == 0 or diffculty > 5 then
        return 0
    end
    opt = opt + 1
    if opt > #awardPool[diffculty] then
        return 0
    end
    local award = awardPool[diffculty][opt][1] * 65536 + awardPool[diffculty][opt][2]
    return award
end

function onGetAthlRandomDiffculty()
    local prob = {8580, 9580, 9880, 9980, 10000}
    local p
    p = math.random(1, 10000)
    local i = 1
    for n = 1, #prob do
        if p <= prob[n] then
            i = n
            break
        end
    end
    return i
end


-- 1:聊天 2:牵手 3:险境 4:喜鹊 5:名胜 6:心动 7:神坛
function onRoamingQueqiao(player, pos)
    local roamPlace = {
     -- 1  2  3  4  5  6  7  8
        2, 3, 5, 1, 3, 2, 2, 6,
        5, 1, 4, 2, 6, 3, 5, 2,
        3, 2, 5, 6, 3, 2, 1, 7
    }

    local eventItem = {
        {{502, 2, 10}, {510, 1, 10}, {29, 10, 10}},
        {{56, 1, 20}, {500, 1, 20}, {57, 1, 20}},
        {{511, 2, 20}, {512, 1, 30}, {517, 1, 30}},
        {{9122, 1, 10}, {9122, 1, 10}, {9122, 1, 10}},
        {{503, 1, 30}, {514, 1, 30}, {501, 1, 30}},
        {{509, 1, 40}, {1528, 1, 40}, {1325, 1, 40}},
        {{1647, 1, 50}, {1648, 1, 50}, {1649, 1, 50}},
    }

    step = math.random(1, 3)
    pos2 = pos + step
    if pos2 > 24 then
        pos2 = pos2 - 24
    end

    local package = player:GetPackage()
    i = roamPlace[pos2]
    j = math.random(1, 3)

    package:Add(eventItem[i][j][1], eventItem[i][j][2], true, true, 32)
    player:lastQueqiaoAwardPush(eventItem[i][j][1], eventItem[i][j][2]);
    player:postRoamResult(pos2, j, eventItem[i][j][3]);

    return pos2;
end

function Qixi(player, lootlvl)
    if getQixi() then
        -- 喜鹊
        local package = player:GetPackage();
        package:AddItem(9122, 1, true, 0, 10);
    end
end


