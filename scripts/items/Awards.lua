
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
    local package = player:GetPackage();

    if cts == 0 then
        return false
    end
    if cts > 7 then
        return false
    end

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
        [1] = {{515,3},{507,2},{509,2},{503,10},{1325,4},{47,3},{134,4},{5026,1}},
        [2] = {{515,1},{507,1},{509,1},{503,1},{1325,1},{47,1},{134,1},{5026,1}}
    };
    local ch = {
        [1] = {1852, 2593, 3333, 4444, 5926, 6667, 8148, 10000},
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
    local date_9191_0 = { ['year'] = 2012, ['month'] = 12, ['day'] = 24, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 }
    local date_9191_1 = { ['year'] = 2013, ['month'] = 1, ['day'] = 1, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 }
    local date_9217_0 = { ['year'] = 2012, ['month'] = 11, ['day'] = 20, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 }
    local date_9217_1 = { ['year'] = 2012, ['month'] = 11, ['day'] = 27, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 }
    
    local date_0 = os.time(date_9190_0);
    local date_1 = os.time(date_9190_1);

    local chance = {785,2833,4881,5823,7202,7987,9366,10000}
    local chance_1 = {1428, 1428*2, 1428*3, 1428*4, 1428*5, 1428*6, 10000, 10000}
    local item_9190 = {{515,1},{507,1},{509,1},{503,5},{1325,2},{47,2},{134,2},{5026,1}}
    local item_9191 = {{515,1},{507,1},{509,1},{503,5},{1325,2},{47,2},{134,2},{5026,1}}
    local item_9217 = {{515,3},{507,2},{509,2},{503,10},{1325,4},{47,3},{134,4},{5026,1}}
    local item_9284 = {{507,2},{509,2},{503,10},{1325,4},{47,3},{134,4},{515,10},{'ipad',1}}

    local items = item_9190;
    local itemId = 9190;
    local ch = chance;

    if opt == 2 then
        itemId = 9191;
        items = item_9191;
        date_0 = os.time(date_9191_0);
        date_1 = os.time(date_9191_1);
    elseif opt == 3 then
        itemId = 9217;
        items = item_9217;
        date_0 = os.time(date_9217_0);
        date_1 = os.time(date_9217_1);
    elseif opt == 4 then
        itemId = 9284;
        items = item_9284;
        ch = chance_1;
    end
--    now = os.time()
--    if now < date_0 or now >= date_1 then
--        return 0;
--    end

    if  not package:DelItem(itemId, 1, true) then
        if  not package:DelItem(itemId, 1, false) then
            player:sendMsgCode(2, 1110, 0);
            return 0;
        end
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

    local VAR_BLUE_AWARD_COUNT = 196;
    local VAR_YELLOW_AWARD_COUNT = 197;
    local VAR_QQVIP_AWARD_COUNT = 245;
    local count = 0;
    if opt == 1 then
        player:AddVar(VAR_BLUE_AWARD_COUNT, 1);
        count = player:GetVar(VAR_BLUE_AWARD_COUNT);
        if count == 12 then
            package:Add(1707, 1, true, 0, 31);
        end
    elseif opt == 2 then
        player:AddVar(VAR_YELLOW_AWARD_COUNT, 1);
        count = player:GetVar(VAR_YELLOW_AWARD_COUNT);
        if count == 12 then
            package:Add(9076, 5, true, 0, 31); 
            package:Add(515, 5, true, 0, 31); 
            package:Add(507, 5, true, 0, 31); 
            package:Add(509, 5, true, 0, 31); 
        end
    elseif opt == 3 then
        player:AddVar(VAR_QQVIP_AWARD_COUNT, 1);
        count = player:GetVar(VAR_QQVIP_AWARD_COUNT);
        if count == 5 then
             package:Add(1708, 1, true, 0, 31);
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
    [4] = {{15, 1},{1326, 1},{56, 1},{57, 1}},
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
    local award = QQGameawards[awardOrder]
    if award == nil then
        return false
    end
    local package = player:GetPackage()
    if package:GetRestPackageSize() < #award then
        player:sendMsgCode(2, 1011, 0);
        return false
    end
    for i = 1, #awards do
        package:Add(awards[i][1], awards[i][2], true, false, 2)
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
    [6] = {{56, 2},{57, 2},{134, 2},{30, 2},{9076,1}},
}

function onGetNewYearQQGameAward(player, type)
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

