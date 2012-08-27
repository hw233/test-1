
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

local base = 25

local taskawardfactor = {
    [1] = {7,10,14,21,36}, -- 师门 奖励经验系统
    [2] = {600,800,1000,1300,1600} -- 衙门 奖励钱
}

function GetFlushTaskFactor(ttype, ftype)
    return flushtaskfactor[ttype][ftype]
end

function GetTaskAwardFactor(ttype, color, lvl)
    if color > 5 then
        return 0
    end
    if color == 0 then
        return 0
    end
    if ttype == 0 then
        if lvl < 30 then
            return taskawardfactor[1][color] * lvl + base
        end
        -- (等级-10)*INT(IF(等级>99,99,等级)/10)*5+25
        local yalvl = lvl
        if lvl > 99 then
            yalvl = 99
        end
        return taskawardfactor[1][color] * ((lvl-10)*math.floor(yalvl/10)*5 + base)
    end
    return taskawardfactor[ttype][color]
end
