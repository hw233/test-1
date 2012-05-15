
-- 消耗一个物品所得到的积分
local item2point = 10

-- { 物品概率(累积值), 物品ID, 积分}，对应5个副本
local items = {
    [1] = {
        {1629,400,5},
        {3232,519,10},
        {4779,518,20},
        {6022,509,75},
        {7237,507,80},
        {7513,6006,250},
        {7790,6007,250},
        {8066,6008,250},
        {8342,6009,250},
        {8618,6010,250},
        {8895,6034,250},
        {9171,6035,250},
        {9447,6036,250},
        {9723,6037,250},
        {10000,6038,250},

    },

    [2] = {
        {1629,400,5},
        {3232,519,10},
        {4779,518,20},
        {6022,509,75},
        {7237,507,80},
        {7513,6006,250},
        {7790,6007,250},
        {8066,6008,250},
        {8342,6009,250},
        {8618,6010,250},
        {8895,6034,250},
        {9171,6035,250},
        {9447,6036,250},
        {9723,6037,250},
        {10000,6038,250},

    },

    [3] = {
        {1629,400,5},
        {3232,519,10},
        {4779,518,20},
        {6022,509,75},
        {7237,507,80},
        {7513,6006,250},
        {7790,6007,250},
        {8066,6008,250},
        {8342,6009,250},
        {8618,6010,250},
        {8895,6034,250},
        {9171,6035,250},
        {9447,6036,250},
        {9723,6037,250},
        {10000,6038,250},

    },

    [4] = {
        {1629,400,5},
        {3232,519,10},
        {4779,518,20},
        {6022,509,75},
        {7237,507,80},
        {7513,6006,250},
        {7790,6007,250},
        {8066,6008,250},
        {8342,6009,250},
        {8618,6010,250},
        {8895,6034,250},
        {9171,6035,250},
        {9447,6036,250},
        {9723,6037,250},
        {10000,6038,250},

    },

    [5] = {
        {1629,400,5},
        {3232,519,10},
        {4779,518,20},
        {6022,509,75},
        {7237,507,80},
        {7513,6006,250},
        {7790,6007,250},
        {8066,6008,250},
        {8342,6009,250},
        {8618,6010,250},
        {8895,6034,250},
        {9171,6035,250},
        {9447,6036,250},
        {9723,6037,250},
        {10000,6038,250},

    },
}

-- 触发积分概率 { 次数, 概率 },
local pointProb = {
    {50,10000},
    {49,1500},
    {48,1500},
    {47,1500},
    {46,1400},
    {45,1400},
    {44,1400},
    {43,1300},
    {42,1300},
    {41,1300},
    {40,1200},
    {39,1200},
    {38,1200},
    {37,1100},
    {36,1100},
    {35,1100},
    {34,1000},
    {33,1000},
    {32,1000},
    {31,900},
    {30,900},
    {29,900},
    {28,800},
    {27,800},
    {26,800},
    {25,700},
    {24,700},
    {23,700},
    {22,600},
    {21,600},
    {20,600},
    {19,500},
    {18,500},
    {17,500},
    {16,400},
    {15,400},
    {14,400},
    {13,300},
    {12,300},
    {11,300},
    {10,200},
    {9,200},
    {8,200},
    {7,100},
    {6,100},
    {5,100},
    {4,0},
    {3,0},
    {2,0},
    {1,0},
}

-- 副本对应消耗物品
local copyitem = {
    [1] = 9012,
    [2] = 9013,
    [3] = 9014,
    [4] = 9015,
    [5] = 9016,
}

function luckyDraw(player, id, num)
    local got = {}

    print('luckyDraw, id:' .. id .. 'num:' .. num)
    local needitem = copyitem[id]
    if needitem == nil then
        return got 
    end 

    local package = player:GetPackage();
    local ninum = package:GetItemAnyNum(needitem)
    if ninum < num then
        return got 
    end 

    print('num*1.5: ' .. num*1.5)
    if package:GetRestPackageSize() < num * 1.5 then
        player:sendMsgCode(2, 1011, 0); 
        return got 
    end

    local item = items[id]
    if item == nil then
        return got 
    end 

    local money = 0
    local point = 0
    local binum = package:GetItemNum(needitem, true)
    local bind = true 
    for o = 1, num do
        if binum > 0 then
            binum = binum - 1
        else
            bind = false
        end

        if package:DelItem(needitem, 1, bind) == false then
            break
        end

        local x = math.random(1,10000)
        local sz = #item
        for i = 1,sz do
            if x <= item[i][1] then
                package:Add(item[i][2], 1, bind, 0, 14)
                point = point + item2point
                money = money + item[i][3]
                break
            end 
        end
        player:AddVar(100, 1)

        local p = math.random(1,10000)
        local count = player:GetVar(100)
        local sz1 = #pointProb
        for n = 1,sz1 do
            if count >= pointProb[n][1] and p <= pointProb[n][2] then
                player:AddVarS(99, point - money)
                local yapoint = player:GetVarS(99)

                if yapoint > 0 then
                    local pp = 0
                    for k = 1, sz do
                        if yapoint <= item[k][3] then
                            pp = item[k][3]
                            break
                        end
                    end

                    if pp ~= 0 then
                        local yaitem = {}
                        for l = k, sz do
                            if pp == item[l][3] then
                                table.insert(yaitem, item[l][2])
                            else
                                break
                            end
                        end

                        local yasz = #yaitem
                        if yasz ~= 0 then
                            if yasz == 1 then
                                package:Add(yaitem[1], 1, false, 0, 14) -- bind or not?
                            else
                                package:Add(yaitem[math.random(1,yasz)], 1, false, 0, 14)
                            end
                        end
                    end
                end

                money = 0
                point = 0
                player:SetVar(100, 0)
                break
            end 
        end 
    end 

    if point ~= 0 then
        player:AddVarS(99, point - money)
    end

    return got 
end

