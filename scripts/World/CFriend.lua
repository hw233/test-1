
local items = {
    {}, --{{15,5,1}},
    --{}, --{{9010,1,1},},
    {}, --{{56,2,1}, {15,2,1}},
    {}, --{{56,2,1}, {15,2,1}},
    {}, --{{56,5,1}, {15,5,1}, {1600,1,1}},
    {}, --{{56,10,1}, {15,10,1}},
    {}, --{{56,10,1}, {15,10,1}},
    {{57,1,1}, {502,1,1}},                  --密友45级
    {{57,2,1}, {502,2,1}, {1605,1,1}},
    {{57,2,1}, {502,2,1}, {514,5,1}},
    {{57,5,1}, {502,5,1}, {515,2,1}},
    {{57,10,1}, {502,10,1}, {515,5,1}},
    {{57,10,1}, {502,10,1}, {515,5,1}},
    {{514,1,1}, {503,1,1}},
    {{514,2,1}, {503,2,1}, {1610,1,1}},
    {{514,2,1}, {503,2,1}, {515,1,1}},
    {{514,3,1}, {503,5,1}, {515,2,1}},
    {{514,4,1}, {503,10,1}, {515,5,1}},
    {{514,5,1}, {503,10,1}, {515,5,1}},
    {{507,1,1}, {503,1,1}},
    {{507,2,1}, {503,2,1}, {1607,1,1}},
    {{507,2,1}, {503,2,1}, {514,5,1}},
    {{507,5,1}, {503,5,1}, {515,2,1}},
    {{507,10,1}, {503,10,1}, {515,5,1}},
    {{507,10,1}, {503,10,1}, {515,5,1}},
    {{509,1,1}, {503,1,1}},
    {{509,2,1}, {503,2,1}, {1621,1,1}},
    {{509,2,1}, {503,2,1}, {514,5,1}},
    {{509,5,1}, {503,5,1}, {515,2,1}},
    {{509,10,1}, {503,10,1}, {515,5,1}},
    {{509,10,1}, {503,10,1}, {515,5,1}},
    {{503,1,1}, {507,1,1}},
    {{503,2,1}, {507,2,1},{1616,1,1}},
    {{503,2,1}, {507,2,1},{514,5,1}},
    {{503,5,1}, {507,5,1},{515,2,1}},
    {{503,1,1}, {509,1,1}},
    {{503,2,1}, {509,2,1},{1615,1,1}},
    {{503,2,1}, {509,2,1},{514,5,1}},
    {{503,5,1}, {509,5,1},{515,2,1}},
    {},     --邀请好友成功2位
    {},
    {},
    {},
};

local CFtickets = {
    1, 1, 1, 1, 1, 1,   --召回,赠送,领取,邀请好友
    1, 2, 3, 4, 5, 6,   --密友等级达到45级
    1, 2, 3, 4, 5, 6,   --密友等级达到50级
    1, 2, 3, 4, 5, 6,   --密友等级达到60级
    1, 2, 4, 6, 8, 10,  --密友等级达到70级
    1, 2, 3, 4,         --密友斗剑排名进入500名以内
    2, 4, 6, 9,         --密友斗剑排名进入200名以内
    1, 2, 3, 6,         --邀请好友成功2,5,10,20位
}

function onGetCFriendAward(player, idx)
    if idx == 0 then
        return false
    end

    if items[idx] == nil then
        return false
    end

    local package = player:GetPackage()
    if package:GetRestPackageSize() < #items[idx] then 
        player:sendMsgCode(2, 1011, 0)
        return false
    end  

    for k, v in pairs(items[idx]) do
        package:Add(v[1], v[2], v[3], false, 40)
    end  
    if CFtickets[idx] ~= nil and CFtickets[idx] ~= 0 then
        player:AddVar(300, CFtickets[idx])
    end
    return true 
end

function onUseTickets(player)
    if nil == player then
        return 0
    end
    local var = player:GetVar(300)
    if var <= 0 then
        player:sendMsgCode(2, 1110, 0)
        return 0
    end
    local items = { {502, 1}, {510, 1}, {499, 10}, {503, 1}, {57, 1}, {56, 1}, {15, 1} }
    local chance = {2700, 5400, 6000, 6400, 7600, 8800, 10000}
    if not isFBVersion() then    --10QB奖励
        for i = 33, 40 do
            if player:hasRealItemAward(i) then
                player:getRealItemAward(i)
                Broadcast(0x27, "恭喜玩家[p:"..player:getCountry()..":"..player:getPName().."]使用抽奖券抽得了10QB,真是人品大爆发啊！还在等什么？邀请好友来蜀山传奇即可获得抽奖券抽取Q币！")
                player:SetVar(300, var - 1)
                player:lastCFTicketsAward(0, 10)
                return  #items + 1
            end
        end
    end
    local package = player:GetPackage()
    if package:GetRestPackageSize() < 1 then
        player:sendMsgCode(2, 1011, 0)
        return 0
    end
    local r = math.random(1, 10000)
    local tmp = 0
    for i = 1, #chance do
        if r <= chance[i] then
            tmp = i
            break
        end
    end
    if items[tmp][1] == 499 then
        player:pendCoupon(items[tmp][2])
    else
        package:Add(items[tmp][1], items[tmp][2], true, true, 40)
    end
    player:SetVar(300, var - 1)
    player:lastCFTicketsAward(items[tmp][1], items[tmp][2])
    return tmp
end

