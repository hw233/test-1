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
    Broadcast(0x17, "[p:"..player:getCountry()..":"..player:getPName().."] 领取了立冬礼包，幸运地获得了 888 礼券!")
    return true
  elseif pkgId == 10001 then
    player:getCoupon(88)
    Broadcast(0x17, "[p:"..player:getCountry()..":"..player:getPName().."] 领取了立冬礼包，幸运地获得了 88 礼券!")
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
    Broadcast(0x17, "[p:"..player:getCountry()..":"..player:getPName().."] 领取了立冬礼包，幸运地获得了 [4:8923] x2!")
  elseif pkgId == 10003 then
    Broadcast(0x17, "[p:"..player:getCountry()..":"..player:getPName().."] 领取了立冬礼包，幸运地获得了 [4:8922] x2!")
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
end

function onDungeonWin(player, id, count)
end

function onClanBattleAttend(player)
	local stage = getActivityStage()
  	if stage == 1 then
		checkExpire(player)
		if player:hasStatus(0x400) then
			return
		end
		player:addStatus(0x400)
		sendRewardMail(player, '愚人节鱼宴邀请函', '通过勤奋努力，你在帮派战后获得了2个鱼宴邀请函，请再接再厉，多多努力，食霸非你莫属！', 9224, 2)
	end
end

function onCountryBattleAttend(player)
	local stage = getActivityStage();
	if stage == 1 then	
		sendRewardMail(player, '愚人节鱼宴邀请函', '通过勤奋努力，你在阵营战后获得了2个鱼宴邀请函，请再接再厉，多多努力，食霸非你莫属！', 9224, 2);
	end
end

function onCountryBattleWinStreak(player, count)
end

function onAttakerAward(attacker, defender, award)
	local isMale1 = attacker:IsMale();
	local isMale2 = defender:IsMale();
	if (isMale1 and not isMale2) or (not isMale1 and isMale2) then
		SendMsg(attacker, 0x35, "恭喜您通过情人节乱世佳人活动获得50%额外奖励");
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
			sendRewardMail(player, '愚人节鱼宴邀请函', '通过勤奋努力，你今日斗剑场胜利5次，获得了2个鱼宴邀请函，请再接再厉，多多努力，食霸非你莫属！', 9224, 2)
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
			sendRewardMail(player, '愚人节鱼宴邀请函', '通过勤奋努力，您今日成功完成 10 次日常循环任务，获得了2个鱼宴邀请函，请再接再厉，多多努力，食霸非你莫属！', 9224, 2)
		end
		player:setStatusBit(26, 4, cnt2)
	end
end

function onExchange(player)
end

function onMergeGem(player, lev, num)
	local stage = getActivityStage();
	local playerLev = player:GetLev();
	local orangeFavor = {5820, 5821, 5822, 5823, 5824};
	local dietyFavor = {5825, 5826, 5827, 5828};
	if stage >= 1 and stage <= 3 then
		if lev == 6 then
			sendRewardMail(player, '合成6级宝石奖励', '恭喜您！您的不懈努力终于感动了铁匠铺老板，他送了你'..num..'个三级洗炼符，赶紧领取吧！', 8916, 1 * num);
		elseif lev == 7 then
			local table_items = {8916, 1 * num, 1};			
			if playerLev >= 85 then
				for i = 1, num do 
					table.insert(table_items, dietyFavor[math.random(1, 4)]);
					table.insert(table_items, 1);
					table.insert(table_items, 0);
				end
			else
				for i = 1, num do 
					table.insert(table_items, orangeFavor[math.random(1, 5)]);
					table.insert(table_items, 1);
					table.insert(table_items, 0);
				end
			end
			-- sendItemPackageMail(player, '合成7级宝石奖励', '恭喜您！您的不懈努力终于感动了铁匠铺老板，他送了你'..num..'个喜好品、'..num..'个三级洗炼符，赶紧领取吧！', table_items);
		elseif lev == 8 then
			local table_items = {9000, 1 * num, 1};
			if playerLev >= 85 then
				for i = 1, num do 
					table.insert(table_items, dietyFavor[math.random(1, 4)]);
					table.insert(table_items, 2);
					table.insert(table_items, 0);
				end
			else
				for i = 1, num do 
					table.insert(table_items, orangeFavor[math.random(1, 5)]);
					table.insert(table_items, 2);
					table.insert(table_items, 0);
				end
			end
			-- sendItemPackageMail(player, '合成8级宝石奖励', '恭喜您！您的不懈努力终于感动了铁匠铺老板，他送了你'..(2 * num)..'个喜好品、'..num..'个潜力保护符，赶紧领取吧！', table_items);
		elseif lev == 9 then
			local table_items = {9000, 2 * num, 1};
			if playerLev >= 85 then
				for i = 1, num do 
					table.insert(table_items, dietyFavor[math.random(1, 4)]);
					table.insert(table_items, 3);
					table.insert(table_items, 0);
				end
			else
				for i = 1, num do 
					table.insert(table_items, orangeFavor[math.random(1, 5)]);
					table.insert(table_items, 3);
					table.insert(table_items, 0);
				end
			end
			-- sendItemPackageMail(player, '合成9级宝石奖励', '恭喜您！您的不懈努力终于感动了铁匠铺老板，他送了你'..(3 * num)..'个喜好品、'..(2 * num)..'个潜力保护符，赶紧领取吧！', table_items);
		elseif lev == 10 then
			local table_items = {9000, 3 * num, 1};
			if playerLev >= 85 then
				for i = 1, num do 
					table.insert(table_items, dietyFavor[math.random(1, 4)]);
					table.insert(table_items, 4);
					table.insert(table_items, 0);
				end
			else
				for i = 1, num do 
					table.insert(table_items, orangeFavor[math.random(1, 5)]);
					table.insert(table_items, 4);
					table.insert(table_items, 0);
				end
			end
			-- sendItemPackageMail(player, '合成10级宝石奖励', '恭喜您！您的不懈努力终于感动了铁匠铺老板，他送了你'..(4 * num)..'个喜好品、'..(3 * num)..'个潜力保护符，赶紧领取吧！', table_items);
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
		SendMsg(player, 0x35, "恭喜您非常幸运地获得了春节期间在线双倍奖励！");
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
		sendRewardMail(player, '装备强化7级奖励', '恭喜您！您的不懈努力终于感动了铁匠铺老板，他送了你一个银票，赶紧领取吧！', 8996, 1);		
	elseif level == 8 then
		sendRewardMail(player, '装备强化8级奖励', '恭喜您！您的不懈努力终于感动了铁匠铺老板，他送了你一个潜力保护符，赶紧领取吧！', 9000, 1);		
	elseif level == 9 then
		sendRewardMail(player, '装备强化9级奖励', '恭喜您！您的不懈努力终于感动了铁匠铺老板，他送了你一个潜力保护符，赶紧领取吧！', 9000, 1);		
	elseif level == 10 then
		local favor = {5820, 5821, 5822, 5823, 5824};
		local rand_favor = math.random(1, #favor);
		local table_items = {favor[rand_favor], 10, 0};
		-- sendItemPackageMail(player, '装备强化10级奖励', '恭喜您！您的不懈努力终于感动了铁匠铺老板，他送了你10个橙色喜好品，赶紧领取吧！', table_items);
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
			sendRewardMail(player, '愚人节鱼宴邀请函', '通过勤奋努力，你今日攻击Boss 3次，获得了2个鱼宴邀请函，请再接再厉，多多努力，食霸非你莫属！', 9224, 2)
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
	sendRewardMail(player, '充值有礼', '您在活动期间充值获得“蜀山传奇银币嘉奖礼包”一个，内含银票2张。请在背包中留出足够空间后点击接收。蜀山传奇感谢您一路支持和关爱。', 8996, 2)
  end
  if stage == 5 then
	local rand = math.random(1, 16);
	local gold = newGold - oldGold;
	player:sendTopupMail('恭喜您获得了春节字符奖券', "恭喜，您在活动期间充值了"..gold.."仙石，获得了一张相应的春节字符奖券，奖券号码为："..rand.."。\n如果您的奖券号码和2月6日公布的幸运字符一致，您将额外获得"..gold.."仙石的奖励。\n详见官网公告。", gold, rand);
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
dname = {'焚骨窟', '天人福邸', '困仙阵', '无间炼狱', '海底幻境'}
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
		sendItemPackageMail(player, '清明节拼接清明上河图', '你在副本'..dname[id]..l..'层边打怪边看风景，无意中看到地上一张纸片，捡起一看，哇！清明上河图碎片！赶紧集齐一套去活动侍女处兑换奖励吧！', table_items);
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
    Broadcast(0x17, "[p:"..player:getCountry()..":"..player:getPName().."] 使用 [4:"..itemId.."] 兑换获得礼券" .. c);
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
  Broadcast(0x17, "[p:"..player:getCountry()..":"..player:getPName().."] 使用 [4:"..itemId.."] 兑换获得[4:" .. itemId .. "]x" .. itemCount);
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
		Broadcast(0x17, "[p:"..player:getCountry()..":"..player:getPName().."] 凑齐了清明上河图,兑换获得了[4:9223]");
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
    Broadcast(0x17, "[p:"..player:getCountry()..":"..player:getPName().."] 使用生肖兔兑换获得[4:9039]");
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
    Broadcast(0x17, "[p:"..player:getCountry()..":"..player:getPName().."] 使用福星画像+禄星画像+寿星画像兑换获得[4:9036]");
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
    Broadcast(0x17, "[p:"..player:getCountry()..":"..player:getPName().."] 使用福星画像+禄星画像+寿星画像+三星高照兑换获得[4:9037]");
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
			Broadcast(0x17, "[p:"..player:getCountry()..":"..player:getPName().."]惊为天人，义无反顾的兑换了"..exchangeCount.."个[4:9210]，获得了大量奇珍异宝，称霸天下指日可待！");
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
            action.m_ActionMsg = "中秋月饼";
            table.insert(ActiveTask, action);
        else
            local action = ActionTable:Instance();
            action.m_ActionType = 0x70;
            action.m_ActionToken = 1;
            action.m_ActionID = 0;
            action.m_ActionStep = 0;
            action.m_ActionMsg = "今日已领取中秋月饼";
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
        sendItemPackageMail(player, '散仙召集令', '您在【散仙召集令】活动中，成功招募传奇散仙一名，恭喜您获得奖励1000礼券，感谢您对我们游戏的支持。', table_items)
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
                SendMsg(player, 0x35, "你获得了杨花珍藏的：智通的僧衣");
                SendMsg(player, 1, "你获得了杨花珍藏的：智通的僧衣");
            end
        end

        if id == 1 and floor == 3 and spot == 5 then -- 智通
            local package = player:GetPackage();
            local apron = player:GetVar(3);
            if apron == 0 then
                package:AddItem(71, 1, true)
                player:SetVar(3, 71)
                SendMsg(player, 0x35, "获得了智通珍藏的：杨花的肚兜");
                SendMsg(player, 1, "获得了智通珍藏的：杨花的肚兜");
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
        --if where == 0 then
        --    Broadcast(0x17, "恭喜[p:"..player:getCountry()..":"..player:getPName().."] 副本通关，获得【圣诞萌萌变身卡】x" .. itemNum[lootlvl])
        --else
        --    Broadcast(0x17, "恭喜[p:"..player:getCountry()..":"..player:getPName().."] 阵图通关，获得【圣诞萌萌变身卡】x" .. itemNum[lootlvl])
        --end
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

function onCopyWin(player, id, floor, spot, lootlvl)
    SingleDayReward(player, lootlvl);
    Christmas(player, lootlvl, 0);
    ValentineDay(player, lootlvl)
    GirlDay(player, lootlvl)
    WhiteLoveDay(player, lootlvl, 0)
end


function onFrontMapFloorWin(player, id, spot, lootlvl)
end

function onFrontMapWin(player, id, spot, lootlvl)
    SingleDayReward(player, lootlvl);
    Christmas(player, lootlvl, 1);
    ValentineDay(player, lootlvl)
    GirlDay(player, lootlvl)
    WhiteLoveDay(player, lootlvl, 1)
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

    sendItemPackageMail(player, "感恩节在线奖励", "恭喜您获得感恩节在线奖励【感恩节大餐】*3\n 11月23日-11月29日活动期间，每日在线达到3小时就可以获得感恩节在线奖励【感恩节大餐】*3\n 11月23日-11月29日活动期间，连续七天，每天在线都达到3小时还可以获得法宝【火鸡】*5\n 【火鸡】可在法宝熔炼时使用，提升法宝熔炼值。", {8,3,1});

    player:SetVar(4, -1)
    local cnt = player:GetVar(5)
    player:SetVar(5, cnt+1);

    if cnt == 6 then
        sendItemPackageMail(player, "感恩节连续在线奖励", "恭喜您获得感恩节连续七天在线奖励【火鸡】*5\n 11月23日-11月29日活动期间，每日在线达到3小时就可以获得感恩节在线奖励【感恩节大餐】*3\n 11月23日-11月29日活动期间，连续七天，每天在线都达到3小时还可以获得法宝【火鸡】*5\n 【火鸡】可在法宝熔炼时使用，提升法宝熔炼值。", {1525,5,1});
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
        sendItemPackageMail(player, "圣诞节活动奖励", "恭喜您获得法宝【雪人】\n 2011年12月23日-2012年1月3日登陆并且等级达到30级玩家，即可获得法宝【雪人】 ", {1637,1,1});
        player:SetVar(7,1)
    end

    if lvl >= 40 and player:GetVar(8) == 0 then
        sendItemPackageMail(player, "圣诞节活动奖励（二）", "恭喜您获得节日套装奖励【圣诞萌萌靴】\n 2011年12月23日-2012年1月3日登陆并且等级达到40级玩家，即可获得节日套装奖励【圣诞萌萌靴】 ", {1752,1,1});
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
        local msg = "恭喜您获得龙年迎新礼包，礼包的封印将会在对应日期解除， 届时可打开获得丰厚道具，若是在解除封印当天打开的话可获得双倍奖励哦。（如果觉得占包包可以先不接受的说）"
        sendItemPackageMail(player, "龙年迎新礼包-小年夜礼包", msg, {417,1,1});
        sendItemPackageMail(player, "龙年迎新礼包-大年夜礼包", msg, {418,1,1});
        sendItemPackageMail(player, "龙年迎新礼包-初一礼包", msg, {419,1,1});
        sendItemPackageMail(player, "龙年迎新礼包-初二礼包", msg, {420,1,1});
        sendItemPackageMail(player, "龙年迎新礼包-初三礼包", msg, {421,1,1});
        sendItemPackageMail(player, "龙年迎新礼包-初四礼包", msg, {422,1,1});
        sendItemPackageMail(player, "龙年迎新礼包-初五礼包", msg, {423,1,1});
        sendItemPackageMail(player, "龙年迎新礼包-初六礼包", msg, {424,1,1});
        sendItemPackageMail(player, "龙年迎新礼包-初七礼包", msg, {425,1,1});
        sendItemPackageMail(player, "龙年迎新礼包-初八礼包", msg, {426,1,1});
        sendItemPackageMail(player, "龙年迎新礼包-立春礼包", msg, {427,1,1});
        sendItemPackageMail(player, "龙年迎新礼包-元宵礼包", msg, {428,1,1});
        sendItemPackageMail(player, "节日套装奖励", "恭喜您获得节日套装【中国结】。\n 1月12日-2月6日 登陆游戏，并且等级达到30的玩家可以获得节日套装【中国结】 ", {1753,1,1});
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

    sendItemPackageMail(player, "在线1小时奖励", "恭喜您在线达一小时，获得以下奖励：自动回血符*1，太乙真金*1，初级打孔石*1，初级挂机加速符*1，初级道法金丹*1，混元离土*2。 \n1月16日-2月6日，每日登陆《蜀山传奇》在线达到一小时都可以获得此奖励。", {9,1,1, 502,1,1, 510,1,1, 55,1,1, 29,1,1, 51,2,1});
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
        sendItemPackageMail(player, "情人节套装奖励", "恭喜您，获得情人节活动奖励【恋人之戒】；\n2012/2/11-2/17登陆游戏，并且等级达到30级以上（包含30级）所有用户，都将获得【恋人之戒】", {1754,1,1});
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
        sendItemPackageMail(player, "【愚人节礼包】奖励", "恭喜您获得【愚人节礼包】，4月1日打开有惊喜哦！\n等级≥40级所有玩家，在3/29-4/1每日登陆游戏就可以获得一个【愚人节礼包】", {479,1,1});
        local date = os.date("%m%d", os.time())
        if date == "0401" then
            sendItemPackageMail(player, "【愚人节卡片】奖励", "恭喜您获得【愚人节卡片】*5，使用卡片会有惊喜哦！\n等级≥40级所有玩家，在4月1日登陆游戏就可以获得【愚人节卡片】*5", {480,5,1});
        end
        player:SetVar(58, 1)
    end
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

local cfriend_awards = {
    {{56,6,1}, {57,6,1}},
    {{56,12,1}, {57,12,1}},
    {{56,24,1}, {57,24,1}},
    {{503,5,1}, {508,3,1}, {506,3,1}, {56,6,1}, {57,6,1}},
    {{503,10,1}, {508,6,1}, {506,6,1}, {56,12,1}, {57,12,1}},
    {{503,20,1}, {508,12,1}, {506,12,1}, {56,24,1}, {57,24,1}},
    {{509,2,1}, {507,2,1}, {515,2,1}, {56,6,1}, {57,6,1}},
    {{509,4,1}, {507,4,1}, {515,4,1}, {56,12,1}, {57,12,1}},
    {{509,8,1}, {507,8,1}, {515,8,1}, {56,24,1}, {57,24,1}},
}
function onGetCFriendAward(player, idx)
    local package = player:GetPackage()
    if package:GetRestPackageSize() < #cfriend_awards[idx] then
        player:sendMsgCode(2, 1011, 0)
        return false
    end

    for k, v in pairs(cfriend_awards[idx]) do
        package:Add(v[1], v[2], v[3])
    end
    return true
end

function onInvitedBy(player)
    sendItemPackageMail(player, "好友邀请奖励", "您的好友邀请您一同游玩蜀山传奇，系统赠送您一份大礼包", {503,5,1, 56,10,1, 57,10,1, 5035,1,1});
end

