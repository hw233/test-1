-- 第四职业——寻墨相关lua配置
local normalMonster = {
    -- 普通怪物id
    [1] = {12051, 12052, 12053},
    [2] = {12051, 12052, 12053},
    [3] = {12051, 12052, 12053},
    [4] = {12051, 12052, 12053},
}

local bossMonster = {
    -- Boss id
    [1] = {12051, 12052, 12053},
    [2] = {12051, 12052, 12053},
    [3] = {12051, 12052, 12053},
    [4] = {12051, 12052, 12053},
}

local caveMonster = {
    -- 秘洞守卫 id
    [1] = 5211,
    [2] = 5211,
    [3] = 5211,
    [4] = 5211,
}

--[[
local stepAward = {
    [5]  = {{1325, 1, 1}},
    [10] = {{1326, 4, 1}},
    [15] = {{1326, 2, 1}},
    [20] = {{1326, 1, 1}},
    [25] = {{1327, 2, 1}},
}
]]--

local stepAward = {
    {{1325, 1, 1}},
    {{1326, 4, 1}},
    {{1326, 2, 1}},
    {{1326, 1, 1}},
    {{1327, 2, 1}},
}

local treasure1 = {
    -- 副本一宝箱配置
    {{1,1,1}, {2,1,1}},
    {{1,2,1}, {2,2,1}},
    {{1,3,1}, {2,3,1}},
    {{1,4,1}, {2,4,1}},
}

local treasure2 = {
    -- 副本二宝箱配置
    {{1,1,1}, {2,1,1}},
    {{1,2,1}, {2,2,1}},
    {{1,3,1}, {2,3,1}},
    {{1,4,1}, {2,4,1}},
}

local treasure3 = {
    -- 副本三宝箱配置
    {{1,1,1}, {2,1,1}},
    {{1,2,1}, {2,2,1}},
    {{1,3,1}, {2,3,1}},
    {{1,4,1}, {2,4,1}},
}

local treasure4 = {
    -- 副本四宝箱配置
    {{1,1,1}, {2,1,1}},
    {{1,2,1}, {2,2,1}},
    {{1,3,1}, {2,3,1}},
    {{1,4,1}, {2,4,1}, {550,19,1}},
}

-- 宝箱掉落概率配置
local chance1 = {379, 1895, 2400, 5768, 9558, 10000, 10000, 10000}
local chance2 = {379, 1895, 2400, 5768, 9558, 10000, 10000, 10000}
local chance3 = {379, 1895, 2400, 5768, 9558, 10000, 10000, 10000}
local chance4 = {379, 1895, 2400, 5768, 9558, 10000, 10000, 10000}

local treasure = {
    -- 宝箱道具
    [1] = treasure1,
    [2] = treasure2,
    [3] = treasure3,
    [4] = treasure4,
}

local chance = {
    -- 宝箱概率
    [1] = chance1,
    [2] = chance2,
    [3] = chance3,
    [4] = chance4,
}

local lengendMonster = {
    -- 神兽id, 下标为对应的等级，四个为四个等级的副本
    [1]  = {12051, 12052, 12053, 12054},
    [2]  = {12051, 12052, 12053, 12054},
    [3]  = {12051, 12052, 12053, 12054},
    [4]  = {12051, 12052, 12053, 12054},
    [5]  = {12051, 12052, 12053, 12054},
    [6]  = {12051, 12052, 12053, 12054},
    [7]  = {12051, 12052, 12053, 12054},
    [8]  = {12051, 12052, 12053, 12054},
    [9]  = {12051, 12052, 12053, 12054},
    [10] = {12051, 12052, 12053, 12054},
}

function getRandomNormalMonster(id)
    if id >= 1 and id <= 4 then
        return normalMonster[id][math.random(1, #(normalMonster[id]))]
    else
        return 0
    end
end

function getBossMonster(id)
    if id >= 1 and id <= 4 then
        return bossMonster[id]
    else
        return 0
    end
end

function getLengendMonster(id, val)
    if id > 4 and id == 0 then
        return 0
    end
    val = val % 10
    if val > 10 then
        return 0
    end
    return lengendMonster[val][id]
end

function calcGridType(prob)
    local chance = {20, 75, 90, 95, 100}
    for i = 1, #chance do
        if prob <=chance[i] then
            return i
        end
    end
    return 0
end

function getTreasure(id)
    -- 获得相应宝箱的道具
    local prob = math.random(1, 10000)
    local chanceTmp = chance[id]
    for i = 1, #chanceTmp do
        if prob > chanceTmp[i] then
            return treasure[id][i]
        end
    end
end

function foundCave(id)
    -- TODO: 找到山洞后的的守卫ID
    return caveMonster[id];
end

function getStepAward(step)
    for i = 1, #stepAward do
        if i*5 > step then
            return stepAward[i]
        end
    end
end

