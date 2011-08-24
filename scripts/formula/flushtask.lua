
local flushtaskfactor = {
    [1] = { -- 师门
        [1] = {45,35,15,4,1}, -- 免费
        [2] = {0,10,45,35,10} -- 收费
    },
    [2] = { -- 衙门
        [1] = {15,44,35,5,1}, -- 免费
        [2] = {0,0,40,30,30} -- 收费
    }
}

local lvlfactor = {5,10,15,20,25,30,35,40,45}
local base = 25

local taskawardfactor = {
    [1] = {7,10,14,21,36}, -- 师门 奖励经验系统
    [2] = {600,800,1000,1300,1600} -- 衙门 奖励钱
}

function GetFlushTaskFactor(ttype, ftype)
    return flushtaskfactor[ttype][ftype]
end

function GetTaskAwardFactor(ttype, color, lvl)
    if ttype == 0 then
        if lvl < 30 then
            return taskawardfactor[1][color] * lvl + base
        end
        return taskawardfactor[1][color] * (lvlfactor[math.floor(lvl/10)] * (lvl - 10) + base)
    end
    return taskawardfactor[ttype][color]
end

