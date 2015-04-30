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

function TgcEvent(player, _type)
    if not getTgcEvent() then
        return
    end
    local package = player:GetPackage();
    if _type == 0 and player:GetVar(200) == 0 then
        sendItemPackageMail(player, msg_112, msg_112, {9187,1,1})
        player:SetVar(200, 1)
    elseif _type == 1 and player:GetVar(201)  == 0 and package:GetRestPackageSize() >= 1 then
        package:AddItem(9184, 1, true)
        player:SetVar(201, 1)
    elseif _type == 2 and player:GetVar(202) == 0 and package:GetRestPackageSize() >= 1 then
        package:AddItem(9186, 1, true)
        player:SetVar(202, 1)
    elseif _type == 3 and player:GetVar(203) == 0 and package:GetRestPackageSize() >= 1 then
        package:AddItem(9185, 1, true)
        player:SetVar(203, 1)
    end
end

function _9215Act(player, _type)
    if not get9215Act() then
        return
    end
    local package = player:GetPackage();
    if _type == 0 then
        package:AddItem(9215, 1, true)
        player:luaUdpLog("huodong", "F_10000_10", "act")
    elseif _type == 1 then
        package:AddItem(9215, 2, true)
        player:luaUdpLog("huodong", "F_10000_11", "act")
    elseif _type == 2 then
        package:AddItem(9215, 4, true)
        player:luaUdpLog("huodong", "F_10000_12", "act")
    elseif _type == 3 then
        package:AddItem(9215, 6, true)
        player:luaUdpLog("huodong", "F_10000_14", "act")
    elseif _type == 5 then
        package:AddItem(9215, 5, true)
        player:luaUdpLog("huodong", "F_10000_13", "act")
    end
end

function _snowAct(player, _type)
    if not getSnowAct() then
        return
    end
    local package = player:GetPackage();
    if _type == 0 then
        package:AddItem(16019, 1, true)
    elseif _type == 1 then
        package:AddItem(16019, 2, true)
    elseif _type == 2 then
        package:AddItem(16019, 4, true)
    elseif _type == 3 then
        package:AddItem(16019, 6, true)
    elseif _type == 5 then
        package:AddItem(16019, 5, true)
    end
end


function _collectCardAct(player, _type)
    if not getCollectCardAct() then
        return
    end
    local package = player:GetPackage();
    package:Add(9415, 1, true)
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

    --if getJune() then
    --    onJune(player)
    --end
    if getJune1() then
        onJune1(player)
    end
    if getDuanWu() then
        onDuanWu(player)
    end
    if getJuly() then
        onJuly(player)
    end

    onWansheng(player)
    TgcEvent(player, 0)
    onJune_20130601(player)
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
    --[[
    if getFoolsDay() then
        onFoolsDay(player)
    end
    --]]
    if getMayDay() then
        onMayDay(player)
    end
    if getMayDay1() then
        onMayDay1(player)
    end
    --if getJune() then
    --    onJune(player)
    --end
    if getJune1() then
        onJune1(player)
    end
    if getJuly() then
        onJuly(player)
    end

    onJune_20130601(player)
end

function onDungeonWin(player, id, count, free)
    June(player, 0);
    Qixi(player, 0);
    Wansheng(player, lootlvl);
    Qingren(player, 0);
    fairyPetLoot(player, 0);
    GGLoot(player);
    FlyRoadLoot(player);

    if free == true then
        FoolBaoLoot(player,0);
        HappyFireLoot(player,0);
        WorldCupLoot(player,0);
        XCTJLoot(player,0);
        SurnameLegendLoot(player,0);
        FallActivity(player, 1)
        Guoqing(player, 0);
        ChingMingDay(player, 0)
    else
        FoolBaoLoot(player,2);
        HappyFireLoot(player,1);
        WorldCupLoot(player,1);
        XCTJLoot(player,1);
        SurnameLegendLoot(player,0);
        FallActivity(player, 2)
        Guoqing(player, 3);
        ChingMingDay(player, 2)
    end
    if getKillMonsterAct() then
        local package = player:GetPackage();
        package:Add(16049, 1, true)
    end
    if getTYSSTime() ~= 0 then
        local package = player:GetPackage();
        package:Add(9492, 1, true)
    end
    local lootlvl = 1;
    if  free then
        lootlvl = 0
    end
    sendWinReward(player, lootlvl, 3);
    TgcEvent(player, 3)
    local count = 0;
    if not free then
        count = 5
    end
    _9215Act(player, count);
    _snowAct(player, count);
    _collectCardAct(player, count);
    IceCreamLoot(player)
    RedBeanLoot(player)
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
function CompassAct(player, lootlvl)
    if getCompassAct() then
        if lootlvl > 3 then
            lootlvl = 0
        end

        local itemNum = {
            [0] = 1,
            [1] = 1,
            [2] = 2,
            [3] = 3,
        };

        local package = player:GetPackage();
        package:AddItem(497, itemNum[lootlvl], true);
    end
end

function Item9344Act(player, lootlvl)
    if getItem9344Act() then
        if lootlvl > 3 then
            lootlvl = 0
        end

        local itemNum = {
            [0] = 1,
            [1] = 1,
            [2] = 2,
            [3] = 3,
        };

        local package = player:GetPackage();
        package:AddItem(9344, itemNum[lootlvl], true);
    end
end

function IceCreamLoot(player)
    if getCoolSummer() then
        local package = player:GetPackage()
        package:AddItem(16052, 1, true)
    end
end

function RedBeanLoot(player)
    if getSeekingHer() then
        local package = player:GetPackage()
        package:AddItem(16054, 1, true)
    end
end

function Item9343Act(player, lootlvl)
    if getItem9343Act() then
        if lootlvl > 3 then
            lootlvl = 0
        end

        local itemNum = {
            [0] = 1,
            [1] = 1,
            [2] = 2,
            [3] = 3,
        };

        local package = player:GetPackage();
        package:AddItem(9343, itemNum[lootlvl], true);
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
        [8] = 9391,
        [9] = 9430,
        [10] = 9491,
        [11] = 9885,
    }

    local item = items[id]
    if item == nil then
        return
    end

    local package = player:GetPackage()
    if getNationalDayHigh() then
        package:Add(item, 2, true)
    else
        package:Add(item, 1, true)
    end
end

function sendWinReward(player, lootlvl, typeId)
    --return;
    local package = player:GetPackage();
    --[[local items = {{51,30,1}, {48,35,1}, {49,20,1}, {50,15,1}};
    local i = math.random(1, 100)
    local v = 0;
    for n = 1, #items do
        v = v + items[n][2]
        if i <= v then
            package:Add(items[n][1], items[n][3], true);
            break
        end
    end]]
    local actItems = {493, 494, 495};
    local startTm = { ['year'] = 2012, ['month'] = 10, ['day'] = 15, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local endTm = { ['year'] = 2012, ['month'] = 10, ['day'] = 22, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(startTm);
    local e = os.time(endTm);
    local n = os.time();
    if n >= s and n < e then
        if lootlvl==0 then
            package:Add(492, 1, true);
        elseif lootlvl==1 and typeId==3 then
            package:Add(495, 1, true);
        else
            package:Add(actItems[lootlvl], 1, true);
        end
    end
end

function onVipLevelAward(player, vipLevel)
    
    local itemCount = {3, 10, 20, 45, 100, 150, 200, 270, 360, 500, 650, 800, 950, 1300, 1800};

    local startTime = { ['year'] = 2013, ['month'] = 7, ['day'] = 5, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local endTime = { ['year'] = 2013, ['month'] = 7, ['day'] = 12, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(startTime);
    local e = os.time(endTime);
    local n = os.time();

    if vipLevel < 1 and vipLevel > 15 then
        return 3;
    end

    if n >= s and n < e then
        local package = player:GetPackage();
        package:AddItem(503, itemCount[vipLevel], true, 0, 57);
        return 1;
    else
        return 2;
    end
end

function onCopyWin(player, id, floor, spot, lootlvl)
    if id >= 100 then
        return 
    end
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
    Wansheng(player, lootlvl);
    Qingren(player, lootlvl);
    fairyPetLoot(player, lootlvl);
    FoolBaoLoot(player,lootlvl);
    HappyFireLoot(player,lootlvl);
    WorldCupLoot(player,lootlvl);
    XCTJLoot(player,lootlvl);
    SurnameLegendLoot(player,lootlvl);
    Guoqing(player, lootlvl);
    LuckyDrawBox(player, id)
    ExJob(player, id, lootlvl)
    GGLoot(player);
    FlyRoadLoot(player);
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
    if getKillMonsterAct() then
        local package = player:GetPackage();
        package:Add(16049, 1, true)
    end
    sendWinReward(player, lootlvl, 1);
    TgcEvent(player, 1)
    _9215Act(player, lootlvl);
    _snowAct(player, lootlvl);
--    CompassAct(player, lootlvl);
    Item9344Act(player, lootlvl);
    Item9343Act(player, lootlvl);
    IceCreamLoot(player)
    RedBeanLoot(player)
    player:AddZRYJCount(20); -- 逐日印记
    _collectCardAct(player, lootlvl);
    if getTYSSTime() ~= 0 then
        local package = player:GetPackage();
        package:Add(9492, 1, true)
    end

end

function onFrontMapFloorWin(player, id, spot, lootlvl)
end

function onFrontMapWin(player, id, spot, lootlvl)
    print(id)
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
    Wansheng(player, lootlvl);
    Qingren(player, lootlvl);
    Guoqing(player, lootlvl);
    fairyPetLoot(player, lootlvl);
    FoolBaoLoot(player,lootlvl);
    HappyFireLoot(player,lootlvl);
    WorldCupLoot(player,lootlvl);
    XCTJLoot(player,lootlvl);
    SurnameLegendLoot(player,0);
    IceCreamLoot(player)
    RedBeanLoot(player)
    GGLoot(player);
    FlyRoadLoot(player);
    if lootlvl == 0 then
        FallActivity(player, 1)
    else
        FallActivity(player, lootlvl)
    end
    if getKillMonsterAct() then
        local package = player:GetPackage();
        package:Add(16049, 1, true)
    end
    if getTYSSTime() ~= 0 then
        local package = player:GetPackage();
        package:Add(9492, 1, true)
    end
    --if id >= 9 then
    --    local package = player:GetPackage();
    --    print("XX")
    --    package:Add(554, 1, true)
    --end
    sendWinReward(player, lootlvl, 2);
    TgcEvent(player, 2)
    _9215Act(player, lootlvl);
    _snowAct(player, lootlvl);
--    CompassAct(player, lootlvl);
    Item9344Act(player, lootlvl);
    Item9343Act(player, lootlvl);
    player:AddHYYJCount(20); -- 皓月印记
    _collectCardAct(player, lootlvl);
end

function onDropAwardAct(player, param)
    Qixi(player, 0);
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

    if package:GetRestPackageSize() < needsize or package:GetRestPackageSize(3) < needsize then
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

    if lootlvl == nil or lootlvl > 3 then
        lootlvl = 0
    end

    local itemNum = {
            [0] = 1,
            [1] = 1,
            [2] = 2,
            [3] = 3,
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
            --[11] = "3366",
            --[12] = "官网",
            [17] = "逍遥游戏平台",
        }
        local titles = {
            [1] = pfnames[1].."登陆奖励",
            [2] = pfnames[2].."登陆奖励",
            [4] = pfnames[4].."登陆奖励",
            [10] = pfnames[10].."登陆奖励",
            --[11] = pfnames[11].."登陆奖励",
            --[12] = pfnames[12].."登陆奖励",
            [17] = pfnames[17].."登陆奖励",
        }
        local ctxs = {
            [1] = "每日通过"..pfnames[1].."登陆《蜀山传奇》都可以获得【"..pfnames[1].."登陆礼包】 \n登陆礼包每日只能领取一次\n<font color=\"#FF0000\">黄钻贵族通过"..pfnames[1].."登陆，可以领取黄钻每日礼包，内容包括：太乙真金、高级挂机加速符、银票、一级宝石、七星元木等。请在游戏主界面领取。</font><font color=\"#3399FF\"><u><a href=\"event:openYellow\">开通黄钻</a></u></font>",
            [2] = "每日通过"..pfnames[2].."登陆《蜀山传奇》都可以获得【"..pfnames[2].."登陆礼包】 \n登陆礼包每日只能领取一次\n<font color=\"#FF0000\">黄钻贵族通过"..pfnames[2].."登陆，可以领取黄钻每日礼包，内容包括：太乙真金、高级挂机加速符、银票、一级宝石、七星元木等。请在游戏主界面领取。</font><font color=\"#3399FF\"><u><a href=\"event:openYellow\">开通黄钻</a></u></font>",
            [4] = "每日通过"..pfnames[4].."登陆《蜀山传奇》都可以获得【"..pfnames[4].."登陆礼包】 登陆礼包每日只能领取一次\n<font color=\"#FF0000\">QQ会员贵族通过"..pfnames[4].."登陆，可以领取QQ会员每日礼包，内容包括：太乙真金、高级挂机加速符、银票、一级宝石、七星元木等。请在游戏主界面领取。</font><font color=\"#3399ff\"><u><a href=\"event:openQQVip\">开通QQ会员</a></u></font>",
            [10] = "每日通过"..pfnames[10].."登陆《蜀山传奇》都可以获得【"..pfnames[10].."登陆礼包】 \n登陆礼包每日只能领取一次\n<font color=\"#FF0000\">蓝钻贵族通过"..pfnames[10].."登陆，可以领取蓝钻每日礼包，内容包括：太乙真金、高级挂机加速符、银票、一级宝石、七星元木等。请在游戏主界面领取。</font><font color=\"#3399ff\"><u><a href=\"event:openBlue\">开通蓝钻</a></u></font>",
            --[11] = "每日通过"..pfnames[11].."登陆《蜀山传奇》都可以获得【"..pfnames[11].."登陆礼包】 登陆礼包每日只能领取一次\n<font color=\"#FF0000\">蓝钻贵族通过"..pfnames[11].."登陆，可以领取蓝钻每日礼包，内容包括：太乙真金、高级挂机加速符、银票、一级宝石、七星元木等。请在游戏主界面领取。</font><font color=\"#00FFFF\"><u><a href=\"event:openYellow\">开通蓝钻</a></u></font>",
            --[12] = "每日通过"..pfnames[12].."登陆《蜀山传奇》都可以获得【"..pfnames[12].."登陆礼包】 登陆礼包每日只能领取一次\n<font color=\"#FF0000\">QQ会员贵族通过"..pfnames[12].."登陆，可以领取QQ会员每日礼包，内容包括：太乙真金、高级挂机加速符、银票、一级宝石、七星元木等。请在游戏主界面领取。</font><font color=\"#FF0000\"><u><a href=\"event:openQQVip\">开通QQ会员</a></u></font>",
            [17] = "每日通过"..pfnames[17].."登陆《蜀山传奇》都可以获得【"..pfnames[17].."登陆礼包】 \n登陆礼包每日只能领取一次",
        }
        local pkgs = {
            [1] = {469,1,1},
            [2] = {470,1,1},
            [4] = {448,1,1},
            [10] = {468,1,1},
            --[11] = {448,1,1},
            -- [12] = {448,1,1},
            [17] = {9193,1,1},
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
    if true --[[isFBVersion()--]] then
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
    if false --[[isFBVersion()--]] then
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

function onUseMDSoul(player, _type, _v)
    if _type == 0 or _type > 3  or _v > 2 then
        return 0
    end
    local package = player:GetPackage();
    local items1 = {
        {9000,47,509,507,515,509,507,515,},
        {503,514,506,508,517,512,501,513,},
        {1411,133,503,511,9000,9076,514,516}
    }
    local items2 = {
        {9000,47,509,507,515,509,507,515,},
        {503,514,506,508,517,512,501,513,},
        {1411,133,503,511,9000,9076,514,516}
    }
    local items = items1;
    if _v == 2 then
        items = items2;
    end

    local broad = {
        [1] = {0,0,0,0,1,1,0,0},
        [2] = {0,0,0,0,1,1,0,0}
    };
    local count = {
        [1] = {1,1,2,1,2,2,1,2},
        [2] = {2,2,2,2,3,3,1,3}
    };

    if _type == 3 then
        local prob = {
            [1] = {2400,4800,5430,7830,8080,8240,9600,10000},
            [2] = {2125,4250,4950,7075,7435,7565,9690,10000}
        };
        local p = math.random(1,10000)
        local i = 1
        for n = 1,#prob[_v] do
            if p <= prob[_v][n] then
                i = n
                break
            end
        end
        if broad[_v][i] == 1 then
            Broadcast(0x27, "御风雷之变化，".."[p:"..player:getCountry()..":"..player:getPName().."]成功显罗盘秘宝于世，获得了[4:"..items[_type][i].."]x"..count[_v][i])
        end
        package:Add(items[_type][i], count[_v][i], true, true);
        player:appendCompassItem(items[_type][i], count[_v][i],_v);
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
    if false --[[isFBVersion()--]] then
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

function onJune_20130601(player)
    local startTm = { ['year'] = 2013, ['month'] = 6, ['day'] = 01, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local endTm = { ['year'] = 2013, ['month'] = 6, ['day'] = 04, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(startTm);
    local e = os.time(endTm);
    local n = os.time();
    if n < s or n > e then
        return
    end

    local lvl = player:GetLev()
    if lvl < 40 then
        return
    end

    if player:GetVar(111) == 0 then
        sendItemPackageMail(player, msg_51, msg_52, {1765,1,1});
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
        {134,1,1},
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
        {515,5,1, 30,10,1, 134,2,1},
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
        {134,1,1, 1325,1,1},
        {515,3,1, 509,2,1},
        {503,2,1, 516,2,1},
        {1325,2,1, 515,1,1},
        {517,3,1, 549,2,1, 509,2,1},
        {134,2,1},
        {1325,5,1, 516,3,1},
        {134,2,1, 549,2,1, 515,2,1},
        {509,2,1, 507,2,1, 515,3,1, 1325,2,1},
        {509,2,1, 507,2,1, 515,3,1, 134,5,1},
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

function sendRechargeMails7(player, ototal, ntotal)
    local lvls = {
        10,1000,1999,3599,5599,7999,12999,20999,35999,
    }
    local items = {
        {1325,1,1, 134,1,1},
        {503,5,1, 516,5,1},
        {1325,2,1, 515,1,1},
        {9088,10,1, 515,1,1},
        {134,5,1, 1325,5,1},
        {1325,3,1, 503,5,1},
        {515,5,1, 503,10,1},
        {515,5,1, 1325,3,1, 134,3,1},
        {515,8,1, 1325,5,1, 134,5,1},
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

function sendRechargeMails_2012_09_13(player, ototal, ntotal)
    local lvls = {
        10,50,99,199,399,699,999,
    }
    local items = {
        {516,1,1},
        {503,1,1, 15,2,1},
        {514,2,1, 512,2,1},
        {516,1,1, 547,2,1},
        {549,1,1},
        {509,1,1, 500,1,1},
        {9017,1,1, 9019,1,1},
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

function sendRechargeMails_2012_09_22(player, ototal, ntotal)
    local lvls = {
        199,399,599,999,1299,1599,1999,2999,3999,4999,5999,6999,7999,8999,9999
    }
    local items = {
        {514,3,1},
        {503,3,1},
        {516,3,1},
        {514,5,1, 503,5,1, 516,5,1},
        {0xA000,80,1},
        {0xA000,100,1},
        {507,2,1, 509,2,1, 30,10,1, 0xA000,100,1},
        {0xA000,120,1},
        {0xA000,140,1},
        {0xA000,160,1},
        {0xA000,180,1},
        {0xA000,200,1},
        {0xA000,220,1},
        {0xA000,240,1},
        {515,10,1, 507,10,1, 509,10,1, 30,10,1, 0xA000,300,1},
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

function sendRechargeMails_2012_09_27(player, ototal, ntotal)
    local lvls = {
        10,99,199,399,699,1099,1599,2199,2899,3699,4599,5599,8999,15999,26999,42999,64999,
    }
    local items = {
        {1325,1,1, 551,2,1},
        {503,2,1, 516,2,1},
        {8000,1,1, 551,2,1},
        {517,2,1, 1325,1,1},
        {505,2,1, 15,6,1},
        {512,1,1, 513,1,1, 9076,1,1},
        {134,1,1, 1325,1,1, 9076,1,1},
        {515,3,1, 509,2,1, 9076,1,1},
        {503,2,1, 516,2,1, 9076,1,1},
        {1325,2,1, 515,1,1, 9076,1,1},
        {517,3,1, 549,2,1, 509,2,1, 9076,1,1},
        {134,2,1, 9076,1,1},
        {1325,5,1, 516,3,1, 9076,1,1},
        {134,2,1, 549,2,1, 515,2,1, 9076,2,1},
        {509,2,1, 507,2,1, 515,3,1, 9076,3,1},
        {509,2,1, 507,2,1, 515,3,1, 9076,3,1},
        {509,3,1, 507,3,1, 515,5,1, 9076,5,1},
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

function sendRechargeMails_2012_10_05(player, ototal, ntotal)
    local lvls = {
        10,99,199,399,699,999,
    }
    local items = {
        {1325,1,1},
        {503,2,1, 15,2,1},
        {516,1,1, 547,2,1},
        {507,1,1},
        {509,1,1, 33,1,1},
        {134,1,1, 515,1,1},
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

function sendRechargeMails_2012_10_12(player, ototal, ntotal)
    local lvls = {
        10,99,199,399,699,1099,1599,2199,2899,3699,4599,5599,8999,15999,26999,42999,
    }
    local items = {
        {551,1,1, 134,1,1},
        {33,1,1, 515,1,1},
        {8000,1,1, 551,1,1},
        {515,2,1},
        {512,2,1, 15,2,1},
        {515,1,1, 9076,1,1},
        {134,1,1, 9076,1,1},
        {515,3,1, 509,1,1, 9076,1,1},
        {503,2,1, 9076,1,1},
        {134,2,1,1325,2,1,  9076,1,1},
        {549,1,1, 509,1,1, 9076,1,1},
        {134,2,1, 9076,1,1},
        {1325,5,1, 9076,2,1},
        {134,2,1, 515,4,1, 9076,2,1},
        {9076,10,1},
        {9076,10,1, 509,5,1, 507,5,1},
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

function sendRechargeMails_2012_10_16(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {8000,1,1, 551,1,1},
        {134,2,1, 1325,2,1},
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

function sendRechargeMails_2012_10_17(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,
    }
    local items = {
        {134,1,1},
        {503,2,1},
        {516,2,1, 517,2,1},
        {515,2,1, 509,2,1},
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

function sendRechargeMails_2012_10_18(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,
    }
    local items = {
        {503,2,1},
        {551,2,1},
        {134,2,1, 1325,2,1},
        {515,4,1},
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

function sendRechargeMails_2012_10_19(player, ototal, ntotal)
    local lvls = {
        10,99,399,799,1299,1899,2599,3399,4299,5299,6599,8899,12599,17599,26999,39999,
    }
    local items = {
        {134,1,1},
        {503,2,1},
        {8000,2,1, 551,1,1},
        {515,1,1},
        {509,2,1, 1325,1,1},
        {516,3,1, 500,3,1},
        {549,1,1, 513,2,1},
        {515,2,1, 1325,1,1},
        {9076,3,1},
        {134,2,1, 1325,2,1, 548,100,1},
        {507,2,1, 509,2,1},
        {9076,5,1},
        {9022,3,1},
        {9076,10,1},
        {515,5,1, 134,5,1, 9177,5,1},
        {9022,5,1, 1325,20,1},
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


function _sendRechargeMails(player, ototal, ntotal, lvls, items)
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

function sendRechargeMails_2012_10_23(player, ototal, ntotal)
    local lvls = {
        10,99,199,399,699,1099,1599,2199,2899,3699,4599,5599,7999,10999,14999,19999,
    }
    local items = {
        {516,1,1},
        {503,2,1, 514,1,1},
        {500,3,1},
        {1325,1,1},
        {512,2,1, 8000,2,1},
        {515,2,1},
        {513,2,1, 547,2,1},
        {134,1,1, 503,2,1},
        {515,1,1, 509,1,1},
        {549,1,1, 516,3,1},
        {509,1,1, 507,1,1},
        {9076,2,1},
        {515,3,1},
        {507,2,1, 509,2,1},
        {9076,5,1},
        {515,3,1, 1325,3,1, 134,3,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_10_26(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,
    }
    local items = {
        {503,1,1},
        {515,1,1},
        {551,2,1},
        {509,2,1, 507,2,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_10_27(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,
    }
    local items = {
        {1325,1,1},
        {503,2,1},
        {516,2,1, 507,1,1},
        {515,2,1, 509,2,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_10_28(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,
    }
    local items = {
        {503,2,1},
        {516,2,1},
        {1325,1,1, 134,1,1},
        {515,3,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_10_30(player, ototal, ntotal)
    local lvls = {
        100,299,699,1199,1899,2699,3699,4999,6599,8599,11999,15999,21999,29999,39999
    }
    local items = {
        {503,2,1},
        {500,1,1, 517,1,1},
        {15,2,1},
        {547,2,1, 515,1,1, 1325,2,1},
        {505,2,1, 512,2,1, 134,1,1},
        {516,1,1, 515,1,1, 509,2,1},
        {513,2,1, 551,2,1},
        {503,2,1, 134,2,1},
        {507,2,1, 509,2,1},
        {1325,2,1, 134,2,1, 515,2,1},
        {9076,3,1},
        {549,2,1, 515,2,1},
        {9076,5,1},
        {515,5,1, 134,5,1},
        {9076,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_11_03(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {515,3,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_11_04(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_11_05(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,
    }
    local items = {
        {503,1,1},
        {516,2,1},
        {509,1,1, 507,1,1},
        {515,3,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_11_06(player, ototal, ntotal)
    local lvls = {
        10,99,199,399,699,1099,1599,2199,2899,3699,4599,5599,8999,15999,26999,42999,64999,
    }
    local items = {
        {1325,1,1, 551,2,1},
        {503,2,1, 516,2,1},
        {8000,1,1, 551,2,1},
        {517,2,1, 1325,1,1},
        {15,6,1},
        {515,1,1},
        {134,1,1, 1325,1,1},
        {515,3,1, 509,2,1},
        {503,2,1, 516,2,1},
        {1325,2,1, 515,1,1},
        {517,3,1, 549,2,1, 509,2,1},
        {134,2,1, 1325,2,1},
        {509,3,1, 507,3,1},
        {134,2,1, 549,2,1, 515,2,1},
        {509,2,1, 507,2,1, 515,3,1},
        {9076,6,1, 515,6,1, 134,6,1},
        {9076,8,1, 515,8,1, 1325,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_11_10(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {515,3,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_11_11(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_11_12(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,
    }
    local items = {
        {503,1,1},
        {516,2,1},
        {509,1,1, 507,1,1},
        {515,3,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_11_13(player, ototal, ntotal)
    local lvls = {
        10,99,199,399,699,1099,1599,2199,2899,3699,4599,5599,7999,10999,14999,19999,
    }
    local items = {
        {516,1,1},
        {503,2,1, 514,1,1},
        {500,3,1},
        {1325,1,1},
        {512,2,1, 8000,2,1},
        {515,2,1},
        {513,2,1, 547,2,1},
        {134,1,1, 503,2,1},
        {515,1,1, 509,1,1},
        {549,1,1, 516,3,1},
        {509,1,1, 507,1,1},
        {9076,2,1},
        {515,3,1},
        {509,2,1, 507,2,1},
        {9076,5,1},
        {515,3,1, 1325,3,1, 134,3,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_11_16(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {515,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_11_17(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {1325,33,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_11_18(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,5888,
    }
    local items = {
        {503,1,1},
        {516,2,1},
        {509,1,1, 507,1,1},
        {515,3,1},
        {9022,5,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_11_19(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {503,2,1},
        {516,2,1, 517,2,1},
        {515,2,1, 509,2,1},
        {9076,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_11_20(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,5888,
    }
    local items = {
        {503,2,1},
        {551,2,1},
        {134,2,1, 1325,2,1},
        {515,4,1},
        {134,33,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_11_21(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {1325,3,1, 134,3,1},
        {509,15,1, 507,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_11_22(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {515,3,1},
        {9076,10,1, 1325,10,1, 507,10,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_11_23(player, ototal, ntotal)
    local lvls = {
        10,99,199,399,699,1099,1599,2199,2899,3699,4599,5599,8999,13999,19999,26999,
    }
    local items = {
        {551,2,1},
        {503,2,1, 516,2,1},
        {8000,1,1, 551,2,1},
        {517,2,1, 1325,1,1},
        {505,2,1, 15,6,1},
        {512,1,1, 513,1,1},
        {134,1,1, 1325,1,1},
        {515,3,1, 509,2,1},
        {503,2,1, 516,2,1},
        {1325,2,1, 515,1,1},
        {517,3,1, 549,2,1, 509,2,1},
        {134,2,1},
        {1325,5,1, 516,3,1},
        {134,2,1, 549,2,1, 515,2,1},
        {509,1,1, 507,1,1, 515,3,1, 1325,2,1},
        {509,2,1, 507,2,1, 515,5,1, 134,5,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_11_27(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {549,1,1},
        {515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end
function sendRechargeMails_2012_11_28(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {1325,1,1, 134,1,1},
        {509,5,1, 507,5,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end
function sendRechargeMails_2012_11_29(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,5888,
    }
    local items = {
        {503,1,1},
        {516,2,1},
        {509,1,1, 507,1,1},
        {515,2,1},
        {1325,18,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end
function sendRechargeMails_2012_11_30(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {503,1,1},
        {515,1,1},
        {551,2,1},
        {509,2,1, 507,2,1},
        {134,18,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_08(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_09(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {1325,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_10(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,5888,
    }
    local items = {
        {503,1,1},
        {516,2,1},
        {509,1,1, 507,1,1},
        {515,3,1},
        {9022,5,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_11(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {503,2,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {9076,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_12(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,5888,
    }
    local items = {
        {503,2,1},
        {551,2,1},
        {1325,1,1, 134,1,1},
        {515,2,1},
        {134,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_13(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {503,2,1},
        {515,1,1},
        {1325,2,1, 134,2,1},
        {509,10,1, 507,10,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_14(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {515,3,1},
        {9076,10,1, 1325,10,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_15(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_16(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {1325,5,1, 515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_17(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,5888,
    }
    local items = {
        {503,1,1},
        {516,2,1},
        {509,1,1, 507,1,1},
        {515,3,1},
        {9022,5,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_18(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {503,2,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {9076,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_19(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,5888,
    }
    local items = {
        {503,2,1},
        {551,2,1},
        {1325,1,1, 134,1,1},
        {515,2,1},
        {134,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_20(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {503,2,1},
        {551,3,1},
        {1325,3,1},
        {134,5,1, 515,3,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_21(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {9076,3,1},
        {515,5,1, 1325,5,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_22(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_23(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {1325,5,1, 515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_24(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,5888,
    }
    local items = {
        {503,1,1},
        {516,2,1,},
        {509,1,1, 507,1,1},
        {515,3,1},
        {9022,5,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_25(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,5888,
    }
    local items = {
        {503,2,1},
        {551,2,1},
        {1325,1,1, 134,1,1},
        {515,2,1},
        {134,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_26(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {503,2,1},
        {551,3,1},
        {1325,3,1},
        {134,5,1, 515,3,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_27(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {9076,3,1},
        {515,5,1, 1325,5,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_28(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {1325,5,1, 515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_29(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {503,2,1},
        {551,3,1},
        {1325,3,1},
        {134,5,1, 515,3,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_30(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {9076,3,1},
        {515,5,1, 1325,5,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2012_12_31(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {1325,5,1, 515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_01(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_02(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,5888,
    }
    local items = {
        {503,2,1},
        {551,2,1},
        {1325,1,1, 134,1,1},
        {515,2,1},
        {134,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_03(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,5888,
    }
    local items = {
        {503,1,1},
        {516,2,1},
        {509,1,1, 507,1,1},
        {515,3,1},
        {9022,5,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_04(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {503,2,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {9076,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_05(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {515,3,1},
        {9076,10,1, 1325,10,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_06(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {503,2,1},
        {515,1,1},
        {1325,2,1, 134,2,1},
        {515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_07(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {503,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {134,10,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_19(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {503,2,1},
        {551,3,1},
        {1325,3,1},
        {134,5,1, 515,3,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_20(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {9076,3,1},
        {515,5,1, 1325,5,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_21(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {1325,5,1, 515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_22(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_23(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,5888,
    }
    local items = {
        {503,2,1},
        {551,2,1},
        {1325,1,1, 134,1,1},
        {515,2,1},
        {134,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_24(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,5888,
    }
    local items = {
        {503,1,1},
        {516,2,1},
        {509,1,1, 507,1,1},
        {515,3,1},
        {9022,5,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_25(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {503,2,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {9076,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_26(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {515,3,1},
        {9076,10,1, 1325,10,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_27(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {503,2,1},
        {515,1,1},
        {1325,2,1, 134,2,1},
        {515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_28(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {503,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {134,10,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_29(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {1325,5,1, 515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_30(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {503,2,1},
        {551,3,1},
        {1325,3,1},
        {134,5,1, 515,3,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_01_31(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {9076,3,1},
        {515,5,1, 1325,5,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_02_01(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {1325,5,1, 515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_02_02(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_02_03(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,5888,
    }
    local items = {
        {503,2,1},
        {551,2,1},
        {1325,1,1, 134,1,1},
        {515,2,1},
        {134,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_02_04(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,5888,
    }
    local items = {
        {503,1,1},
        {516,2,1},
        {509,1,1, 507,1,1},
        {515,3,1},
        {9022,5,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_02_05(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {503,2,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {9076,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_02_06(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {515,3,1},
        {9076,10,1, 1325,10,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_02_07(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {503,2,1},
        {515,1,1},
        {1325,2,1, 134,2,1},
        {515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_02_08(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {503,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {134,10,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_02_09_15(player, ototal, ntotal)
    local lvls = {
        20,99,399,799,1299,1899,2599,3399,4999,5999,7299,9599,12599,17599,26999,39999,
    }
    local items = {
        {134,1,1},
        {503,1,1},
        {56,2,1, 514,1,1},
        {515,1,1},
        {57,2,1, 513,2,1},
        {516,1,1, 500,3,1},
        {549,1,1},
        {1325,1,1, 503,1,1},
        {515,3,1},
        {1325,1,1, 134,1,1},
        {507,1,1, 509,1,1},
        {9076,3,1},
        {516,3,1, 515,1,1},
        {9076,5,1},
        {509,5,1, 507,5,1},
        {9076,10,1, 134,5,1, 1325,5,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_02_16(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {503,2,1},
        {551,3,1},
        {1325,3,1},
        {134,5,1, 515,3,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_02_17(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {9076,3,1},
        {515,5,1, 1325,5,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_02_18(player, ototal, ntotal)
    local lvls = {
        10,99,399,999,5888,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {1325,5,1, 515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_02_19(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {515,8,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_02_20(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,5888,
    }
    local items = {
        {503,2,1},
        {551,2,1},
        {1325,1,1, 134,1,1},
        {515,2,1},
        {134,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_02_21(player, ototal, ntotal)
    local lvls = {
        10,399,899,1599,5888,
    }
    local items = {
        {503,1,1},
        {516,2,1},
        {509,1,1, 507,1,1},
        {515,3,1},
        {9022,5,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_02_22(player, ototal, ntotal)
    local lvls = {
        10,199,599,1299,5888,
    }
    local items = {
        {134,1,1},
        {503,2,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {9076,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_09(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {1325,1,1},
        {503,2,1},
        {551,3,1},
        {134,5,1},
        {1325,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_10(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {9076,3,1},
        {134,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_11(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {516,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_12(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {9338,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {9338,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_13(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {503,2,1},
        {551,2,1},
        {1325,1,1, 134,1,1},
        {515,2,1},
        {503,35,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_14(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {9092,1,1},
        {516,2,1},
        {509,1,1, 507,1,1},
        {515,3,1},
        {9092,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_15(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {9093,1,1},
        {503,2,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {9093,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_16(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {500,5,1},
        {503,2,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {500,50,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_17(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {1325,1,1},
        {503,2,1},
        {551,3,1},
        {134,5,1},
        {1325,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_18(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {9076,3,1},
        {134,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_19(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {516,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_20(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {9338,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {9338,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_21(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {503,2,1},
        {551,2,1},
        {1325,1,1, 134,1,1},
        {515,2,1},
        {503,35,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_22(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {9092,1,1},
        {516,2,1},
        {509,1,1, 507,1,1},
        {515,3,1},
        {9092,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_23(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {9093,1,1},
        {503,2,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {9093,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end
function sendRPRechargeMails_1(player, ototal, ntotal)
    local lvls = {
        10,599,1299,5888,9999,
    }
    local items = {
        {0xA000,50,1},
        {15,5,1},
        {516,2,1},
        {503,8,1},
        {509,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end
function sendRPRechargeMails_2(player, ototal, ntotal)
    local lvls = {
        10,599,1299,5888,9999,
    }
    local items = {
        {134,1,1},
        {0xA000,100,1},
        {500,5,1},
        {549,1,1},
        {507,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end
function sendRPRechargeMails_3(player, ototal, ntotal)
    local lvls = {
        10,599,1299,5888,9999,
    }
    local items = {
        {516,1,1},
        {515,2,1},
        {0xA000,180,1},
        {509,2,1, 507,2,1},
        {516,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end
function sendRPRechargeMails_4(player, ototal, ntotal)
    local lvls = {
        10,599,1299,5888,9999,
    }
    local items = {
        {134,1,1},
        {516,2,1},
        {0xA000,180,1},
        {134,2,1, 1325,2,1},
        {503,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end
function sendRPRechargeMails_5(player, ototal, ntotal)
    local lvls = {
        10,599,1299,5888,9999,
    }
    local items = {
        {134,1,1},
        {503,2,1},
        {0xA000,180,1},
        {509,2,1, 507,2,1},
        {515,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end
function sendRPRechargeMails_6(player, ototal, ntotal)
    local lvls = {
        10,599,1299,5888,9999,
    }
    local items = {
        {134,1,1},
        {503,2,1},
        {0xA000,180,1},
        {509,2,1, 507,2,1},
        {134,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end
function sendRPRechargeMails_7(player, ototal, ntotal)
    local lvls = {
        10,599,1299,5888,9999,
    }
    local items = {
        {134,1,1},
        {0xA000,100,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {515,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_24(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {500,5,1},
        {503,2,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {500,50,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_25(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {1325,1,1},
        {503,2,1},
        {551,3,1},
        {134,5,1},
        {1325,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_26(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {134,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {9076,3,1},
        {134,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_27(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {516,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_28(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {9338,1,1},
        {509,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {9338,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_29(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {503,2,1},
        {551,2,1},
        {1325,1,1, 134,1,1},
        {515,2,1},
        {503,35,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_30(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {9371,3,1},
        {516,2,1},
        {509,1,1, 507,1,1},
        {515,3,1},
        {9371,60,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_03_31(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {516,1,1},
        {503,2,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {516,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_04_14(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {500,5,1},
        {503,5,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {500,50,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_04_15(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {1325,1,1},
        {503,5,1},
        {551,3,1},
        {134,5,1},
        {1325,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_04_16(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {134,1,1},
        {503,5,1},
        {516,2,1, 507,1,1},
        {9076,3,1},
        {134,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_04_17(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {516,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_04_18(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {9338,1,1},
        {515,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {9338,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_04_19(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {9371,3,1},
        {515,1,1},
        {509,1,1, 507,1,1},
        {515,3,1},
        {9371,60,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_04_20(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {516,1,1},
        {515,1,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {516,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_04_21(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {9088,5,1},
        {503,5,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {500,50,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_04_22(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {9088,5,1},
        {503,5,1},
        {551,3,1},
        {134,5,1},
        {1325,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_04_23(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {9088,5,1},
        {503,5,1},
        {516,2,1, 507,1,1},
        {9076,3,1},
        {134,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_04_24(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {9088,5,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {516,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_04_25(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {9088,5,1},
        {515,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {9338,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_04_26(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {9088,5,1},
        {515,1,1},
        {509,1,1, 507,1,1},
        {515,3,1},
        {9371,60,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_04_27(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {9088,5,1},
        {515,1,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {516,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_04_28(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {9367,3,1, 9369,3,1},
        {503,5,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {500,50,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_04_29(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {9367,3,1, 9369,3,1},
        {503,5,1},
        {551,3,1},
        {134,5,1},
        {1325,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_04_30(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {9367,3,1, 9369,3,1},
        {503,5,1},
        {516,2,1, 507,1,1},
        {9076,3,1},
        {134,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_01(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {9367,3,1, 9369,3,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {516,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_02(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {9367,3,1, 9369,3,1},
        {515,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {9338,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_03(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {9367,3,1, 9369,3,1},
        {515,1,1},
        {509,1,1, 507,1,1},
        {515,3,1},
        {9371,60,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_04(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {9367,3,1, 9369,3,1},
        {515,1,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {516,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_05(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {0xA000,30,1},
        {503,5,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {500,50,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_06(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {9367,3,1, 9369,3,1},
        {503,5,1},
        {551,3,1},
        {134,5,1},
        {1325,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_07(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {8555,1,1},
        {503,5,1},
        {516,2,1, 507,1,1},
        {9076,3,1},
        {134,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_08(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {5034,1,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {516,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_09(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {9088,3,1},
        {515,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {9338,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_10(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {78,1,1},
        {515,1,1},
        {509,1,1, 507,1,1},
        {515,3,1},
        {9371,60,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_11(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {465,2,1},
        {515,1,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {516,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_12(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {0xA000,30,1},
        {503,5,1},
        {516,2,1, 517,2,1},
        {509,2,1},
        {500,50,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_13(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {0xA000,30,1},
        {503,5,1},
        {551,3,1},
        {134,5,1},
        {1325,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_14(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {0xA000,30,1},
        {503,5,1},
        {516,2,1, 507,1,1},
        {9076,3,1},
        {134,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_15(player, ototal, ntotal)
    local lvls = {
        10,99,599,999,5999,
    }
    local items = {
        {0xA000,30,1},
        {515,1,1},
        {503,2,1},
        {509,2,1, 507,2,1},
        {516,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_16(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {0xA000,30,1},
        {515,1,1},
        {516,2,1, 507,1,1},
        {1325,5,1},
        {9338,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_17(player, ototal, ntotal)
    local lvls = {
        10,99,499,999,5999,
    }
    local items = {
        {0xA000,30,1},
        {515,1,1},
        {509,1,1, 507,1,1},
        {515,3,1},
        {9371,60,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_18(player, ototal, ntotal)
    local lvls = {
        299,499,999,5999,
    }
    local items = {
        {503,5,1},
        {516,2,1, 517,2,1},
        {509,3,1},
        {500,50,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_19(player, ototal, ntotal)
    local lvls = {
        299,599,999,5999,
    }
    local items = {
        {503,5,1},
        {551,3,1},
        {134,5,1},
        {1325,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_20(player, ototal, ntotal)
    local lvls = {
        299,499,999,5999,
    }
    local items = {
        {503,5,1},
        {516,2,1, 507,1,1},
        {9076,3,1},
        {134,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_21(player, ototal, ntotal)
    local lvls = {
        299,599,999,5999,
    }
    local items = {
        {503,5,1},
        {515,2,1},
        {509,2,1, 507,2,1},
        {516,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_22(player, ototal, ntotal)
    local lvls = {
        299,499,999,5999,
    }
    local items = {
        {503,5,1},
        {516,2,1, 507,2,1},
        {1325,5,1},
        {9338,15,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_23(player, ototal, ntotal)
    local lvls = {
        299,499,999,5999,
    }
    local items = {
        {503,5,1},
        {509,2,1, 507,2,1},
        {515,3,1},
        {9371,60,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

function sendRechargeMails_2013_05_24(player, ototal, ntotal)
    local lvls = {
        299,599,999,5999,
    }
    local items = {
        {503,5,1},
        {516,2,1, 517,2,1},
        {509,3,1},
        {516,20,1},
    }
    _sendRechargeMails(player, ototal, ntotal, lvls, items)
end

-- RECHARGE
function sendRechargeMails(player, ototal, ntotal)
    if isRPServer() then
        local n = os.time() + 11
        local s_rp = getOpenTime();
        if n >= s_rp and n < (s_rp + 1*86400) then
            sendRPRechargeMails_1(player, ototal, ntotal)
        elseif n >= (s_rp+1*86400) and n < (s_rp + 2*86400) then
            sendRPRechargeMails_2(player, ototal, ntotal)
        elseif n >= (s_rp+2*86400) and n < (s_rp + 3*86400) then
            sendRPRechargeMails_3(player, ototal, ntotal)
        elseif n >= (s_rp+3*86400) and n < (s_rp + 4*86400) then
            sendRPRechargeMails_4(player, ototal, ntotal)
        elseif n >= (s_rp+4*86400) and n < (s_rp + 5*86400) then
            sendRPRechargeMails_5(player, ototal, ntotal)
        elseif n >= (s_rp+5*86400) and n < (s_rp + 6*86400) then
            sendRPRechargeMails_6(player, ototal, ntotal)
        elseif n >= (s_rp+6*86400) and n < (s_rp + 7*86400) then
            sendRPRechargeMails_7(player, ototal, ntotal)
        end
        if n >= s_rp and n < (s_rp + 7*86400) then
            return
        end
    end

    local t = { ['year'] = 2014, ['month'] = 8, ['day'] = 2, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11

    if n >= s and n < (s + 1*86400) then
        sendRechargeMails_2013_05_18(player, ototal, ntotal)
    elseif n >= (s + 1*86400) and n < (s + 2*86400) then
        sendRechargeMails_2013_05_19(player, ototal, ntotal)
    elseif n >= (s + 2*86400) and n < (s + 3*86400) then
        sendRechargeMails_2013_05_20(player, ototal, ntotal)
    elseif n >= (s + 3*86400) and n < (s + 4*86400) then
        sendRechargeMails_2013_05_21(player, ototal, ntotal)
    elseif n >= (s + 4*86400) and n < (s + 5*86400) then
        sendRechargeMails_2013_05_22(player, ototal, ntotal)
    elseif n >= (s + 5*86400) and n < (s + 6*86400) then
        sendRechargeMails_2013_05_23(player, ototal, ntotal)
    elseif n >= (s + 6*86400) and n < (s + 7*86400) then
        sendRechargeMails_2013_05_24(player, ototal, ntotal)
    end

    local t = { ['year'] = 2014, ['month'] = 8, ['day'] = 9, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11

    if n >= s and n < (s + 1*86400) then
        sendRechargeMails_2013_05_18(player, ototal, ntotal)
    elseif n >= (s + 1*86400) and n < (s + 2*86400) then
        sendRechargeMails_2013_05_19(player, ototal, ntotal)
    elseif n >= (s + 2*86400) and n < (s + 3*86400) then
        sendRechargeMails_2013_05_20(player, ototal, ntotal)
    elseif n >= (s + 3*86400) and n < (s + 4*86400) then
        sendRechargeMails_2013_05_21(player, ototal, ntotal)
    elseif n >= (s + 4*86400) and n < (s + 5*86400) then
        sendRechargeMails_2013_05_22(player, ototal, ntotal)
    elseif n >= (s + 5*86400) and n < (s + 6*86400) then
        sendRechargeMails_2013_05_23(player, ototal, ntotal)
    elseif n >= (s + 6*86400) and n < (s + 7*86400) then
        sendRechargeMails_2013_05_24(player, ototal, ntotal)
    end

    local t = { ['year'] = 2014, ['month'] = 8, ['day'] = 16, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11

    if n >= s and n < (s + 1*86400) then
        sendRechargeMails_2013_05_18(player, ototal, ntotal)
    elseif n >= (s + 1*86400) and n < (s + 2*86400) then
        sendRechargeMails_2013_05_19(player, ototal, ntotal)
    elseif n >= (s + 2*86400) and n < (s + 3*86400) then
        sendRechargeMails_2013_05_20(player, ototal, ntotal)
    elseif n >= (s + 3*86400) and n < (s + 4*86400) then
        sendRechargeMails_2013_05_21(player, ototal, ntotal)
    elseif n >= (s + 4*86400) and n < (s + 5*86400) then
        sendRechargeMails_2013_05_22(player, ototal, ntotal)
    elseif n >= (s + 5*86400) and n < (s + 6*86400) then
        sendRechargeMails_2013_05_23(player, ototal, ntotal)
    elseif n >= (s + 6*86400) and n < (s + 7*86400) then
        sendRechargeMails_2013_05_24(player, ototal, ntotal)
    end

    local t = { ['year'] = 2014, ['month'] = 8, ['day'] = 23, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11

    if n >= s and n < (s + 1*86400) then
        sendRechargeMails_2013_05_18(player, ototal, ntotal)
    elseif n >= (s + 1*86400) and n < (s + 2*86400) then
        sendRechargeMails_2013_05_19(player, ototal, ntotal)
    elseif n >= (s + 2*86400) and n < (s + 3*86400) then
        sendRechargeMails_2013_05_20(player, ototal, ntotal)
    elseif n >= (s + 3*86400) and n < (s + 4*86400) then
        sendRechargeMails_2013_05_21(player, ototal, ntotal)
    elseif n >= (s + 4*86400) and n < (s + 5*86400) then
        sendRechargeMails_2013_05_22(player, ototal, ntotal)
    elseif n >= (s + 5*86400) and n < (s + 6*86400) then
        sendRechargeMails_2013_05_23(player, ototal, ntotal)
    elseif n >= (s + 6*86400) and n < (s + 7*86400) then
        sendRechargeMails_2013_05_24(player, ototal, ntotal)
    end

    local t = { ['year'] = 2014, ['month'] = 8, ['day'] = 30, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11

    if n >= s and n < (s + 1*86400) then
        sendRechargeMails_2013_05_18(player, ototal, ntotal)
    elseif n >= (s + 1*86400) and n < (s + 2*86400) then
        sendRechargeMails_2013_05_19(player, ototal, ntotal)
    elseif n >= (s + 2*86400) and n < (s + 3*86400) then
        sendRechargeMails_2013_05_20(player, ototal, ntotal)
    elseif n >= (s + 3*86400) and n < (s + 4*86400) then
        sendRechargeMails_2013_05_21(player, ototal, ntotal)
    elseif n >= (s + 4*86400) and n < (s + 5*86400) then
        sendRechargeMails_2013_05_22(player, ototal, ntotal)
    elseif n >= (s + 5*86400) and n < (s + 6*86400) then
        sendRechargeMails_2013_05_23(player, ototal, ntotal)
    elseif n >= (s + 6*86400) and n < (s + 7*86400) then
        sendRechargeMails_2013_05_24(player, ototal, ntotal)
    end

    local t = { ['year'] = 2014, ['month'] = 9, ['day'] = 6, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11

    if n >= s and n < (s + 1*86400) then
        sendRechargeMails_2013_05_18(player, ototal, ntotal)
    elseif n >= (s + 1*86400) and n < (s + 2*86400) then
        sendRechargeMails_2013_05_19(player, ototal, ntotal)
    elseif n >= (s + 2*86400) and n < (s + 3*86400) then
        sendRechargeMails_2013_05_20(player, ototal, ntotal)
    elseif n >= (s + 3*86400) and n < (s + 4*86400) then
        sendRechargeMails_2013_05_21(player, ototal, ntotal)
    elseif n >= (s + 4*86400) and n < (s + 5*86400) then
        sendRechargeMails_2013_05_22(player, ototal, ntotal)
    elseif n >= (s + 5*86400) and n < (s + 6*86400) then
        sendRechargeMails_2013_05_23(player, ototal, ntotal)
    elseif n >= (s + 6*86400) and n < (s + 7*86400) then
        sendRechargeMails_2013_05_24(player, ototal, ntotal)
    end

    local t = { ['year'] = 2014, ['month'] = 9, ['day'] = 13, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11

    if n >= s and n < (s + 1*86400) then
        sendRechargeMails_2013_05_18(player, ototal, ntotal)
    elseif n >= (s + 1*86400) and n < (s + 2*86400) then
        sendRechargeMails_2013_05_19(player, ototal, ntotal)
    elseif n >= (s + 2*86400) and n < (s + 3*86400) then
        sendRechargeMails_2013_05_20(player, ototal, ntotal)
    elseif n >= (s + 3*86400) and n < (s + 4*86400) then
        sendRechargeMails_2013_05_21(player, ototal, ntotal)
    elseif n >= (s + 4*86400) and n < (s + 5*86400) then
        sendRechargeMails_2013_05_22(player, ototal, ntotal)
    elseif n >= (s + 5*86400) and n < (s + 6*86400) then
        sendRechargeMails_2013_05_23(player, ototal, ntotal)
    elseif n >= (s + 6*86400) and n < (s + 7*86400) then
        sendRechargeMails_2013_05_24(player, ototal, ntotal)
    end

    local t = { ['year'] = 2014, ['month'] = 9, ['day'] = 20, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11

    if n >= s and n < (s + 1*86400) then
        sendRechargeMails_2013_05_18(player, ototal, ntotal)
    elseif n >= (s + 1*86400) and n < (s + 2*86400) then
        sendRechargeMails_2013_05_19(player, ototal, ntotal)
    elseif n >= (s + 2*86400) and n < (s + 3*86400) then
        sendRechargeMails_2013_05_20(player, ototal, ntotal)
    elseif n >= (s + 3*86400) and n < (s + 4*86400) then
        sendRechargeMails_2013_05_21(player, ototal, ntotal)
    elseif n >= (s + 4*86400) and n < (s + 5*86400) then
        sendRechargeMails_2013_05_22(player, ototal, ntotal)
    elseif n >= (s + 5*86400) and n < (s + 6*86400) then
        sendRechargeMails_2013_05_23(player, ototal, ntotal)
    elseif n >= (s + 6*86400) and n < (s + 7*86400) then
        sendRechargeMails_2013_05_24(player, ototal, ntotal)
    end

    local t = { ['year'] = 2014, ['month'] = 9, ['day'] = 27, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11

    if n >= s and n < (s + 1*86400) then
        sendRechargeMails_2013_05_18(player, ototal, ntotal)
    elseif n >= (s + 1*86400) and n < (s + 2*86400) then
        sendRechargeMails_2013_05_19(player, ototal, ntotal)
    elseif n >= (s + 2*86400) and n < (s + 3*86400) then
        sendRechargeMails_2013_05_20(player, ototal, ntotal)
    elseif n >= (s + 3*86400) and n < (s + 4*86400) then
        sendRechargeMails_2013_05_21(player, ototal, ntotal)
    elseif n >= (s + 4*86400) and n < (s + 5*86400) then
        sendRechargeMails_2013_05_22(player, ototal, ntotal)
    elseif n >= (s + 5*86400) and n < (s + 6*86400) then
        sendRechargeMails_2013_05_23(player, ototal, ntotal)
    elseif n >= (s + 6*86400) and n < (s + 7*86400) then
        sendRechargeMails_2013_05_24(player, ototal, ntotal)
    end

    local t = { ['year'] = 2014, ['month'] = 10, ['day'] = 4, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11

    if n >= s and n < (s + 1*86400) then
        sendRechargeMails_2013_05_18(player, ototal, ntotal)
    elseif n >= (s + 1*86400) and n < (s + 2*86400) then
        sendRechargeMails_2013_05_19(player, ototal, ntotal)
    elseif n >= (s + 2*86400) and n < (s + 3*86400) then
        sendRechargeMails_2013_05_20(player, ototal, ntotal)
    elseif n >= (s + 3*86400) and n < (s + 4*86400) then
        sendRechargeMails_2013_05_21(player, ototal, ntotal)
    elseif n >= (s + 4*86400) and n < (s + 5*86400) then
        sendRechargeMails_2013_05_22(player, ototal, ntotal)
    elseif n >= (s + 5*86400) and n < (s + 6*86400) then
        sendRechargeMails_2013_05_23(player, ototal, ntotal)
    elseif n >= (s + 6*86400) and n < (s + 7*86400) then
        sendRechargeMails_2013_05_24(player, ototal, ntotal)
    end

    local t = { ['year'] = 2014, ['month'] = 10, ['day'] = 11, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11

    if n >= s and n < (s + 1*86400) then
        sendRechargeMails_2013_05_18(player, ototal, ntotal)
    elseif n >= (s + 1*86400) and n < (s + 2*86400) then
        sendRechargeMails_2013_05_19(player, ototal, ntotal)
    elseif n >= (s + 2*86400) and n < (s + 3*86400) then
        sendRechargeMails_2013_05_20(player, ototal, ntotal)
    elseif n >= (s + 3*86400) and n < (s + 4*86400) then
        sendRechargeMails_2013_05_21(player, ototal, ntotal)
    elseif n >= (s + 4*86400) and n < (s + 5*86400) then
        sendRechargeMails_2013_05_22(player, ototal, ntotal)
    elseif n >= (s + 5*86400) and n < (s + 6*86400) then
        sendRechargeMails_2013_05_23(player, ototal, ntotal)
    elseif n >= (s + 6*86400) and n < (s + 7*86400) then
        sendRechargeMails_2013_05_24(player, ototal, ntotal)
    end

    local t = { ['year'] = 2014, ['month'] = 10, ['day'] = 18, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11

    if n >= s and n < (s + 1*86400) then
        sendRechargeMails_2013_05_18(player, ototal, ntotal)
    elseif n >= (s + 1*86400) and n < (s + 2*86400) then
        sendRechargeMails_2013_05_19(player, ototal, ntotal)
    elseif n >= (s + 2*86400) and n < (s + 3*86400) then
        sendRechargeMails_2013_05_20(player, ototal, ntotal)
    elseif n >= (s + 3*86400) and n < (s + 4*86400) then
        sendRechargeMails_2013_05_21(player, ototal, ntotal)
    elseif n >= (s + 4*86400) and n < (s + 5*86400) then
        sendRechargeMails_2013_05_22(player, ototal, ntotal)
    elseif n >= (s + 5*86400) and n < (s + 6*86400) then
        sendRechargeMails_2013_05_23(player, ototal, ntotal)
    elseif n >= (s + 6*86400) and n < (s + 7*86400) then
        sendRechargeMails_2013_05_24(player, ototal, ntotal)
    end

    local t = { ['year'] = 2014, ['month'] = 10, ['day'] = 25, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11

    if n >= s and n < (s + 1*86400) then
        sendRechargeMails_2013_05_18(player, ototal, ntotal)
    elseif n >= (s + 1*86400) and n < (s + 2*86400) then
        sendRechargeMails_2013_05_19(player, ototal, ntotal)
    elseif n >= (s + 2*86400) and n < (s + 3*86400) then
        sendRechargeMails_2013_05_20(player, ototal, ntotal)
    elseif n >= (s + 3*86400) and n < (s + 4*86400) then
        sendRechargeMails_2013_05_21(player, ototal, ntotal)
    elseif n >= (s + 4*86400) and n < (s + 5*86400) then
        sendRechargeMails_2013_05_22(player, ototal, ntotal)
    elseif n >= (s + 5*86400) and n < (s + 6*86400) then
        sendRechargeMails_2013_05_23(player, ototal, ntotal)
    elseif n >= (s + 6*86400) and n < (s + 7*86400) then
        sendRechargeMails_2013_05_24(player, ototal, ntotal)
    end

    local t = { ['year'] = 2014, ['month'] = 11, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11

    if n >= s and n < (s + 1*86400) then
        sendRechargeMails_2013_05_18(player, ototal, ntotal)
    elseif n >= (s + 1*86400) and n < (s + 2*86400) then
        sendRechargeMails_2013_05_19(player, ototal, ntotal)
    elseif n >= (s + 2*86400) and n < (s + 3*86400) then
        sendRechargeMails_2013_05_20(player, ototal, ntotal)
    elseif n >= (s + 3*86400) and n < (s + 4*86400) then
        sendRechargeMails_2013_05_21(player, ototal, ntotal)
    elseif n >= (s + 4*86400) and n < (s + 5*86400) then
        sendRechargeMails_2013_05_22(player, ototal, ntotal)
    elseif n >= (s + 5*86400) and n < (s + 6*86400) then
        sendRechargeMails_2013_05_23(player, ototal, ntotal)
    elseif n >= (s + 6*86400) and n < (s + 7*86400) then
        sendRechargeMails_2013_05_24(player, ototal, ntotal)
    end

    local t = { ['year'] = 2014, ['month'] = 11, ['day'] = 8, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11

    if n >= s and n < (s + 1*86400) then
        sendRechargeMails_2013_05_18(player, ototal, ntotal)
    elseif n >= (s + 1*86400) and n < (s + 2*86400) then
        sendRechargeMails_2013_05_19(player, ototal, ntotal)
    elseif n >= (s + 2*86400) and n < (s + 3*86400) then
        sendRechargeMails_2013_05_20(player, ototal, ntotal)
    elseif n >= (s + 3*86400) and n < (s + 4*86400) then
        sendRechargeMails_2013_05_21(player, ototal, ntotal)
    elseif n >= (s + 4*86400) and n < (s + 5*86400) then
        sendRechargeMails_2013_05_22(player, ototal, ntotal)
    elseif n >= (s + 5*86400) and n < (s + 6*86400) then
        sendRechargeMails_2013_05_23(player, ototal, ntotal)
    elseif n >= (s + 6*86400) and n < (s + 7*86400) then
        sendRechargeMails_2013_05_24(player, ototal, ntotal)
    end

end

function sendRCAward(player, pos, total, f7, item)
    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_114, player:getCountry(), player:getPName(), total, pos)

    for n = 1,#f7 do
        local pinfo = f7[n]
        local p = pinfo[1]
        local t = pinfo[2]
        if p == 0 or p == nil then
            break
        end
        ctx = ctx .. tostring(n) .. ". [p:"..p:getCountry()..":"..p:getPName().."] : " .. tostring(t) .. "\n"
    end
    ctx = ctx .. msg_115

    if item ~= nil then
        sendItemPackageMail(player, title, ctx, item)
    else
        player:GetMailBox():normalMail(title, ctx)
    end
end

function sendRechargeRankAward_2012_10_16(player, pos)
    local items = {
        {9076,50,1, 9177,10,1},
        {9076,30,1, 9177,8,1},
        {9076,20,1, 9177,6,1},
        {9076,10,1, 9177,5,1},
        {9076,10,1, 9177,5,1},
        {9076,10,1, 9177,5,1},
        {9076,10,1, 9177,5,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_10_17(player, pos)
    local items = {
        {33,100,1, 134,50,1},
        {33,80,1, 134,30,1},
        {33,60,1, 134,20,1},
        {33,30,1, 134,10,1},
        {33,30,1, 134,10,1},
        {33,30,1, 134,10,1},
        {33,30,1, 134,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_10_18(player, pos)
    local items = {
        {9022,20,1, 1325,20,1},
        {9022,15,1, 1325,15,1},
        {9022,10,1, 1325,10,1},
        {9022,5,1, 1325,5,1},
        {9022,5,1, 1325,5,1},
        {9022,5,1, 1325,5,1},
        {9022,5,1, 1325,5,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_10_26(player, pos)
    local items = {
        {9022,20,1, 1325,30,1},
        {9022,15,1, 1325,20,1},
        {9022,10,1, 1325,15,1},
        {9022,5,1, 1325,10,1},
        {9022,5,1, 1325,10,1},
        {9022,5,1, 1325,10,1},
        {9022,5,1, 1325,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_10_27(player, pos)
    local items = {
        {9076,50,1, 134,30,1},
        {9076,30,1, 134,20,1},
        {9076,20,1, 134,15,1},
        {9076,10,1, 134,10,1},
        {9076,10,1, 134,10,1},
        {9076,10,1, 134,10,1},
        {9076,10,1, 134,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_10_28(player, pos)
    local items = {
        {33,100,1, 509,30,1, 507,30,1},
        {33,80,1, 509,20,1, 507,20,1},
        {33,50,1, 509,15,1, 507,15,1},
        {33,30,1, 509,10,1, 507,10,1},
        {33,30,1, 509,10,1, 507,10,1},
        {33,30,1, 509,10,1, 507,10,1},
        {33,30,1, 509,10,1, 507,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_11_03(player, pos)
    local items = {
        {9076,60,1},
        {9076,45,1},
        {9076,30,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_11_04(player, pos)
    local items = {
        {9022,20,1},
        {9022,15,1},
        {9022,10,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_11_05(player, pos)
    local items = {
        {1325,50,1, 134,50,1},
        {1325,40,1, 134,40,1},
        {1325,30,1, 134,30,1},
        {1325,20,1, 134,20,1},
        {1325,20,1, 134,20,1},
        {1325,20,1, 134,20,1},
        {1325,20,1, 134,20,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_11_10(player, pos)
    local items = {
        {9076,60,1},
        {9076,45,1},
        {9076,30,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_11_11(player, pos)
    local items = {
        {9022,20,1},
        {9022,15,1},
        {9022,10,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_11_12(player, pos)
    local items = {
        {1325,50,1, 134,50,1},
        {1325,40,1, 134,40,1},
        {1325,30,1, 134,30,1},
        {1325,20,1, 134,20,1},
        {1325,20,1, 134,20,1},
        {1325,20,1, 134,20,1},
        {1325,20,1, 134,20,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_11_16(player, pos)
    local items = {
        {515,60,1},
        {515,45,1},
        {515,30,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_11_17(player, pos)
    local items = {
        {1325,88,1},
        {1325,66,1},
        {1325,55,1},
        {1325,33,1},
        {1325,33,1},
        {1325,33,1},
        {1325,33,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_11_18(player, pos)
    local items = {
        {9022,20,1},
        {9022,15,1},
        {9022,10,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_11_19(player, pos)
    local items = {
        {9076,60,1},
        {9076,45,1},
        {9076,30,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_11_20(player, pos)
    local items = {
        {134,88,1},
        {134,66,1},
        {134,55,1},
        {134,33,1},
        {134,33,1},
        {134,33,1},
        {134,33,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_11_21(player, pos)
    local items = {
        {509,50,1, 507,50,1},
        {509,40,1, 507,40,1},
        {509,30,1, 507,30,1},
        {509,15,1, 507,15,1},
        {509,15,1, 507,15,1},
        {509,15,1, 507,15,1},
        {509,15,1, 507,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_11_22(player, pos)
    local items = {
        {9076,30,1, 1325,30,1, 507,30,1},
        {9076,22,1, 1325,22,1, 507,22,1},
        {9076,16,1, 1325,16,1, 507,16,1},
        {9076,10,1, 1325,10,1, 507,10,1},
        {9076,10,1, 1325,10,1, 507,10,1},
        {9076,10,1, 1325,10,1, 507,10,1},
        {9076,10,1, 1325,10,1, 507,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_11_27(player, pos)
    local items = {
        {9076,60,1},
        {9076,45,1},
        {9076,30,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_11_28(player, pos)
    local items = {
        {134,66,1},
        {134,55,1},
        {134,40,1},
        {134,28,1},
        {134,28,1},
        {134,28,1},
        {134,28,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_11_29(player, pos)
    local items = {
        {1325,66,1},
        {1325,55,1},
        {1325,40,1},
        {1325,28,1},
        {1325,28,1},
        {1325,28,1},
        {1325,28,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_11_30(player, pos)
    local items = {
        {509,30,1, 507,30,1},
        {509,22,1, 507,22,1},
        {509,15,1, 507,15,1},
        {509,10,1, 507,10,1},
        {509,10,1, 507,10,1},
        {509,10,1, 507,10,1},
        {509,10,1, 507,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_08(player, pos)
    local items = {
        {515,45,1},
        {515,35,1},
        {515,25,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_09(player, pos)
    local items = {
        {1325,66,1},
        {1325,55,1},
        {1325,42,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_10(player, pos)
    local items = {
        {9022,20,1},
        {9022,15,1},
        {9022,10,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_11(player, pos)
    local items = {
        {9076,60,1},
        {9076,45,1},
        {9076,30,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_12(player, pos)
    local items = {
        {134,66,1},
        {134,55,1},
        {134,42,1},
        {134,30,1},
        {134,30,1},
        {134,30,1},
        {134,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_13(player, pos)
    local items = {
        {509,35,1, 507,35,1},
        {509,28,1, 507,28,1},
        {509,20,1, 507,20,1},
        {509,10,1, 507,10,1},
        {509,10,1, 507,10,1},
        {509,10,1, 507,10,1},
        {509,10,1, 507,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_14(player, pos)
    local items = {
        {9076,30,1, 1325,30,1},
        {9076,22,1, 1325,22,1},
        {9076,16,1, 1325,16,1},
        {9076,10,1, 1325,10,1},
        {9076,10,1, 1325,10,1},
        {9076,10,1, 1325,10,1},
        {9076,10,1, 1325,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_15(player, pos)
    local items = {
        {515,45,1},
        {515,35,1},
        {515,25,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_16(player, pos)
    local items = {
        {1325,66,1},
        {1325,55,1},
        {1325,42,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_17(player, pos)
    local items = {
        {9022,20,1},
        {9022,15,1},
        {9022,10,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_18(player, pos)
    local items = {
        {9076,60,1},
        {9076,45,1},
        {9076,30,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_19(player, pos)
    local items = {
        {134,66,1},
        {134,55,1},
        {134,42,1},
        {134,30,1},
        {134,30,1},
        {134,30,1},
        {134,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_20(player, pos)
    local items = {
        {134,20,1, 1325,20,1, 515,20,1},
        {134,16,1, 1325,16,1, 515,16,1},
        {134,12,1, 1325,12,1, 515,12,1},
        {134,8,1, 1325,8,1, 515,8,1},
        {134,8,1, 1325,8,1, 515,8,1},
        {134,8,1, 1325,8,1, 515,8,1},
        {134,8,1, 1325,8,1, 515,8,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_21(player, pos)
    local items = {
        {515,30,1, 1325,30,1},
        {515,22,1, 1325,22,1},
        {515,16,1, 1325,16,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_22(player, pos)
    local items = {
        {515,45,1},
        {515,35,1},
        {515,25,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_23(player, pos)
    local items = {
        {1325,66,1},
        {1325,55,1},
        {1325,42,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_24(player, pos)
    local items = {
        {9022,20,1},
        {9022,15,1},
        {9022,10,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_25(player, pos)
    local items = {
        {134,66,1},
        {134,55,1},
        {134,42,1},
        {134,30,1},
        {134,30,1},
        {134,30,1},
        {134,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_26(player, pos)
    local items = {
        {134,20,1, 1325,20,1, 515,20,1},
        {134,16,1, 1325,16,1, 515,16,1},
        {134,12,1, 1325,12,1, 515,12,1},
        {134,8,1, 1325,8,1, 515,8,1},
        {134,8,1, 1325,8,1, 515,8,1},
        {134,8,1, 1325,8,1, 515,8,1},
        {134,8,1, 1325,8,1, 515,8,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_27(player, pos)
    local items = {
        {515,30,1, 1325,30,1},
        {515,22,1, 1325,22,1},
        {515,16,1, 1325,16,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_28(player, pos)
    local items = {
        {1325,66,1},
        {1325,55,1},
        {1325,42,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_29(player, pos)
    local items = {
        {134,20,1, 1325,20,1, 515,20,1},
        {134,16,1, 1325,16,1, 515,16,1},
        {134,12,1, 1325,12,1, 515,12,1},
        {134,8,1, 1325,8,1, 515,8,1},
        {134,8,1, 1325,8,1, 515,8,1},
        {134,8,1, 1325,8,1, 515,8,1},
        {134,8,1, 1325,8,1, 515,8,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_30(player, pos)
    local items = {
        {515,30,1, 1325,30,1},
        {515,22,1, 1325,22,1},
        {515,16,1, 1325,16,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2012_12_31(player, pos)
    local items = {
        {1325,66,1},
        {1325,55,1},
        {1325,42,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_01(player, pos)
    local items = {
        {515,45,1},
        {515,35,1},
        {515,25,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_02(player, pos)
    local items = {
        {134,66,1},
        {134,55,1},
        {134,42,1},
        {134,30,1},
        {134,30,1},
        {134,30,1},
        {134,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_03(player, pos)
    local items = {
        {9022,20,1},
        {9022,15,1},
        {9022,10,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_04(player, pos)
    local items = {
        {9076,60,1},
        {9076,45,1},
        {9076,30,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_05(player, pos)
    local items = {
        {9076,30,1, 1325,30,1},
        {9076,22,1, 1325,22,1},
        {9076,16,1, 1325,16,1},
        {9076,10,1, 1325,10,1},
        {9076,10,1, 1325,10,1},
        {9076,10,1, 1325,10,1},
        {9076,10,1, 1325,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_06(player, pos)
    local items = {
        {509,35,1, 507,35,1},
        {509,28,1, 507,28,1},
        {509,20,1, 507,20,1},
        {509,10,1, 507,10,1},
        {509,10,1, 507,10,1},
        {509,10,1, 507,10,1},
        {509,10,1, 507,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_07(player, pos)
    local items = {
        {515,35,1},
        {515,28,1},
        {515,20,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_19(player, pos)
    local items = {
        {134,20,1, 1325,20,1, 515,20,1},
        {134,16,1, 1325,16,1, 515,16,1},
        {134,12,1, 1325,12,1, 515,12,1},
        {134,8,1, 1325,8,1, 515,8,1},
        {134,8,1, 1325,8,1, 515,8,1},
        {134,8,1, 1325,8,1, 515,8,1},
        {134,8,1, 1325,8,1, 515,8,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_20(player, pos)
    local items = {
        {515,30,1, 1325,30,1},
        {515,22,1, 1325,22,1},
        {515,16,1, 1325,16,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_21(player, pos)
    local items = {
        {1325,66,1},
        {1325,55,1},
        {1325,42,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_22(player, pos)
    local items = {
        {515,45,1},
        {515,35,1},
        {515,25,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_23(player, pos)
    local items = {
        {134,66,1},
        {134,55,1},
        {134,42,1},
        {134,30,1},
        {134,30,1},
        {134,30,1},
        {134,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_24(player, pos)
    local items = {
        {9022,20,1},
        {9022,15,1},
        {9022,10,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_25(player, pos)
    local items = {
        {9076,60,1},
        {9076,45,1},
        {9076,30,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_26(player, pos)
    local items = {
        {9076,30,1, 1325,30,1},
        {9076,22,1, 1325,22,1},
        {9076,16,1, 1325,16,1},
        {9076,10,1, 1325,10,1},
        {9076,10,1, 1325,10,1},
        {9076,10,1, 1325,10,1},
        {9076,10,1, 1325,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_27(player, pos)
    local items = {
        {509,35,1, 507,35,1},
        {509,28,1, 507,28,1},
        {509,20,1, 507,20,1},
        {509,10,1, 507,10,1},
        {509,10,1, 507,10,1},
        {509,10,1, 507,10,1},
        {509,10,1, 507,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_28(player, pos)
    local items = {
        {515,35,1},
        {515,28,1},
        {515,20,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_29(player, pos)
    local items = {
        {1325,66,1},
        {1325,55,1},
        {1325,42,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_30(player, pos)
    local items = {
        {134,20,1, 1325,20,1, 515,20,1},
        {134,16,1, 1325,16,1, 515,16,1},
        {134,12,1, 1325,12,1, 515,12,1},
        {134,8,1, 1325,8,1, 515,8,1},
        {134,8,1, 1325,8,1, 515,8,1},
        {134,8,1, 1325,8,1, 515,8,1},
        {134,8,1, 1325,8,1, 515,8,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_01_31(player, pos)
    local items = {
        {515,30,1, 1325,30,1},
        {515,22,1, 1325,22,1},
        {515,16,1, 1325,16,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_02_01(player, pos)
    local items = {
        {1325,66,1},
        {1325,55,1},
        {1325,42,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_02_02(player, pos)
    local items = {
        {515,45,1},
        {515,35,1},
        {515,25,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_02_03(player, pos)
    local items = {
        {134,66,1},
        {134,55,1},
        {134,42,1},
        {134,30,1},
        {134,30,1},
        {134,30,1},
        {134,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_02_04(player, pos)
    local items = {
        {9022,20,1},
        {9022,15,1},
        {9022,10,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_02_05(player, pos)
    local items = {
        {9076,60,1},
        {9076,45,1},
        {9076,30,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_02_06(player, pos)
    local items = {
        {9076,30,1, 1325,30,1},
        {9076,22,1, 1325,22,1},
        {9076,16,1, 1325,16,1},
        {9076,10,1, 1325,10,1},
        {9076,10,1, 1325,10,1},
        {9076,10,1, 1325,10,1},
        {9076,10,1, 1325,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_02_07(player, pos)
    local items = {
        {509,35,1, 1325,35,1},
        {509,28,1, 1325,28,1},
        {509,20,1, 1325,20,1},
        {509,10,1, 1325,10,1},
        {509,10,1, 1325,10,1},
        {509,10,1, 1325,10,1},
        {509,10,1, 1325,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_02_08(player, pos)
    local items = {
        {515,35,1},
        {515,28,1},
        {515,20,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_02_16(player, pos)
    local items = {
        {134,20,1, 1325,20,1, 515,10,1},
        {134,16,1, 1325,16,1, 515,8,1},
        {134,12,1, 1325,12,1, 515,6,1},
        {134,8,1, 1325,8,1, 515,5,1},
        {134,8,1, 1325,8,1, 515,5,1},
        {134,8,1, 1325,8,1, 515,5,1},
        {134,8,1, 1325,8,1, 515,5,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_02_17(player, pos)
    local items = {
        {515,30,1, 1325,30,1},
        {515,22,1, 1325,22,1},
        {515,16,1, 1325,16,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
        {515,10,1, 1325,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_02_18(player, pos)
    local items = {
        {1325,66,1},
        {1325,55,1},
        {1325,42,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_02_19(player, pos)
    local items = {
        {515,45,1},
        {515,35,1},
        {515,25,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_02_20(player, pos)
    local items = {
        {134,66,1},
        {134,55,1},
        {134,42,1},
        {134,30,1},
        {134,30,1},
        {134,30,1},
        {134,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_02_21(player, pos)
    local items = {
        {9022,20,1},
        {9022,15,1},
        {9022,10,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_02_22(player, pos)
    local items = {
        {9076,60,1},
        {9076,45,1},
        {9076,30,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_09(player, pos)
    local items = {
        {9338,66,1},
        {9338,55,1},
        {9338,44,1},
        {9338,33,1},
        {9338,33,1},
        {9338,33,1},
        {9338,33,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_10(player, pos)
    local items = {
        {9092,60,1},
        {9092,50,1},
        {9092,40,1},
        {9092,30,1},
        {9092,30,1},
        {9092,30,1},
        {9092,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_11(player, pos)
    local items = {
        {1325,66,1},
        {1325,55,1},
        {1325,42,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
        {1325,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_12(player, pos)
    local items = {
        {515,45,1},
        {515,35,1},
        {515,25,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_13(player, pos)
    local items = {
        {134,66,1},
        {134,55,1},
        {134,42,1},
        {134,30,1},
        {134,30,1},
        {134,30,1},
        {134,30,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_14(player, pos)
    local items = {
        {9022,20,1},
        {9022,15,1},
        {9022,10,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_15(player, pos)
    local items = {
        {9076,60,1},
        {9076,45,1},
        {9076,30,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_16(player, pos)
    local items = {
        {500,99,1, 501,99,1},
        {500,77,1, 501,77,1},
        {500,55,1, 501,55,1},
        {500,33,1, 501,33,1},
        {500,33,1, 501,33,1},
        {500,33,1, 501,33,1},
        {500,33,1, 501,33,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_17(player, pos)
    local items = {
        {9338,66,1},
        {9338,55,1},
        {9338,44,1},
        {9338,33,1},
        {9338,33,1},
        {9338,33,1},
        {9338,33,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_18(player, pos)
    local items = {
        {9371,245,1},
        {9371,215,1},
        {9371,185,1},
        {9371,125,1},
        {9371,125,1},
        {9371,125,1},
        {9371,125,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_19(player, pos)
    local items = {
        {1325,66,1},
        {1325,55,1},
        {1325,44,1},
        {1325,33,1},
        {1325,33,1},
        {1325,33,1},
        {1325,33,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_20(player, pos)
    local items = {
        {515,45,1},
        {515,35,1},
        {515,25,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_21(player, pos)
    local items = {
        {9425,45,1},
        {9425,35,1},
        {9425,25,1},
        {9425,15,1},
        {9425,15,1},
        {9425,15,1},
        {9425,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_22(player, pos)
    local items = {
        {9022,20,1},
        {9022,15,1},
        {9022,10,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_23(player, pos)
    local items = {
        {9076,60,1},
        {9076,45,1},
        {9076,30,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end
function sendRPRechargeRankAward_1(player, pos, total, f7)
    local items = {
        {509,50,1},
        {509,40,1},
        {509,30,1},
        {509,15,1},
        {509,15,1},
        {509,15,1},
        {509,15,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end
function sendRPRechargeRankAward_2(player, pos, total, f7)
    local items = {
        {507,50,1},
        {507,40,1},
        {507,30,1},
        {507,15,1},
        {507,15,1},
        {507,15,1},
        {507,15,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end
function sendRPRechargeRankAward_3(player, pos, total, f7)
    local items = {
        {516,50,1},
        {516,40,1},
        {516,30,1},
        {516,15,1},
        {516,15,1},
        {516,15,1},
        {516,15,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end
function sendRPRechargeRankAward_4(player, pos, total, f7)
    local items = {
        {503,50,1},
        {503,40,1},
        {503,30,1},
        {503,15,1},
        {503,15,1},
        {503,15,1},
        {503,15,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end
function sendRPRechargeRankAward_5(player, pos, total, f7)
    local items = {
        {515,50,1},
        {515,40,1},
        {515,30,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end
function sendRPRechargeRankAward_6(player, pos, total, f7)
    local items = {
        {134,50,1},
        {134,40,1},
        {134,30,1},
        {134,15,1},
        {134,15,1},
        {134,15,1},
        {134,15,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end
function sendRPRechargeRankAward_7(player, pos, total, f7)
    local items = {
        {515,50,1},
        {515,40,1},
        {515,30,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_03_24(player, pos)
    local items = {
        {500,99,1, 501,99,1},
        {500,77,1, 501,77,1},
        {500,55,1, 501,55,1},
        {500,33,1, 501,33,1},
        {500,33,1, 501,33,1},
        {500,33,1, 501,33,1},
        {500,33,1, 501,33,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_25(player, pos)
    local items = {
        {503,180,1},
        {503,150,1},
        {503,120,1},
        {503,90,1},
        {503,90,1},
        {503,90,1},
        {503,90,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_26(player, pos)
    local items = {
        {9371,245,1},
        {9371,215,1},
        {9371,185,1},
        {9371,125,1},
        {9371,125,1},
        {9371,125,1},
        {9371,125,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_27(player, pos)
    local items = {
        {9600,180,1},
        {9600,150,1},
        {9600,120,1},
        {9600,90,1},
        {9600,90,1},
        {9600,90,1},
        {9600,90,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_28(player, pos)
    local items = {
        {515,45,1},
        {515,35,1},
        {515,25,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_29(player, pos, total, f7)
    local items = {
        {134,66,1},
        {134,55,1},
        {134,44,1},
        {134,33,1},
        {134,33,1},
        {134,33,1},
        {134,33,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2014_01_14(player, pos, total, f7)
    local items = {
        {1126,120,1},
        {1126,100,1},
        {1126,80,1},
        {1126,60,1},
        {1126,60,1},
        {1126,60,1},
        {1126,60,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end
function sendRechargeRankAward_2014_01_28(player, pos, total, f7)
    local items = {
        {501,120,1},
        {501,90,1},
        {501,60,1},
        {501,30,1},
        {501,30,1},
        {501,30,1},
        {501,30,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end
function sendRechargeRankAward_2014_02_04(player, pos, total, f7)
    local items = {
        {9414,120,1},
        {9414,100,1},
        {9414,80,1},
        {9414,60,1},
        {9414,60,1},
        {9414,60,1},
        {9414,60,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end
function sendRechargeRankAward_2014_02_11(player, pos, total, f7)
    local items = {
        {1126,180,1},
        {1126,150,1},
        {1126,120,1},
        {1126,90,1},
        {1126,90,1},
        {1126,90,1},
        {1126,90,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_03_30(player, pos)
    local items = {
        {9022,20,1},
        {9022,15,1},
        {9022,10,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_03_31(player, pos)
    local items = {
        {9076,60,1},
        {9076,45,1},
        {9076,30,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_04_08(player, pos)
    local items = {
        {516,60,1},
        {516,45,1},
        {516,30,1},
        {516,15,1},
        {516,15,1},
        {516,15,1},
        {516,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_04_14(player, pos)
    local items = {
        {500,99,1, 501,99,1},
        {500,77,1, 501,77,1},
        {500,55,1, 501,55,1},
        {500,33,1, 501,33,1},
        {500,33,1, 501,33,1},
        {500,33,1, 501,33,1},
        {500,33,1, 501,33,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_04_15(player, pos, total, f7)
    local items = {
        {503,180,1},
        {503,150,1},
        {503,120,1},
        {503,90,1},
        {503,90,1},
        {503,90,1},
        {503,90,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_04_16(player, pos)
    local items = {
        {9371,245,1},
        {9371,215,1},
        {9371,185,1},
        {9371,125,1},
        {9371,125,1},
        {9371,125,1},
        {9371,125,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_04_17(player, pos)
    local items = {
        {1325,66,1},
        {1325,55,1},
        {1325,44,1},
        {1325,33,1},
        {1325,33,1},
        {1325,33,1},
        {1325,33,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_04_18(player, pos, total, f7)
    local items = {
        {515,45,1},
        {515,35,1},
        {515,25,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
        {515,15,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_04_19(player, pos, total, f7)
    local items = {
        {9022,20,1},
        {9022,15,1},
        {9022,10,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
        {9022,5,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_04_20(player, pos)
    local items = {
        {516,60,1},
        {516,45,1},
        {516,30,1},
        {516,15,1},
        {516,15,1},
        {516,15,1},
        {516,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_06_11(player, pos)
    local items = {
        {5028,1,1},
        {5027,2,1},
        {5027,1,1},
        {5026,2,1},
        {5026,2,1},
        {5026,2,1},
        {5026,2,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_06_22(player, pos)
    local items = {
        {1126,99,1},
        {1126,77,1},
        {1126,55,1},
        {1126,33,1},
        {1126,33,1},
        {1126,33,1},
        {1126,33,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_06_25(player, pos)
    local items = {
        {5008,1,1},
        {5007,2,1},
        {5007,1,1},
        {5006,2,1},
        {5006,2,1},
        {5006,2,1},
        {5006,2,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_07_02(player, pos)
    local items = {
        {5028,1,1},
        {5027,2,1},
        {5027,1,1},
        {5026,2,1},
        {5026,2,1},
        {5026,2,1},
        {5026,2,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_07_09(player, pos)
    local items = {
        {5058,1,1},
        {5057,2,1},
        {5057,1,1},
        {5056,2,1},
        {5056,2,1},
        {5056,2,1},
        {5056,2,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_07_16(player, pos)
    local items = {
        {5038,1,1},
        {5037,2,1},
        {5037,1,1},
        {5036,2,1},
        {5036,2,1},
        {5036,2,1},
        {5036,2,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_08_13(player, pos)
    local items = {
        {5028,1,1},
        {5027,2,1},
        {5027,1,1},
        {5026,2,1},
        {5026,2,1},
        {5026,2,1},
        {5026,2,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_08_16(player, pos)
    local items = {
        {9371,245,1},
        {9371,215,1},
        {9371,185,1},
        {9371,125,1},
        {9371,125,1},
        {9371,125,1},
        {9371,125,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_08_24(player, pos, total, f7)
    local items = {
        {500,198,1},
        {500,154,1},
        {500,110,1},
        {500,66,1},
        {500,66,1},
        {500,66,1},
        {500,66,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_09_06(player, pos, total, f7)
    local items = {
        {1325,66,1},
        {1325,55,1},
        {1325,44,1},
        {1325,33,1},
        {1325,33,1},
        {1325,33,1},
        {1325,33,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_09_10(player, pos)
    local items = {
        {5038,1,1},
        {5037,2,1},
        {5037,1,1},
        {5036,2,1},
        {5036,2,1},
        {5036,2,1},
        {5036,2,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_09_24(player, pos)
    local items = {
        {5028,1,1},
        {5027,2,1},
        {5027,1,1},
        {5026,2,1},
        {5026,2,1},
        {5026,2,1},
        {5026,2,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_10_01(player, pos)
    local items = {
        {516,60,1},
        {516,45,1},
        {516,30,1},
        {516,15,1},
        {516,15,1},
        {516,15,1},
        {516,15,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_111, pos)
    local ctx = string.format(msg_111, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendRechargeRankAward_2013_10_08(player, pos, total, f7)
    local items = {
        {9076,60,1},
        {9076,45,1},
        {9076,30,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
        {9076,15,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_10_15(player, pos, total, f7)
    local items = {
        {501,120,1},
        {501,90,1},
        {501,60,1},
        {501,30,1},
        {501,30,1},
        {501,30,1},
        {501,30,1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_10_19(player, pos, total, f7)
    local items = {
        {516, 65, 1},
        {516, 45, 1},
        {516, 35, 1},
        {516, 25, 1},
        {516, 25, 1},
        {516, 25, 1},
        {516, 25, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_10_22(player, pos, total, f7)
    local items = {
        {547, 120, 1},
        {547, 90, 1},
        {547, 60, 1},
        {547, 30, 1},
        {547, 30, 1},
        {547, 30, 1},
        {547, 30, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_10_26(player, pos, total, f7)
    local items = {
        {500, 198, 1},
        {500, 154, 1},
        {500, 110, 1},
        {500, 66, 1},
        {500, 66, 1},
        {500, 66, 1},
        {500, 66, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_10_29(player, pos, total, f7)
    local items = {
        {501, 120, 1},
        {501, 90, 1},
        {501, 60, 1},
        {501, 30, 1},
        {501, 30, 1},
        {501, 30, 1},
        {501, 30, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_11_02(player, pos, total, f7)
    local items = {
        {515, 45, 1},
        {515, 35, 1},
        {515, 25, 1},
        {515, 15, 1},
        {515, 15, 1},
        {515, 15, 1},
        {515, 15, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_11_12(player, pos, total, f7)
    local items = {
        {500, 198, 1},
        {500, 154, 1},
        {500, 110, 1},
        {500, 66, 1},
        {500, 66, 1},
        {500, 66, 1},
        {500, 66, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_11_19(player, pos, total, f7)
    local items = {
        {501, 120, 1},
        {501, 90, 1},
        {501, 60, 1},
        {501, 30, 1},
        {501, 30, 1},
        {501, 30, 1},
        {501, 30, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_11_26(player, pos, total, f7)
    local items = {
        {516, 65, 1},
        {516, 45, 1},
        {516, 35, 1},
        {516, 25, 1},
        {516, 25, 1},
        {516, 25, 1},
        {516, 25, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_12_03(player, pos, total, f7)
    local items = {
        {500, 198, 1},
        {500, 154, 1},
        {500, 110, 1},
        {500, 66, 1},
        {500, 66, 1},
        {500, 66, 1},
        {500, 66, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_12_10(player, pos, total, f7)
    local items = {
        {501, 120, 1},
        {501, 90, 1},
        {501, 60, 1},
        {501, 30, 1},
        {501, 30, 1},
        {501, 30, 1},
        {501, 30, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_12_17(player, pos, total, f7)
    local items = {
        {5008, 1, 1},
        {5007, 2, 1},
        {5007, 1, 1},
        {5006, 2, 1},
        {5006, 2, 1},
        {5006, 2, 1},
        {5006, 2, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_12_24(player, pos, total, f7)
    local items = {
        {5028, 1, 1},
        {5027, 2, 1},
        {5027, 1, 1},
        {5026, 2, 1},
        {5026, 2, 1},
        {5026, 2, 1},
        {5026, 2, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2013_12_31(player, pos, total, f7)
    local items = {
        {501, 120, 1},
        {501, 90, 1},
        {501, 60, 1},
        {501, 30, 1},
        {501, 30, 1},
        {501, 30, 1},
        {501, 30, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2014_01_07(player, pos, total, f7)
    local items = {
        {1126, 120, 1},
        {1126, 100, 1},
        {1126, 80, 1},
        {1126, 60, 1},
        {1126, 60, 1},
        {1126, 60, 1},
        {1126, 60, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2014_03_01(player, pos, total, f7)
    local items = {
        {9425, 45, 1},
        {9425, 35, 1},
        {9425, 25, 1},
        {9425, 15, 1},
        {9425, 15, 1},
        {9425, 15, 1},
        {9425, 15, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2014_03_08(player, pos, total, f7)
    local items = {
        {515, 45, 1},
        {515, 35, 1},
        {515, 25, 1},
        {515, 15, 1},
        {515, 15, 1},
        {515, 15, 1},
        {515, 15, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2014_03_15(player, pos, total, f7)
    local items = {
        {501, 120, 1},
        {501, 90, 1},
        {501, 60, 1},
        {501, 30, 1},
        {501, 30, 1},
        {501, 30, 1},
        {501, 30, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2014_03_22(player, pos, total, f7)
    local items = {
        {500, 198, 1},
        {500, 154, 1},
        {500, 110, 1},
        {500, 66, 1},
        {500, 66, 1},
        {500, 66, 1},
        {500, 66, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2014_03_29(player, pos, total, f7)
    local items = {
        {9425, 45, 1},
        {9425, 35, 1},
        {9425, 25, 1},
        {9425, 15, 1},
        {9425, 15, 1},
        {9425, 15, 1},
        {9425, 15, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end
function sendRechargeRankAward_2014_04_12(player, pos, total, f7)
    local items = {
        {515, 45, 1},
        {515, 35, 1},
        {515, 25, 1},
        {515, 15, 1},
        {515, 15, 1},
        {515, 15, 1},
        {515, 15, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2014_04_26(player, pos, total, f7)
    local items = {
        {9425, 45, 1},
        {9425, 35, 1},
        {9425, 25, 1},
        {9425, 15, 1},
        {9425, 15, 1},
        {9425, 15, 1},
        {9425, 15, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2014_05_3(player, pos, total, f7)
    local items = {
        {515, 45, 1},
        {515, 35, 1},
        {515, 25, 1},
        {515, 15, 1},
        {515, 15, 1},
        {515, 15, 1},
        {515, 15, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2014_05_17(player, pos, total, f7)
    local items = {
        {9425, 45, 1},
        {9425, 35, 1},
        {9425, 25, 1},
        {9425, 15, 1},
        {9425, 15, 1},
        {9425, 15, 1},
        {9425, 15, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2014_05_26(player, pos, total, f7)
    local items = {
        {9438, 120, 1},
        {9438, 100, 1},
        {9438, 80, 1},
        {9438, 60, 1},
        {9438, 60, 1},
        {9438, 60, 1},
        {9438, 60, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2014_05_30(player, pos, total, f7)
    local items = {
        {9338, 66, 1},
        {9338, 55, 1},
        {9338, 44, 1},
        {9338, 33, 1},
        {9338, 33, 1},
        {9338, 33, 1},
        {9338, 33, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2014_06_01(player, pos, total, f7)
    local items = {
        {515, 45, 1},
        {515, 35, 1},
        {515, 25, 1},
        {515, 15, 1},
        {515, 15, 1},
        {515, 15, 1},
        {515, 15, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2014_06_07(player, pos, total, f7)
    local items = {
        {9600, 180, 1},
        {9600, 150, 1},
        {9600, 120, 1},
        {9600, 90, 1},
        {9600, 90, 1},
        {9600, 90, 1},
        {9600, 90, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2014_06_13(player, pos, total, f7)
    local items = {
        {9425, 45, 1},
        {9425, 35, 1},
        {9425, 25, 1},
        {9425, 15, 1},
        {9425, 15, 1},
        {9425, 15, 1},
        {9425, 15, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

function sendRechargeRankAward_2014_08_02(player, pos, total, f7)
    local items = {
        {9498, 120, 1},
        {9498, 100, 1},
        {9498, 80, 1},
        {9498, 60, 1},
        {9498, 60, 1},
        {9498, 60, 1},
        {9498, 60, 1},
    }
    sendRCAward(player, pos, total, f7, items[pos])
end

-- RANK
function sendRechargeRankAward(player, pos, total, f7)
    if isRPServer() then
        local s_rp = getOpenTime();
        local n = os.time()
        if (n >= s_rp+600) and n < (s_rp + 1*86400+600) then
            sendRPRechargeRankAward_1(player, pos, total, f7)
        elseif n >= (s_rp+1*86400+600) and n < (s_rp + 2*86400+600) then
            sendRPRechargeRankAward_2(player, pos, total, f7)
        elseif n >= (s_rp+2*86400+600) and n < (s_rp + 3*86400+600) then
            sendRPRechargeRankAward_3(player, pos, total, f7)
        elseif n >= (s_rp+3*86400+600) and n < (s_rp + 4*86400+600) then
            sendRPRechargeRankAward_4(player, pos, total, f7)
        elseif n >= (s_rp+4*86400+600) and n < (s_rp + 5*86400+600) then
            sendRPRechargeRankAward_5(player, pos, total, f7)
        elseif n >= (s_rp+5*86400+600) and n < (s_rp + 6*86400+600) then
            sendRPRechargeRankAward_6(player, pos, total, f7)
        elseif n >= (s_rp+6*86400+600) and n < (s_rp + 7*86400+600) then
            sendRPRechargeRankAward_7(player, pos, total, f7)
        end
        if n >= s_rp and n < (s_rp + 7*86400) then
            return
        end
    end

    local t = { ['year'] = 2014, ['month'] = 11, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time()

    if n >= (s + 10 * 60) and n < (s + 86400 + 10*60) then
        sendRechargeRankAward_2013_03_21(player, pos, total, f7)
    elseif n >= (s + 86400 + 10*60) and n < (s + 2*86400 + 10*60) then
        sendRechargeRankAward_2014_06_01(player, pos, total, f7)
    elseif n >= (s + 2*86400 + 10*60) and n < (s + 3*86400 + 10*60) then
        sendRechargeRankAward_2014_05_26(player, pos, total, f7)
    elseif n >= (s + 3*86400 + 10*60) and n < (s + 4*86400 + 10*60) then
        sendRechargeRankAward_2014_02_04(player, pos, total, f7)
    elseif n >= (s + 4*86400 + 10*60) and n < (s + 5*86400 + 10*60) then
        sendRechargeRankAward_2013_04_18(player, pos, total, f7)
    elseif n >= (s + 5*86400 + 10*60) and n < (s + 6*86400 + 10*60) then
        sendRechargeRankAward_2013_04_19(player, pos, total, f7)
    elseif n >= (s + 6*86400 + 10*60) and n < (s + 7*86400 + 10*60) then
        sendRechargeRankAward_2013_03_27(player, pos, total, f7)
    end

    local t = { ['year'] = 2014, ['month'] = 11, ['day'] = 8, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time()

    if n >= (s + 10 * 60) and n < (s + 86400 + 10*60) then
        sendRechargeRankAward_2013_04_17(player, pos, total, f7)
    elseif n >= (s + 86400 + 10*60) and n < (s + 2*86400 + 10*60) then
        sendRechargeRankAward_2014_06_01(player, pos, total, f7)
    elseif n >= (s + 2*86400 + 10*60) and n < (s + 3*86400 + 10*60) then
        sendRechargeRankAward_2014_05_26(player, pos, total, f7)
    elseif n >= (s + 3*86400 + 10*60) and n < (s + 4*86400 + 10*60) then
        sendRechargeRankAward_2014_02_04(player, pos, total, f7)
    elseif n >= (s + 4*86400 + 10*60) and n < (s + 5*86400 + 10*60) then
        sendRechargeRankAward_2013_04_18(player, pos, total, f7)
    elseif n >= (s + 5*86400 + 10*60) and n < (s + 6*86400 + 10*60) then
        sendRechargeRankAward_2013_04_19(player, pos, total, f7)
    elseif n >= (s + 6*86400 + 10*60) and n < (s + 7*86400 + 10*60) then
        sendRechargeRankAward_2013_03_29(player, pos, total, f7)
    end

 end

function sendConsumeRankAward_2012_10_19(player, pos)
    local items = {
        {509,30,1, 507,30,1, 1325,30,1, 134,30,1, 9189,1,0},
        {509,20,1, 507,20,1, 1325,30,1, 134,30,1},
        {509,15,1, 507,15,1, 1325,20,1, 134,20,1},
        {509,10,1, 507,10,1, 1325,10,1, 134,10,1},
        {509,10,1, 507,10,1, 1325,10,1, 134,10,1},
        {509,10,1, 507,10,1, 1325,10,1, 134,10,1},
        {509,10,1, 507,10,1, 1325,10,1, 134,10,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_113, pos)
    local ctx = string.format(msg_113, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendConsumeRankAward_2012_11_23(player, pos)
    local items = {
        {9076,23,1},
        {9076,18,1},
        {9076,13,1},
        {9076,8,1},
        {9076,8,1},
        {9076,8,1},
        {9076,8,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_113, pos)
    local ctx = string.format(msg_113, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendConsumeRankAward_2012_11_24(player, pos)
    local items = {
        {134,20,1, 1325,20,1},
        {134,16,1, 1325,16,1},
        {134,12,1, 1325,12,1},
        {134,8,1, 1325,8,1},
        {134,8,1, 1325,8,1},
        {134,8,1, 1325,8,1},
        {134,8,1, 1325,8,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_113, pos)
    local ctx = string.format(msg_113, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendConsumeRankAward_2012_11_25(player, pos)
    local items = {
        {509,10,1, 507,10,1, 515,10,1},
        {509,8,1, 507,8,1, 515,8,1},
        {509,6,1, 507,6,1, 515,6,1},
        {509,3,1, 507,3,1, 515,3,1},
        {509,3,1, 507,3,1, 515,3,1},
        {509,3,1, 507,3,1, 515,3,1},
        {509,3,1, 507,3,1, 515,3,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_113, pos)
    local ctx = string.format(msg_113, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendConsumeRankAward_2012_11_26(player, pos)
    local items = {
        {9076,15,1, 134,15,1},
        {9076,12,1, 134,12,1},
        {9076,8,1, 134,8,1},
        {9076,5,1, 134,5,1},
        {9076,5,1, 134,5,1},
        {9076,5,1, 134,5,1},
        {9076,5,1, 134,5,1},
    }

    if items[pos] == nil then
        return
    end

    local title = string.format(msg_113, pos)
    local ctx = string.format(msg_113, pos)
    sendItemPackageMail(player, title, ctx, items[pos]);
end

function sendConsumeRankAward(player, pos)
    local t = { ['year'] = 2012, ['month'] = 11, ['day'] = 23, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time()
    if n >= s and n < (s + 86400 + 10*60) then
        sendConsumeRankAward_2012_11_23(player, pos)
    elseif n >= (s + 86400 + 10*60) and n < (s + 2*86400 + 10*60) then
        sendConsumeRankAward_2012_11_24(player, pos)
    elseif n >= (s + 2*86400 + 10*60) and n < (s + 3*86400 + 10*60) then
        sendConsumeRankAward_2012_11_25(player, pos)
    elseif n >= (s + 3*86400 + 10*60) and n < (s + 4*86400 + 10*60) then
        sendConsumeRankAward_2012_11_26(player, pos)
    end
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
        {505,2,1, 512,2,1, 134,1,1},
        {8000,2,1, 516,1,1, 515,1,1, 509,2,1},
        {513,2,1, 9082,6,1},
        {503,2,1, 1325,1,1},
        {509,1,1, 507,1,1},
        {1325,2,1, 134,2,1, 515,2,1},
        {9076,3,1, 515,1,1},
        {9022,5,1, 549,2,1, 515,2,1},
        {9022,5,1, 515,3,1, 1325,2,1, 134,2,1},
        {9076,5,1, 515,5,1, 1325,1,1, 134,1,1},
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

function sendConsumeMails_2012_10_01(player, ototal, ntotal)
    local lvls = {
        10,99,399,899,1599,2599,4099,5699,7599,9999,15999,22999,31999,45999,69999,
    }    
    local items = {
        {503,1,1},
        {516,1,1},
        {517,1,1, 9088,1,1},
        {134,1,1},
        {549,1,1},
        {509,2,1, 507,2,1},
        {515,2,1},
        {1325,2,1},
        {507,3,1, 509,3,1},
        {515,3,1},
        {9076,5,1},
        {1325,3,1, 515,3,1},
        {134,3,1, 515,4,1},
        {9076,8,1, 9177,1,1},
        {9076,10,1, 9177,2,1},
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

function _sendConsumeMails(player, ototal, ntotal, lvls, items)
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

function sendConsumeMails_2012_10_19(player, ototal, ntotal)
    local lvls = {
        100,299,699,1199,1899,2699,3699,4999,6599,8599,11999,15999,21999,29999,39999,
    }    
    local items = {
        {503,2,1},
        {500,1,1, 517,1,1},
        {15,2,1},
        {547,2,1, 515,1,1, 1325,2,1},
        {505,2,1, 512,2,1, 134,1,1},
        {516,1,1, 515,1,1, 509,2,1},
        {513,2,1, 551,2,1},
        {503,2,1, 134,2,1},
        {509,2,1, 507,2,1},
        {1325,2,1, 134,2,1, 515,2,1},
        {9076,3,1},
        {549,2,1, 515,2,1},
        {9076,5,1},
        {515,5,1, 134,5,1},
        {9076,20,1},
    }
    _sendConsumeMails(player, ototal, ntotal, lvls, items)
end

function sendConsumeMails_2012_11_23(player, ototal, ntotal)
    local lvls = {
        99,599,1999,4999,9999,
    }    
    local items = {
        {500,2,1},
        {549,1,1},
        {134,2,1, 1325,2,1},
        {515,5,1},
        {9076,8,1},
    }
    _sendConsumeMails(player, ototal, ntotal, lvls, items)
end

function sendConsumeMails_2012_11_24(player, ototal, ntotal)
    local lvls = {
        99,599,1999,4999,9999,
    }    
    local items = {
        {505,2,1},
        {512,2,1, 501,2,1},
        {547,2,1, 551,3,1, 513,3,1},
        {509,3,1, 507,3,1},
        {134,8,1, 1325,8,1},
    }
    _sendConsumeMails(player, ototal, ntotal, lvls, items)
end

function sendConsumeMails_2012_11_25(player, ototal, ntotal)
    local lvls = {
        99,599,1999,4999,9999, 
    }    
    local items = {
        {50,1,1},
        {548,33,1},
        {500,3,1, 516,2,1, 517,2,1},
        {9093,10,1, 515,3,1},
        {33,50,1},
    }
    _sendConsumeMails(player, ototal, ntotal, lvls, items)
end

function sendConsumeMails_2012_11_26(player, ototal, ntotal)
    local lvls = {
        99,599,1999,4999,9999,
    }    
    local items = {
        {551,1,1},
        {515,1,1},
        {509,1,1, 507,1,1},
        {1325,3,1, 134,3,1},
        {515,8,1},
    }
    _sendConsumeMails(player, ototal, ntotal, lvls, items)
end

function sendConsumeMails_2012_12_15(player, ototal, ntotal)
    local lvls = {
        100,299,699,1199,1899,2699,3699,4999,6599,8599,11999,15999,21999,29999,39999,
    }    
    local items = {
        {503,2,1},
        {500,1,1, 517,1,1},
        {15,2,1},
        {547,2,1, 515,1,1, 1325,2,1},
        {505,2,1, 512,2,1, 134,1,1},
        {516,1,1, 515,1,1, 509,2,1},
        {513,2,1, 551,2,1},
        {503,2,1, 134,2,1},
        {509,2,1, 507,2,1},
        {1325,2,1, 134,2,1, 515,2,1},
        {9076,3,1},
        {549,2,1, 515,2,1},
        {9076,5,1},
        {515,5,1, 134,5,1},
        {9076,20,1},
    }
    _sendConsumeMails(player, ototal, ntotal, lvls, items)
end

function sendConsumeMails_2013_01_01(player, ototal, ntotal)
    local lvls = {
        100,299,699,1199,1899,2699,3699,4999,6599,8599,11999,15999,20999,26999,33999,41999,50999,
    }
    local items = {
        {503,2,1},
        {500,1,1, 517,1,1},
        {15,2,1},
        {547,2,1, 515,1,1, 1325,2,1},
        {505,2,1, 512,2,1, 134,1,1},
        {516,1,1, 515,1,1, 509,2,1},
        {513,2,1, 551,2,1},
        {503,2,1, 134,2,1},
        {509,2,1, 507,2,1},
        {1325,2,1, 134,2,1, 515,2,1},
        {9076,3,1},
        {549,2,1, 515,2,1},
        {9076,5,1},
        {515,6,1},
        {9076,8,1},
        {1325,5,1, 134,5,1},
        {515,10,1},
    }
    _sendConsumeMails(player, ototal, ntotal, lvls, items)
end

function sendConsumeMails(player, ototal, ntotal)
    local t = { ['year'] = 2013, ['month'] = 1, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11
    if n >= s and n < (s + 7*86400) then
        --sendConsumeMails_2012_12_15(player, ototal, ntotal)
        sendConsumeMails_2013_01_01(player, ototal, ntotal)
    end
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
        {{500, 1, 10}, {15, 1, 10}, {30, 1, 10}},
        {{56, 1, 20}, {9123, 1, 20}, {57, 1, 20}},
        {{9371, 1, 20}, {9424, 1, 30}, {517, 1, 30}},
        {{9122, 1, 10}, {9122, 1, 10}, {9122, 1, 10}},
        {{503, 1, 30}, {513, 1, 30}, {501, 1, 30}},
        {{509, 1, 40}, {507, 1, 40}, {515, 1, 40}},
        {{16047, 1, 50}, {16047, 1, 50}, {16047, 1, 50}},
    }

    step = math.random(1, 3)
    pos2 = pos + step
    if pos2 > 24 then
        pos2 = pos2 - 24
    end

    local package = player:GetPackage()
    i = roamPlace[pos2]
    j = math.random(1, #eventItem[i])

    --[[if eventItem[i][j][1] == 9896 then
        player:GetCollectCard():Add61Card(305)
    end
    if eventItem[i][j][1] == 9897 then
        player:GetCollectCard():Add61Card(306)
    end
    if eventItem[i][j][1] == 9898 then
        player:GetCollectCard():Add61Card(307)
    end
    if eventItem[i][j][1] == 9899 then
        player:GetCollectCard():Add61Card(308)
    end]]

    package:Add(eventItem[i][j][1], eventItem[i][j][2], true, true, 32)
    player:lastQueqiaoAwardPush(eventItem[i][j][1], eventItem[i][j][2]);
    player:postRoamResult(pos2, j, eventItem[i][j][3]);

    return pos2;
end
function onRoamingGuangGun(player, pos)
    local roamPlace = {
        1,5,13,8,10,3,11,12,2,9,10,1,
        3,2,1,9,10,1,6,5,9,11,     
    }

    local eventItem = {
        [1]={3,2,2,3,2,5,3},--炼器 
        [2]={3,2,5},--技能
        [3]={1},--帮派
        [4]={1,1,1,2},--灵宠
        [5]={1,1},--锁妖塔
        [6]={3,2},--上古龙界
        [7]={5,2,2,3},--墨侠
        [8]={3},--许愿树
        [9]={2,3},--斗劍
        [10]={3},--挂机
        [11]={1,1,2},--人物
        [12]={3},--九疑鼎
        [13]={1},--修炼
    }

    step = math.random(1, 6)
    pos2 = pos + step
    if pos2 > #roamPlace then
        pos2 = pos2 - #roamPlace;
    end

    i = roamPlace[pos2]
    j = math.random(1, #eventItem[i])
    if (i == 4 or i == 7 )and player:GetLev() < 70 then
        j = #eventItem[i]; 
    end
    if i == 6 and player:GetLev() < 50 then
        j = #eventItem[i]; 
    end

    local tasknum =0 ;
    for k=1,i-1 do
        tasknum=tasknum+#eventItem[k];
    end
    tasknum=tasknum+j;
    player:setGuangGunTask( tasknum, eventItem[i][j]);
    return pos2;
end
function onNewRoamingGuangGun(player, pos)
    local roamPlace = {
        14,5,13,8,10,1,11,1,2,9,4,14,
        3,2,7,9,10,1,13,4,6,7,     
    }

    local eventItem = {
        [1]={3,2,2,3,2,5,3},--炼器 
        [2]={3,2,5},--技能
        [3]={1},--帮派
        [4]={1,1,1,2},--灵宠
        [5]={1,1},--锁妖塔
        [6]={3,2},--上古龙界
        [7]={5,2,2,3},--墨侠
        [8]={3},--许愿树
        [9]={2,3},--斗劍
        [10]={3},--挂机
        [11]={1,1,2},--人物
        [12]={3},--九疑鼎
        [13]={1},--修炼
        [14]={0},--卡牌
        [15]={0},--缺省
        [16]={0},--缺省
        [17]={0},--缺省
    }

    step = math.random(1, 6)
    pos2 = pos + step
    if pos2 > #roamPlace then
        pos2 = pos2 - #roamPlace;
    end

    i = roamPlace[pos2]
    rand = math.random(1, 4) - 1
    if i == 14 then
        i = i + rand
    end

    j = math.random(1, #eventItem[i])
    if (i == 4 or i == 7 )and player:GetLev() < 70 then
        j = #eventItem[i]; 
    end
    if i == 6 and player:GetLev() < 50 then
        j = #eventItem[i]; 
    end

    local tasknum =0 ;
    for k=1,i-1 do
        tasknum=tasknum+#eventItem[k];
    end
    tasknum=tasknum+j;
    --player:setGuangGunTask( tasknum, eventItem[i][j]);
    player:setGuangGunTask( tasknum, 0);
    return pos2;
end



-- 1:聊天 2:避开 3:遇险 4:糖果 5:奇观 6:击杀 7:补给站
function onRoamingWansheng(player, pos)
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
        {{9194, 1, 10}, {9194, 1, 10}, {9194, 1, 10}},
        {{503, 1, 30}, {514, 1, 30}, {501, 1, 30}},
        {{509, 1, 40}, {134, 1, 40}, {1325, 1, 40}},
        {{509, 1, 50}, {507, 1, 50}, {515, 1, 50}},
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

-- 1:聊天 2:牵手 3:危机 4:蜀山之恋 5:名胜 6:心动 7:宝箱
function onRoamingQingRen(player, pos)
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
        {{9355, 1, 10}, {9355, 1, 10}, {9355, 1, 10}},
        {{503, 1, 30}, {514, 1, 30}, {501, 1, 30}},
        {{516, 1, 40}, {509, 1, 40}, {507, 1, 40}},
        {{515, 1, 50}, {134, 1, 50}, {1325, 1, 50}},
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

function Wansheng(player, lootlvl)
    if getWansheng() then
        -- 糖果
        local package = player:GetPackage();
        package:AddItem(9194, 1, true, 0, 10);
    end
end

function Qingren(player, lootlvl)
    if getQingren() then
        -- 蜀山之恋
        local package = player:GetPackage();
        package:AddItem(9355, 1, true, 0, 10);
    end
end

--仙宠仙缘石掉落
function fairyPetLoot(player, lootlvl)
    local package = player:GetPackage();
    local num = 1;
    if getHalfGold() then
        num = 2;
    end
    if getNationalDayHigh() then
        num = num * 2
    end
    package:AddItem(9371, num, true, 0, 10);
end

--愚公宝箱掉落
function FoolBaoLoot(player,lootlvl)
   if not getFoolBao() then
             return
   end
    if lootlvl > 3 then
        lootlvl = 0
    end
    local itemNum = {
            [0] = 1,
            [1] = 1,
            [2] = 2,
            [3] = 3,
    };
    local package = player:GetPackage();
    package:AddItem(9375, itemNum[lootlvl], true,0,10);
end
---欢乐爆竹掉落
function HappyFireLoot(player,lootlvl)
   if not getHappyFireTime() then
       return
   end
    if lootlvl > 3 then
        lootlvl = 0
    end
    local itemNum = {
            [0] = 1,
            [1] = 1,
            [2] = 1,
            [3] = 1,
    };
    local package = player:GetPackage();
    --package:AddItem(9458, itemNum[lootlvl], true,0,10); --欢乐礼包
    --package:AddItem(16048, itemNum[lootlvl], true,0,10); --欢乐礼包
    package:AddItem(16059, itemNum[lootlvl], true,0,10); --欢乐礼包
end

--欢乐爆竹掉落
function WorldCupLoot(player,lootlvl)
    if not ( getWorldCupTime() or getWorldCupTime2() )then
        return
    end
    if lootlvl > 3 then
        lootlvl = 0
    end
    local itemNum = {
        [0] = 1,
        [1] = 1,
        [2] = 1,
        [3] = 1,
    };
    local package = player:GetPackage();
    package:AddItem(16017, itemNum[lootlvl], true,0,10); --欢乐礼包
end
--喜从天降
function XCTJLoot(player,lootlvl)
    if not getXCTJTime() then
        return
    end
    if lootlvl > 3 then
        lootlvl = 0
    end
    local itemNum = {
        [0] = 1,
        [1] = 1,
        [2] = 1,
        [3] = 1,
    };
    local package = player:GetPackage();
    package:AddItem(16058, itemNum[lootlvl], true,0,10); --欢乐礼包
end

function GGLoot(player)
    if getGGTime() < 2 then
        return
    end
    local package = player:GetPackage();
    package:AddItem(16021, 1, true,0,10); --欢乐礼包
end

function FlyRoadLoot(player)
    if not getFlyRoadActivity() then
        return
    end
    local package = player:GetPackage();
    package:AddItem(16042, 1, true,0,10); --玉灵仙碑
end

function DropActLoot(player,lootlvl)
    if not getDropAct() then
        return
    end
    if lootlvl > 3 then
        lootlvl = 0
    end
    local itemNum = {
        [0] = 1,
        [1] = 1,
        [2] = 1,
        [3] = 1,
    };
    local package = player:GetPackage();
    package:Add(16051, itemNum[lootlvl], true,0,10);
end

--蜀山传奇掉落活动
function SurnameLegendLoot(player,lootlvl)
    DropActLoot(player,lootlvl)
    if (not getSurnameLegend()) and (not getSurnameLegend2()) then
        return
    end
    if lootlvl > 3 then
        lootlvl = 0
    end
    local itemNum = {
        [0] = 1,
        [1] = 1,
        [2] = 1,
        [3] = 1,
    };
    local package = player:GetPackage();
    if getSurnameLegend() then
        package:AddItem(16050, itemNum[lootlvl], true,0,10);
    else
        package:AddItem(16010, itemNum[lootlvl], true,0,10);
    end
end

-- 万圣节套装
function onWansheng(player)
    if not getWansheng() then
        return
    end

    local lvl = player:GetLev()
    if lvl < 40 then
        return
    end

    if lvl >= 40 and player:GetVar(198) == 0 then
        sendItemPackageMail(player, msg_51, msg_52, {1761,1,1});
        player:SetVar(198, 1)
    end
end

function Guoqing(player, lootlvl)
    if getGuoqing() then
        if lootlvl > 3 then
            return;
        end
        local itemNum = {
            [0] = 1,
            [1] = 1,
            [2] = 1,
            [3] = 2,
        };
        -- 月饼
        local package = player:GetPackage();
        package:AddItem(9179, itemNum[lootlvl], true, 0, 10);
    end
end

function onGetYearActAward(player, type)
    if type ==1 then
        local package = player:GetPackage()
        if package:GetRestPackageSize() < 9 or package:GetRestPackageSize(3) < 1 then
            return false
        end
        player:getCoupon(50)
        package:Add(56, 5, true)
        package:Add(57, 5, true)
        package:Add(15, 5, true)
        package:Add(49, 1, true)
        package:Add(502, 1, true)
        package:Add(510, 1, true)
        package:Add(5031, 1, true)
        package:Add(9163, 1, true)
        return true
    end
    if type ==2 then
        local package = player:GetPackage()
        if package:GetRestPackageSize() < 9 then
            return false
        end
        package:Add(509, 1, true)
        package:Add(503, 1, true)
        package:Add(514, 1, true)
        package:Add(1325, 1, true)
        package:Add(15, 5, true)
        package:Add(51, 1, true)
        package:Add(48, 1, true)
        package:Add(49, 2, true)
        package:Add(50, 2, true)
        return true
    end
end

function onGetKillMonsterReward(player)
    local roamPlace = {
        -- 1  2  3  4  5  6  7  8
        1, 2, 3, 4, 5, 6, 7, 8,
        9, 10, 1, 3, 11, 12, 8, 2,
        6, 13, 1, 14, 8, 3, 13, 15,
        1, 3, 8, 16,
    }

    local eventItem = {
        --{物品ID，物品数，剑侠（或柔情、或财富、或传奇），剑侠个数}
        --除魔 卫道  正义  侠义
        --幸福 开心  伤心  愤怒
        {{505,  1, 1, 1}, {15,  1, 0, 0}, {500,  1, 0, 0}},
        {{9390, 1, 0, 0}, {57,    1, 0, 0}, {56,  1, 1, 1}},
        {{56,   1, 1, 1}, {511,  1, 0, 0}, {512,  1, 0, 0}},
        {{514, 1, 0, 0}, {9371,  1, 0, 0}, {517, 1, 3, 1}},
        {{56,   1, 0, 0}, {466,    1, 0, 0}, {503, 1, 3, 1}},
        {{465,   1, 2, 1}, {33,  1, 0, 0}, {9308,  1, 0, 0}},
        {{9390, 1, 0, 0}, {517,   1, 0, 0}, {8000, 1, 2, 1}},
        {{511,  1, 3, 1}, {15,   1, 0, 0}, {501,  1, 0, 0}},
        {{500,  1, 4, 1}, {33,    1, 0, 0}, {503,  1, 0, 0}},
        {{1325, 1, 0, 0}, {500,   1, 0, 0}, {551,  1, 3, 1}},
        {{9424, 1, 0, 0}, {9418,   1, 0, 0}, {8000, 1, 2, 1}},
        {{503,  1, 4, 1}, {514,   1, 0, 0}, {512,  1, 0, 0}},
        {{503,  1, 3, 1}, {33,    1, 0, 0}, {1126,  1, 0, 0}},
        {{503, 1, 0, 0}, {9600,   1, 0, 0}, {1126, 1, 2, 1}},
        {{134,  1, 0, 0}, {9457,  1, 0, 0}, {1325, 1, 2, 1}},
        {{17018, 1, 4, 1}, {17019,  1, 0, 0}, {17020, 1, 0, 0}, {17021, 1, 0, 0}},
    }

    local pos = player:GetVar(182)
    local step = math.random(1, 3)
    local pos2 = pos + step
    --if pos2 > 28 then
    --    pos2 = pos2 - 28
    --end
    --player:SetVar(182, pos2)

    local posTmp = pos2 % 28
    if posTmp == 0 then
        posTmp  = 28
    end

    local package = player:GetPackage()
    local i = roamPlace[posTmp]
    local j = math.random(1, #eventItem[i])

    if eventItem[i][j][1] == 17018 then
        player:GetCollectCard():AddCelebrationCard(6005)
    elseif eventItem[i][j][1] == 17019 then
        player:GetCollectCard():AddCelebrationCard(6006)
    elseif eventItem[i][j][1] == 17020 then
        player:GetCollectCard():AddCelebrationCard(6007)
    elseif eventItem[i][j][1] == 17021 then
        player:GetCollectCard():AddCelebrationCard(6008)
    end
    package:Add(eventItem[i][j][1], eventItem[i][j][2], true, true, 33)
    player:lastKillMonsterAwardPush(eventItem[i][j][1], eventItem[i][j][2]);
    if eventItem[i][j][3] >=1 and eventItem[i][j][3] <= 4 then
        local curType = eventItem[i][j][3]
        local extAward = 0
        if curType == 1 and player:GetVar(183) == (1-1) then
            extAward = 1766
        elseif curType == 2 and player:GetVar(184) == (1-1) then
            extAward = 1767
        elseif curType == 3 and player:GetVar(185) == (1-1) then
            extAward = 1768
        elseif curType == 4 and player:GetVar(186) == (1-1) then
            extAward = 1769
        elseif curType == 1 and player:GetVar(183) == (5-1) then
            extAward = 1770
        elseif curType == 2 and player:GetVar(184) == (3-1) then
            extAward = 1771
        elseif curType == 3 and player:GetVar(185) == (4-1) then
            extAward = 1772
        elseif curType == 4 and player:GetVar(186) == (2-1) then
            extAward = 1773

        elseif curType == 1 and player:GetVar(183) == (10-1) then
            if player:GetVar(184) >= 10 and player:GetVar(185) >= 10 and player:GetVar(186) >= 10 then
                extAward = 16040
            end
        elseif curType == 2 and player:GetVar(184) == (10-1) then
            if player:GetVar(183) >= 10 and player:GetVar(185) >= 10 and player:GetVar(186) >= 10 then
                extAward = 16040
            end
        elseif curType == 3 and player:GetVar(185) == (10-1) then
            if player:GetVar(183) >= 10 and player:GetVar(184) >= 10 and player:GetVar(186) >= 10 then
                extAward = 16040
            end
        elseif curType == 4 and player:GetVar(186) == (10-1) then
            if player:GetVar(183) >= 10 and player:GetVar(184) >= 10 and player:GetVar(185) >= 10 then
                extAward = 16040
            end
        end

        if extAward ~= 0 then
            --package:Add(extAward, 1, true, true, 33)
            --player:lastKillMonsterAwardPush(extAward, 1);
            player:luaUdpLog("916", "F_1099_"..curType, "act")
            sendItemPackageMail(player, msg_120, msg_121, {extAward,1,1})
        end
    end
    player:postKillMonsterRoamResult(pos2, eventItem[i][j][3], eventItem[i][j][4], j);

    return pos2;
end

--蓝钻优惠活动(购买蓝砖超人)
function GetBDSupermanPrice(player, itemId, isUsed)
    if nil == player or nil == itemId
        then
            return 0
        end
        if isUsed then
            --if player:isBD()
            if player:GetPackage():ExistItem(0xFFFF)
                then
                    itemId = itemId * 10 + 1
                else
                    itemId = itemId * 10
                end
            else
                itemId = itemId * 10
            end
            local items =
            {
                [17070] = 599; --原价
                [17071] = 399;
            }
            if nil == items[itemId]
                then
                    return 0
                end
                player:GetPackage():DelItemSendMsg(0xFFFF, player)
                player:GetPackage():DelItem(0xFFFF, 1, 1) 
                return items[itemId]
            end

            function ExJob(player, id)
                if id >= 4 then
                    local package = player:GetPackage()
                    if getNationalDayHigh() then
                        package:AddItem(9229, 2, true)
                    else
                        package:AddItem(9229, 1, true)
                    end
                end
            end

            function onFirstRecharge(player, index)
                if index == 0 or index > 8 then
                    return false
                end

                local items = {
                    {{56, 5}, {57, 2}, {503, 5}, {514, 10}, {515, 2}, {507, 2}, {509, 2}, {15, 3}, {517, 2}},
                    {{56, 10}, {57, 4}, {503, 10}, {514, 20}, {515, 4}, {507, 4}, {509, 4}, {15, 6}, {517, 4}},

                    --{{2544, 3}}, --{2545, 1}, {2546, 1}, {2547, 1}, {2548, 1}, {2549, 1}, {2550, 1}, {2551, 1}, {1600, 1}},
                    --{{2712, 7}}, --{2713, 1}, {2714, 1}, {2715, 1}, {2716, 1}, {2717, 1}, {2718, 1}, {2719, 1}, {1608, 1}},
                    {{2544, 1}, {2545, 1}, {2546, 1}, {2547, 1}, {2548, 1}, {2549, 1}, {2550, 1}, {2551, 1}, {502, 20}},
                    {{2712, 1}, {2713, 1}, {2714, 1}, {2715, 1}, {2716, 1}, {2717, 1}, {2718, 1}, {2719, 1}, {502, 99}},

                    --{{2552, 3}}, --{2553, 1}, {2554, 1}, {2555, 1}, {2556, 1}, {2557, 1}, {2558, 1}, {2559, 1}, {1601, 1}},
                    --{{2720, 7}}, --{2721, 1}, {2722, 1}, {2723, 1}, {2724, 1}, {2725, 1}, {2726, 1}, {2727, 1}, {1609, 1}},
                    {{2552, 1}, {2553, 1}, {2554, 1}, {2555, 1}, {2556, 1}, {2557, 1}, {2558, 1}, {2559, 1}, {502, 20}},
                    {{2720, 1}, {2721, 1}, {2722, 1}, {2723, 1}, {2724, 1}, {2725, 1}, {2726, 1}, {2727, 1}, {502, 99}},

                    --{{2560, 3}}, --{2561, 1}, {2562, 1}, {2563, 1}, {2564, 1}, {2565, 1}, {2566, 1}, {2567, 1}, {1602, 1}},
                    --{{2728, 7}}, --{2729, 1}, {2730, 1}, {2731, 1}, {2732, 1}, {2733, 1}, {2734, 1}, {2735, 1}, {1610, 1}},
                    {{2560, 1}, {2561, 1}, {2562, 1}, {2563, 1}, {2564, 1}, {2565, 1}, {2566, 1}, {2567, 1}, {502, 20}},
                    {{2728, 1}, {2729, 1}, {2730, 1}, {2731, 1}, {2732, 1}, {2733, 1}, {2734, 1}, {2735, 1}, {502, 99}},
                }
                local item = items[index]

                local package = player:GetPackage()
                if package:GetRestPackageSize() < #item then
                    player:sendMsgCode(2, 1011, 0)
                    return false
                end

                for k, v in pairs(item) do
                    --if index < 3 then
                    package:Add(v[1], v[2], 1)
                    --else
                    --    package:AddEquipEnchant(v[1], v[2], true, true, 55)
                    --end

                end

                if index <= 2 then
                    Broadcast(0x27, "[p:"..player:getCountry()..":"..player:getPName().."]"..msg_137)
                end

                return true
            end

            local equipTrump1 = {
                -- 副本
                -- level 30
                [776] = {
                    -- 装备或法宝
                    [1] = {{2376,30},{2377,30},{2378,30},{2379,30},{2380,30},{2381,30},{2382,30},{2383,30},{2384,30},{2385,30},{2386,30},{2387,30},{2388,30},{2389,30},{2390,30},{2391,30},{2392,30},{2393,30},{2394,30},{2395,30},{2396,30},{2397,30},{2398,30},{2399,30},},
                    -- 心法
                    [2] = {{1208,20},},
                },
                -- level 45
                [2067] = {
                    [1] = {{2544,80},{2545,80},{2546,80},{2547,80},{2548,80},{2549,80},{2550,80},{2551,80},{2552,80},{2553,80},{2554,80},{2555,80},{2556,80},{2557,80},{2558,80},{2559,80},{2560,80},{2561,80},{2562,80},{2563,80},{2564,80},{2565,80},{2566,80},{2567,80},},
                    [2] = {{1235,20},{1310,20},},
                },
                -- level 60
                [5906] = {
                    [1] = {{2568,30},{2569,30},{2570,30},{2571,30},{2572,30},{2573,30},{2574,30},{2575,30},{2576,30},{2577,30},{2578,30},{2579,30},{2580,30},{2581,30},{2582,30},{2583,30},{2584,30},{2585,30},{2586,30},{2587,30},{2588,30},{2589,30},{2590,30},{2591,30},},
                    [2] = {{1240,20},{1313,20},},},
                    -- level 70
                    [8198] = {
                        [1] = {{2592,30},{2593,30},{2594,30},{2595,30},{2596,30},{2597,30},{2598,30},{2599,30},{2600,30},{2601,30},{2602,30},{2603,30},{2604,30},{2605,30},{2606,30},{2607,30},{2608,30},{2609,30},{2610,30},{2611,30},{2612,30},{2613,30},{2614,30},{2615,30},},
                        [2] = {{1416,10},{1417,10},{1418,10},{1419,10},{1420,10},{6056,10},},
                    },
                    -- level 80
                    [12818] = {
                        [1] = {{2616,30},{2617,30},{2618,30},{2619,30},{2620,30},{2621,30},{2622,30},{2623,30},{2624,30},{2625,30},{2626,30},{2627,30},{2628,30},{2629,30},{2630,30},{2631,30},{2632,30},{2633,30},{2634,30},{2635,30},{2636,30},{2637,30},{2638,30},{2639,30},},
                        [2] = {{1433,10},{1434,10},{1435,10},{1436,10},{1437,10},{1438,10},{1439,10},{6073,10},},
                    },
                    -- level 90
                    [10512] = {
                        [1] = {{2640,30},{2641,30},{2642,30},{2643,30},{2644,30},{2645,30},{2646,30},{2647,30},{2648,30},{2649,30},{2650,30},{2651,30},{2652,30},{2653,30},{2654,30},{2655,30},{2656,30},{2657,30},{2658,30},{2659,30},{2660,30},{2661,30},{2662,30},{2663,30},},
                        [2] = {{1491,10},{1492,10},{1493,10},{1494,10},{1495,10},{1496,10},{1497,10},{1498,10},{1499,10},{6103,10},},
                    },
                    -- level 100
                    [5137] = {
                        [1] = {{2664,30},{2665,30},{2666,30},{2667,30},{2668,30},{2669,30},{2670,30},{2671,30},{2672,30},{2673,30},{2674,30},{2675,30},{2676,30},{2677,30},{2678,30},{2679,30},{2680,30},{2681,30},{2682,30},{2683,30},{2684,30},{2685,30},{2686,30},{2687,30},},
                        [2] = {{6039,10},{6040,10},{6041,10},{6042,10},{6043,10},{6044,10},{6045,10},{6046,10},{6047,10},{6048,10},{6049,10},{6050,10},{6051,10},{6052,10},{6137,10},{6138,10},{6139,10},{6140,10},{6141,10},{6142,10},{6143,10},{6144,10},{6145,10},},
                    },
                    -- level 110
                    [9991] = {
                        [1] = {{2936,30},{2937,30},{2938,30},{2939,30},{2940,30},{2941,30},{2942,30},{2943,30},{2944,30},{2945,30},{2946,30},{2947,30},{2948,30},{2949,30},{2950,30},{2951,30},{2952,30},{2953,30},{2954,30},{2955,30},{2956,30},{2957,30},{2958,30},{2959,30},},
                        [2] = {{6146,10},{6147,10},{6148,10},{6149,10},{6150,10},{6151,10},{6152,10},{6153,10},{6154,10},{6155,10},{6156,10},{6157,10},{6158,10},{6159,10},{6160,10},{6161,10},{6162,10},{6163,10},{6164,10},{6165,10},{6166,10},{6167,10},{6168,10},},
                    },
                    -- level 120
                    [10506] = {
                        [1] = {{3000,30},{3001,30},{3002,30},{3003,30},{3004,30},{3005,30},{3006,30},{3007,30},{3008,30},{3009,30},{3010,30},{3011,30},{3012,30},{3013,30},{3014,30},{3015,30},{3016,30},{3017,30},{3018,30},{3019,30},{3020,30},{3021,30},{3022,30},{3023,30},},
                        [2] = {{6170,10},{6171,10},{6172,10},{6173,10},{6174,10},{6175,10},{6176,10},{6177,10},{6178,10},{6179,10},{6180,10},{6181,10},{6182,10},{6183,10},{6184,10},{6185,10},{6186,10},{6187,10},{6188,10},{6189,10},{6190,10},{6191,10},{6192,10},},
                    },
                    -- level 130
                    [4871] = {
                        [1] = {{3064,30},{3065,30},{3066,30},{3067,30},{3068,30},{3069,30},{3070,30},{3071,30},{3072,30},{3073,30},{3074,30},{3075,30},{3076,30},{3077,30},{3078,30},{3079,30},{3080,30},{3081,30},{3082,30},{3083,30},{3084,30},{3085,30},{3086,30},{3087,30},},
                        [2] = {{6196,10},{6197,10},{6198,10},{6199,10},{6200,10},{6201,10},{6202,10},{6203,10},{6204,10},{6205,10},{6206,10},{6207,10},{6208,10},{6209,10},{6211,10},{6212,10},{6213,10},{6214,10},{6215,10},{6216,10},{6217,10},{6218,10},{6219,10},},
                    },
                    -- level 140
                    [4628] = {
                        [1] = {{3128,30},{3129,30},{3130,30},{3131,30},{3132,30},{3133,30},{3134,30},{3135,30},{3136,30},{3137,30},{3138,30},{3139,30},{3140,30},{3141,30},{3142,30},{3143,30},{3144,30},{3145,30},{3146,30},{3147,30},{3148,30},{3149,30},{3150,30},{3151,30},},
                        [2] = {{6230,10},{6231,10},{6232,10},{6233,10},{6234,10},{6235,10},{6236,10},{6237,10},{6238,10},{6239,10},{6240,10},{6241,10},{6242,10},{6243,10},{6244,10},{6245,10},{6246,10},{6247,10},{6248,10},{6249,10},{6250,10},{6251,10},{6252,10},},
    },

    -- 阵图
    -- level 35
    [1284] = {
        [1] = {{1600,30},{1601,30},{1602,30},{1500,30},{1501,30},{1502,30},},
        [2] = {{1205,20},{1220,20},{1221,20},  {1116,40},{1118,40},{1119,40},{1120,40},{1121,40},{1122,40},{1123,40},{1124,40}},
    },
    -- level 45
    [2053] = {
        [1] = {{1604,30},{1605,30},{1606,30},{1503,30},},
        [2] = {{1231,20},{1224,20},{1308,20},  {1059,40},{1061,40},{1062,40},{1063,40},{1064,40}},
    },
    -- level 50
    [4360] = {
        [1] = {{1603,30},{1608,30},{1609,30},{1505,30},{1506,30},},
        [2] = {{1238,20},{1233,20},{1225,20},{1311,20},  {1107,40},{1109,40},{1110,40},{1111,40},{1112,40},{1113,40},{1114,40},{1115,40}},
    },
    -- level 55
    [4611] = {
        [1] = {{1610,30},{1614,30},{1615,30},{1507,30},{1508,30},},
        [2] = {{1246,20},{1229,20},{1243,20},{1315,20},  {1096,40},{1098,40},{1099,40},{1100,40},{1101,40},{1102,40},{1103,40},{1104,40},{1105,40},{1106,40}},
    },
    -- level 60
    [5893] = {
        [1] = {{1612,30},{1616,30},{1607,30},{1611,30},{1509,30},},
        [2] = {{1251,20},{1317,20},  {1084,40},{1086,40},{1087,40},{1088,40},{1089,40},{1090,40},{1091,40},{1092,40},{1093,40},{1094,40},{1095,40}},
    },
    -- level 65
    [5637] = {
        [1] = {{200,30},{201,30},{202,30},{203,30},{204,30},{205,30},{206,30},{207,30},{208,30},{82,30},{83,30},{84,30},{85,30},{86,30},{87,30},{88,30},{89,30},},
        [2] = {{1413,20},{1414,20},{1415,20},{1241,20},{1242,20},{6053,20},  {1074,40},{1076,40},{1077,40},{1078,40},{1079,40},{1080,40},{1081,40},{1082,40},{1083,40}},
    },
    -- level 70
    [8195] = {
        [1] = {{209,30},{210,30},{211,30},{212,30},{213,30},{214,30},{215,30},{216,30},{217,30},{218,30},{219,30},{220,30},{221,30},{222,30},{223,30},{224,30},{103,30},{104,30},{105,30},{106,30},{107,30},{108,30},{109,30},{110,30},},
        [2] = {{1421,10},{1422,10},{1423,10},{1424,10},{1425,10},{1426,10},{1253,20},{1250,20},{6061,10},  {1020,40},{1022,40},{1023,40},{1024,40},{1025,40},{1026,40},{1027,40},{1028,40},{1029,40},{1030,40}},
    },
    -- level 75
    [6153] = {
        [1] = {{228,30},{229,30},{230,30},{231,30},{232,30},{233,30},{234,30},{235,30},{236,30},{237,30},{238,30},{239,30},{240,30},{241,30},{242,30},{91,30},{92,30},{93,30},{94,30},{95,30},{96,30},{97,30},{98,30},{99,30},{100,30},{101,30},{102,30},},
        [2] = {{1427,10},{1428,10},{1429,10},{1430,10},{1431,10},{1432,10},{1245,20},{6067,10},  {1065,40},{1067,40},{1068,40},{1069,40},{1070,40},{1071,40},{1072,40},{1073,40}},
    },
    -- level 80
    [9222] = {
        [1] = {{243,30},{244,30},{245,30},{246,30},{247,30},{248,30},{249,30},{250,30},{251,30},{252,30},{253,30},{254,30},{255,30},{256,30},{257,30},{258,30},{259,30},{260,30},{261,30},{262,30},{263,30},{264,30},{265,30},{266,30},{267,30},{268,30},{269,30},},
        [2] = {{1440,10},{1441,10},{1442,10},{1443,10},{1444,10},{1445,10},{1446,10},{1247,20},{1249,20},{6080,10},  {1040,40},{1042,40},{1043,40},{1044,40},{1045,40},{1046,40},{1047,40},{1048,40},{1049,40},{1050,40},{1051,40}},
    },
    -- level 85
    [9481] = {
        [1] = {{271,30},{272,30},{273,30},{274,30},{275,30},{276,30},{277,30},{278,30},{279,30},{280,30},{281,30},{282,30},{283,30},{284,30},{285,30},{286,30},{287,30},{288,30},{289,30},{290,30},{291,30},{292,30},{293,30},{294,30},{295,30},{296,30},{297,30},},
        [2] = {{1447,10},{1448,10},{1449,10},{1450,10},{1451,10},{1452,10},{1453,10},{1254,20},{1255,20},{6087,10},  {1052,40},{1054,40},{1055,40},{1056,40},{1057,40},{1058,40}},
    },
    -- level 90
    [10244] = {
        [1] = {{120,30},{121,30},{122,30},{123,30},{124,30},{125,30},{126,30},{127,30},{128,30},},
        [2] = {{1482,10},{1483,10},{1484,10},{1485,10},{1486,10},{1487,10},{1488,10},{1489,10},{1490,10},{1468,10},{1469,10},{1470,10},{1471,10},{1472,10},{1473,10},{1474,10},{1475,10},{1476,10},{1477,10},{1478,10},{1479,10},{1480,10},{1481,10},{6094,10},  {1031,40},{1033,40},{1034,40},{1035,40},{1036,40},{1037,40},{1038,40},{1039,40}},
    },
    -- level 95
    [5129] = {
        [1] = {{299,30},{300,30},{301,30},{302,30},{303,30},{304,30},{305,30},{306,30},{307,30},},
        [2] = {{6011,10},{6012,10},{6013,10},{6014,10},{6015,10},{6016,10},{6017,10},{6018,10},{6019,10},{6020,10},{6021,10},{6022,10},{6023,10},{6024,10},{6025,10},{6026,10},{6027,10},{6028,10},{6029,10},{6030,10},{6031,10},{6032,10},{6033,10},{6112,10},  {1012,40},{1014,40},{1015,40},{1016,40},{1017,40},{1018,40},{1019,40}},
    },
}
local common2 = {{50,20},{49,20},{514,20},{135,20},{511,20},{1412,20},{1411,20},}
local common3 = {{9283,40},}
local gem3= {{5001,40},{5011,40},{5021,40},{5031,40},{5041,40},{5051,40},{5061,40},{5071,40},{5081,40},{5091,40},{5101,40},{5111,40},{5121,40},{5131,40},{5141,40},}
local extra1 = {{50,20},{49,20},{135,20},{1411,20},{507,1},{509,1},}
local extra_2 = {{50,20},{49,20},{135,20},{1411,20},}
local item = {}

local dungeonAward1 = {
    --level 30
    [772] = {{1201,10},{1214,10},{1213,10},},
    --level 45
    [2050] = {{1226,10},{1222,20},{1223,20},{1306,10},},
    --level 60
    [5123] = {{1230,20},{1227,10},{1234,10},{1307,20},},
    --level 75
    [8194] = {{1236,10},{1237,10},},
    --level 90
    [10001] = {{1454,20},{1455,20},{1456,20},{1457,20},{1458,20},{1459,20},{1460,20},{1461,20},{1462,20},{1463,20},{1464,20},{1465,20},{1466,20},{1467,20},},
}
local dungeonAward2 = {{50,25},{49,20},{135,20},{1411,20},}
local dungeonAward3 = {{507,1},{509,1},{9283,60},{50,25},{9420,5},}
local dungeonAward1_common = {{135,20},{1411,20},{500,20},{56,30},{57,30},{9283,60},{50,25}}

function getDungeonAward(step, localtion)
    local order
    if step == 1 then
        local items = dungeonAward1[localtion];
        if items == nil then
            return {}
        end
        local cnt1 = #items
        local cnt1_common = #dungeonAward1_common
        order = math.random(1, cnt1 + cnt1_common)
        if order <= cnt1 then
            item = items[order]
        else
            item = dungeonAward1_common[order - cnt1]
        end
    elseif step == 2 then
        order = math.random(1, #dungeonAward2)
        item = dungeonAward2[order]
    else
        local items = dungeonAward1[localtion];
        if items == nil then
            return {}
        end
        local cnt1 = #items
        local cnt1_common = #dungeonAward1_common
        local cnt2 = #dungeonAward2
        local cnt3 = #dungeonAward3
        order = math.random(1, cnt1 + cnt1_common + cnt2 + cnt3)
        if order <= cnt1 then
            item = items[order]
        elseif order <= cnt1 + cnt1_common then
            item = dungeonAward1_common[order - cnt1]
        elseif order <= cnt1 + cnt1_common + cnt2 then
            item = dungeonAward2[order - cnt1 - cnt1_common]
        else
            item = dungeonAward3[order - cnt1 - cnt1_common - cnt2]
        end
    end
    return item;
end

function getCopyFrontmapAward(step, localtion, cf)
    if cf == 3 then
        return getDungeonAward(step, localtion)
    end

    if step > 2 then
        return {}
    end

    local order
    if step == 1 then
        if true then
            if cf == 1 then
                if math.random(1, 100) <= 90 then
                    order = 1
                else
                    order = 2
                end
            else
                if math.random(1, 100) <= 50 then
                    order = 1
                else
                    order = 2
                end
            end
            local items_1 = equipTrump1[localtion];
            if items_1 == nil then
                return {}
            end
            local items = items_1[order]
            if items == nil then
                return {}
            end
            order = math.random(1, #items)
            item = items[order]
            return item;
        else
            order = math.random(1, #extra1)
            item = extra1[order]
            return item;
        end
    elseif step == 2 then
        order = math.random(1, #common2)
        item = common2[order]
        return item;
    else
        if cf == 1 then
            order = math.random(1, 14)
            if order <= 1 then
                local items_1 = equipTrump1[localtion];
                if items_1 == nil then
                    return {}
                end
                local items = items_1[order]
                if items == nil then
                    return {}
                end
                order = math.random(1, #items)
                item = items[order]
                return item
            elseif order <= 5 then
                order = math.random(1, #extra_2)
                item = extra_2[order]
                return item
            elseif order <= 12 then
                order = math.random(1, #common2)
                item = common2[order]
                return item;
            elseif order <= 13 then
                order = math.random(1, #common3)
                item = common3[order]
                return item;
            else
                order = math.random(1, #gem3)
                item = gem3[order]
                return item;
            end
        else
            order = math.random(1, 14)
            if order <= 0 then
                local items_1 = equipTrump1[localtion];
                if items_1 == nil then
                    return {}
                end
                local items = items_1[order]
                if items == nil then
                    return {}
                end
                order = math.random(1, #items)
                item = items[order]
                return item
            elseif order <= 4 then
                order = math.random(1, #extra_2)
                item = extra_2[order]
                return item
            elseif order <= 12 then
                order = math.random(1, #common2)
                item = common2[order]
                return item;
            elseif order <= 13 then
                order = math.random(1, #common3)
                item = common3[order]
                return item;
            else
                order = math.random(1, #gem3)
                item = gem3[order]
                return item;
            end
        end
    end
end


--flag == 1,2,3
--1:大闹龙宫
--2:大闹龙宫之金蛇起舞
--3:大闹龙宫之天芒神梭
function checkDragonKingCanSucceed(player, step, flag)
    if nil == player or nil == flag then
        return false
    end
    if nil == step or step < 1 or step > 5 then
        return false
    end
    local chances = {
        [1] = 8000,
        [2] = 5000,
        [3] = 5000,
        [4] = {500, 1000, 6000, 9000},
        [5] = 10000,
    }
    local rand = math.random(1, 10000)
    if step ~= 4 then
        if rand <= chances[step] then
            return true
        end
    else
        local fail = player:GetVar(362) + 1
        if fail > #chances[step] then
            fail = #chances[step]
        end
        if rand <= chances[step][fail] then
            player:SetVar(362, 0)
            return true
        end
        player:SetVar(362, fail)
    end
    return false
end

function getDragonKingAward(step, flag)
    local items = {
        [1] = {{16, 1}, {29, 2}, {1328, 5}, {51, 1}, {48, 1}, {35, 2}, {548, 1}, {30, 1}},
        [2] = {{15, 1}, {9283, 1}, {500, 1}, {57, 1}, {1412, 5}, {56, 1}, {135, 1}, {30, 1}},
        [3] = {{33, 1}, {516, 1}, {501, 1}, {506, 1}, {508, 1}, {512, 1}, {513, 1}, {514, 1}, {517, 1}, {1411, 1}},
        [4] = {{1325, 1}, {134, 1}, {551, 1}, {8000, 1}, {47, 1}, {515, 1}, {549, 1}, {50, 1}},
        [5] = {
            [1] = {{6134,1}},
            [2] = {{6135,1}},
            [3] = {{136,1}},
            [4] = {{6136,1}},
            [5] = {{1357,1}},
            [6] = {{137,1}},
            [7] = {{1362,1}},
            [8] = {{139,1}},
            [9] = {{8520,1}, {8521,1}, {8522,1}},
            [11] = {{140,1}},
            [12] = {{6193,1}},
            [13] = {{141,1}},
            [14] = {{6194,1}},
            [15] = {{312,1}},
            [16] = {{8550,1}, {8551,1}, {8552,1}},
            [17] = {{6210,1}},
            [18] = {{313,1}},
            [19] = {{6220,1}},
            [20] = {{314,1}},
            [21] = {{315,1}},
            [22] = {{317,1}},
            [23] = {{318,1}},
            [24] = {{6253,1}},
            [25] = {{17032,1}},
            [26] = {{319,1}},
        },
    }
    local chances = {
        [1] = {1400, 2800, 4200, 5600, 7000, 8400, 9800, 10000},
        [2] = {1400, 2800, 4200, 5600, 7000, 8400, 9800, 10000},
        [3] = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000},
        [4] = {1250, 2500, 3750, 5000, 6250, 7500, 8750, 10000},
        [5] = {10000},
    }
    if nil == step then
        return {}
    end
    if step < 1 or step > 5 then
        step = 1
    end
    if 5 == step then
        if nil == flag or nil == items[step][flag] then
            return {}
        end
    end
    local r = math.random(1, 10000)
    for i = 1, #chances[step] do
        if r <= chances[step][i] then
            if 5 == step then
                if flag == 9 or flag == 16 then  --大闹龙宫之元神出窍 天佑术
                    local chance = { 1400, 8400, 10000 }
                    local rnd = math.random(1, 10000)
                    for j = 1, #chance do
                        if rnd <= chance[j] then
                            return items[step][flag][j]
                        end
                    end
                end
                return items[step][flag][i]
            end
            return items[step][i]
        end
    end
    return {}
end

function getSaveGoldActAward(gold)
    if nil == gold then
        gold = 1
    end
    if nil == times then
        times = 1
    end
    local awards = {
        [1] = {{511, 1}, {548, 1}, {57, 1}, {29, 10}, {502, 1}, {48, 1}},
        [2] = {{56, 1}, {15, 1}, {499, 20}, {33, 1}, {547, 1}, {500, 1}},
        [3] = {{503, 1}, {501, 1}, {50, 1}, {551, 1}, {514, 1}, {499, 30}},
        [4] = {{515, 1}, {47, 1}, {1325, 1}, {516, 1}, {499, 50}, {8000, 1}},
        [5] = {{509, 2}, {134, 2}, {1325, 2}, {515, 2}, {507, 2}, {499, 80}},
        [6] = {{1325, 3}, {134, 3}, {515, 3}, {509, 3}, {507, 3}, {499, 100}},
        [7] = {{515, 5}, {1325, 5}, {134, 5}, {509, 5}, {507, 5}, {499, 150}},
    }
    local golds = {1000, 10000, 30000, 50000, 80000, 100000}
    local idx = 0
    local num = #golds
    for i = 1, num do
        if gold < golds[i] then
            idx = i
            break
        end
    end
    if gold >= golds[num] then
        idx = num + 1
    end
    if idx <= 0 or idx > #awards then
        idx = 1
    end
    return awards[idx]
end

function getSaveGoldActExtraAward(gold)
    local extraAwards = {
        [1] = {{5023, 1},{5003, 1}},
        [2] = {{1325, 1}, {1411, 1}},
        [3] = {{134, 1}, {515, 1}},
        [4] = {{134, 3}, {515, 3}},
        [5] = {{1700, 1}, {8555, 1}},
        [6] = {{1700, 1}, {8555, 3}},
        [7] = {{1700, 1}, {8555, 5}},
    }
    local golds = {1000, 10000, 30000, 50000, 80000, 100000}
    local idx = 0
    local num = #golds
    for i = 1, num do
        if gold < golds[i] then
            idx = i
            break
        end
    end
    if gold >= golds[num] then
        idx = num + 1
    end
    return extraAwards[idx]
end

function onGetNewYearGiveGiftAward(player, index, times)
    if index == 0 or index > 8 then
        return false
    end

    local items = {
        {{512, 2}, {516, 2}, {466, 2}, {548, 5}, {9413, 2}},
        {{500, 2}, {503, 2}, {56, 2}, {15, 2}, {511, 2}},
        {{501, 2}, {517, 2}, {57, 2}, {9371, 2}, {512, 2}},
        {{551, 2}, {33, 2}, {505, 2}, {466, 2}, {548, 10}},

        {{516, 2}, {465, 2}, {50, 2}, {514, 2}, {15, 2}},
        {{547, 2}, {503, 2}, {8000, 2}, {9082, 2}, {9413, 2}},
        {{134, 2}, {509, 2}, {9424, 2}, {56, 2}, {9371, 2}},
        {{1325, 2}, {1126, 2}, {9604, 2}, {57, 2}, {15, 2}},
     --   {{503, 2}, {1126, 2}, {134, 2}, {1325, 2}, {547, 2}, {499, 50}}, --2月6号
     --   {{515, 2}, {503, 2}, {516, 2}, {549, 1}, {1126, 2}, {499, 288}}, --2月14号
    }
    local item = items[index]
    local package = player:GetPackage()
    if package:GetRestPackageSize() < #item * times then
        player:sendMsgCode(2, 1011, 0)
        return false
    end

    for k, v in pairs(item) do
        if v[1] == 499 then
            player:getCoupon(v[2] * times)
        else
            package:Add(v[1], v[2] * times, 1)
        end
    end

    return true
end

local answers = {
    ["0907"] = {
            --70级以上题库
        [0] = {  
                2 , 4 , 4 , 1 , 2 , 3 , 1 , 2 , 1 , 3 ,
                4 , 2 , 2 , 2 , 1 , 2 , 3 , 3 , 1 , 4 ,
                3 , 3 , 3 , 1 , 4 , 2 , 4 , 1 , 1 , 1 ,
              },
            --70级以下题库
        [1] = {
                2 , 4 , 4 , 1 , 2 , 3 , 1 , 2 , 1 , 3 ,
                4 , 2 , 2 , 2 , 1 , 2 , 3 , 3 , 1 , 4 ,
                3 , 3 , 3 , 1 , 4 , 2 , 4 , 1 , 1 , 1 ,
             },
    },
    ["0908"] = {
            --70级以上题库
        [0] = { 
                2, 2, 3, 1, 3, 4, 2, 4, 1, 1,
                4, 2, 4, 3, 4, 3, 1, 4, 2, 1,
                3, 1, 1, 2, 4, 3, 2, 4, 3, 1,
              },
            --70级以下题库
        [1] = { 
                2, 2, 3, 1, 3, 4, 2, 4, 1, 1,
                4, 2, 4, 3, 4, 3, 1, 4, 2, 1,
                3, 1, 1, 2, 4, 3, 2, 4, 3, 1,
              },
    },
    ["0909"] = {
        [0] = {
                2 , 2 , 1 , 4 , 3 , 4 , 4 , 4 , 2 , 3 ,
                3 , 3 , 4 , 2 , 3 , 1 , 2 , 2 , 4 , 2 ,
                4 , 2 , 1 , 4 , 3 , 4 , 1 , 2 , 4 , 3 ,
              },
        [1] = { 
                2 , 2 , 1 , 4 , 3 , 4 , 4 , 4 , 2 , 3 ,
                3 , 3 , 4 , 2 , 3 , 1 , 2 , 2 , 4 , 2 ,
                4 , 2 , 1 , 4 , 3 , 4 , 1 , 2 , 4 , 3 ,
              },
    },
}

function getAnswerInFoolsDay(qid, type)
    if nil == qid or nil == type or type > 1 or type < 0 then
        return 0
    end
    local date = os.date("%m%d", os.time())
    if nil == answers[date] or nil == answers[date][type] or nil == answers[date][type][qid] then
        return 0
    end
    return (answers[date][type][qid] + 64)
end

local answerAwardItems = {
    ["0907"] = { {500, 2}, {501, 2}, {9600, 2}, {1126, 2}, {1325, 1}, {515, 1} },
    ["0908"] = { {15, 2}, {503, 2}, {9418, 2}, {9457, 2}, {134, 1}, {9338, 1} },
    ["0909"] = { {56, 3}, {505, 3}, {9371, 3}, {517, 2}, {1126, 1}, {515, 1} },
}

function getAwardInFoolsDay(player, startId, endId)
    if nil == player or nil == startId or nil == endId then
        return
    end
    local date = os.date("%m%d", os.time())
    if nil == answerAwardItems[date] then
        return
    end
    if startId == 0 or startId > endId or startId > #answerAwardItems[date] then
        return
    end
    for k = startId, endId do
        local item = answerAwardItems[date][k]
        player:GetPackage():Add(item[1], item[2], true, false, 32)
    end
end

function getLuckyStarAward(player, idx)
    local items = {
        {{514, 5}, {503, 10}, {508, 5}},
        {{"vip", 1}},   --vip限时特权
        {{79, 1}},
        {{9082, 10}},
        {{501, 10}, {513, 10}, {8000, 50}},
        {{5035, 1}},
        {{5025, 1}},
        {{5005, 1}},
        {{1703, 1}},
        {{72, 1}},
        {{9177, 1}},
        {{9374, 1}},
    }
    if nil == player or nil == idx or idx > #items or idx <= 0 then
        return false
    end
    local golds = {0, 99, 99, 20, 0, 59, 59, 59, 0, 568, 268, 999}
    local recharge = { 100, 500, 1000 }
    if player:GetVar(369) < recharge[math.floor((idx-1)/4)+1] then
        SendMsg(player, 0x35, "充值仙石未达到对应额度，不能领取或购买！")
        return false
    end
    if player:getGoldInLua() < golds[idx] then
        player:sendMsgCode(0, 1104)
        return false
    end
    if player:GetFreePackageSize() < #items[idx] or player:GetFreePackageSize(3) < #items[idx] then
        player:sendMsgCode(2, 1011)
        return false
    end
    if items[idx][1][1] == "vip" then
        if player:SetVipPrivilege() == false then
            player:sendMsgCode(0, 1091)
            return false
        end
        player:sendVipPrivilege(true)
    else
        for _, val in pairs(items[idx]) do
            player:GetPackage():Add(val[1], val[2], true, false, 43)
        end
    end
    player:useGoldInLua(golds[idx], 101)
    return true
end

function GetLuckyBagAward(player)
    if nil == player then
        return false;
    end

    local items
    local items1 = {
       {9371,5}, {134,2}, {9457,2}, {9338,2}, {9600,2}, {1325,2}
    }

    local items2 = {
       {513,6}, {505,6}, {511,6}, {512,6}, {510,6}
    }
    if getSurnameLegend() then
        items = items1
    else
        items = items2
    end

    for i = 1 , 5  do
        local num = player:GetVar(452+i);
        if num < 1 then
            return false;
        end
    end
    for i= 1, 5 do
        local num = player:GetVar(452+i);
        player:SetVar(452+i,num-1);
    end
    for i = 1, #items do
        local item = items[i];
        player:GetPackage():Add(item[1],item[2],true,false,32);
    end
    player:sendLuckyBagInfo();
    if getSurnameLegend() then
        Broadcast(0x27, msg_68.."[p:"..player:getCountry()..":"..player:getPName().."]"..msg_136)
    else
        Broadcast(0x27, msg_68.."[p:"..player:getCountry()..":"..player:getPName().."]"..msg_145)
    end

    player:luaUdpLog("huodong", "F_130910_1", "act")
    return true
end

function UseToOther(player, other)
    if nil == player or nil == other then
        return false;
    end

    local counts = other:GetVar(518);
    if(counts >= 5) then
        return false;
    end

    local items = {
       { 9367,5} , {9369,5},{ 503,5},{515,1},{1525,1},{1325,2}
    }
    for i = 1 , 5  do
        local num = player:GetVar(452+i);
        if num < 1 then
            return false;
        end
    end
    for i= 1, 5 do
        local num = player:GetVar(452+i);
        player:SetVar(452+i,num-1);
    end
    for i = 1,#items do
        local item = items[i];
        player:GetPackage():Add(item[1],item[2],true,false,32);
    end

    local title = string.format(msg_138);
    local ctx = string.format(msg_139, player:getCountry(), player:getPName());
    local toItems = { 9367,5,1, 9369,5,1, 503,5,1, 515,1,1, 1525,1,1, 1325,2,1};
    other:GetMailBox():newItemPackageMail(title, ctx, toItems);

    other:SetVar(518, counts+1);

    player:sendLuckyBagInfo();
    Broadcast(0x27, msg_68.."[p:"..player:getCountry()..":"..player:getPName().."]"..msg_140)
    player:luaUdpLog("huodong", "F_130910_1", "act")
    return true
end

function UseToSystem(player)
    if nil == player then
        return false;
    end

    local items = {{ 9367,5} , {9369,5},{ 503,5},{515,1},{1525,1},{1325,2}}

    for i = 1 , 5  do
        local num = player:GetVar(452+i);
        if num < 1 then
            return false;
        end
    end
    for i= 1, 5 do
        local num = player:GetVar(452+i);
        player:SetVar(452+i,num-1);
    end

    for i = 1,#items do
        local item = items[i];
        player:GetPackage():Add(item[1],item[2],true,false,32);
    end
    
    player:GetPackage():Add(15, 1, true, false, 32);
    player:sendLuckyBagInfo();
    Broadcast(0x27, msg_68.."[p:"..player:getCountry()..":"..player:getPName().."]"..msg_140)
    player:luaUdpLog("huodong", "F_130910_1", "act")
    return true
end

local serverNoSpreadCount = {
    [358] = 12000,
    [357] = 12000,
    [356] = 7300,
    [355] = 9100,
    [354] = 9600,
    [501] = 600,
    [353] = 6700,
    [352] = 7500,
    [351] = 8200,
    [350] = 7200,
    [349] = 5400,
    [348] = 7600,
    [347] = 5800,
    [346] = 6000,
    [345] = 6600,
    [344] = 8100,
    [343] = 9100,
    [342] = 13300,
    [341] = 13200,
    [340] = 15000,
    [339] = 9700,
    [338] = 15100,
    [337] = 12000,
    [336] = 14700,
    [335] = 13800,
    [334] = 9700,
    [333] = 11500,
    [332] = 6000,
    [331] = 5200,
    [330] = 7500,
    [329] = 4500,
    [328] = 7400,
    [327] = 5400,
    [326] = 6700,
    [325] = 4800,
    [324] = 1400,
    [323] = 5900,
    [322] = 2800,
    [321] = 7100,
    [320] = 7600,
    [319] = 6700,
    [318] = 9800,
    [317] = 6000,
    [316] = 6500,
    [315] = 5100,
    [314] = 8200,
    [313] = 3900,
    [311] = 9800,
    [309] = 13000,
    [307] = 8600,
    [305] = 7400,
    [303] = 7400,
    [301] = 6100,
    [299] = 10000,
    [296] = 10100,
    [293] = 6000,
    [290] = 7200,
    [288] = 9900,
    [286] = 4900,
    [284] = 11200,
    [282] = 5300,
    [280] = 12000,
    [278] = 3600,
    [276] = 8100,
    [274] = 6300,
    [272] = 9800,
    [270] = 5400,
    [266] = 3700,
    [263] = 6100,
    [261] = 3800,
    [253] = 12900,
    [247] = 7300,
    [244] = 4900,
    [238] = 12400,
    [231] = 14200,
    [225] = 12100,
    [221] = 12600,
    [217] = 8300,
    [214] = 8400,
    [211] = 10800,
    [208] = 8900,
    [201] = 14600,
    [198] = 9000,
    [195] = 11300,
    [191] = 8200,
    [187] = 7400,
    [185] = 8300,
    [183] = 4700,
    [181] = 5700,
    [176] = 16800,
    [171] = 14700,
    [169] = 9600,
    [167] = 9600,
    [165] = 13400,
    [163] = 8100,
    [161] = 6800,
    [159] = 9300,
    [157] = 5000,
    [154] = 12700,
    [151] = 12800,
    [149] = 8100,
    [146] = 14100,
    [143] = 7800,
    [141] = 9800,
    [139] = 5500,
    [137] = 4500,
    [134] = 8000,
    [131] = 6900,
    [128] = 7100,
    [125] = 8200,
    [123] = 5800,
    [121] = 4900,
    [118] = 7100,
    [115] = 6300,
    [113] = 3700,
    [111] = 4000,
    [108] = 4900,
    [106] = 3900,
    [103] = 5300,
    [100] = 4900,
    [94] = 8800,
    [85] = 9000,
    [75] = 3300,
    [65] = 5200,
    [56] = 3700,
    [41] = 3600,
    [33] = 3500,
    [22] = 7000,
    [17] = 7400,
    [11] = 8600,
    [7] = 7300,
    [4] = 5500,
    [2] = 4800,
    [1] = 4400,
    [9990] = 2900,
}
function GetSpreadCountForAward(serverNo)
    return 200
    --[[local spreadCount = 5000
    if serverNo ~= 9990 and serverNo ~= 501 and serverNo > 358 then
        serverNo = 358
    end
    if serverNoSpreadCount[serverNo] ~= nil then
        spreadCount = serverNoSpreadCount[serverNo]
    end
    spreadCount = spreadCount * 4 / 50
    if spreadCount > 500 then
        spreadCount = 500
    end
    return spreadCount]]
end

function GetSpreadAward()
    local award = {{1325,2}, {9457,2}, {1126,2}, {9600,2}, {515,2}}
    return award
end

function sendAccRechargeAwards(player, awards)
    local c = player:GetVar(174)
    if awards[c] == nil then
        return
    end
    local ctx = string.format(msg_135, player:getPName(), c)
    sendItemPackageMail(player, msg_134, ctx, awards[c])
end

function sendAccRechargeAwards2(player, awards)
    local c = player:GetVar(174)
    if awards[c] == nil then
        return
    end
    local ctx = string.format(msg_135, player:getPName(), c)
    sendItemPackageMail(player, msg_134, ctx, awards[c])
    if c == 7 then
        local ctx = string.format(msg_146, player:getPName())
        sendItemPackageMail(player, msg_134, ctx, awards[c+1])
    end
end

function onAccRecharge_2013_05_18(player)
    local awards = {
        [1] = {134,1,1, 0xA000,50,1},
        [3] = {503,3,1, 500,4,1, 0xA000,100,1},
        [5] = {516,2,1, 547,2,1, 1325,3,1, 0xA000,200,1},
        [7] = {515,2,1, 509,1,1, 507,1,1, 47,1,1, 0xA000,300,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_05_25(player)
    local awards = {
        [1] = {9092,2,1, 0xA000,50,1},
        [3] = {514,6,1, 501,3,1, 0xA000,100,1},
        [5] = {547,4,1, 5065,1,1, 0xA000,200,1},
        [7] = {515,2,1, 5025,1,1, 5005,1,1, 0xA000,300,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_06_01(player)
    local awards = {
        [1] = {9092,2,1, 0xA000,50,1},
        [3] = {514,5,1, 501,3,1, 5055,1,1},
        [5] = {547,4,1, 5065,1,1, 0xA000,200,1},
        [7] = {515,2,1, 5025,1,1, 5005,1,1, 0xA000,300,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_06_08(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5035,1,1},
        [5] = {9371,5,1, 5025,1,1, 5005,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_06_15(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5055,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_06_22(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5035,1,1},
        [5] = {9371,5,1, 5115,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_06_29(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5025,1,1},
        [5] = {9371,5,1, 5085,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5026,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_07_06(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5115,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5026,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_07_20(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5025,1,1},
        [5] = {9371,5,1, 5115,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5006,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_08_03(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5035,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_08_17(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5026,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_08_24(player)
    local awards = {
        [1] = {0xA000,50,1, 549,1,1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {1717,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_08_31(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5035,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_09_07(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5025,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5006,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_09_14(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5025,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_09_28(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5025,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {1717,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_10_05(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5025,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {1719,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_10_12(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5035,1,1},
        [5] = {9371,5,1, 5025,1,1, 5005,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_10_19(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5025,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5006,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_10_26(player)
    local awards = {
        [1] = {9092,2,1, 0xA000,50,1},
        [3] = {514,6,1, 501,3,1, 0xA000,100,1},
        [5] = {547,4,1, 5065,1,1, 0xA000,200,1},
        [7] = {515,2,1, 5025,1,1, 5005,1,1, 0xA000,300,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_11_02(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5025,1,1},
        [5] = {9371,5,1, 5065,1,1},
        [7] = {1717,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_11_09(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5035,1,1},
        [5] = {9371,5,1, 5025,1,1, 5005,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_11_16(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5035,1,1},
        [5] = {9371,5,1, 5025,1,1, 5005,1,1, 0xA000,100,1},
        [7] = {515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_11_23(player)
    local awards = {
        [1] = {9092,2,1, 0xA000,50,1},
        [3] = {514,5,1, 501,3,1, 0xA000,100,1},
        [5] = {547,4,1, 5065,1,1, 0xA000,200,1},
        [7] = {515,5,1, 5025,1,1, 5005,1,1, 0xA000,100,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_11_30(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5025,1,1},
        [5] = {9371,5,1, 5085,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5026,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_12_07(player)
    local awards = {
        [1] = {0xA000,50,1, 549,1,1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {1717,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_12_14(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5035,1,1},
        [5] = {9371,5,1, 5025,1,1, 5005,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_12_21(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5035,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2013_12_28(player)
    local awards = {
        [1] = {9092,2,1, 0xA000,50,1},
        [3] = {514,5,1, 501,3,1, 0xA000,100,1},
        [5] = {547,4,1, 5065,1,1, 0xA000,200,1},
        [7] = {515,5,1, 5025,1,1, 5005,1,1, 0xA000,100,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_01_04(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5025,1,1},
        [5] = {9371,5,1, 5085,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5026,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_01_11(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5025,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end
function onAccRecharge_2014_01_18(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5025,1,1},
        [5] = {9371,5,1, 5085,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5026,1,1},
    }
    sendAccRechargeAwards(player, awards)
end
function onAccRecharge_2014_01_25(player)
    local awards = {
        [1] = {9092,2,1 , 0xA000,50,1} ,
        [3] = {514,5,1,501,3,1,0xA000,100,1},
        [5] = {547,4,1,5065,1,1,0xA000,200,1},
        [7] = {515,5,1,5025,1,1,5005,1,1,0xA000,100,1},
    }
    sendAccRechargeAwards(player, awards)
end
function onAccRecharge_2014_02_01(player)
    local awards = {
        [1] = {0xA000,50,1,549,1,1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {1717,1,1},
    }
    sendAccRechargeAwards(player, awards)
end
function onAccRecharge_2014_02_08(player)
    local awards = {
        [1] = {0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5025,1,1},
        [5] = {9371,5,1, 5085,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5026,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_02_15(player)
    local awards = {
        [1] = { 9092,2,1, 0xA000,50,1},
        [3] = {514,5,1, 501,3,1, 0xA000,100,1},
        [5] = {547,4,1, 5065,1,1, 0xA000,200,1},
        [7] = {515,5,1, 5025,1,1 ,5005,1,1 , 0xA000,100,1},
    }
    sendAccRechargeAwards(player, awards)
end
function onAccRecharge_2014_02_22(player)
    local awards = {
        [1] = { 0xA000,50,1 , 549,1,1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {1717,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_03_01(player)
    local awards = {
        [1] = { 9092,2,1,  0xA000,50,1},
        [3] = {514,5,1, 501,3,1, 0xA000,100,1},
        [5] = {547,4,1, 5065,1,1, 0xA000,200,1},
        [7] = {515,5,1, 5025,1,1, 5005,1,1, 0xA000,100,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_03_08(player)
    local awards = {
        [1] = { 0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5035,1,1},
        [5] = {9371,5,1, 5025,1,1, 5005,1,1, 0xA000,100,1},
        [7] = {515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_03_15(player)
    local awards = {
        [1] = { 0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5025,1,1},
        [5] = {9371,5,1, 5065,1,1},
        [7] = {1719,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_03_22(player)
    local awards = {
        [1] = { 0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5025,1,1},
        [5] = {9371,5,1, 5085,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5026,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_03_29(player)
    local awards = {
        [1] = { 0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5025,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_04_05(player)
    local awards = {
        [1] = { 0xA000,50,1 , 9092,2,1},
        [3] = {514,5,1, 501,3,1, 0xA000,100,1},
        [5] = {547,4,1, 5065,1,1, 0xA000,200,1},
        [7] = {515,5,1, 5025,1,1 , 5005,1,1, 0xA000,100,1},
    }
    sendAccRechargeAwards(player, awards)
end
function onAccRecharge_2014_04_12(player)
    local awards = {
        [1] = { 0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5085,1,1, 0xA000,150,1},
        [7] = {515,2,1,  5006,1,1 },
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_04_19(player)
    local awards = {
        [1] = { 0xA000,50,1, 549,1,1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = { 1717,1,1 },
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_04_26(player)
    local awards = {
        [1] = { 9092,2,1, 0xA000,50,1},
        [3] = {514,5,1, 501,3,1, 0xA000,100,1},
        [5] = {547,4,1, 5065,1,1, 0xA000,200,1},
        [7] = { 515,5,1, 5025,1,1, 5005,1,1, 0xA000,100,1 },
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_05_3(player)
    local awards = {
        [1] = { 549,1,1, 0xA000,50,1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = { 1728,1,1 },
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_05_10(player)
    local awards = {
        [1] = { 0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5085,1,1, 0xA000,150,1},
        [7] = { 515,2,1, 5006,1,1 },
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_05_17(player)
    local awards = {
        [1] = { 9092,2,1, 0xA000,50,1},
        [3] = {514,5,1, 501,3,1, 0xA000,100,1},
        [5] = {547,4,1, 5065,1,1, 0xA000,200,1},
        [7] = { 515,5,1, 5025,1,1, 5005,1,1, 0xA000,100,1 },
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_05_24(player)
    local awards = {
        [1] = { 549,1,1, 0xA000,50,1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = { 1728,1,1,  },
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_05_31(player)
    local awards = {
        [1] = { 0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5085,1,1, 0xA000,150,1},
        [7] = { 515,2,1, 5006,1,1 },
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_06_07(player)
    local awards = {
        [1] = { 9092, 2, 1, 0xA000,50,1},
        [3] = {514,5,1, 501,3,1, 0xA000,100,1},
        [5] = {547,4,1, 5065,1,1, 0xA000,200,1},
        [7] = { 515,5,1, 5025,1,1, 5005,1,1, 0xA000,100,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_06_14(player)
    local awards = {
        [1] = { 549, 1, 1, 0xA000,50,1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = { 1719,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_06_21(player)
    local awards = {
        [1] = { 0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5035,1,1},
        [5] = {9371,5,1, 5025,1,1, 0xA000,100,1, 5005,1,1},
        [7] = { 515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_06_28(player)
    local awards = {
        [1] = { 0xA000,50,1, 9092, 2, 1},
        [3] = {514,5,1, 501,3,1, 0xA000,100,1},
        [5] = {547,4,1, 5065,1,1, 0xA000,200,1},
        [7] = { 515,5,1, 5025,1,1, 5005,1,1, 0xA000, 100, 1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_07_5(player)
    local awards = {
        [1] = { 0xA000,50,1, 549, 1, 1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = { 1719,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_07_12(player)
    local awards = {
        [1] = { 0xA000,50,1, 9092, 2, 1},
        [3] = {514,5,1, 501,3,1, 0xA000,100,1},
        [5] = {547,4,1, 5065,1,1, 0xA000,200,1},
        [7] = { 515,5,1, 5025,1,1, 5005,1,1, 0xA000,100,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_07_19(player)
    local awards = {
        [1] = { 0xA000,50,1, 549, 1, 1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {1719,1,1 },
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_08_02(player)
    local awards = {
        [1] = { 0xA000,50,1, 549, 1, 1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {1727,1,1 },
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_08_09(player)
    local awards = {
        [1] = { 0xA000,50,1, 9092, 2, 1},
        [3] = {514,5,1, 501,3,1, 0xA000,100,1},
        [5] = {547,4,1, 5065,1,1, 0xA000,200,1},
        [7] = {515,5,1, 5025,1,1, 5005,1,1, 0xA000,100,1 },
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_08_16(player)
    local awards = {
        [1] = { 0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5035,1,1},
        [5] = {9371,5,1, 5025,1,1, 5005,1,1, 0xA000,100,1},
        [7] = {515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_08_23(player)
    local awards = {
        [1] = { 0xA000,50,1, 549,1,1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {1727,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_08_30(player)
    local awards = {
        [1] = { 0xA000,50,1, 9092,2,1},
        [3] = {514,5,1, 501,3,1, 0xA000,100,1},
        [5] = {547,4,1, 5065,1,1, 0xA000,200,1},
        [7] = {515,5,1, 5025,1,1, 5005,1,1, 0xA000,100,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_09_06(player)
    local awards = {
        [1] = { 0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5035,1,1},
        [5] = {9371,5,1, 5025,1,1, 5005,1,1, 0xA000,100,1},
        [7] = {515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_09_13(player)
    local awards = {
        [1] = { 0xA000,50,1, 549,1,1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {1727,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_09_20(player)
    local awards = {
        [1] = { 0xA000,50,1, 9092,2,1},
        [3] = {514,5,1, 501,3,1, 0xA000,100,1},
        [5] = {547,4,1, 5065,1,1, 0xA000,200,1},
        [7] = {515,5,1, 5025,1,1, 5005,1,1, 0xA000,100,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_09_27(player)
    local awards = {
        [1] = { 0xA000,100,1},
        [3] = {514,5,1, 501,3,1, 5035,1,1},
        [5] = {9371,5,1, 5025,1,1, 5005,1,1, 0xA000,100,1},
        [7] = {515,2,1, 5136,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_10_4(player)
    local awards = {
        [1] = { 0xA000,50,1, 549,1,1},
        [3] = {514,5,1, 501,3,1, 5005,1,1},
        [5] = {9371,5,1, 5065,1,1, 0xA000,150,1},
        [7] = {1727,1,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_10_11(player)
    local awards = {
        [1] = { 0xA000,50,1, 9092,2,1},
        [3] = {514,5,1, 501,3,1, 0xA000,100,1},
        [5] = {547,4,1, 5065,1,1, 0xA000,200,1},
        [7] = {515,5,1, 5025,1,1, 5005,1,1, 0xA000,100,1},
    }
    sendAccRechargeAwards(player, awards)
end

function onAccRecharge_2014_10_18(player)
    local awards = {
        [1] = { 0xA000,30,1, 9092,1,1},
        [2] = { 0xA000,50,1, 9093,1,1},
        [3] = {501,2,1, 503,1,1, 0xA000,80,1},
        [4] = {547,2,1, 503,1,1, 0xA000,80,1},
        [5] = {1325,1,1, 517,2,1, 9498,2,1, 0xA000,100,1},
        [6] = {515,1,1, 5055,1,1, 0xA000,150,1},
        [7] = {5135,1,1, 551,3,1, 9457,3,1},
        [8] = {1729,1,1},
    }
    sendAccRechargeAwards2(player, awards)
end

function onAccRecharge_2014_10_25(player)
    local awards = {
        [1] = { 0xA000,30,1, 9361,1,1},
        [2] = { 0xA000,20,1, 500,2,1},
        [3] = {516,1,1, 503,1,1, 0xA000,50,1},
        [4] = {17103,1,1, 9498,2,1, 0xA000,50,1},
        [5] = {134,1,1, 551,2,1, 9600,2,1, 0xA000,100,1},
        [6] = {17105,1,1, 5035,1,1, 0xA000,100,1},
        [7] = {5095,1,1, 9338,1,1, 9308,1,1, 9310,1,1},
        [8] = {5026,1,1, 0xA000,500,1},
    }
    sendAccRechargeAwards2(player, awards)
end

function onAccRecharge_2014_11_1(player)
    local awards = {
        [1] = { 0xA000,50,1},
        [2] = { 0xA000,30,1, 17109,2,1},
        [3] = {517,1,1, 503,1,1, 0xA000,50,1},
        [4] = {16001,2,1, 501,2,1, 0xA000,50,1},
        [5] = {17107,1,1, 9600,2,1, 17103,2,1, 0xA000,100,1},
        [6] = {13007,1,1, 0xA000,100,1},
        [7] = {13067,1,1, 17105,1,1, 9457,1,1, 0xA000,100,1},
        [8] = {13150,1,1},
    }
    sendAccRechargeAwards2(player, awards)
end

function onAccRecharge_2014_11_8(player)
    local awards = {
        [1] = { 0xA000,20,1, 9092,1,1},
        [2] = { 0xA000,40,1, 9093,1,1},
        [3] = {513,1,1, 17103,1,1, 0xA000,50,1},
        [4] = {9498,2,1, 503,2,1, 0xA000,50,1},
        [5] = {9457,2,1, 134,1,1, 554,1,1, 0xA000,100,1},
        [6] = {5005,1,1, 9338,1,1, 0xA000,100,1},
        [7] = {5025,1,1, 515,1,1, 17105,1,1, 1325,1,1, 0xA000,100,1},
        [8] = {5106,1,1, 0xA000, 288,1,1},
    }
    sendAccRechargeAwards2(player, awards)
end


-- ACCRECHARGE
function onRecharge(player, r)
    if getAccRecharge() == 1 then -- 老天池
        local cond = 100
        if player:GetVar(172) >= cond and player:GetVar(173) == 0 then
            player:AddVar(174, 1);
            player:SetVar(173, 1)

            local t = { ['year'] = 2014, ['month'] = 10, ['day'] = 4, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
            local s = os.time(t)
            local n = os.time() + 11
            if n >= s and n < (s + 7*86400) then
                onAccRecharge_2014_10_4(player)
            end

            local t = { ['year'] = 2014, ['month'] = 10, ['day'] = 11, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
            local s = os.time(t)
            local n = os.time() + 11
            if n >= s and n < (s + 7*86400) then
                onAccRecharge_2014_10_11(player)
            end


        end
    end
    
    if getAccRecharge() == 2 then -- 新天池
        local cond1 = {10,20,40,80,140,180,230} -- 刻度
        local v = player:GetVar(174)
        if v >= 7 then
            return
        end
        if v == nil or v == 0 then
            v = 1
        else
            v = v + 1
        end
        local vv = cond1[v]
        if player:GetVar(172) >= vv then
            if player:GetVar(173) == 0 then
                player:AddVar(174, 1);
                player:SetVar(173, 1)
                player:UdpAccRecharge(player:GetVar(174))

                local t = { ['year'] = 2014, ['month'] = 10, ['day'] = 18, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
                local s = os.time(t)
                local n = os.time() + 11
                if n >= s and n < (s + 7*86400) then
                    onAccRecharge_2014_10_18(player)
                end

                local t = { ['year'] = 2014, ['month'] = 10, ['day'] = 25, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
                local s = os.time(t)
                local n = os.time() + 11
                if n >= s and n < (s + 7*86400) then
                    onAccRecharge_2014_10_25(player)
                end

                local t = { ['year'] = 2014, ['month'] = 11, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
                local s = os.time(t)
                local n = os.time() + 11
                if n >= s and n < (s + 7*86400) then
                    onAccRecharge_2014_11_1(player)
                end

                local t = { ['year'] = 2014, ['month'] = 11, ['day'] = 8, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
                local s = os.time(t)
                local n = os.time() + 11
                if n >= s and n < (s + 7*86400) then
                    onAccRecharge_2014_11_8(player)
                end


                v = v + 1
            end
            
            if player:GetVar(173) == 1 and v <= 7 then
                print("v is .." .. v)
                if v <= 1 then
                    return
                end
                vv = cond1[v] +  cond1[v-1]
                if player:GetVar(172) >= vv then

                    local t = { ['year'] = 2014, ['month'] = 10, ['day'] = 18, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
                    local s = os.time(t)
                    local n = os.time() + 11
                    if n >= (s + 6*86400) and n < (s + 7*86400) then
                        player:AddVar(174, 1);
                        player:SetVar(173, 2)
                        player:UdpAccRecharge(player:GetVar(174))

                        onAccRecharge_2014_10_18(player)
                    end

                    local t = { ['year'] = 2014, ['month'] = 10, ['day'] = 25, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
                    local s = os.time(t)
                    local n = os.time() + 11
                    if n >= (s + 6*86400) and n < (s + 7*86400) then
                        player:AddVar(174, 1);
                        player:SetVar(173, 2)
                        player:UdpAccRecharge(player:GetVar(174))

                        onAccRecharge_2014_10_25(player)
                    end

                    local t = { ['year'] = 2014, ['month'] = 11, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
                    local s = os.time(t)
                    local n = os.time() + 11
                    if n >= (s + 6*86400) and n < (s + 7*86400) then
                        player:AddVar(174, 1);
                        player:SetVar(173, 2)
                        player:UdpAccRecharge(player:GetVar(174))

                        onAccRecharge_2014_11_1(player)
                    end

                    local t = { ['year'] = 2014, ['month'] = 11, ['day'] = 8, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
                    local s = os.time(t)
                    local n = os.time() + 11
                    if n >= (s + 6*86400) and n < (s + 7*86400) then
                        player:AddVar(174, 1);
                        player:SetVar(173, 2)
                        player:UdpAccRecharge(player:GetVar(174))

                        onAccRecharge_2014_11_8(player)
                    end

                end
            end

        end
    end

end

function onFishUserPackage(player, idx)
    if player == nil or idx == nil or idx > 4 or idx < 0 then
        return false
    end
    local items = {
        [0] = { {35, 20}, {499, 50}, {29, 30}, {55, 10} },
        [1] = { {29, 50}, {499, 50}, {55, 20}, {15, 5} },
        [2] = { {55, 30}, {499, 50}, {510, 20}, {15, 5} },
        [3] = { {499, 50}, {502, 30}, {15, 5}, {30, 5} },
        [4] = { {1277, 1}, {499, 50}, {30, 5}, {15, 5} },
    }
    local package = player:GetPackage()
    local reqGrids = #items[idx] - 1
    if idx == 3 then
        reqGrids = reqGrids + 10
    elseif idx == 4 then
        reqGrids = reqGrids + 1
    end
    if package:GetRestPackageSize() < reqGrids or package:GetRestPackageSize(3) < 10 then
        player:sendMsgCode(2, 1011, 0)
        return false
    end
    for _, val in pairs(items[idx]) do
        if val[1] == 499 then
            player:getCoupon(val[2])
        else
            package:Add(val[1], val[2], true)
        end
    end
    if idx == 3 then
        local gems = { 5001, 5011, 5021, 5031, 5041, 5051, 5061, 5071, 5081, 5091, 5101, 5111, 5121, 5131, 5141 }
        for k = 1, 10 do
            package:Add(gems[math.random(1, #gems)], 1, true)
        end
    elseif idx == 4 then
        local equipId = math.random(2544, 2567)
        package:Add(equipId, 1, true)
    end
    return true
end

function onCollectCardAct(player, idx)
    if player == nil then
        return false
    end
    --local fashionId = {1700,1701,1702,1703,1704,1705,1706,1707,1709,1710,1711,1712}
    local items = {
        [1] = { {503, 4}, {515, 2}, {1325, 2}, {509, 2}, {1126,1} },
        [2] = { {503, 2}, {515, 2}, {1325, 1}, {509, 1}, {1126,1} },
        [3] = { {9088, 2}, {134, 2}, {509, 2}, {507, 2} },
        --[4] = { {515, 5}, {1325, 10}, {9076, 5}, {507, 5}, {fashionId[math.random(1,#fashionId)],1} },
        [4] = { {9419, 1} },
    }
    local item = items[idx]
    if item == nil then
        return false
    end
    local package = player:GetPackage()
    local reqGrids
    if idx == 4 then
        reqGrids = 1
    else
        reqGrids = #item
    end
    if package:GetRestPackageSize() < reqGrids then
        player:sendMsgCode(2, 1011, 0)
        return false
    end
    if idx == 4 then
        --local cur = math.random(1,#item)
        package:Add(item[1][1], item[1][2], true)
    else
        for _, val in pairs(item) do
            package:Add(val[1], val[2], true)
        end
    end

    if idx == 1 then
        player:luaUdpLog("huodong", "F_130815_1", "act")
    elseif idx == 2 then
        player:luaUdpLog("huodong", "F_130815_4", "act")
    elseif idx == 3 then
        player:luaUdpLog("huodong", "F_130815_2", "act")
    else
        player:luaUdpLog("huodong", "F_130815_3", "act")
    end

    return true
end

local answerInMarryBoard = {4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 ,4 ,3 ,2 ,1 
}
function getAnswerInMarryBoard(id)
    if id > #answerInMarryBoard then
        return 0;
    end
    if answerInMarryBoard[id] == nil then 
        return 0;
    end
    return (answerInMarryBoard[id])
end

local copyAward = {
    [30] = {
        ["equip1"] = {2024,2025,2026,2027,2028,2029,2030,2031,2032,2033,2034,2035,2036,2037,2038,2039,2040,2041,2042,2043,2044,2045,2046,2047,2048,2049,2050,2051,2052,2053,2054,2055,2056,2057,2058,2059,2060,2061,2062,2063,2064,2065,2066,2067,2068,2069,2070,2071},
        ["equip2"] = {2208,2209,2210,2211,2212,2213,2214,2215,2216,2217,2218,2219,2220,2221,2222,2223,2224,2225,2226,2227,2228,2229,2230,2231,2232,2233,2234,2235,2236,2237,2238,2239,2240,2241,2242,2243,2244,2245,2246,2247,2248,2249,2250,2251,2252,2253,2254,2255},
        ["equip3"] = {2376,2377,2378,2379,2380,2381,2382,2383,2384,2385,2386,2387,2388,2389,2390,2391,2392,2393,2394,2395,2396,2397,2398,2399},
        ["equip4"] = {},
        ["citta"] = {1208},
        ["trump"] = {},
        ["gems"] = {5011,5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142},
        ["huiji"] = {},
        ["other"] = {48,51,55,510},
        ["exp"] = 13418,
    },
    [45] = {
        ["equip1"] = {2072,2073,2074,2075,2076,2077,2078,2079,2080,2081,2082,2083,2084,2085,2086,2087,2088,2089,2090,2091,2092,2093,2094,2095,2048,2049,2050,2051,2052,2053,2054,2055,2056,2057,2058,2059,2060,2061,2062,2063,2064,2065,2066,2067,2068,2069,2070,2071},
        ["equip2"] = {2256,2257,2258,2259,2260,2261,2262,2263,2264,2265,2266,2267,2268,2269,2270,2271,2272,2273,2274,2275,2276,2277,2278,2279,2232,2233,2234,2235,2236,2237,2238,2239,2240,2241,2242,2243,2244,2245,2246,2247,2248,2249,2250,2251,2252,2253,2254,2255},
        ["equip3"] = {2400,2401,2402,2403,2404,2405,2406,2407,2408,2409,2410,2411,2412,2413,2414,2415,2416,2417,2418,2419,2420,2421,2422,2423},
        ["equip4"] = {},
        ["citta"] = {1235,1310},
        ["trump"] = {},
        ["gems"] = {5011,5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142},
        ["huiji"] = {9012},
        ["other"] = {48,51,55,510},
        ["exp"] = 31335,
    },
    [60] = {
        ["equip1"] = {2096,2097,2098,2099,2100,2101,2102,2103,2104,2105,2106,2107,2108,2109,2110,2111,2112,2113,2114,2115,2116,2117,2118,2119},
        ["equip2"] = {2280,2281,2282,2283,2284,2285,2286,2287,2288,2289,2290,2291,2292,2293,2294,2295,2296,2297,2298,2299,2300,2301,2302,2303},
        ["equip3"] = {2424,2425,2426,2427,2428,2429,2430,2431,2432,2433,2434,2435,2436,2437,2438,2439,2440,2441,2442,2443,2444,2445,2446,2447},
        ["equip4"] = {2568,2569,2570,2571,2572,2573,2574,2575,2576,2577,2578,2579,2580,2581,2582,2583,2584,2585,2586,2587,2588,2589,2590,2591},
        ["citta"] = {1240,1313},
        ["trump"] = {},
        ["gems"] = {5001,5011,5021,5031,5041,5051,5061,5071,5081,5091,5101,5111,5121,5131,5141,5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142},
        ["huiji"] = {9371,9013},
        ["other"] = {48,51,55,510},
        ["exp"] = 56915,
    },
    [70] = {
        ["equip1"] = {2120,2121,2122,2123,2124,2125,2126,2127,2128,2129,2130,2131,2132,2133,2134,2135,2136,2137,2138,2139,2140,2141,2142,2143},
        ["equip2"] = {2304,2305,2306,2307,2308,2309,2310,2311,2312,2313,2314,2315,2316,2317,2318,2319,2320,2321,2322,2323,2324,2325,2326,2327},
        ["equip3"] = {2448,2449,2450,2451,2452,2453,2454,2455,2456,2457,2458,2459,2460,2461,2462,2463,2464,2465,2466,2467,2468,2469,2470,2471},
        ["equip4"] = {2592,2593,2594,2595,2596,2597,2598,2599,2600,2601,2602,2603,2604,2605,2606,2607,2608,2609,2610,2611,2612,2613,2614,2615 },
        ["citta"] = {1416,1417,1418,1419,1420,6056},
        ["trump"] = {},
        ["gems"] = {5001,5011,5021,5031,5041,5051,5061,5071,5081,5091,5101,5111,5121,5131,5141,5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142},
        ["huiji"] = {9371,9229,9014},
        ["other"] = {48,51,55,510},
        ["exp"] = 77298,
    },
    [80] = {
        ["equip1"] = {2144,2145,2146,2147,2148,2149,2150,2151,2152,2153,2154,2155,2156,2157,2158,2159,2160,2161,2162,2163,2164,2165,2166,2167 },
        ["equip2"] = {2328,2329,2330,2331,2332,2333,2334,2335,2336,2337,2338,2339,2340,2341,2342,2343,2344,2345,2346,2347,2348,2349,2350,2351},
        ["equip3"] = {2472,2473,2474,2475,2476,2477,2478,2479,2480,2481,2482,2483,2484,2485,2486,2487,2488,2489,2490,2491,2492,2493,2494,2495},
        ["equip4"] = {2616,2617,2618,2619,2620,2621,2622,2623,2624,2625,2626,2627,2628,2629,2630,2631,2632,2633,2634,2635,2636,2637,2638,2639 },
        ["citta"] = {1433,1434,1435,1436,1437,1438,1439,6073},
        ["trump"] = {},
        ["gems"] = {5001,5011,5021,5031,5041,5051,5061,5071,5081,5091,5101,5111,5121,5131,5141,5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142},
        ["huiji"] = {9371,9229,9015},
        ["other"] = {48,51,55,510},
        ["exp"] = 121568,
    },
    [90] = {
        ["equip1"] = {2168,2169,2170,2171,2172,2173,2174,2175,2176,2177,2178,2179,2180,2181,2182,2183,2184,2185,2186,2187,2188,2189,2190,2191},
        ["equip2"] = {2352,2353,2354,2355,2356,2357,2358,2359,2360,2361,2362,2363,2364,2365,2366,2367,2368,2369,2370,2371,2372,2373,2374,2375},
        ["equip3"] = {2496,2497,2498,2499,2500,2501,2502,2503,2504,2505,2506,2507,2508,2509,2510,2511,2512,2513,2514,2515,2516,2517,2518,2519},
        ["equip4"] = {2640,2641,2642,2643,2644,2645,2646,2647,2648,2649,2650,2651,2652,2653,2654,2655,2656,2657,2658,2659,2660,2661,2662,2663},
        ["citta"] = {1491,1492,1493,1494,1495,1496,1497,1498,1499,6103},
        ["trump"] = {},
        ["gems"] = {},
        ["huiji"] = {9371,9229,9016},
        ["other"] = {48,51,55,510},
        ["exp"] = 219353,
    },
    [100] = {
        ["equip1"] = {4000,4001,4002,4003,4004,4005,4006,4007,4008,4009,4010,4011,4012,4013,4014,4015,4016,4017,4018,4019,4020,4021,4022,4023},
        ["equip2"] = {4024,4025,4026,4027,4028,4029,4030,4031,4032,4033,4034,4035,4036,4037,4038,4039,4040,4041,4042,4043,4044,4045,4046,4047},
        ["equip3"] = {2520,2521,2522,2523,2524,2525,2526,2527,2528,2529,2530,2531,2532,2533,2534,2535,2536,2537,2538,2539,2540,2541,2542,2543},
        ["equip4"] = {2664,2665,2666,2667,2668,2669,2670,2671,2672,2673,2674,2675,2676,2677,2678,2679,2680,2681,2682,2683,2684,2685,2686,2687},
        ["citta"] = {6039,6040,6041,6042,6043,6044,6045,6046,6047,6048,6049,6050,6051,6052,6137,6138,6139,6140,6141,6142,6143,6144,6145},
        ["trump"] = {},
        ["gems"] = {},
        ["huiji"] = {9371,9229,9035},
        ["other"] = {48,51,55,510},
        ["exp"] = 194202,
    },
    [110] = {
        ["equip1"] = {9359},
        ["equip2"] = {9359},
        ["equip3"] = {9359},
        ["equip4"] = {2936,2937,2938,2939,2940,2941,2942,2943,2944,2945,2946,2947,2948,2949,2950,2951,2952,2953,2954,2955,2956,2957,2958,2959},
        ["citta"] = {6146,6147,6148,6149,6150,6151,6152,6153,6154,6155,6156,6157,6158,6159,6160,6161,6162,6163,6164,6165,6166,6167,6168},
        ["trump"] = {},
        ["gems"] = {},
        ["huiji"] = {9371,9229,9391},
        ["other"] = {48,51,55,510},
        ["exp"] = 233073,
    },
    [120] = {
        ["equip1"] = {9359},
        ["equip2"] = {9359},
        ["equip3"] = {9359},
        ["equip4"] = {3000,3001,3002,3003,3004,3005,3006,3007,3008,3009,3010,3011,3012,3013,3014,3015,3016,3017,3018,3019,3020,3021,3022,3023},
        ["citta"] = {6170,6171,6172,6173,6174,6175,6176,6177,6178,6179,6180,6181,6182,6183,6184,6185,6186,6187,6188,6189,6190,6191,6192},
        ["trump"] = {},
        ["gems"] = {},
        ["huiji"] = {9371,9229,9430,9491},
        ["other"] = {48,51,55,510},
        ["exp"] = 165278,
    },
    [130] = {
        ["equip1"] = {9359},
        ["equip2"] = {9359},
        ["equip3"] = {9359},
        ["equip4"] = {3064,3065,3066,3067,3068,3069,3070,3071,3072,3073,3074,3075,3076,3077,3078,3079,3080,3081,3082,3083,3084,3085,3086,3087},
        ["citta"] = {6211,6212,6213,6214,6215,6216,6217,6218,6219,6196,6197,6198,6199,6200,6201,6202,6203,6204,6205,6206,6207,6208,6209},
        ["trump"] = {},
        ["gems"] = {},
        ["huiji"] = {9371,9229,9491},
        ["other"] = {48,51,55,510},
        ["exp"] = 179452,
    },
    [140] = {
        ["equip1"] = {9359},
        ["equip2"] = {9359},
        ["equip3"] = {9359},
        ["equip4"] = {3128,3129,3130,3131,3132,3133,3134,3135,3136,3137,3138,3139,3140,3141,3142,3143,3144,3145,3146,3147,3148,3149,3150,3151},
        ["citta"] = {6230,6231,6232,6233,6234,6235,6236,6237,6238,6239,6240,6241,6242,6243,6244,6245,6246,6247,6248,6249,6250,6251,6252},
        ["trump"] = {},
        ["gems"] = {},
        ["huiji"] = {9371,9229,9885},
        ["other"] = {48,51,55,510},
        ["exp"] = 179452,
    },
}
local yuanshenAward = {
    ["30_0"] = {},
    ["30_1"] = {},
    ["45_0"] = {},
    ["45_1"] = {},
    ["60_0"] = {8004},
    ["60_1"] = {8001,8002,8003,8004},
    ["70_0"] = {8008},
    ["70_1"] = {8005,8006,8007,8008},
    ["80_0"] = {8012},
    ["80_1"] = {8009,8010,8011,8012},
    ["90_0"] = {8016},
    ["90_1"] = {8013,8014,8015,8016},
    ["100_0"] = {8056},
    ["100_1"] = {8053,8054,8055,8056},
    ["110_0"] = {},
    ["110_1"] = {},
    ["120_0"] = {},
    ["120_1"] = {},
    ["130_0"] = {},
    ["130_1"] = {},
    ["140_0"] = {},
    ["140_1"] = {},
}

function getJiqirenAward_Copy(player, isFree)
    if nil == player or nil == isFree then
        return
    end
    local level = player:GetLev()
    if level < 30 then
        return
    end
    local key = 0
    if level < 45 then
        key = 30
    elseif level < 60 then
        key = 45
    elseif level < 70 then
        key = 60
    elseif level < 80 then
        key = 70
    elseif level < 90 then
        key = 80
    elseif level < 100 then
        key = 90
    elseif level < 110 then
        key = 100
    elseif level < 120 then
        key = 110
    elseif level < 130 then
        key = 120
    else
        key = 130
    end
    if nil == copyAward[key] then
        return
    end
    local package = player:GetPackage()
    local equip1Num = #copyAward[key]["equip1"]
    local equip2Num = #copyAward[key]["equip2"]
    local equip3Num = #copyAward[key]["equip3"]
    local equip4Num = #copyAward[key]["equip4"]
    for k = 1, 5 do
        if equip1Num > 0 and math.random(1, 10000) <= 5000 then
            package:Add(copyAward[key]["equip1"][math.random(1, equip1Num)], 1, isFree, false, 69)
        end
        if equip2Num > 0 and math.random(1, 10000) <= 5000 then
            package:Add(copyAward[key]["equip2"][math.random(1, equip2Num)], 1, isFree, false, 69)
        end
    end
    for k = 1, 2 do
        if equip3Num > 0 and math.random(1, 10000) <= 5000 then
            package:Add(copyAward[key]["equip3"][math.random(1, equip3Num)], 1, isFree, false, 69)
        end
    end
    if equip4Num > 0 and math.random(1, 10000) <= 5000 then
        package:Add(copyAward[key]["equip4"][math.random(1, equip4Num)], 1, isFree, false, 69)
    end

    local cittaNum = #copyAward[key]["citta"]
    local trumpNum = #copyAward[key]["trump"]
    local gemsNum = #copyAward[key]["gems"]
    local otherNum = #copyAward[key]["other"]
    for k = 1, 2 do
        if cittaNum > 0 and math.random(1, 10000) <= 3000 then
            package:Add(copyAward[key]["citta"][math.random(1, cittaNum)], 1, isFree, false, 69)
        end
        if trumpNum > 0 and math.random(1, 10000) <= 3000 then
            package:Add(copyAward[key]["trump"][math.random(1, trumpNum)], 1, isFree, false, 69)
        end
        if gemsNum > 0 and math.random(1, 10000) <= 2000 then
            package:Add(copyAward[key]["gems"][math.random(1, gemsNum)], 1, isFree, false, 69)
        end
        if otherNum > 0 and math.random(1, 10000) <= 10000 then
            package:Add(copyAward[key]["other"][math.random(1, otherNum)], 1, isFree, false, 69)
        end
    end
    for _, id in pairs(copyAward[key]["huiji"]) do
        package:Add(id, 1, true, false, 69)
    end
    --元神
    local key1 = key .. "_" .. isFree
    if nil ~= yuanshenAward[key1] then
        local ysNum = #yuanshenAward[key1]
        if ysNum > 0 and math.random(1, 10000) <= 10000 then
            package:Add(yuanshenAward[key1][math.random(1, ysNum)], 1, isFree, false, 69)
        end
    end
    player:AddExp(copyAward[key]["exp"])
    --类似抽奖功能
    local rnd = math.random(1, 3)
    if rnd == 1 and equip4Num > 0 then
        package:Add(copyAward[key]["equip4"][math.random(1, equip4Num)], 1, isFree, false, 69)
    elseif rnd == 2 and cittaNum > 0 then
        package:Add(copyAward[key]["citta"][math.random(1, cittaNum)], 1, isFree, false, 69)
    elseif rnd == 3 and gemsNum > 0 then
        package:Add(copyAward[key]["gems"][math.random(1, gemsNum)], 1, isFree, false, 69)
    end
end

local frontAward = {
    [35] = {
        ["zhenqi"] = {1116,1118,1119,1120,1121,1122,1123,1124},
        ["citta"] = {1221,1222,1205,1220},
        ["gems"] = {5001,5011,5021,5031,5041,5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142},
        ["trump"] = {1600,1601,1602,1500,1501,1502},
        ["huiji"] = {},
        ["other"] = {48,49,50,51},
        ["exp"] = 9242,
    },
    [45] = {
        ["zhenqi"] = {1059,1061,1062,1063,1064},
        ["citta"] = {1221,1222},
        ["trump"] = {1604,1605,1606,1503},
        ["gems"] = {5001,5011,5021,5031,5041,5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142},
        ["huiji"] = {},
        ["other"] = {48,49,50,51},
        ["exp"] = 14752,
    },
    [50] = {
        ["zhenqi"] = {1107,1109,1110,1111,1112,1113,1114,1115},
        ["citta"] = {1233,1225},
        ["trump"] = {1603,1608,1609,1505,1506},
        ["gems"] = {5001,5011,5021,5031,5041,5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142},
        ["huiji"] = {9371},
        ["other"] = {48,49,50,51},
        ["exp"] = 17225,
    },
    [55] = {
        ["zhenqi"] = {1096,1098,1099,1100,1101,1102,1103,1104,1105,1106},
        ["citta"] = {1229,1243},
        ["trump"] = {1610,1614,1615,1507,1508},
        ["gems"] = {5001,5011,5021,5031,5041,5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142},
        ["huiji"] = {9371},
        ["other"] = {48,49,50,51},
        ["exp"] = 23712,
    },
    [60] = {
        ["zhenqi"] = {1084,1086,1087,1088,1089,1090,1091,1092,1093,1094,1095},
        ["citta"] = {1251,1317},
        ["trump"] = {1612,1616,1607,1611,1509,1523},
        ["gems"] = {5001,5011,5021,5031,5041,5051,5061,5071,5081,5091,5101,5111,5121,5131,5141,5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142},
        ["huiji"] = {9371},
        ["other"] = {48,49,50,51},
        ["exp"] = 30087,
    },
    [65] = {
        ["zhenqi"] = {1074,1076,1077,1078,1079,1080,1081,1082,1083},
        ["citta"] = {1242,1241},
        ["trump"] = {200,201,202,203,204,205,206,207,208,82,83,84,85,86,87,88,89},
        ["gems"] = {5001,5011,5021,5031,5041,5051,5061,5071,5081,5091,5101,5111,5121,5131,5141,5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142},
        ["huiji"] = {9371},
        ["other"] = {48,49,50,51},
        ["exp"] = 31852,
    },
    [70] = {
        ["zhenqi"] = {1020,1022,1023,1024,1025,1026,1027,1028,1029,1030},
        ["citta"] = {1253,1250},
        ["trump"] = {209,210,211,212,213,214,215,216,217,218,219,220,221,222,223,224,103,104,105,106,107,108,109,110},
        ["gems"] = {5001,5011,5021,5031,5041,5051,5061,5071,5081,5091,5101,5111,5121,5131,5141,5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142},
        ["huiji"] = {9371},
        ["other"] = {48,49,50,51},
        ["exp"] = 39235,
    },
    [75] = {
        ["zhenqi"] = {1065,1067,1068,1069,1070,1071,1072,1073},
        ["citta"] = {1245},
        ["trump"] = {228,229,230,231,232,233,234,235,236,237,238,239,240,241,242,91,92,93,94,95,96,97,98,99,100,101,102},
        ["gems"] = {5001,5011,5021,5031,5041,5051,5061,5071,5081,5091,5101,5111,5121,5131,5141,5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142},
        ["huiji"] = {9371},
        ["other"] = {48,49,50,51},
        ["exp"] = 41127,
    },
    [80] = {
        ["zhenqi"] = {1040,1042,1043,1044,1045,1046,1047,1048,1049,1050,1051},
        ["citta"] = {1247,1249},
        ["trump"] = {243,244,245,246,247,248,249,250,251,252,253,254,255,256,257,258,259,260,261,262,263,264,265,266,267,268,269},
        ["gems"] = {5001,5011,5021,5031,5041,5051,5061,5071,5081,5091,5101,5111,5121,5131,5141,5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142},
        ["huiji"] = {9371},
        ["other"] = {48,49,50,51},
        ["exp"] = 54962,
    },
    [85] = {
        ["zhenqi"] = {1052,1054,1055,1056,1057,1058},
        ["citta"] = {1255,1254},
        ["trump"] = {271,272,273,274,275,276,277,278,279,280,281,282,283,284,285,286,287,288,289,290,291,292,293,294,295,296,297},
        ["gems"] = {5001,5011,5021,5031,5041,5051,5061,5071,5081,5091,5101,5111,5121,5131,5141,5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142},
        ["huiji"] = {9371},
        ["other"] = {48,49,50,51},
        ["exp"] = 61862,
    },
    [90] = {
        ["zhenqi"] = {1031,1033,1034,1035,1036,1037,1038,1039},
        ["citta"] = {1468,1469,1470,1471,1472,1473,1474,1475,1476,1477,1478,1479,1480,1481},
        ["trump"] = {120,121,122,123,124,125,126,127,128},
        ["gems"] = {5001,5011,5021,5031,5041,5051,5061,5071,5081,5091,5101,5111,5121,5131,5141,5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142},
        ["huiji"] = {9371},
        ["other"] = {48,49,50,51},
        ["exp"] = 58820,
    },
    [95] = {
        ["zhenqi"] = {1012,1014,1015,1016,1017,1018,1019},
        ["citta"] = {6011,6012,6013,6014,6015,6016,6017,6018,6019,6020,6021,6022,6023,6024},
        ["trump"] = {299,300,301,302,303,304,305,306,307},
        ["gems"] = {5001,5011,5021,5031,5041,5051,5061,5071,5081,5091,5101,5111,5121,5131,5141,5002,5012,5022,5032,5042,5052,5062,5072,5082,5092,5102,5112,5122,5132,5142},
        ["huiji"] = {9371},
        ["other"] = {48,49,50,51},
        ["exp"] = 58187,
    },
}
function getJiqirenAward_FrontMap(player, isFree)
    if nil == player or nil == isFree then
        return
    end
    local level = player:GetLev()
    if level < 35 then
        return
    end
    local key = 0
    if level < 45 then
        key = 35
    elseif level < 45 then
        key = 40
    elseif level < 50 then
        key = 45
    elseif level < 55 then
        key = 50
    elseif level < 60 then
        key = 55
    elseif level < 65 then
        key = 60
    elseif level < 70 then
        key = 65
    elseif level < 75 then
        key = 70
    elseif level < 80 then
        key = 75
    elseif level < 85 then
        key = 80
    elseif level < 90 then
        key = 85
    elseif level < 95 then
        key = 90
    else
        key = 95
    end
    if nil == frontAward[key] then
        return
    end
    local package = player:GetPackage()
    local zhenqiNum = #frontAward[key]["zhenqi"]
    local cittaNum = #frontAward[key]["citta"]
    local trumpNum = #frontAward[key]["trump"]
    local gemsNum = #frontAward[key]["gems"]
    local otherNum = #frontAward[key]["other"]
    for k = 1, 2 do
        if zhenqiNum > 0 and math.random(1, 10000) <= 5000 then
            package:Add(frontAward[key]["zhenqi"][math.random(1, zhenqiNum)], 1, isFree, false, 69)
        end
        if cittaNum > 0 and math.random(1, 10000) <= 3000 then
            package:Add(frontAward[key]["citta"][math.random(1, cittaNum)], 1, isFree, false, 69)
        end
        if trumpNum > 0 and math.random(1, 10000) <= 3000 then
            package:Add(frontAward[key]["trump"][math.random(1, trumpNum)], 1, isFree, false, 69)
        end
        if gemsNum > 0 and math.random(1, 10000) <= 2000 then
            package:Add(frontAward[key]["gems"][math.random(1, gemsNum)], 1, isFree, false, 69)
        end
        if otherNum > 0 and math.random(1, 10000) <= 10000 then
            package:Add(frontAward[key]["other"][math.random(1, otherNum)], 1, isFree, false, 69)
        end
    end
    for _, id in pairs(frontAward[key]["huiji"]) do
        package:Add(id, 1, true, false, 69)
    end
    player:AddExp(frontAward[key]["exp"])
    --类似抽奖功能
    local rnd = math.random(1, 4)
    if rnd == 1 and zhenqiNum > 0 then
        package:Add(frontAward[key]["zhenqi"][math.random(1, zhenqiNum)], 1, isFree, false, 69)
    elseif rnd == 2 and cittaNum > 0 then
        package:Add(frontAward[key]["citta"][math.random(1, cittaNum)], 1, isFree, false, 69)
    elseif rnd == 3 and trumpNum > 0 then
        package:Add(frontAward[key]["trump"][math.random(1, trumpNum)], 1, isFree, false, 69)
    elseif rnd == 4 and gemsNum > 0 then
        package:Add(frontAward[key]["gems"][math.random(1, gemsNum)], 1, isFree, false, 69)
    end
end

local dungeonAward = {
    [30] = {
        ["equip1"] = {2024,2025,2026,2027,2028,2029,2030,2031,2032,2033,2034,2035,2036,2037,2038,2039,2040,2041,2042,2043,2044,2045,2046,2047,2048,2049,2050,2051,2052,2053,2054,2055,2056,2057,2058,2059,2060,2061,2062,2063,2064,2065,2066,2067,2068,2069,2070,2071},
        ["equip2"] = {2232,2233,2234,2235,2236,2237,2238,2239,2240,2241,2242,2243,2244,2245,2246,2247,2248,2249,2250,2251,2252,2253,2254,2255},
        ["equip3"] = {2376,2377,2378,2379,2380,2381,2382,2383,2384,2385,2386,2387,2388,2389,2390,2391,2392,2393,2394,2395,2396,2397,2398,2399},
        ["citta"] = {1214,1213},
        ["huiji"] = {9371},
        ["other"] = {48,51,55,510},
    },
    [45] = {
        ["equip1"] = {2072,2073,2074,2075,2076,2077,2078,2079,2080,2081,2082,2083,2084,2085,2086,2087,2088,2089,2090,2091,2092,2093,2094,2095,2048,2049,2050,2051,2052,2053,2054,2055,2056,2057,2058,2059,2060,2061,2062,2063,2064,2065,2066,2067,2068,2069,2070,2071},
        ["equip2"] = {2232,2233,2234,2235,2236,2237,2238,2239,2240,2241,2242,2243,2244,2245,2246,2247,2248,2249,2250,2251,2252,2253,2254,2255},
        ["equip3"] = {2400,2401,2402,2403,2404,2405,2406,2407,2408,2409,2410,2411,2412,2413,2414,2415,2416,2417,2418,2419,2420,2421,2422,2423},
        ["citta"] = {1222,1223,1226,1306},
        ["huiji"] = {9371},
        ["other"] = {48,51,55,510},
    },
    [60] = {
        ["equip1"] = {2096,2097,2098,2099,2100,2101,2102,2103,2104,2105,2106,2107,2108,2109,2110,2111,2112,2113,2114,2115,2116,2117,2118,2119,2120,2121,2122,2123,2124,2125,2126,2127,2128,2129,2130,2131,2132,2133,2134,2135,2136,2137,2138,2139,2140,2141,2142,2143},
        ["equip2"] = {2280,2281,2282,2283,2284,2285,2286,2287,2288,2289,2290,2291,2292,2293,2294,2295,2296,2297,2298,2299,2300,2301,2302,2303},
        ["equip3"] = {2424,2425,2426,2427,2428,2429,2430,2431,2432,2433,2434,2435,2436,2437,2438,2439,2440,2441,2442,2443,2444,2445,2446,2447},
        ["citta"] = {1227,1234,1230,1307},
        ["huiji"] = {9371},
        ["other"] = {48,51,55,510},
    },
    [75] = {
        ["equip1"] = {2120,2121,2122,2123,2124,2125,2126,2127,2128,2129,2130,2131,2132,2133,2134,2135,2136,2137,2138,2139,2140,2141,2142,2143},
        ["equip2"] = {2304,2305,2306,2307,2308,2309,2310,2311,2312,2313,2314,2315,2316,2317,2318,2319,2320,2321,2322,2323,2324,2325,2326,2327},
        ["equip3"] = {2448,2449,2450,2451,2452,2453,2454,2455,2456,2457,2458,2459,2460,2461,2462,2463,2464,2465,2466,2467,2468,2469,2470,2471},
        ["citta"] = {1236,1237},
        ["huiji"] = {9371},
        ["other"] = {48,51,55,510},
    },
    [90] = {
        ["equip1"] = {2168,2169,2170,2171,2172,2173,2174,2175,2176,2177,2178,2179,2180,2181,2182,2183,2184,2185,2186,2187,2188,2189,2190,2191},
        ["equip2"] = {2352,2353,2354,2355,2356,2357,2358,2359,2360,2361,2362,2363,2364,2365,2366,2367,2368,2369,2370,2371,2372,2373,2374,2375},
        ["equip3"] = {2496,2497,2498,2499,2500,2501,2502,2503,2504,2505,2506,2507,2508,2509,2510,2511,2512,2513,2514,2515,2516,2517,2518,2519},
        ["citta"] = {1454,1455,1456,1457,1458,1459,1460,1461,1462,1463,1464,1465,1466,1467},
        ["huiji"] = {9371},
        ["other"] = {48,51,55,510},
    },
}
local dungeonAward_diff = {
    ["30_0"] = {
        ["special"] = {},
        ["exp"] = 165875,
    },
    ["30_1"] = {
        ["special"] = {30,9420},
        ["exp"] = 182463,
    },
    ["45_0"] = {
        ["special"] = {},
        ["exp"] = 348858,
    },
    ["45_1"] = {
        ["special"] = {13,30,9420},
        ["exp"] = 383743,
    },
    ["60_0"] = {
        ["special"] = {},
        ["exp"] = 624838,
    },
    ["60_1"] = {
        ["special"] = {13,30,9420},
        ["exp"] = 687321,
    },
    ["75_0"] = {
        ["special"] = {},
        ["exp"] = 969034,
    },
    ["75_1"] = {
        ["special"] = {13,30,9420},
        ["exp"] = 1065937,
    },
    ["90_0"] = {
        ["special"] = {},
        ["exp"] = 1290609,
    },
    ["90_1"] = {
        ["special"] = {13,30,9420},
        ["exp"] = 1419670,
    },
}
function getJiqirenAward_Dungeon(player, diff, isFree)
    if nil == player or nil == diff or nil == isFree then
        return
    end
    local level = player:GetLev()
    if level < 30 then
        return
    end
    if level < 45 then
        key = 30
    elseif level < 60 then
        key = 45
    elseif level < 75 then
        key = 60
    elseif level < 90 then
        key = 75
    else
        key = 90
    end
    if nil == dungeonAward[key] then
        return
    end
    local package = player:GetPackage()
    local equip1Num = #dungeonAward[key]["equip1"]
    local equip2Num = #dungeonAward[key]["equip2"]
    local equip3Num = #dungeonAward[key]["equip3"]
    for k = 1, 5 do
        if equip1Num > 0 and math.random(1, 10000) <= 5000 then
            package:Add(dungeonAward[key]["equip1"][math.random(1, equip1Num)], 1, isFree, false, 69)
        end
        if equip2Num > 0 and math.random(1, 10000) <= 5000 then
            package:Add(dungeonAward[key]["equip2"][math.random(1, equip2Num)], 1, isFree, false, 69)
        end
    end
    for k = 1, 2 do
        if equip3Num > 0 and math.random(1, 10000) <= 5000 then
            package:Add(dungeonAward[key]["equip3"][math.random(1, equip3Num)], 1, isFree, false, 69)
        end
    end
    local cittaNum = #dungeonAward[key]["citta"]
    local otherNum = #dungeonAward[key]["other"]
    for k = 1, 2 do
        if cittaNum > 0 and math.random(1, 10000) <= 3000 then
            package:Add(dungeonAward[key]["citta"][math.random(1, cittaNum)], 1, isFree, false, 69)
        end
        if otherNum > 0 and math.random(1, 10000) <= 10000 then
            package:Add(dungeonAward[key]["other"][math.random(1, otherNum)], 1, isFree, false, 69)
        end
    end
    for _, id in pairs(dungeonAward[key]["huiji"]) do
        package:Add(id, 1, true, false, 69)
    end

    local key1 = key .. "_" .. diff
    local specailNum = 0
    if nil ~= dungeonAward_diff[key1] then
        specailNum = #dungeonAward_diff[key1]["special"]
        if specailNum > 0 then
            for k = 1, math.random(2, 5) do
                package:Add(dungeonAward_diff[key1]["special"][math.random(1, specailNum)], 1, isFree, false, 69)
            end
        end
        player:AddExp(dungeonAward_diff[key1]["exp"])
    end
    --类似抽奖功能
    local rnd = math.random(1, 2)
    if rnd == 1 and cittaNum > 0 then
        package:Add(dungeonAward[key]["citta"][math.random(1, cittaNum)], 1, isFree, false, 69)
    elseif rnd == 2 and specailNum > 0 then
        package:Add(dungeonAward_diff[key1]["special"][math.random(1, specailNum)], 1, isFree, false, 69)
    end
end

local xjfrontAward = {
    [75] = {
        ["zhenyuan"] = {15001,15021,15041,15061},
        ["zhenqi"] = {1116,1118,1119,1120,1121,1122,1123,1124},
        ["other"] = {35},
        ["exp"] = 9242,
    },
    [80] = {
        ["zhenyuan"] = {15001,15021,15041,15061,15002,15022,15042,15062},
        ["zhenqi"] = {1059,1061,1062,1063,1064},
        ["other"] = {35},
        ["exp"] = 14752,
    },
    [85] = {
        ["zhenyuan"] = {15001,15021,15041,15061,15002,15022,15042,15062,15003,15023,15043,15063},
        ["zhenqi"] = {1107,1109,1110,1111,1112,1113,1114,1115},
        ["other"] = {35},
        ["exp"] = 17225,
    },
    [90] = {
        ["zhenyuan"] = {15001,15021,15041,15061,15002,15022,15042,15062,15003,15023,15043,15063,15004,15024,15044,15064},
        ["zhenqi"] = {1096,1098,1099,1100,1101,1102,1103,1104,1105,1106},
        ["other"] = {35},
        ["exp"] = 23712,
    },
    [95] = {
        ["zhenyuan"] = {15001,15021,15041,15061,15002,15022,15042,15062,15003,15023,15043,15063,15004,15024,15044,15064,15005,15025,15045,15065},
        ["zhenqi"] = {1084,1086,1087,1088,1089,1090,1091,1092,1093,1094,1095},
        ["other"] = {35},
        ["exp"] = 30087,
    },
    [100] = {
        ["zhenyuan"] = {15001,15021,15041,15061,15002,15022,15042,15062,15003,15023,15043,15063,15004,15024,15044,15064,15005,15025,15045,15065,15006,15026,15046,15066},
        ["zhenqi"] = {1074,1076,1077,1078,1079,1080,1081,1082,1083},
        ["other"] = {35},
        ["exp"] = 31852,
    },
    [105] = {
        ["zhenyuan"] = {15001,15021,15041,15061,15002,15022,15042,15062,15003,15023,15043,15063,15004,15024,15044,15064,15005,15025,15045,15065,15006,15026,15046,15066,15007,15027,15047,15067},
        ["zhenqi"] = {1020,1022,1023,1024,1025,1026,1027,1028,1029,1030},
        ["other"] = {35},
        ["exp"] = 39235,
    },
    [110] = {
        ["zhenyuan"] = {15001,15021,15041,15061,15002,15022,15042,15062,15003,15023,15043,15063,15004,15024,15044,15064,15005,15025,15045,15065,15006,15026,15046,15066,15007,15027,15047,15067,15008,15028,15048,15068},
        ["zhenqi"] = {1065,1067,1068,1069,1070,1071,1072,1073},
        ["other"] = {35},
        ["exp"] = 41127,
    },
    [115] = {
        ["zhenyuan"] = {15001,15021,15041,15061,15002,15022,15042,15062,15003,15023,15043,15063,15004,15024,15044,15064,15005,15025,15045,15065,15006,15026,15046,15066,15007,15027,15047,15067,15008,15028,15048,15068,15009,15029,15049,15069},
        ["zhenqi"] = {1040,1042,1043,1044,1045,1046,1047,1048,1049,1050,1051},
        ["other"] = {35},
        ["exp"] = 54962,
    },
    [120] = {
        ["zhenyuan"] = {15001,15021,15041,15061,15002,15022,15042,15062,15003,15023,15043,15063,15004,15024,15044,15064,15005,15025,15045,15065,15006,15026,15046,15066,15007,15027,15047,15067,15008,15028,15048,15068,15009,15029,15049,15069,15010,15030,15050,15070},
        ["zhenqi"] = {1052,1054,1055,1056,1057,1058},
        ["other"] = {35},
        ["exp"] = 61862,
    },
    [125] = {
        ["zhenyuan"] = {15001,15021,15041,15061,15002,15022,15042,15062,15003,15023,15043,15063,15004,15024,15044,15064,15005,15025,15045,15065,15006,15026,15046,15066,15007,15027,15047,15067,15008,15028,15048,15068,15009,15029,15049,15069,15010,15030,15050,15070,15011,15031,15051,15071},
        ["zhenqi"] = {1031,1033,1034,1035,1036,1037,1038,1039},
        ["other"] = {35},
        ["exp"] = 58820,
    },
    [130] = {
        ["zhenyuan"] = {15001,15021,15041,15061,15002,15022,15042,15062,15003,15023,15043,15063,15004,15024,15044,15064,15005,15025,15045,15065,15006,15026,15046,15066,15007,15027,15047,15067,15008,15028,15048,15068,15009,15029,15049,15069,15010,15030,15050,15070,15011,15031,15051,15071,15012,15032,15052,15072},
        ["zhenqi"] = {1012,1014,1015,1016,1017,1018,1019},
        ["other"] = {35},
        ["exp"] = 58187,
    },
}
function getJiqirenAward_XJFrontMap(player, isFree)
    if nil == player or nil == isFree then
        return
    end
    local level = player:GetLev()
    if level < 75 then
        return
    end
    local key = 0
    if level < 80 then
        key = 75
    elseif level < 85 then
        key = 80
    elseif level < 90 then
        key = 85
    elseif level < 95 then
        key = 90
    elseif level < 100 then
        key = 95
    elseif level < 105 then
        key = 100
    elseif level < 110 then
        key = 105
    elseif level < 115 then
        key = 110
    elseif level < 120 then
        key = 115
    elseif level < 125 then
        key = 120
    elseif level < 130 then
        key = 125
    else
        key = 130
    end
    if nil == xjfrontAward[key] then
        return
    end
    local package = player:GetPackage()
    local zhenyuanNum = #xjfrontAward[key]["zhenyuan"]
    local zhenqiNum = #xjfrontAward[key]["zhenqi"]
    local otherNum = #xjfrontAward[key]["other"]

    for k = 1, 5 do
        if zhenyuanNum > 0 and math.random(1, 10000) <= 5000 then
            package:Add(xjfrontAward[key]["zhenyuan"][math.random(1, zhenyuanNum)], 1, isFree, false, 69)
        end
    end

    for k = 1, 2 do
        if zhenqiNum > 0 and math.random(1, 10000) <= 5000 then
            package:Add(xjfrontAward[key]["zhenqi"][math.random(1, zhenqiNum)], 1, isFree, false, 69)
        end
        if otherNum > 0 and math.random(1, 10000) <= 10000 then
            package:Add(xjfrontAward[key]["other"][math.random(1, otherNum)], 1, isFree, false, 69)
        end
    end
    
    player:AddExp(xjfrontAward[key]["exp"])
end

local fairycopyAward = {
    [80] = {
        ["equip1"] = {18049,18050,18051,18052,18053,18097,18098,18099,18100,18101,18102},
        ["equip2"] = {18145,18146,18147,18148,18149,18150,18801,18802,18803,18804,18805,18806,18807,18808,18809,18810,18811,18812},
        ["other"] = {17102},
        ["exp"] = 10000,
    },
    [90] = {
        ["equip1"] = {18057,18058,18059,18060,18061,18062,18105,18106,18107,18108,18109,18110},
        ["equip2"] = {18153,18154,18155,18156,18157,18158,18801,18802,18803,18804,18805,18806,18807,18808,18809,18810,18811,18812},
        ["other"] = {17102},
        ["exp"] = 10000,
    },
    [100] = {
        ["equip1"] = {18065,18066,18067,18068,18069,18070,18113,18114,18115,18116,18117,18118},
        ["equip2"] = {18177,18178,18179,18180,18181,18182,18801,18802,18803,18804,18805,18806,18807,18808,18809,18810,18811,18812},
        ["other"] = {17102},
        ["exp"] = 10000,
    },
    [110] = {
        ["equip1"] = {18065,18066,18067,18068,18069,18070,18113,18114,18115,18116,18117,18118},
        ["equip2"] = {18169,18170,18171,18172,18173,18174,18801,18802,18803,18804,18805,18806,18807,18808,18809,18810,18811,18812},
        ["other"] = {17102},
        ["exp"] = 10000,
    },
    [120] = {
        ["equip1"] = {18065,18066,18067,18068,18069,18070,18113,18114,18115,18116,18117,18118},
        ["equip2"] = {18161,18162,18163,18164,18165,18166,18801,18802,18803,18804,18805,18806,18807,18808,18809,18810,18811,18812},
        ["other"] = {17102},
        ["exp"] = 10000,
    },
    [130] = {
        ["equip1"] = {18065,18066,18067,18068,18069,18070,18113,18114,18115,18116,18117,18118},
        ["equip2"] = {18185,18186,18187,18188,18189,18190,18801,18802,18803,18804,18805,18806,18807,18808,18809,18810,18811,18812},
        ["other"] = {17102},
        ["exp"] = 10000,
    },
    
}
function getJiqirenAward_FairyCopy(player, isFree)
    if nil == player or nil == isFree then
        return
    end
    local level = player:GetLev()
    if level < 80 then
        return
    end
    local key = 0
    if level < 90 then
        key = 80 
    elseif level < 100 then
        key = 90
    elseif level < 110 then
        key = 100
    elseif level < 120 then
        key = 110
    elseif level < 130 then
        key = 120
    else
        key = 130
    end
    if nil == fairycopyAward[key] then
        return
    end
    local package = player:GetPackage()
    local equip1Num = #fairycopyAward[key]["equip1"]
    local equip2Num = #fairycopyAward[key]["equip2"]
    local otherNum = #fairycopyAward[key]["other"]

    for k = 1, 2 do
        if equip1Num > 0 and math.random(1, 10000) <= 5000 then
            package:Add(fairycopyAward[key]["equip1"][math.random(1, equip1Num)], 1, isFree, false, 69)
        end
        if otherNum > 0 and math.random(1, 10000) <= 10000 then
            package:Add(fairycopyAward[key]["other"][math.random(1, otherNum)], 1, isFree, false, 69)
        end
    end

    if equip2Num > 0 and math.random(1, 10000) <= 5000 then
        package:Add(fairycopyAward[key]["equip2"][math.random(1, equip2Num)], 1, isFree, false, 69)
    end
    
    player:AddExp(fairycopyAward[key]["exp"])
end

local WeiXinShopItems = {
    [1] = {5066, 388, 10},
    [2] = {9076, 66, 10},
    [3] = {509, 29, 99},
    [4] = {515, 29, 99},
    [5] = {9498, 20, 59},
    [6] = {16001, 15, 59},
}

function getWeiXinShopItemNum()
    return #WeiXinShopItems
end

function getWeiXinShop(index)
    return WeiXinShopItems[index]
end

local FlyRoadAward = {
    [1] = {{17102, 5}, {17104,1}, {17107,1}, {17110,1}},
    [2] = {{17102, 5}, {17105,1}, {17106,2}, {17109,1}, {17111,1}},
    [3] = {{17102, 7}, {17103,1}, {17104,1}, {17107,1}},
    [4] = {{17102, 7}, {17106,2}, {17109,2}},
    [5] = {{17102, 10}, {17104,2}, {17107,1}, {17111,2}},
    [6] = {{17102, 10}, {17103,1}, {17106,2}, {17110,2}},
    [7] = {{17102, 13}, {17104,2}, {17105,1}, {17107,1}, {17111,2}},
    [8] = {{17102, 13}, {17106,2}, {17109,2}, {17110,2}},
    [9] = {{17102, 15}, {17103,1}, {17104,2}, {17105,1}, {17107,1}},
    [10] = {{17102, 15}, {17103,2}, {17104,2}, {17106,2}, {17109,5}, {17110,5}, {17111,5}}
}

function getFlyRoadAward(player, index, isdouble)
    local item = FlyRoadAward[index]
    if item == nil then
        return false
    end

    local package = player:GetPackage()
    if #item > package:GetRestPackageSize() then
        player:sendMsgCode(2, 1011, 0)
        return false
    end

    for n = 1, #item do
        if isdouble ~=0 then
            package:AddItem(item[n][1], item[n][2] * 2, true)
        else
            package:AddItem(item[n][1], item[n][2], true)
        end
    end
    return true
end

local TreasureHouseAward = {
    [1] = {13192, 1500},
    [2] = {13012, 1888},
    [3] = {13032, 1888},
    [4] = {13052, 1888},
    [5] = {13092, 1888},
    [6] = {13152, 1888},
    [7] = {13112, 1888},
    [8] = {13132, 1888},
    [9] = {13072, 1888},
    [10] = {7021, 88},
    [11] = {7121, 88},
    [12] = {7221, 88},
    [13] = {7321, 88},
    [14] = {7421, 88},
    [15] = {7521, 88},
    [16] = {7621, 88},
    [17] = {7721, 88},
    [18] = {503, 28},
    [19] = {515, 78},
    [20] = {9457, 28},
    [21] = {9600, 28},
    [22] = {9498, 28},
    [23] = {9649, 20},
    [24] = {17103, 28},
    [25] = {17109, 18},
    [26] = {17107, 66},
    [27] = {17105, 78},
    [28] = {1733, 5088},
    [29] = {1732, 5088},
    [30] = {1734, 5088},
    [31] = {1735, 5088},
    [32] = {9076, 88},
    [33] = {9022, 168},
    [34] = {9075, 148},
    [35] = {9068, 158},
    [36] = {1742, 6088},
    [37] = {1741, 6088}
}

function exchangeTreasure(player,index,num,now)
    local item = TreasureHouseAward[index]
    if item == nil then
        return false
    end

    local package = player:GetPackage()
    if #item > package:GetRestPackageSize() and #item > package:GetRestPackageSize(1) then
        player:sendMsgCode(2, 1011, 0)
        return false
    end
    local var = player:GetVar(873)
    local tmp = item[2]
    if var < tmp * num then
        player:sendMsgCode(2, 1011, 0)
        return false;
    end
    var = var - tmp * num
    player:SetVar(873,var)
    package:Add(item[1], num, true)
    player:SetExchangeTreasureLog(now,item[1],num,true)

    return true
end


