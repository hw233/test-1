
local flushtaskfactor = {
    [1] = { -- 师门
        [1] = {15,44,35,5,1}, -- 免费
        [2] = {0,0,40,30,30} -- 收费
    },
    [2] = { -- 衙门
        [1] = {15,44,35,5,1}, -- 免费
        [2] = {0,0,40,30,30} -- 收费
    }
}

local taskawardfactor = {
    [1] = {1000,2000,2000,3000,5000}, -- 师门 奖励经验
    [2] = {600,800,1000,1300,1600} -- 衙门 奖励钱
}

function GetFlushTaskFactor(ttype, ftype)
    return flushtaskfactor[ttype][ftype]
end

function GetTaskAwardFactor(ttype)
    return taskawardfactor[ttype]
end

