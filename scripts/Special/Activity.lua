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
        package:AddItem(9275, 1, true)
    elseif _type == 1 then
        package:AddItem(9275, 2, true)
    elseif _type == 2 then
        package:AddItem(9275, 4, true)
    elseif _type == 3 then
        package:AddItem(9275, 6, true)
    elseif _type == 5 then
        package:AddItem(9275, 5, true)
    end
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

    onWansheng(player)
    TgcEvent(player, 0)
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
    Wansheng(player, lootlvl);
    if free == true then
        FallActivity(player, 1)
        Guoqing(player, 0);
    else
        FallActivity(player, 2)
        Guoqing(player, 3);
    end
    if getKillMonsterAct() then
        local package = player:GetPackage();
        package:Add(9163, 1, true)
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
    Wansheng(player, lootlvl);
    Guoqing(player, lootlvl);
    LuckyDrawBox(player, id)
    ExJob(player, id, lootlvl)
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
        package:Add(9163, 1, true)
    end
    sendWinReward(player, lootlvl, 1);
    TgcEvent(player, 1)
    _9215Act(player, lootlvl);
    _snowAct(player, lootlvl);
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
    Wansheng(player, lootlvl);
    Guoqing(player, lootlvl);
    if lootlvl == 0 then
        FallActivity(player, 1)
    else
        FallActivity(player, lootlvl)
    end
    if getKillMonsterAct() then
        local package = player:GetPackage();
        package:Add(9163, 1, true)
    end
    sendWinReward(player, lootlvl, 2);
    TgcEvent(player, 2)
    _9215Act(player, lootlvl);
    _snowAct(player, lootlvl);
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

function sendRechargeMails(player, ototal, ntotal)
    local t = { ['year'] = 2012, ['month'] = 12, ['day'] = 8, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11

    if n >= s and n < (s + 1*86400) then
        sendRechargeMails_2012_12_08(player, ototal, ntotal)
    elseif n >= (s + 1*86400) and n < (s + 2*86400) then
        sendRechargeMails_2012_12_09(player, ototal, ntotal)
    elseif n >= (s + 2*86400) and n < (s + 3*86400) then
        sendRechargeMails_2012_12_10(player, ototal, ntotal)
    elseif n >= (s + 3*86400) and n < (s + 4*86400) then
        sendRechargeMails_2012_12_11(player, ototal, ntotal)
    elseif n >= (s + 4*86400) and n < (s + 5*86400) then
        sendRechargeMails_2012_12_12(player, ototal, ntotal)
    elseif n >= (s + 5*86400) and n < (s + 6*86400) then
        sendRechargeMails_2012_12_13(player, ototal, ntotal)
    elseif n >= (s + 6*86400) and n < (s + 7*86400) then
        sendRechargeMails_2012_12_14(player, ototal, ntotal)
    elseif n >= (s + 7*86400) and n < (s + 8*86400) then
        sendRechargeMails_2012_12_15(player, ototal, ntotal)
    elseif n >= (s + 8*86400) and n < (s + 9*86400) then
        sendRechargeMails_2012_12_16(player, ototal, ntotal)
    elseif n >= (s + 9*86400) and n < (s + 10*86400) then
        sendRechargeMails_2012_12_17(player, ototal, ntotal)
    elseif n >= (s + 10*86400) and n < (s + 11*86400) then
        sendRechargeMails_2012_12_18(player, ototal, ntotal)
    elseif n >= (s + 11*86400) and n < (s + 12*86400) then
        sendRechargeMails_2012_12_19(player, ototal, ntotal)
    elseif n >= (s + 12*86400) and n < (s + 13*86400) then
        sendRechargeMails_2012_12_20(player, ototal, ntotal)
    elseif n >= (s + 13*86400) and n < (s + 14*86400) then
        sendRechargeMails_2012_12_21(player, ototal, ntotal)
    elseif n >= (s + 14*86400) and n < (s + 15*86400) then
        sendRechargeMails_2012_12_22(player, ototal, ntotal)
    elseif n >= (s + 15*86400) and n < (s + 16*86400) then
        sendRechargeMails_2012_12_23(player, ototal, ntotal)
    elseif n >= (s + 16*86400) and n < (s + 17*86400) then
        sendRechargeMails_2012_12_24(player, ototal, ntotal)
    elseif n >= (s + 17*86400) and n < (s + 18*86400) then
        sendRechargeMails_2012_12_25(player, ototal, ntotal)
    elseif n >= (s + 18*86400) and n < (s + 19*86400) then
        sendRechargeMails_2012_12_26(player, ototal, ntotal)
    elseif n >= (s + 19*86400) and n < (s + 20*86400) then
        sendRechargeMails_2012_12_27(player, ototal, ntotal)
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

function sendRechargeRankAward(player, pos)
    local t = { ['year'] = 2012, ['month'] = 12 , ['day'] = 8, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time()
    if n >= s and n < (s + 86400 + 10*60) then
        sendRechargeRankAward_2012_12_08(player, pos)
    elseif n >= (s + 86400 + 10*60) and n < (s + 2*86400 + 10*60) then
        sendRechargeRankAward_2012_12_09(player, pos)
    elseif n >= (s + 2*86400 + 10*60) and n < (s + 3*86400 + 10*60) then
        sendRechargeRankAward_2012_12_10(player, pos)
    elseif n >= (s + 3*86400 + 10*60) and n < (s + 4*86400 + 10*60) then
        sendRechargeRankAward_2012_12_11(player, pos)
    elseif n >= (s + 4*86400 + 10*60) and n < (s + 5*86400 + 10*60) then
        sendRechargeRankAward_2012_12_12(player, pos)
    elseif n >= (s + 5*86400 + 10*60) and n < (s + 6*86400 + 10*60) then
        sendRechargeRankAward_2012_12_13(player, pos)
    elseif n >= (s + 6*86400 + 10*60) and n < (s + 7*86400 + 10*60) then
        sendRechargeRankAward_2012_12_14(player, pos)
    elseif n >= (s + 7*86400 + 10*60) and n < (s + 8*86400 + 10*60) then
        sendRechargeRankAward_2012_12_15(player, pos)
    elseif n >= (s + 8*86400 + 10*60) and n < (s + 9*86400 + 10*60) then
        sendRechargeRankAward_2012_12_16(player, pos)
    elseif n >= (s + 9*86400 + 10*60) and n < (s + 10*86400 + 10*60) then
        sendRechargeRankAward_2012_12_17(player, pos)
    elseif n >= (s + 10*86400 + 10*60) and n < (s + 11*86400 + 10*60) then
        sendRechargeRankAward_2012_12_18(player, pos)
    elseif n >= (s + 11*86400 + 10*60) and n < (s + 12*86400 + 10*60) then
        sendRechargeRankAward_2012_12_19(player, pos)
    elseif n >= (s + 12*86400 + 10*60) and n < (s + 13*86400 + 10*60) then
        sendRechargeRankAward_2012_12_20(player, pos)
    elseif n >= (s + 13*86400 + 10*60) and n < (s + 14*86400 + 10*60) then
        sendRechargeRankAward_2012_12_21(player, pos)
    elseif n >= (s + 14*86400 + 10*60) and n < (s + 15*86400 + 10*60) then
        sendRechargeRankAward_2012_12_22(player, pos)
    elseif n >= (s + 15*86400 + 10*60) and n < (s + 16*86400 + 10*60) then
        sendRechargeRankAward_2012_12_23(player, pos)
    elseif n >= (s + 16*86400 + 10*60) and n < (s + 17*86400 + 10*60) then
        sendRechargeRankAward_2012_12_24(player, pos)
    elseif n >= (s + 17*86400 + 10*60) and n < (s + 18*86400 + 10*60) then
        sendRechargeRankAward_2012_12_25(player, pos)
    elseif n >= (s + 18*86400 + 10*60) and n < (s + 19*86400 + 10*60) then
        sendRechargeRankAward_2012_12_26(player, pos)
    elseif n >= (s + 19*86400 + 10*60) and n < (s + 20*86400 + 10*60) then
        sendRechargeRankAward_2012_12_27(player, pos)
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

function sendConsumeMails(player, ototal, ntotal)
    local t = { ['year'] = 2012, ['month'] = 12, ['day'] = 15, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)
    local n = os.time() + 11
    if n >= s and n < (s + 7*86400) then
        sendConsumeMails_2012_12_15(player, ototal, ntotal)
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
        {{502, 2, 10}, {510, 1, 10}, {29, 10, 10}},
        {{56, 1, 20}, {500, 1, 20}, {57, 1, 20}},
        {{511, 2, 20}, {512, 1, 30}, {517, 1, 30}},
        {{9122, 1, 10}, {9122, 1, 10}, {9122, 1, 10}},
        {{503, 1, 30}, {514, 1, 30}, {501, 1, 30}},
        {{509, 1, 40}, {134, 1, 40}, {1325, 1, 40}},
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
        if package:GetRestPackageSize() < 9 then
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
        {{502,  1, 1, 1}, {55,  1, 0, 0}, {510,  1, 0, 0}},
        {{56,   1, 0, 0}, {57,  1, 0, 0}, {133, 1, 1, 1}},
        {{502,  1, 1, 1}, {55,  1, 0, 0}, {510,  1, 0, 0}},
        {{1326, 1, 0, 0}, {466, 1, 0, 0}, {8000, 1, 3, 1}},
        {{56,   1, 0, 0}, {57,  1, 0, 0}, {133, 1, 3, 1}},
        {{502,  1, 2, 1}, {55,  1, 0, 0}, {510,  1, 0, 0}},
        {{1326, 1, 0, 0}, {466, 1, 0, 0}, {8000, 1, 2, 1}},
        {{502,  1, 3, 1}, {55,  1, 0, 0}, {510,  1, 0, 0}},
        {{503,  1, 4, 1}, {33,  1, 0, 0}, {508,  1, 0, 0}},
        {{1326, 1, 0, 0}, {466, 1, 0, 0}, {8000, 1, 3, 1}},
        {{1326, 1, 0, 0}, {466, 1, 0, 0}, {8000, 1, 2, 1}},
        {{503,  1, 4, 1}, {33,  1, 0, 0}, {508,  1, 0, 0}},
        {{503,  1, 3, 1}, {33,  1, 0, 0}, {508,  1, 0, 0}},
        {{1326, 1, 0, 0}, {466, 1, 0, 0}, {8000, 1, 2, 1}},
        {{1326, 1, 0, 0}, {466, 1, 0, 0}, {8000, 1, 2, 1}},
        {{9076, 1, 4, 1}, {9076,1, 4, 1}, {9076, 1, 4, 1}},
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
    local j = math.random(1, 3)

    package:Add(eventItem[i][j][1], eventItem[i][j][2], true, true, 33)
    player:lastKillMonsterAwardPush(eventItem[i][j][1], eventItem[i][j][2]);
    if eventItem[i][j][3] >=1 and eventItem[i][j][3] <= 4 then
        local curType = eventItem[i][j][3]
        local extAward = 0
        if curType == 1 and player:GetVar(183) == (1-1) then
            extAward = 1757
        elseif curType == 2 and player:GetVar(184) == (1-1) then
            extAward = 1751
        elseif curType == 3 and player:GetVar(185) == (1-1) then
            extAward = 1750
        elseif curType == 4 and player:GetVar(186) == (1-1) then
            extAward = 1752
        elseif curType == 1 and player:GetVar(183) == (5-1) then
            extAward = 1753
        elseif curType == 2 and player:GetVar(184) == (3-1) then
            extAward = 1754
        elseif curType == 3 and player:GetVar(185) == (4-1) then
            extAward = 1755
        elseif curType == 4 and player:GetVar(186) == (2-1) then
            extAward = 1756
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
        package:AddItem(9229, 1, true)
    end
end

function onFirstRecharge(player, index)
    if index == 0 or index > 8 then
        return false
    end

    local items = {
        {{56, 5}, {57, 2}, {503, 5}, {514, 10}, {515, 2}, {507, 2}, {509, 2}, {15, 3}, {517, 2}},
        {{56, 10}, {57, 4}, {503, 10}, {514, 20}, {515, 4}, {507, 4}, {509, 4}, {15, 6}, {517, 4}},

        {{2544, 1}, {2545, 1}, {2546, 1}, {2547, 1}, {2548, 1}, {2549, 1}, {2550, 1}, {2551, 1}, {1600, 1}},
        {{2712, 1}, {2713, 1}, {2714, 1}, {2715, 1}, {2716, 1}, {2717, 1}, {2718, 1}, {2719, 1}, {1608, 1}},

        {{2552, 1}, {2553, 1}, {2554, 1}, {2555, 1}, {2556, 1}, {2557, 1}, {2558, 1}, {2559, 1}, {1601, 1}},
        {{2720, 1}, {2721, 1}, {2722, 1}, {2723, 1}, {2724, 1}, {2725, 1}, {2726, 1}, {2727, 1}, {1609, 1}},

        {{2560, 1}, {2561, 1}, {2562, 1}, {2563, 1}, {2564, 1}, {2565, 1}, {2566, 1}, {2567, 1}, {1602, 1}},
        {{2728, 1}, {2729, 1}, {2730, 1}, {2731, 1}, {2732, 1}, {2733, 1}, {2734, 1}, {2735, 1}, {1610, 1}},
    }
    local item = items[index]

    local package = player:GetPackage()
    if package:GetRestPackageSize() < #item then
        player:sendMsgCode(2, 1011, 0)
        return false
    end

    for k, v in pairs(item) do
        package:Add(v[1], v[2], 1)
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
        [1] = {{2544,30},{2545,30},{2546,30},{2547,30},{2548,30},{2549,30},{2550,30},{2551,30},{2552,30},{2553,30},{2554,30},{2555,30},{2556,30},{2557,30},{2558,30},{2559,30},{2560,30},{2561,30},{2562,30},{2563,30},{2564,30},{2565,30},{2566,30},{2567,30},},
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
        [2] = {{6039,10},{6040,10},{6041,10},{6042,10},{6043,10},{6044,10},{6045,10},{6046,10},{6047,10},{6048,10},{6049,10},{6050,10},{6051,10},{6052,10},},
    },

    -- 阵图
    -- level 35
    [1284] = {
        [1] = {{1600,30},{1601,30},{1602,30},{1500,30},{1501,30},{1502,30},},
        [2] = {{1205,20},{1220,20},{1221,20},},
    },
    -- level 45
    [2053] = {
        [1] = {{1604,30},{1605,30},{1606,30},{1503,30},},
        [2] = {{1231,20},{1224,20},{1308,20},},
    },
    -- level 50
    [4360] = {
        [1] = {{1603,30},{1608,30},{1609,30},{1505,30},{1506,30},},
        [2] = {{1238,20},{1233,20},{1225,20},{1311,20},},
    },
    -- level 55
    [4611] = {
        [1] = {{1610,30},{1614,30},{1615,30},{1507,30},{1508,30},},
        [2] = {{1246,20},{1229,20},{1243,20},{1315,20},},
    },
    -- level 60
    [5893] = {
        [1] = {{1612,30},{1616,30},{1607,30},{1611,30},{1509,30},},
        [2] = {{1251,20},{1317,20},},
    },
    -- level 65
    [5637] = {
        [1] = {{200,30},{201,30},{202,30},{203,30},{204,30},{205,30},{206,30},{207,30},{208,30},{82,30},{83,30},{84,30},{85,30},{86,30},{87,30},{88,30},{89,30},},
        [2] = {{1413,20},{1414,20},{1415,20},{1241,20},{1242,20},{6053,20},},
    },
    -- level 70
    [8195] = {
        [1] = {{209,30},{210,30},{211,30},{212,30},{213,30},{214,30},{215,30},{216,30},{217,30},{218,30},{219,30},{220,30},{221,30},{222,30},{223,30},{224,30},{103,30},{104,30},{105,30},{106,30},{107,30},{108,30},{109,30},{110,30},},
        [2] = {{1421,10},{1422,10},{1423,10},{1424,10},{1425,10},{1426,10},{1253,20},{1250,20},{6061,10},},
    },
    -- level 75
    [6153] = {
        [1] = {{228,30},{229,30},{230,30},{231,30},{232,30},{233,30},{234,30},{235,30},{236,30},{237,30},{238,30},{239,30},{240,30},{241,30},{242,30},{91,30},{92,30},{93,30},{94,30},{95,30},{96,30},{97,30},{98,30},{99,30},{100,30},{101,30},{102,30},},
        [2] = {{1427,10},{1428,10},{1429,10},{1430,10},{1431,10},{1432,10},{1245,20},{6067,10},},
    },
    -- level 80
    [9222] = {
        [1] = {{243,30},{244,30},{245,30},{246,30},{247,30},{248,30},{249,30},{250,30},{251,30},{252,30},{253,30},{254,30},{255,30},{256,30},{257,30},{258,30},{259,30},{260,30},{261,30},{262,30},{263,30},{264,30},{265,30},{266,30},{267,30},{268,30},{269,30},},
        [2] = {{1440,10},{1441,10},{1442,10},{1443,10},{1444,10},{1445,10},{1446,10},{1247,20},{1249,20},{6080,10},},
    },
    -- level 85
    [9481] = {
        [1] = {{271,30},{272,30},{273,30},{274,30},{275,30},{276,30},{277,30},{278,30},{279,30},{280,30},{281,30},{282,30},{283,30},{284,30},{285,30},{286,30},{287,30},{288,30},{289,30},{290,30},{291,30},{292,30},{293,30},{294,30},{295,30},{296,30},{297,30},},
        [2] = {{1447,10},{1448,10},{1449,10},{1450,10},{1451,10},{1452,10},{1453,10},{1254,20},{1255,20},{6087,10},},
    },
    -- level 90
    [10244] = {
        [1] = {{120,30},{121,30},{122,30},{123,30},{124,30},{125,30},{126,30},{127,30},{128,30},},
        [2] = {{1482,10},{1483,10},{1484,10},{1485,10},{1486,10},{1487,10},{1488,10},{1489,10},{1490,10},{1468,10},{1469,10},{1470,10},{1471,10},{1472,10},{1473,10},{1474,10},{1475,10},{1476,10},{1477,10},{1478,10},{1479,10},{1480,10},{1481,10},{6094,10},},
    },
    -- level 95
    [5129] = {
        [1] = {{299,30},{300,30},{301,30},{302,30},{303,30},{304,30},{305,30},{306,30},{307,30},},
        [2] = {{6011,10},{6012,10},{6013,10},{6014,10},{6015,10},{6016,10},{6017,10},{6018,10},{6019,10},{6020,10},{6021,10},{6022,10},{6023,10},{6024,10},{6025,10},{6026,10},{6027,10},{6028,10},{6029,10},{6030,10},{6031,10},{6032,10},{6033,10},{6112,10},},
    },
}
local common2 = {{50,20},{49,20},{514,20},{135,20},{511,20},{1412,20},{1411,20},}
local common3 = {{9283,40},}
local gem3= {{5001,40},{5011,40},{5021,40},{5031,40},{5041,40},{5051,40},{5061,40},{5071,40},{5081,40},{5091,40},{5101,40},{5111,40},{5121,40},{5131,40},{5141,40},}
local extra1 = {{50,20},{49,20},{135,20},{1411,20},{507,1},{509,1},}
local extra_2 = {{50,20},{49,20},{135,20},{1411,20},}
local item = {}

function getCopyFrontmapAward(step, localtion)
    if step > 2 then
        return {}
    end
    local order
    if step == 1 then
        order = math.random(1, 8)
        if order == 1 or order == 2 then
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
    end
end

