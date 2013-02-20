
local Money = { Coin = 0x8000, Tael = 0x9000, Coupon = 0xA000, Gold = 0xB000, Achievement = 0xC000 }

local maxSoul = {
    10,20,30,40,50,60,70,80,90,100
}

local point2Award = {
    [maxSoul[1]] = {Money["Coupon"],10, 55,5, 502,5, 510,5},
    [maxSoul[2]] = {5131,1, 504,5, 5102,1},
    [maxSoul[3]] = {Money["Coupon"],20, 9,3, 15,2, 511,2},
    [maxSoul[4]] = {56,5, 57,5},
    [maxSoul[5]] = {Money["Coupon"],30, 500,3, 133,2, 512,2},
    [maxSoul[6]] = {503,5, 514,3,516,3},
    [maxSoul[7]] = {Money["Coupon"],40, 508,3, 506,3, 517,2},
    [maxSoul[8]] = {505,2,513,2,501,2},
    [maxSoul[9]] = {Money["Coupon"],50, 509,2, 507,2, 515,2},
    [maxSoul[10]] = {509,2, 507,2},
}

local needsize = {
    3, 3, 3, 2, 4, 3, 3, 3, 3, 2
}

function getHeroMemoAward(player, idx, soul)
    local max = #maxSoul
    local package = player:GetPackage();

    if idx == 0 or idx > max then
        return false
    end

    if soul >= maxSoul[idx] then
        if needsize[idx] <=  package:GetRestPackageSize() then
            local v = point2Award[maxSoul[idx]]
            local sz = #v
            for n = 1,sz,2 do
                if v[n] == Money["Coupon"] then
                    player:getCoupon(v[n+1])
                else
                    package:Add(v[n], v[n+1], true, false);
                end
            end
            return true
        else
            player:sendMsgCode(2, 1011, 0);
        end
    else
        player:sendMsgCode(2, 1019, 0)
    end
    return false
end

function flushHeroMemoAward(player, idx)
    -- 自动将所有的奖励全部发送完（因为功能下线了）
    --local max = #maxSoul
    local max = 8
    local package = player:GetPackage();

    if idx == 0 or idx > max then
        return false
    end

    local table_item = {}
    local v = point2Award[maxSoul[idx]]
    local sz = #v
    for n = 1,sz,2 do
        if v[n] == Money["Coupon"] then
            table.insert(table_item,0xA000)
            table.insert(table_item,v[n+1])
            table.insert(table_item,1)
        else
            table.insert(table_item,v[n])
            table.insert(table_item,v[n+1])
            table.insert(table_item,1)
        end
    end
    player:GetMailBox():newItemPackageMail("剑侠秘籍"..(idx*10).."剑魂奖励", "剑侠秘籍"..(idx*10).."剑魂奖励，请点击“接受物品”按钮领取奖励。", table_item);
end

function getHeroMemoMaxSoul()
    return maxSoul
end

