
local Money = { Coin = 0x8000, Tael = 0x9000, Coupon = 0xA000, Gold = 0xB000, Achievement = 0xC000 }

local maxSoul = {
    10,20,30,40,50,60,70,80,90,100
}

local point2Award = {
    [maxSoul[10]] = {509,2, 507,2, Money["Coupon"],50},
    [maxSoul[9]] = {505,1, 513,1, 501,1},
    [maxSoul[8]] = {508,3, 506,3, Money["Coupon"],20},
    [maxSoul[7]] = {503,3, 514,2, 516,3},
    [maxSoul[6]] = {57,3, 500,3, 1526,2, Money["Coupon"],20},
    [maxSoul[5]] = {511,2, 56,3, 512,2},
    [maxSoul[4]] = {9,2, 15,2, Money["Coupon"],10},
    [maxSoul[3]] = {504,2, 5102,1},
    [maxSoul[2]] = {510,2, 5131,1, Money["Coupon"],10},
    [maxSoul[1]] = {55,3, 502,3},
}

local needsize = {
    2, 2, 2, 2, 3, 3, 3, 2, 3, 2
}

function getHeroMemoAward(player, idx, soul)
    local max = #maxSoul
    local package = player:GetPackage();

    for n = 1,max do 
        if n == idx then
            if soul >= maxSoul[n] then
                if needsize[n] <=  package:GetRestPackageSize() then
                    local v = point2Award[maxSoul[n]]
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
                break;
            else
                player:sendMsgCode(2, 1019, 0)
                break;
            end
        end
    end
    return false
end

function getHeroMemoMaxSoul()
    return maxSoul
end

