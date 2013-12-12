
function RunSSDTAward(player, opt)
    if player == nil then
        return false;
    end

    local package = player:GetPackage();

	if package:GetRestPackageSize() < 1 then		
		player:sendMsgCode(2, 1011, 0);
		return false;
	end

    if opt > 3 then
        return false;
    end

    if opt == 0 then
        local gem = {5002, 5012, 5022, 5032, 5042, 5052, 5062, 5072, 5082, 5092, 5102, 5112, 5122, 5132, 5142}
        local g = math.random(1, #gem)
        package:AddItem(gem[g], 1, true, 0, 39);
    else
        local items = { {56, 2}, {503, 2}, {507, 2} }
        package:AddItem(items[opt][1], items[opt][2], true, 0, 39);
    end

    return true;
end

function RunHappyAward(player, opt)
    if player == nil then
        return 0;
    end

    local package = player:GetPackage();

	if package:GetRestPackageSize() < 5 then		
		player:sendMsgCode(2, 1011, 0);
		return 0;
	end

    if opt > 5 then
        return 0;
    end

    local itemId = 0;
    if opt == 0 then
        local chance = {100, 212, 338, 456, 1461, 3470, 5981, 10000}
        local item = {9022, 509, 507, 515, 503, 514, 56, 15}
        local g = math.random(1, 10000)
        for i = 1, #chance do 
            if g <= chance[i] then
                package:AddItem(item[i], 1, true, true, 40);
                itemId = item[i]

                if i <=4 then
                    Broadcast(0x27, "[p:"..player:getCountry()..":"..player:getPName().."]在欢乐大转盘中幸运地获得了[4:"..itemId.."]x1")
                end
                break
            end
        end
    else
        local items = { 
            { 56, 1, 57, 1, 15, 1, 508, 1, 506, 1 },
            { 500, 3, 511, 1, 501, 1, 517, 2 },
            { 5005, 1, 514, 1, 465, 1 },
            { 516, 2, 503, 2, 512, 2, 466, 2 },
            { 507, 1, 509, 1, 9021, 1 },
        }

        itemId = 1;
        for i = 1, #items[opt], 2 do
            if items[opt][i] == 5005 then
                local gem = {5005, 5015, 5025, 5035, 5045, 5055, 5065, 5075, 5085, 5095, 5105, 5115, 5125, 5135, 5145}
                local g = math.random(1, #gem)
                package:AddItem(gem[g], 1, true, 0, 40);
            else
                package:AddItem(items[opt][i], items[opt][i+1], true, 0, 40);
            end
        end
    end

    return itemId;
end

function RunTargetAward(player)
    if player == nil then
        return 0;
    end

    local package = player:GetPackage();

	if package:GetRestPackageSize() < 1 then
		player:sendMsgCode(2, 1011, 0);
		return 0;
	end

    if not isFBVersion() then
        if player:hasRealItemAward(1) then
            player:getRealItemAward(1)
            Broadcast(0x27, "恭喜[p:"..player:getCountry()..":"..player:getPName().."]在七日目标实物抽奖中获得了60QB大奖，让我们一起来祝贺他。玩蜀山传奇，下一个幸运儿就是你！！！！！！！")
            return 7;
        end
        if player:hasRealItemAward(2) then
            player:getRealItemAward(2)
            Broadcast(0x27, "恭喜[p:"..player:getCountry()..":"..player:getPName().."]在七日目标实物抽奖中获得了100QB大奖，让我们一起来祝贺他。玩蜀山传奇，下一个幸运儿就是你！！！！！！！")
            return 8;
        end
    end

    local chance = {379, 1895, 2400, 5768, 9558, 10000, 10000, 10000}
    local item = {515, 503, 507, 56, 57, 509, 60, 100}
    local j = 0;
    local g = math.random(1, 10000)
    for i = 1, #chance do
        if g <= chance[i] then
            player:lastLootPush(item[i], 1);
            package:AddItem(item[i], 1, true, true, 41);
            j = i;
            break
        end
    end

    return j;
end

function RunTargetAwardRF(player)
    if player == nil then
        return 0;
    end

    local package = player:GetPackage();

	if package:GetRestPackageSize() < 1 then
		player:sendMsgCode(2, 1011, 0);
		return 0;
	end

    local chance = {379,1895,2400,5768,9558,10000}
    local item = {515,503,507,56,57,509}
    local j = 0;
    local g = math.random(1, 10000)
    for i = 1, #chance do
        if g <= chance[i] then
            player:lastLootPush(item[i], 1);
            package:AddItem(item[i], 1, true, true, 41);
            j = i;
            break
        end
    end

    return j;
end

function RunNewRegisterAward(player)
    if player == nil then
        return 0;
    end

    local package = player:GetPackage();

	if package:GetRestPackageSize() < 1 then
		player:sendMsgCode(2, 1011, 0);
		return 0;
	end
    if not isFBVersion() then
        for i = 3, 32 do
            if player:hasRealItemAward(i) then
                player:getRealItemAward(i)
                Broadcast(0x27, "恭喜[p:"..player:getCountry()..":"..player:getPName().."]在新手注册邀请好友抽奖活动中获得10QB大奖，让我们一起来祝贺他。玩蜀山传奇，下一个幸运儿就是你！！！！！！！")
                return 8;
            end
        end
    end
    --local chance = {379,1895,2400,5768,9558,9991,10000}
    local chance = {379,1895,5000,5000,5000,9991,10000}
    local item = {515,503,507,56,57,509,500,--[["10QB"]] }
    local j = 0;
    local g = math.random(1, 10000)
    for i = 1, #chance do
        if g <= chance[i] then
            player:RegisterAward(item[i], 1);
            package:AddItem(item[i], 1, true, true, 31);
            j = i;
            break
        end
    end
    
    return j;
end

function RunNewRC7DayLoginAward(player, cts)
    -- 领取新注册七日登录奖励
    local item = {
        [1] = {{499,20},{56, 2}, {57, 2}},
        [2] = {{499,20},{502, 5}, {503, 2},{15,5}},
        [3] = {{499,30},{500, 5}, {501, 2},{15,5}},
        [4] = {{499,30},{509, 1}, {507, 1},{511,3}},
        [5] = {{499,40},{509, 1}, {507, 1},{5025,1},{512,4}},
        [6] = {{499,40},{509, 1}, {507, 1},{514,5},{515,2}},
        [7] = {{499,50},{509, 1}, {507, 1},{517,5},{134,2},{15,5}},
    };

    local item1 = {
        [1] = {{499,10},{56, 1}, {57, 1}},
        [2] = {{499,20},{502, 3}, {503, 1},{15,2}},
        [3] = {{499,30},{500, 3}, {501, 1},{15,2}},
        [4] = {{499,30},{508, 3}, {506, 3},{511,3}},
        [5] = {{499,30},{508, 3}, {506, 3},{5024,1},{512,2}},
        [6] = {{499,40},{509, 1}, {507, 1},{514,3},{515,1}},
        [7] = {{499,50},{509, 1}, {507, 1},{517,3},{134,1},{15,3}},
    };
    
    local package = player:GetPackage();

    if cts == 0 then
        return false
    end
    if cts > 7 then
        return false
    end
    
    local t = { ['year'] = 2013, ['month'] = 9, ['day'] = 13, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };
    local s = os.time(t)

    if getOpenTime() < s then 
        num = #item[cts]
        if package:GetRestPackageSize() < ((num - 1) +((num - 1)*num*4/99)) then
            player:sendMsgCode(2, 1011, 0);
            return false
        end

        for count = 1, #item[cts] do
            if item[cts][count][1] == 499 then
                player:getCoupon(item[cts][count][2])
            else
                package:Add(item[cts][count][1], item[cts][count][2], true, 0, 34);
            end
        end
    else
        num = #item1[cts]
        if package:GetRestPackageSize() < ((num - 1) +((num - 1)*num*4/99)) then
            player:sendMsgCode(2, 1011, 0);
            return false
        end

        for count = 1, #item1[cts] do
            if item1[cts][count][1] == 499 then
                player:getCoupon(item1[cts][count][2])
            else
                package:Add(item1[cts][count][1], item1[cts][count][2], true, 0, 34);
            end
        end
    end

    return true
end

function RunNewRC7DayLoginAward2(player, val)
    -- 领取新注册七日累计登录奖励
    local items = {
        [1] = {{439, 3}},
        [3] = {{516, 5}},
        [4] = {{1610, 1},{1609, 1},{1608, 1}},
    };
    local package = player:GetPackage();

    if val == 0 then
        return false
    end
    if val > 5 then
        return false
    end
    if val == 2 then
        return player:addFighterFromItem(9011, 0)
    end

        
    local item = items[val]
    num = #item
    if package:GetRestPackageSize() < num then
        player:sendMsgCode(2, 1011, 0);
        return false
    end

    for k,v in pairs(item) do
        if val == 4 then
            package:AddEquip(v[1], v[2])
        else
            package:AddItem(v[1], v[2], 1)
        end
    end
    return true
end

function RunNewRC7DayRechargeAward(player, val, gold)
    -- 领取新注册七日充值奖励（神龙许愿）
    local needGold = {
        [1] = 10,
        [2] = 50,
        [3] = 100,
        [4] = 200,
        [5] = 300,
        [6] = 400,
        [7] = 500,
    }
    local items = {
        [1] = {{499,10},{400,2},{509,1},{507,1},{15,2}},
        [2] = {{499,10},{400,2},{509,1},{507,1},{15,2},{500,5}},
        [3] = {{499,10},{400,3},{509,1},{507,1},{15,2},{512,5},{513,3}},
        [4] = {{499,10},{400,3},{509,1},{507,1},{15,2},{502,5},{503,5}},
        [5] = {{499,10},{400,4},{509,1},{507,1},{15,2},{501,5},{505,2}},
        [6] = {{499,10},{400,4},{509,2},{507,2},{15,2},{514,5},{515,2}},
        [7] = {{499,10},{400,5},{509,2},{507,2},{15,2},{517,3},{134,3}},
    }

    if val == 0 then
        return false
    end

    if val > #items then
        return false
    end

    if gold < needGold[val] then
        return false
    end


    local item = items[val]
    local package = player:GetPackage()
    if package:GetRestPackageSize() < #item then
        player:sendMsgCode(2, 1011, 0)
        return false
    end

    for k,v in pairs(item) do
        if v[1] == 499 then
            player:getCoupon(v[2])
        else
            package:AddItem(v[1], v[2], 1)
        end
    end

    return true
end

function RunNewRC7DayTargetAward(player)
    -- 领取新注册七日每日目标奖励
    if player == nil then
        return 0;
    end

    local package = player:GetPackage();

	if package:GetRestPackageSize() < 1 then
		player:sendMsgCode(2, 1011, 0);
		return 0;
	end

    if not isFBVersion() then
        if player:hasRealItemAward(1) then
            player:getRealItemAward(1)
            Broadcast(0x27, "恭喜[p:"..player:getCountry()..":"..player:getPName().."]在七日目标实物抽奖中获得了60QB大奖，让我们一起来祝贺他。玩蜀山传奇，下一个幸运儿就是你！！！！！！！")
            return 7;
        end
        for i = 41, 70 do
            if player:hasRealItemAward(i) then
                player:getRealItemAward(i)
                Broadcast(0x27, "恭喜[p:"..player:getCountry()..":"..player:getPName().."]在七日目标实物抽奖中获得了10QB大奖，让我们一起来祝贺他。玩蜀山传奇，下一个幸运儿就是你！！！！！！！")
                return 8;
            end
        end
    end

    local chance = {379, 1895, 2400, 5768, 9558, 10000, 10000, 10000}
    local item =   {515, 503,  507,  516,  56,   509,   60,    100}
    local j = 0;
    local g = math.random(1, 10000)
    for i = 1, #chance do
        if g <= chance[i] then
            player:lastNew7DayTargetAwardPush(item[i], 1);
            package:AddItem(item[i], 1, true, true, 41);
            j = i;
            break
        end
    end

    return j;
end

function RunQQBoardInstantLoginAward(player, cts)
    -- 领取QQ面板连续登陆登录奖励
    local item = {
        --[[[1] = {{56,1},{57, 1}, {15, 1}},
        [3] = {{56,2},{57, 2}, {15, 2}},
        [5] = {{30,1},{56,3},{57, 3}, {15, 3}},
        [7] = {{30,1},{56,3},{57, 3}, {15, 2},{134,1}},
        [9] = {{30,1},{56,3},{57, 3}, {15, 3},{134,1},{1325,1}},
        [11] = {{30,1},{56,3},{57, 3}, {15, 3},{134,1},{9390,1}},
        [13] = {{30,1},{56,3},{57, 3}, {15, 3},{134,1},{9390,1},{1126,1}},
        [15] = {{30,1},{56,3},{57, 2}, {15, 3},{134,1},{9390,1},{1126,1},{8555,1}},]]
        [1] = {{56,1},{57, 1}, {15, 2}},
        [3] = {{56,2},{57, 2}, {9371, 2}},
        [5] = {{503,2},{514,2},{500, 2}, {9371, 2}},
        [7] = {{509,1},{515,1},{134, 1}, {1325, 1}},
    };
    local package = player:GetPackage();

    --[[if cts == 0 then
        return false
    end
    if cts > 16 then
        return false
    end]]
    if item[cts] == nil then
        return false
    end

    num = #item[cts]
    if package:GetRestPackageSize() < num  then
        player:sendMsgCode(2, 1011, 0);
        return false
    end

    for count = 1, #item[cts] do
            package:Add(item[cts][count][1], item[cts][count][2], true, 0, 59);
    end
    return true
end
function RunQQBoardOnlineAward(player, cts)
    local item = {
        [0] = {{1325 ,1},{134, 1},{551, 1},{500,1}},
        [1] = {{500,2},{56, 2}, {9371, 3},{30,1}},
        [2] = {{503,2},{8000,2},{508, 2}, {57, 2}},
        [3] = {{499, 20},{501,2},{512,2},{506, 2}},
    };
    local package = player:GetPackage();

    if cts > 3 or cts < 0 then
        return false
    end

    num = #item[cts]
    if package:GetRestPackageSize() < num  then
        player:sendMsgCode(2, 1011, 0);
        return false
    end

    for count = 1, #item[cts] do
        if item[cts][count][1] == 499 then
            player:getCoupon(item[cts][count][2])
        else
            package:Add(item[cts][count][1], item[cts][count][2], true, 0, 59);
        end
    end
    return true
end
function RunLuckyMeetInstantLoginAward(player, cts)
    local item = {
        [1] = {{1326,1},{133, 1}, {499, 10}},
        [2] = {{1326,2},{133, 2}, {499, 20}},
        [3] = {{1326,3},{133, 3}, {499, 30}},
        [4] = {{1326,4},{133, 4}, {499, 40}},
        [5] = {{1326,5},{133, 5}, {499, 50}},
        [6] = {{1326,6},{133, 6}, {499, 60}},
        [7] = {{1326,7},{133, 7}, {499, 70}},
    };
    local package = player:GetPackage();

    if cts == 0 then
        return false
    end
    if cts > 7 then
        return false
    end

    num = #item[cts]
    if package:GetRestPackageSize() < (num-1)  then
        player:sendMsgCode(2, 1011, 0);
        return false
    end
    for count = 1, #item[cts] do
        if item[cts][count][1] == 499 then
            player:getCoupon(item[cts][count][2])
        else
            package:Add(item[cts][count][1], item[cts][count][2], true, 0, 59);
        end
    end
    return true
end
function RunPrayAward(player, cts)
    local item = {
        [4] = {{503, 1}},
        [5] = {{134, 1}},
        [6] = {{1325,1}},
    };
    local package = player:GetPackage();

    if cts < 4 then
        return false
    end
    if cts > 6 then
        return false
    end

    num = #item[cts]
    if package:GetRestPackageSize() < (num)  then
        player:sendMsgCode(2, 1011, 0);
        return false
    end
    for count = 1, #item[cts] do
        if item[cts][count][1] == 499 then
            player:getCoupon(item[cts][count][2])
        else
            package:Add(item[cts][count][1], item[cts][count][2], true, 0, 59);
        end
    end
    return true
end
function RunNovLoginAward(player, cts)
    local item = {
        [1] = {{503, 1},{56,1},{133,1}},
        [2] = {{10217,1}},
        [3] = {{10218,1}},
    };
    local package = player:GetPackage();

    if cts < 1 then
        return false
    end
    if cts > 3 then
        return false
    end

    num = #item[cts]
    if package:GetRestPackageSize() < (num)  then
        player:sendMsgCode(2, 1011, 0);
        return false
    end
    for count = 1, #item[cts] do
        if item[cts][count][1] == 499 then
            player:getCoupon(item[cts][count][2])
        else
            package:Add(item[cts][count][1], item[cts][count][2], true, 0, 59);
        end
    end
    return true
end
function RunPresentAward(player, cts)
    local item = {
        [1] = {{502, 2}},
        [2] = {{15, 1}},
        [3] = {{508,1}},
        [4] = {{506, 1}},
        [5] = {{51, 1}},
        [6] = {{500,1}},
        [7] = {{514, 1}},
        [8] = {{133, 1}},
        [9] = {{516,1}},
        [10] ={{9428,1}},
    };
    
    local package = player:GetPackage();

    if cts < 1 then
        return false
    end
    if cts > 10 then
        return false
    end

    num = #item[cts]
    if package:GetRestPackageSize() < (num)  then
        player:sendMsgCode(2, 1011, 0);
        return false
    end
    for count = 1, #item[cts] do
        if item[cts][count][1] == 499 then
            player:getCoupon(item[cts][count][2])
        else
            package:Add(item[cts][count][1], item[cts][count][2], true, 0, 59);
        end
    end
    return true
end
function RunLuckyMeetRechargeAward(player, cts)
    -- 领取蜀山奇遇充值奖励
    local item = {
        [1] = {{549,1}, {551, 2}},
        [2] = {{8000,6},{513, 2}},
        [3] = {{5065,1}},
        [4] = {{1705,1}},
        [5] = {{549,1}, {551, 2}},
        [6] = {{8000,8},{513, 2}},
        [7] = {{5065,1}},
        [8] = {{1354,1}},
        [9] = {{503,5}, {500, 5},{1325,1}},
        [10] = {{1126,15},{507, 3}},
        [11] = {{9076,6},{509,6}},
        [12] = {{515,10},{134,10},{9371,50}},
    };
    local package = player:GetPackage();
    local level = player:GetVar(550);
    local lv = 0;
    if level > 0 and level<5 then
        lv =1
    end
    if level >4 then 
        lv =2 
    end
    local cts2 = lv*4+cts;
    if cts2 == 0 then
        return false
    end
    if cts2 > 12  then
        return false
    end

    num = #item[cts2]
    if package:GetRestPackageSize() < num  then
        player:sendMsgCode(2, 1011, 0);
        return false
    end

    for count = 1, #item[cts2] do
        package:Add(item[cts2][count][1], item[cts2][count][2], true, 0, 59);
    end
    return true
end
function RunLuckyMeetStrengthAward(player, cts)
    -- 领取蜀山奇遇变强奖励
    local item = {
        [1] = {{509,1}, {503, 2},{5123,2},{5103,2}},
        [2] = {{507,1}, {514, 3},{5063,3},{5073,3}},
        [3] = {{515,1}, {5133, 3},{5113,3},{513,3}},
        [4] = {{516,3}, {5023, 3},{5003,3},{9076,2}},
    }
    local package = player:GetPackage();
    if cts == 0 then
        return false
    end
    if cts > 4  then
        return false
    end

    num = #item[cts]
    if package:GetRestPackageSize() < num  then
        player:sendMsgCode(2, 1011, 0);
        return false
    end

    for count = 1, #item[cts] do
        package:Add(item[cts][count][1], item[cts][count][2], true, 0, 59);
    end
    return true
end
function RunSummerMeetRechargeAward(player, cts)
    -- 领取蜀山奇遇充值奖励
    local item = {
        [1] = {{549,1}, {551, 2}},
        [2] = {{8000,8},{513, 2}},
        [3] = {{5065,1}},
        [4] = {{1126,15},{8520, 2}},
        [5] = {{1705,1},{134,8}},
        [6] = {{1354,1}},
        [7] = {{1540,1},{515,10}},
    };
    local package = player:GetPackage();
    if cts == 0 then
        return false
    end
    if cts > 7  then
        return false
    end

    num = #item[cts]
    if package:GetRestPackageSize() < num  then
        player:sendMsgCode(2, 1011, 0);
        return false
    end

    for count = 1, #item[cts] do
        package:Add(item[cts][count][1], item[cts][count][2], true, 0, 59);
    end
    return true
end
function RunQZoneRechargeAward(player, cts)
    local item = {
        [1] = {{517,2}},
        [2] = {{551, 2}},
        [3] = {{514,3},{503,3}},
        [4] = {{9371,5},{9390, 5}},
        [5] = {{509,3},{507,3}},
        [6] = {{9076,5}},
    };
    local package = player:GetPackage();
    if cts == 0 then
        return false
    end
    if cts > 6  then
        return false
    end
    if item[cts] == nil then
        return false 
    end
    num = #item[cts]
    if package:GetRestPackageSize() < num  then
        player:sendMsgCode(2, 1011, 0);
        return false
    end

    for count = 1, #item[cts] do
        package:Add(item[cts][count][1], item[cts][count][2], true, 0, 59);
    end
    return true
end
function RunBirthdayAward(player)
    if player == nil then
        return 0;
    end
    local package = player:GetPackage();
    if package:GetRestPackageSize() < 1 then
        player:sendMsgCode(2, 1011, 0);
        return 0;
    end
    local chance = {5000, 5300, 5500, 5500, 5500, 6000, 6000, 10000}
    local item = {503, 509, 515, "QB", "Ipad", 507, "Iphone", 500 }
    local j = 0; 
    local g = math.random(1, 10000)
    for i = 1, #chance do
        if g <= chance[i] then
            player:BirthdayAward(item[i], 1);
            package:AddItem(item[i], 1, true, true, 31);
            j = i;
            break
        end
    end
    return j;
end

function RunConsumeAward(player, opt)
    if player == nil then
        return 0;
    end
    local package = player:GetPackage();
	if package:GetRestPackageSize() < 1 then
		player:sendMsgCode(2, 1011, 0);
		return 0;
	end
    if opt > 2 then
        return 0;
    end
    local items = {
        --[1] = {{515,3},{507,2},{509,2},{503,10},{1325,4},{47,3},{134,4},{5026,1}},
        [1] = {{515,1},{507,1},{509,1},{503,1},{1325,1},{47,1},{134,1},{1717,1}},
        [2] = {{515,1},{507,1},{509,1},{503,1},{1325,1},{47,1},{134,1},{5026,1}}
    };
    local ch = {
        --[1] = {1852, 2593, 3333, 4444, 5926, 6667, 8148, 10000},
        [1] = {700,1400,2100,4500,6300,7800,9600,10000},
        [2] = {750,1500,2250,4750,6750,7500,9700,10000}
    };
    local j = 0; 
    local g = math.random(1, 10000)
    for i = 1, #ch[opt] do
        if g <= ch[opt][i] then
            package:Add(items[opt][i][1], items[opt][i][2], true, true, 31);
            j = i;
            break
        end
    end
    return j;
end
 
function RunBlueDiamondAward(player, opt)
    if player == nil then
        return 0;
    end
    local package = player:GetPackage();
	if package:GetRestPackageSize() < 5 then
		player:sendMsgCode(2, 1011, 0);
		return 0;
	end
    local date_9190_0 = { ['year'] = 2012, ['month'] = 12, ['day'] = 24, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 }
    local date_9190_1 = { ['year'] = 2013, ['month'] = 1, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 }
    local date_9191_0 = { ['year'] = 2013, ['month'] = 3, ['day'] = 14, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 }
    local date_9191_1 = { ['year'] = 2013, ['month'] = 3, ['day'] = 21, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 }
    local date_9217_0 = { ['year'] = 2012, ['month'] = 11, ['day'] = 20, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 }
    local date_9217_1 = { ['year'] = 2012, ['month'] = 11, ['day'] = 27, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 }
    local date_0 = os.time(date_9190_0);
    local date_1 = os.time(date_9190_1);

    --1:蓝钻 2:黄钻 3:QQ会员 4:红钻 5:好莱坞 6:财付通 8: 跨年欢乐大转盘
    local chance = {
        [1] = {1180, 2630, 4080, 5930, 7780, 8520, 9500, 10000},
        [2] = {1180, 2630, 4080, 5930, 7780, 8520, 9500, 10000},
        [3] = {785,2833,4881,5823,7202,7987,9366,10000},
        [4] = {1050, 2265, 3480, 4695, 6220, 7745, 9270, 10000},
        [5] = {785,2833,4881,5823,7202,7987,9366,10000},
        [6] = {1180,2630,4080,6030,7980,9100,9400,10000},
        [7] = {2200,4150,5350,5850,5950,6450,6950,7050,7650,7700,9850,10000},
        [8] = {100,212,338,456,1461,3470,5981,10000}
    };
    local item = {
        [1] = {{515,3},{134,4},{1325,4},{507,2},{509,2},{9022,1},{1717,1},{5137,1}},
        [2] = {{515,3},{134,4},{1325,4},{507,2},{509,2},{9022,1},{1717,1},{5137,1}},
        [3] = {{515,3},{507,2},{509,2},{503,10},{1325,4},{47,3},{134,4},{5026,1}},
        [4] = {{515,3},{9338,4},{134,4},{1325,4},{507,2},{509,2},{47,3},{5006,1}},
        [5] = {{515,6},{507,4},{509,4},{503,20},{1325,8},{47,6},{134,8},{5026,2}},
        [6] = {{515,3},{134,4},{1325,4},{507,2},{509,2},{503,5},{1719,1},{5135,1}},
        [7] = {{503,1},{500,2},{517,1},{515,1},{9076,2},{1325,1},{516,1},{8555,1},{134,1},{5065,1},{56,2},{5005,1}},
        [8] = {{9022,1},{509,1},{134,1},{515,1},{503,2},{514,1},{500,1},{15,1}}
    };
    local item_id = {9190, 9191, 9217, 9284,10119};
    
    local items = item[opt];
    local itemId = item_id[opt];
    local ch = chance[opt];
    
    if items == nil or ch ==nil then
        return 
    end
    if opt < 6 then
        if  not package:DelItem(itemId, 1, true) then
            if  not package:DelItem(itemId, 1, false) then
                player:sendMsgCode(2, 1110, 0);
                return 0;
            end
        end
    end
    if opt ==8 and not getHappyFireTime() then 
        return 
    else 
        local value = player:GetVar(648);
        if value <20 then 
            return 
        end 
        player:SetVar(648,value - 20)
    end
    
    local j = 0; 
    local g = math.random(1, 10000)
    for i = 1, #ch do
        if g <= ch[i] then
            package:Add(items[i][1], items[i][2], true, true, 31);
            j = i;
            break
        end
    end
    if opt ==7 and (items[j][1] == 134 or items[j][1]==515 or items[j][1] == 1325 or items[j][1]==9076 or items[j][1] == 8555 or items[j][1] == 5065 or items[j][1] ==5005)  then 
        Broadcast(0x27, "[p:"..player:getCountry()..":"..player:getPName().."]".."通过财富罗盘获得了".."[4:"..items[j][1].."]x"..items[j][2])
   end
   if opt ==8 and j < 5 then 
        Broadcast(0x27, "[p:"..player:getCountry()..":"..player:getPName().."]".."在跨年欢乐大转盘中幸运的获得了".."[4:"..items[j][1].."]x"..items[j][2])
    end
    local extraAward_9191 = {
        [2] = {9191,1,1},
        [6] = {549,2,1, 548,20,1},
        [12] = {509,10,1, 8555,4,1, 9076,5,1},
        [20] = {9022,5,1, 515,5,1, 1717,1,1},
    };
    local extraAward_9191_blue = {
        [2] = {9190,1,1},
        [6] = {549,2,1, 548,20,1},
        [12] = {509,10,1, 8555,4,1, 1707,1,1},
        [20]= {9022,5,1, 515,5,1, 1717,1,1},
    };

    local VAR_BLUE_AWARD_COUNT = 196;
    local VAR_YELLOW_AWARD_COUNT = 197;
    local VAR_QQVIP_AWARD_COUNT = 245;
    local count = 0;
    if opt == 1 then
        player:AddVar(VAR_BLUE_AWARD_COUNT, 1);
        count = player:GetVar(VAR_BLUE_AWARD_COUNT);
        --[[if count == 9 then
            package:Add(1707, 1, true, 0, 31);
            package:Add(515, 10, true, 0, 31); 
            package:Add(509, 5, true, 0, 31); 
        end]]--
        local extarAward = extraAward_9191_blue[count];
        if extarAward then
            local title = string.format(msg_132)
            local ctx = string.format(msg_133, count)
            player:GetMailBox():newItemPackageMail(title, ctx, extarAward);
        end
    elseif opt == 2 then
        player:AddVar(VAR_YELLOW_AWARD_COUNT, 1);
        count = player:GetVar(VAR_YELLOW_AWARD_COUNT);
        local extarAward = extraAward_9191[count];
        if extarAward then
            local title = string.format(msg_130)
            local ctx = string.format(msg_131, count)
            player:GetMailBox():newItemPackageMail(title, ctx, extarAward);
        end
    elseif opt == 3 then
        player:AddVar(VAR_QQVIP_AWARD_COUNT, 1);
        count = player:GetVar(VAR_QQVIP_AWARD_COUNT);
        if count == 12 then
            package:Add(9076, 5, true, 0, 31); 
            package:Add(515, 5, true, 0, 31); 
            package:Add(507, 5, true, 0, 31); 
            package:Add(509, 5, true, 0, 31); 
        end
    end

    return j;
end


local PlatformAward =  
{ --平台id
    [1]  = {    -- "QQ空间"
        --1:推广用注册玩家登录奖励领取
        --2:回流用户新区道具奖
        [1] = {}, --id,num
        [2] = {},
        [3] = {{51, 5},{48, 5},{49,5},{438,5},{439,5}},
    },
    [2]  = {    -- "朋友网"
        [1] = {}, --id,num
        [2] = {},
        [3] = {{515, 5},{503, 5},{507,5},{509,5},{134,5},{440,5},{1325,5}},
    },
    [3]  = {    -- "qq微博"
        [1] = {}, --id,num
        [2] = {},
        [3] = {{515, 5},{503, 5},{507,5},{509,5},{134,5},{440,5},{1325,5}},
    },
    [4]  = {    -- "Q+"
        [1] = {}, --id,num
        [2] = {},
        [3] = {{515, 5},{503, 5},{507,5},{509,5},{134,5},{440,5},{1325,5}},
    },
    [5]  = {    -- "财付通"
        [1] = {}, --id,num
        [2] = {},
        [3] = {{515, 5},{503, 5},{507,5},{509,5},{134,5},{440,5},{1325,5}},
    },
    [10] = {    -- "QQ游戏大厅"
        [1] = {}, --id,num
        [2] = {},
        [3] = {{515, 5},{503, 5},{507,5},{509,5},{134,5},{440,5},{1325,5}},
    },
    [11] = {    -- "3366"
        [1] = {}, --id,num
        [2] = {},
        [3] = {{515, 5},{503, 5},{507,5},{509,5},{134,5},{440,5},{1325,5}},
    },
    [12] = {    -- "官网"
        [1] = {}, --id,num
        [2] = {},
        [3] = {{515, 5},{503, 5},{507,5},{509,5},{134,5},{440,5},{1325,5}},
    },
    [16] = {    -- gamelife
        [1] = {}, --id,num
        [2] = {},
        [3] = {{515, 5},{503, 5},{507,5},{509,5},{134,5},{440,5},{1325,5}},
    },
    [17] = {    -- qqunion
        [1] = {}, --id,num
        [2] = {},
        [3] = {{515, 5},{503, 5},{507,5},{509,5},{134,5},{440,5},{1325,5}},
    },
}

function RunNewRegisterAwardAD_RF(player, idx)
    if nil == player or nil == idx then
        return 0
    end
    if 1 ~= idx and 2 ~= idx and 3 ~= idx then
        return 0
    end
    local pf = player:getPlatform()
    local aw = PlatformAward[pf]
    --print("平台id:"..pf)
    if nil == aw or nil == next(aw) then
        return 0
    end
    local award = aw[idx]
    local package = player:GetPackage()
    if package:GetRestPackageSize() < #award then
        player:sendMsgCode(2, 1011, 0)
        return 0
    end
    for _, val in pairs(award) do 
        package:Add(val[1], val[2], true, false, 31)
    end
    return 1
end
function RunSummerFlowAward(player, idx)
    if nil == player or nil == idx then
        return 0
    end
    if 1 ~= idx and 2 ~= idx and 3 ~= idx and 4 ~= idx then
        return 0
    end
    local item = {
        [1] = {{499,88},{9418,8},{49,3},{9141,3},{9144,3}},
        [2] = {{499,188},{9418,8},{50,3},{9141,5},{9144,5}},
        [3] = {{499,288},{15,10},{9418,10},{9141,8},{9144,8},{49,3},{50,3}},
        [4] = {{499,288},{15,20},{9418,20},{9141,8},{9144,8},{49,3},{50,3}},
    };
    local award = item[idx]
    local package = player:GetPackage()
    if package:GetRestPackageSize() < #award then
        player:sendMsgCode(2, 1011, 0)
        return 0
    end
    for _, val in pairs(award) do 
        if val[1] == 499 then
            player:getCoupon(val[2])
        else
            package:Add(val[1], val[2], true, false, 31)
        end
    end
    return 1
end
function RunSummerFlow3OnlineAward(player, idx)
    if nil == player or nil == idx then
        return 0
    end
    if 1 ~= idx and 2 ~= idx and 3 ~= idx and 4 ~= idx then
        return 0
    end
    local item = {
        [1] = {{499,20}},
        [2] = {{503,3 }},
        [3] = {{8000,5}},
        [4] = {{1126,5}},
    };
    local award = item[idx]
    local package = player:GetPackage()
    if package:GetRestPackageSize() < #award then
        player:sendMsgCode(2, 1011, 0)
        return 0
    end
    for _, val in pairs(award) do 
        if val[1] == 499 then
            player:getCoupon(val[2])
        else
            package:Add(val[1], val[2], true, false, 31)
        end
    end
    return 1
end
function RunSummerMeetAward(player, idx)
    if nil == player or nil == idx then
        return 0
    end
    if 1 ~= idx and 2 ~= idx and 3 ~= idx then
        return 0
    end
    local item = {
        [1] = {{499,100},{9418, 8},{9413,8},{9141,5},{9144,5}},
        [2] = {{499,188},{9418,18},{9413,18},{9141,8},{9144,8}},
        [3] = {{499,288},{9418,33},{9413,33},{9141,8},{9144,8}},
    };
    local award = item[idx]
    local package = player:GetPackage()
    if package:GetRestPackageSize() < #award then
        player:sendMsgCode(2, 1011, 0)
        return 0
    end
    for _, val in pairs(award) do 
        if val[1] == 499 then
            player:getCoupon(val[2])
        else
            package:Add(val[1], val[2], true, false, 31)
        end
    end
    return 1
end

function Run11ActAward(player, opt)
    if player == nil or opt < 1 or opt > 2 then
        return false;
    end

    local package = player:GetPackage();

	if package:GetRestPackageSize() < 6 then		
		player:sendMsgCode(2, 1011, 0);
		return false;
	end

    local awards = {
        {{517, 1}, {29, 10}, {510, 1}, {15, 1}, {514, 1}, {9214, 1}},
        {{503, 1}, {501, 1}, {512, 1}, {517, 1}, {30, 1}, {33, 1}}
    }

    local award = awards[opt]
    for i = 1, #award do
        package:Add(award[i][1], award[i][2], true, false, 30)
    end

    return true;
end

function RunThanksGivingDayAward(player, opt)
    if player == nil or opt < 1 or opt > 2 then
        return 0
    end
    --[[ 感恩节
    local awards = {
        [1] = {{517, 1},{29, 10},{511, 1},{15, 1},{514, 1},{548, 1}},
        [2] = {{503, 1},{500, 1},{513, 1},{516, 1},{30, 1},{1525,1}},
    }
    --]]
    local awards = {
        [1] = {{517, 1},{29, 10},{511, 1},{15, 1},{16, 1},{548, 1}},
        [2] = {{514, 1},{465, 1},{501, 1},{516, 1},{551, 1},{134,1}},
    }
    local package = player:GetPackage()
	if package:GetRestPackageSize() < #awards[opt] then
		player:sendMsgCode(2, 1011, 0);
		return 0
	end
    for i = 1, #awards[opt] do
        package:Add(awards[opt][i][1], awards[opt][i][2], true, false, 30)
    end
    return 1
end

function onGetFeastGiftAward(player, type)
    if player == nil then
        return false
    end
    if type == 0 or type > 2 then
        return false
    end
    local awards = {
        [1] = {{9317, 1},{511, 1},{548, 1},{500, 1},{48, 1},{517, 1}},
        [2] = {{9317, 2},{50, 1},{503, 1},{516, 1},{513, 1},{1325,1}},
    }
    local package = player:GetPackage()
    if package:GetRestPackageSize() < #awards[type] then
        player:sendMsgCode(2, 1011, 0);
        return false
    end
    for i = 1, #awards[type] do
        package:Add(awards[type][i][1], awards[type][i][2], true, false, 0)
    end
    return true
end

local tmp1 = { ['year'] = 2013, ['month'] = 2, ['day'] = 4, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 }
local time1 = os.time(tmp1)
local tmp2 = { ['year'] = 2013, ['month'] = 2, ['day'] = 11, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 }
local time2 = os.time(tmp2)
local tmp3 = { ['year'] = 2013, ['month'] = 2, ['day'] = 18, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 }
local time3 = os.time(tmp3)
local QQGameawards = {
    [1] = {{51, 1},{1327, 1},{56, 1},{57, 1}},
    [2] = {{49, 1},{1326, 1},{56, 1},{57, 1}},
    [3] = {{15, 1},{1327, 1},{56, 1},{57, 1}},
    [4] = {{15, 2},{1326, 1},{56, 1},{57, 1}},
    [5] = {{48, 1},{1327, 1},{56, 1},{57, 1}},
    [6] = {{50, 1},{1326, 1},{56, 1},{57, 1}},
}

function onGetNewYearQQGameAward(player, type)
    if player == nil then
        return false
    end
    if type == 0 or type > 2 then
        return false
    end

    local curTime = os.time();
    local actOrder
    if curTime >= time1 and curTime < time2 then
        actOrder = 1
    elseif curTime >= time2 and curTime < time3 then
        actOrder = 2
    else
        actOrder = 3
    end

    local awardOrder = (actOrder - 1) * 2  + type;
    local awards = QQGameawards[awardOrder]
    if awards == nil then
        return false
    end
    local package = player:GetPackage()
    if package:GetRestPackageSize() < #awards then
        player:sendMsgCode(2, 1011, 0);
        return false
    end
    for i = 1, #awards do
        package:Add(awards[i][1], awards[i][2], true, false, 2)
    end
    return true
end

function onGetQZoneQQGameAward(player, type)
    if player == nil then
        return false
    end
    if type == 0 or type > 2 then
        return false
    end
    local awards = {
        --[1] = {{48, 1},{1327, 1},{56, 1},{57, 1}},
        --[2] = {{50, 1},{1326, 1},{56, 1},{57, 1}}
        --[1] = {{51, 1},{1125, 1},{56, 1},{57, 1}, {133, 1}, {1327, 1}},
        --[2] = {{49, 1},{1126, 1},{56, 1},{57, 1}, {134, 1}, {1326, 1}}
        [1] = {{51, 1},{1125, 1},{56, 1},{57, 1}, {439, 1}, {438, 1}},
        [2] = {{48, 1},{1126, 1},{56, 1},{57, 1}, {439, 1}, {438, 1}}
    }
    local award = awards[type]
    local package = player:GetPackage()
    if package:GetRestPackageSize() < #award then
        player:sendMsgCode(2, 1011, 0);
        return false
    end
    for i = 1, #award do
        package:Add(award[i][1], award[i][2], true, false, 2)
    end
    return true
end

local QzonecontinueAwards = {
    [1] = {{56, 1},{57, 1}},
    [2] = {{56, 2},{57, 2}},
    [3] = {{56, 2},{57, 2},{134, 1}},
    [4] = {{56, 2},{57, 2},{134, 2}},
    [5] = {{56, 2},{57, 2},{134, 2},{30, 1}},
    [6] = {{56, 2},{57, 2},{134, 2},{30, 2}},
    [7] = {{56, 2},{57, 2},{134, 2},{30, 2},{9076,1}},
}

function onGetNewYearQzoneContinueAward(player, type)
    if player == nil then
        return false
    end

    local awards = QzonecontinueAwards[type]
    if awards == nil then
        return false
    end

    local package = player:GetPackage()
    if package:GetRestPackageSize() < #awards then
        player:sendMsgCode(2, 1011, 0);
        return false
    end
    for i = 1, #awards do
        package:Add(awards[i][1], awards[i][2], true, false, 2)
    end
    return true
end

function RunVipPrivilegeAward(player, idx, dayth)
    if player == nil then
        return false;
    end
    --print("AwardDayth:"..dayth)

    if idx == 0 or idx > 4 or dayth == 0 or dayth > 9 then
        return false;
    end
    local package = player:GetPackage();
    local VipType = player:GetVar(540);
    if player:in7DayFromCreated()== true and VipType >4 then
            VipType = VipType-2;
    end
    local dayliawards ={
       [0]= {{499,20},{49, 1 }, {50, 1}},
       [1]= {{499,20},{502,30}, {508,1}},
       [2]= {{499,20},{503,5 }, {509,1}},
       [3]= {{499,20},{502,30}, {508,1}},
       [4]= {{499,20},{503,5 }, {509,1}},
       [5]= {{499,20},{514,5 }, {508,1}},
       [6]= {{499,20},{9371,5 },{509,1}},
    }-- 每日登陆奖励
    local limitbuy = {
        [1] = { -- 第一天
            {514, 2, 10}, -- 限购1
            {503, 2, 28}, -- 限购2
            {5065, 1, 160}, -- 限购3
        },
        [2] = { -- 第二天
            {514, 4, 20}, -- 限购1
            {503, 3, 42}, -- 限购2
            {5105, 1, 160}, -- 限购3
        },
        [3] = { -- 第三天
            {514, 6, 30}, -- 限购1
            {503, 4, 56}, -- 限购2
            {5125, 1, 160}, -- 限购3
        },
        [4] = { -- 第四天
            {514, 8, 40}, -- 限购1
            {503, 5, 70}, -- 限购2
            {5115, 1, 160}, -- 限购3
        },
        [5] = { -- 第五天
            {514, 10, 50}, -- 限购1
            {503, 6, 84}, -- 限购2
            {5095, 1, 160}, -- 限购3
        },
        [6] = { -- 第六天
            {514, 12, 60}, -- 限购1
            {503, 7, 98}, -- 限购2
            {5085, 1, 160}, -- 限购3
        },
        [7] = { -- 第七天
            {514, 14, 70}, -- 限购1
            {503, 8, 112}, -- 限购2
            {5145, 1, 160}, -- 限购3
        },
		[8] = { -- 第八天
            {514, 16, 80}, -- 限购1
            {503, 9, 126}, -- 限购2
            {5025, 2, 160}, -- 限购3
        },
		[9] = { -- 第八天
            {514, 18, 80}, -- 限购1
            {503, 10, 126}, -- 限购2
            {5055, 2, 160}, -- 限购3
        },
    };

    local num = 1;
    if idx == 1 then
        num = num + 1;
    end
    if package:GetRestPackageSize() < num then
        player:sendMsgCode(2, 1011, 0);
        return false;
    end

    if idx == 1 then
        for count = 1,3--[[ #dayliawards]] do
            if dayliawards[VipType][count][1] == 499 then
                player:getCoupon(dayliawards[VipType][count][2])
            else
                package:Add(dayliawards[VipType][count][1], dayliawards[VipType][count][2], true, 0, 41);
            end
        end
    else
        idx = idx - 1;
        if player:getGoldInLua() < limitbuy[dayth][idx][3] then
            return false;
        end
        player:useGoldInLua(limitbuy[dayth][idx][3], 98);
        package:Add(limitbuy[dayth][idx][1], limitbuy[dayth][idx][2], true, 0, 41);
    end

    return true;
end

local LevelAwards = {
    [28] = {{502,3},{15, 2},{57, 2},{56, 2}},
    [29] = {{502,3},{15, 2},{57, 2},{56, 2}},
    [30] = {{502,3},{15, 2},{57, 2},{56, 2}},
    [31] = {{502,3},{15, 2},{57, 2},{56, 2}},
    [32] = {{502,3},{15, 2},{57, 2},{56, 2}},
    [33] = {{514, 2},{15, 2},{57, 2},{56, 2}},
    [34] = {{514, 2},{15, 2},{57, 2},{56, 2}},
    [35] = {{502, 3},{15, 2},{518, 5},{526, 5}},
    [36] = {{502, 3},{15, 2},{518, 5},{526, 5}},
    [37] = {{502, 3},{15, 2},{518, 5},{526, 5}},
    [38] = {{514, 2},{15, 2},{518, 5},{526, 5}},
    [39] = {{514, 2},{15, 2},{518, 5},{526, 5}},
    [40] = {{548, 5},{15, 2},{57, 2},{56, 2}},
    [41] = {{548, 5},{15, 2},{57, 2},{56, 2}},
    [42] = {{548, 5},{15, 2},{57, 2},{56, 2}},
    [43] = {{548, 5},{15, 2},{57, 2},{56, 2}},
    [44] = {{548, 5},{15, 2},{57, 2},{56, 2}},
    [45] = {{502, 20},{514, 2},{52, 1},{9371, 3}},
    [46] = {{502, 20},{514, 2},{52, 1},{9371, 3}},
    [47] = {{502, 20},{514, 2},{52, 1},{9371, 3}},
    [48] = {{502, 20},{514, 2},{52, 1},{9371, 3}},
    [49] = {{502, 20},{514, 2},{52, 1},{9371, 3}},
    [50] = {{9371, 5},{52, 2},{502, 33},{514, 5}},
    [51] = {{9371, 5},{52, 2},{502, 33},{514, 5}},
    [52] = {{9371, 5},{52, 2},{502, 33},{514, 5}},
    [53] = {{9371, 5},{52, 2},{503, 5},{514, 5}},
    [54] = {{9371, 5},{52, 2},{503, 5},{514, 5}},
    [55] = {{9371, 5},{52, 3},{503, 5},{514, 5}},
    [56] = {{9371, 5},{52, 3},{500, 5},{515, 3}},
    [57] = {{9371, 5},{52, 3},{500, 5},{515, 3}},
    [58] = {{9371, 5},{52, 3},{500, 5},{515, 3}},
    [59] = {{9371, 5},{52, 3},{500, 5},{515, 3}},

    [60] = {{33, 3},{500, 5},{505, 5},{511, 5}},
    [61] = {{33, 3},{500, 5},{505, 5},{511, 5}},
    [62] = {{33, 5},{500, 5},{505, 5},{511, 5}},
    [63] = {{33, 5},{500, 5},{505, 5},{511, 5}},
    [64] = {{33, 8},{516, 5},{52, 3},{15, 5}},
    [65] = {{33, 8},{516, 5},{52, 3},{15, 5}},
    [66] = {{33, 10},{516, 5},{52, 3},{15, 5}},
    [67] = {{33, 10},{516, 5},{52, 3},{15, 5}},
    [68] = {{33, 12},{509, 3},{507, 3},{56, 3}},
    [69] = {{33, 15},{509, 3},{507, 3},{56, 3}},
    [70] = {{529, 20},{518, 20},{9338, 5},{9308, 1}},
    [71] = {{529, 30},{518, 30},{9338, 5},{9308, 1}},
    [72] = {{529, 40},{518, 40},{9338, 5},{9310, 1}},
    [73] = {{529, 50},{518, 50},{9338, 5},{9308, 1}},
    [74] = {{529, 60},{518, 60},{9338, 5},{9308, 1}},
    [75] = {{9014, 3},{52, 3},{57, 5},{56, 5}},
    [76] = {{9014, 5},{52, 3},{57, 5},{56, 5}},
    [77] = {{9014, 8},{52, 3},{57, 5},{56, 5}},
    [78] = {{9014,10},{52, 3},{503, 5},{515,3}},
    [79] = {{9014,15},{52, 3},{503, 5},{515,3}},
    [80] = {{530,20},{48, 1},{9367, 5},{9369,5}},
    [81] = {{530,30},{48, 2},{9367, 5},{9369,5}},
    [82] = {{530,40},{48, 3},{9367, 8},{9369,8}},
    [83] = {{530,50},{48, 4},{9367, 8},{9369,8}},
    [84] = {{530,60},{48, 5},{9367, 15},{9369,15}},
    [85] = {{9338,8},{134, 8},{1325, 8},{500,8}},
    [86] = {{9338,8},{134, 8},{1325, 8},{500,8}},
    [87] = {{9338,8},{134, 8},{1325, 8},{500,8}},
    [88] = {{9338,8},{134, 8},{1325, 8},{500,15}},
    [89] = {{9338,8},{134, 8},{1325, 8},{500,15}},
    [90] = {{531 ,50},{53, 5},{500, 8},{514,8}},
    [91] = {{531 ,50},{53, 5},{500, 8},{514,8}},
    [92] = {{531 ,50},{53, 5},{500, 8},{514,8}},
    [93] = {{531 ,50},{53, 5},{500, 8},{515,5}},
    [94] = {{531 ,50},{53, 5},{500, 8},{515,5}},
    [95] = {{9016 ,8},{134, 8},{1325, 8},{500,8}},
    [96] = {{9016 ,8},{134, 8},{1325, 8},{500,8}},
    [97] = {{9016 ,8},{134, 8},{1325, 8},{500,8}},
    [98] = {{9016 ,10},{134, 10},{1325, 10},{500,10}},
    [99] = {{9016 ,10},{134, 10},{1325, 10},{500,10}},
    [100] = {{532 ,50},{9338, 8},{500, 15},{503,8}},
    [101] = {{532 ,50},{134, 8},{500, 15},{503,8}},
    [102] = {{532 ,50},{1325, 8},{47, 3},{503,8}},
    [103] = {{9035 ,20},{9338, 8},{500, 15},{53,5}},
    [104] = {{9035 ,20},{134, 8},{500, 15},{53,5}},
    [105] = {{9371 ,8},{134, 8},{500, 15},{53,5}},
    [106] = {{9371 ,8},{1325, 8},{500, 15},{53,5}},
    [107] = {{9371 ,8},{9338, 8},{500, 15},{53,5}},
    [108] = {{9371 ,8},{134, 8},{503, 8},{53,5}},
    [109] = {{9371 ,8},{1325, 8},{503, 8},{53,5}},
    [110] = {{9371 ,8},{9338, 8},{503, 8},{53,5}},
    [111] = {{9371 ,8},{134, 8},{515, 8},{53,5}},
    [112] = {{9371 ,8},{1325, 8},{515, 8},{53,5}},
    [113] = {{9371 ,8},{9338, 8},{515, 8},{53,5}},
    [114] = {{9371 ,8},{134, 8},{8000, 8},{53,5}},
    [115] = {{9371 ,8},{1325, 8},{8000, 8},{53,5}},
    [116] = {{9371 ,8},{9338, 8},{8000, 8},{53,5}},
    [117] = {{503 ,8},{134, 8},{8000, 8},{43,5}},
    [118] = {{503 ,8},{1325, 8},{8000, 8},{43,5}},
    [119] = {{503 ,8},{9338, 8},{8000, 8},{43,5}},
    [120] = {{515 ,8},{134, 8},{8000, 8},{43,5}},
    [121] = {{515 ,8},{1325, 8},{8000, 8},{43,5}},
    [122] = {{515 ,8},{9338, 8},{8000, 8},{43,5}},
    [123] = {{503 ,8},{134, 8},{8000, 8},{43,5}},
    [124] = {{503 ,8},{1325, 8},{8000, 8},{43,5}},
    [125] = {{503 ,8},{9338, 8},{8000, 8},{43,5}},
    [126] = {{515 ,8},{134, 8},{8000, 8},{43,5}},
    [127] = {{515 ,8},{1325, 8},{8000, 8},{43,5}},
    [128] = {{515 ,8},{9338, 8},{8000, 8},{43,5}},
    [129] = {{503 ,8},{134, 8},{8000, 8},{43,5}},
    [130] = {{503 ,8},{1325, 8},{8000, 8},{43,5}},
    [131] = {{503 ,8},{9338, 8},{8000, 8},{43,5}},
    [132] = {{515 ,8},{134, 8},{8000, 8},{43,5}},
    [133] = {{515 ,8},{1325, 8},{8000, 8},{43,5}},
    [134] = {{515 ,8},{9338, 8},{8000, 8},{43,5}},
    [135] = {{503 ,8},{134, 8},{8000, 8},{43,5}},
    [136] = {{503 ,8},{1325, 8},{8000, 8},{43,5}},
    [137] = {{503 ,8},{9338, 8},{8000, 8},{43,5}},
    [138] = {{515 ,8},{134, 8},{8000, 8},{43,5}},
    [139] = {{515 ,8},{1325, 8},{8000, 8},{43,5}},
    [140] = {{515 ,8},{9338, 8},{8000, 8},{43,5}},
    [141] = {{503 ,8},{9338, 8},{8000, 8},{43,5}},
    [142] = {{515 ,8},{134, 8},{8000, 8},{43,5}},
    [143] = {{515 ,8},{1325, 8},{8000, 8},{43,5}},
    [144] = {{515 ,8},{9338, 8},{8000, 8},{43,5}},
    [145] = {{503 ,8},{134, 8},{8000, 8},{43,5}},
    [146] = {{503 ,8},{1325, 8},{8000, 8},{43,5}},
    [147] = {{503 ,8},{9338, 8},{8000, 8},{43,5}},
    [148] = {{515 ,8},{134, 8},{8000, 8},{43,5}},
    [149] = {{515 ,8},{1325, 8},{8000, 8},{43,5}},
    [150] = {{515 ,8},{9338, 8},{8000, 8},{43,5}},
}
local Level_gold = {
    5,5,5,5,5,          8,8,5,5,5,
    8,8,8,8,8,          8,8,20,20,20,
    20,20,25,25,25,     50,50,50,99,99,
    99,99,20,20,20,     20,50,50,35,35,
    35,35,50,50,50,     50,50,20,20,20,
    66,66,30,30,40,     40,50,99,99,99,
    111,111,77,77,77,   150,150,88,88,88,
    99,99,99,99,99,     99,99,99,99,99,
    99,99,99,99,99,     99,99,99,99,99,
    99,99,150,150,150,  99,99,99,150,150,
    150,99,99,99,150,   150,150,99,99,99,
    150,150,150,
    99,150,150,150,99,99,99,150,150,150,
}
function RunLevelAward(player, opt)
    if player == nil then
        return false;
    end
    local var_level = player:GetVar(458)
    local level = player:GetLev();
    local double = 1;
    if var_level >= level then
        player:SetVar(458, level)
        return 0;
    end
    local awards = LevelAwards[level]
    if awards == nil then
        return 0;
    end
    local package = player:GetPackage();
    if package:GetRestPackageSize() < 4 then
       player:sendMsgCode(2, 1011, 0)
       return 0;
    end
    if opt == 1 then
        local gold_need = Level_gold[level - 27]
        if nil == gold_need or player:getGoldInLua() < gold_need then
            player:sendMsgCode(2, 1104,0)
            return 0;
        end
        player:useGoldInLua(gold_need, 105)
        double = 2;
    end
    for idx = 1, #awards do
        package:Add(awards[idx][1], awards[idx][2] * double, true, false, 44);
    end
    player:SetVar(458, level)
    if getGMCheck() == true then
        for lvl = var_level+1, level-1 do
            if nil ~= LevelAwards[lvl] then
                for idx = 1, #LevelAwards[lvl] do
                    package:Add(LevelAwards[lvl][idx][1], LevelAwards[lvl][idx][2], true, false, 44);
                end
            end
        end
    end
    return true;
end

function RunAirBookOnlineAward(player, cts)
    local item = {
        [1] = {{503 ,1},{500, 1},{56, 1},{57,1}},
        [2] = {{514 ,2},{511, 2},{505, 1},{33,1}},
        [3] = {{134 ,1},{512, 1},{508, 3},{503,1}},
        [4] = {{500 ,1},{56, 1},{501, 1},{15,3}},
        [5] = {{503 ,1},{513, 1},{511, 2},{56,2}},
        [6] = {{57 ,2},{517, 2},{8000, 1},{9371,3}},
        [7] = {{9371 ,3},{551, 1},{33, 2},{30,1}},
        [8] = {{134 ,1},{1325, 1},{30, 1},{514,2}},
        [9] = {{8000 ,1},{503, 1},{509, 1},{15,3}},
        [10] = {{515 ,1},{512, 1},{517, 1},{57,2}},
        [11] = {{507 ,1},{509, 1},{57, 2},{551,1}},
    };
    local package = player:GetPackage();

    if cts > 11  or cts <= 0 then
        return false
    end

    num = #item[cts]
    if package:GetRestPackageSize() < num  then
        player:sendMsgCode(2, 1011, 0);
        return false
    end

    for count = 1, #item[cts] do
        if item[cts][count][1] == 499 then
            player:getCoupon(item[cts][count][2])
        else
            package:Add(item[cts][count][1], item[cts][count][2], true, 0, 59);
        end
    end
    return true
end
function RunAirBookLoginAward(player, cts)
    local item = {
        [1] = {{56 ,3},{57 , 3},{30, 1}},
        [2] = {{503 ,1},{56, 3},{57, 3},{9371,2}},
        [3] = {{503 ,2},{514, 2},{500, 2},{9371,3}},
        [4] = {{512 ,2},{501, 2},{517, 2},{8000,2}},
        [5] = {{134 ,2},{509, 2},{1325, 2},{1126,2}},
        [6] = {{9371 ,3},{500, 3},{134, 2}},
        [7] = {{9371 ,3},{514, 3},{1325, 2}},
        [8] = {{1126 ,2},{8000, 2},{501, 2},{51,1}},
        [9] = {{515 ,2},{513, 2},{500, 2}},
        [10] = {{551 ,2},{501, 2},{516 , 2}},
    };
    local package = player:GetPackage();

    if cts > 10  or cts <= 0 then
        return false
    end

    num = #item[cts]
    if package:GetRestPackageSize() < num  then
        player:sendMsgCode(2, 1011, 0);
        return false
    end

    for count = 1, #item[cts] do
        if item[cts][count][1] == 499 then
            player:getCoupon(item[cts][count][2])
        else
            package:Add(item[cts][count][1], item[cts][count][2], true, 0, 59);
        end
    end
    return true
end
function RunGameBoxDailyActionAward(player, cts)
    local item = {
        [0] = {{503 ,1},{500 , 1},{50, 1},{49,1}},
        [1] = {{15 ,1},{48 , 1}},
    };
    local package = player:GetPackage();

    if cts > 1  or cts < 0 then
        return false
    end

    num = #item[cts]
    if package:GetRestPackageSize() < num  then
        player:sendMsgCode(2, 1011, 0);
        return false
    end

    for count = 1, #item[cts] do
        if item[cts][count][1] == 499 then
            player:getCoupon(item[cts][count][2])
        else
            package:Add(item[cts][count][1], item[cts][count][2], true, 0, 59);
        end
    end
    return true
end
function RunHappyValueAward(player, cts)
    local item = {
        [1] = {{56,2},{57,2},{15,2},{508,2},{506,2}},
        [2] = {{500, 3},{511,3},{501,2},{517,3}},
        [3] = {{514,1},{465,3}},
        [4] = {{516,3},{503, 3},{551,3},{547,3}},
        [5] = {{509,2},{134,2},{9076,1}},
    };
    local baoshi = {5005,5015,5025,5035,5045,5055,5065,5075,5085,5095,5105,5115,5125,5135,5145}
    local package = player:GetPackage();
    if cts == 0 then
        return false
    end
    if cts > 5  then
        return false
    end
    if item[cts] == nil then
        return false 
    end
    num = #item[cts]
    if package:GetRestPackageSize() < num  then
        player:sendMsgCode(2, 1011, 0);
        return false
    end

    for count = 1, #item[cts] do
        package:Add(item[cts][count][1], item[cts][count][2], true, 0, 59);
    end
    if cts == 3 then 
        local index = math.random(1,#baoshi)
        package:Add(baoshi[index], 1, true, 0, 59);
    end
    return true
end
