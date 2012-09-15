
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
                    Broadcast(0x27, "[p:"..player:getCountry()..":"..player:getPName().."]在歡樂大轉盤中幸運地獲得了[4:"..itemId.."]x1")
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

local PlatformAward =  
{ --平台id
    [1]  = {    -- "QQ空间"
        --1:推广用注册玩家登录奖励领取
        --2:回流用户新区道具奖
        [1] = {}, --id,num
		[2] = {},
		[3] = {{515, 5},{503, 5},{507,5},{509,5},{1528,5},{440,5},{1325,5}},
    },
    [2]  = {    -- "朋友网"
        [1] = {}, --id,num
		[2] = {},
		[3] = {{515, 5},{503, 5},{507,5},{509,5},{1528,5},{440,5},{1325,5}},
    },
    [3]  = {    -- "qq微博"
        [1] = {}, --id,num
		[2] = {},
		[3] = {{515, 5},{503, 5},{507,5},{509,5},{1528,5},{440,5},{1325,5}},
    },
    [4]  = {    -- "Q+"
        [1] = {}, --id,num
		[2] = {},
		[3] = {{515, 5},{503, 5},{507,5},{509,5},{1528,5},{440,5},{1325,5}},
    },
    [5]  = {    -- "财付通"
        [1] = {}, --id,num
		[2] = {},
		[3] = {{515, 5},{503, 5},{507,5},{509,5},{1528,5},{440,5},{1325,5}},
    },
    [10] = {    -- "QQ游戏大厅"
        [1] = {}, --id,num
		[2] = {},
		[3] = {{515, 5},{503, 5},{507,5},{509,5},{1528,5},{440,5},{1325,5}},
    },
    [11] = {    -- "3366"
        [1] = {}, --id,num
		[2] = {},
		[3] = {{515, 5},{503, 5},{507,5},{509,5},{1528,5},{440,5},{1325,5}},
    },
    [12] = {    -- "官网"
        [1] = {}, --id,num
        [2] = {},
		[3] = {{515, 5},{503, 5},{507,5},{509,5},{1528,5},{440,5},{1325,5}},
    },
    [16] = {    -- gamelife
        [1] = {}, --id,num
		[2] = {},
		[3] = {{515, 5},{503, 5},{507,5},{509,5},{1528,5},{440,5},{1325,5}},
    },
    [17] = {    -- qqunion
        [1] = {}, --id,num
		[2] = {},
		[3] = {{515, 5},{503, 5},{507,5},{509,5},{1528,5},{440,5},{1325,5}},
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


