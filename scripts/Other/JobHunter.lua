-- 第四职业——寻墨相关lua配置
local normalMonster = {
    -- 普通怪物id
    [1] = {12051, 12052, 12053},
    [2] = {12051, 12052, 12053},
    [3] = {12051, 12052, 12053},
    [4] = {12051, 12052, 12053},
}

local bossMonster = {
    -- boss id
    [1] = {11540},
    [2] = {11541},
    [3] = {11542},
    [4] = {11543},
}

local caveMonster = {
    -- 秘洞守卫 id
    [1] = {11540},
    [2] = {11541},
    [3] = {11542},
    [4] = {11543},
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

local treasure = {
    -- 宝箱道具
    [1] = treasure1,
    [2] = treasure2,
    [3] = treasure3,
    [4] = treasure4,
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

local bossMonster = {
    -- boss id
    [1] = {11540},
    [2] = {11541},
    [3] = {11542},
    [4] = {11543},
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
    local items = treasure[id][math.random(1, #treasure[id])]
    return items
end


function foundCave(id)
    -- TODO: 找到山洞后的的守卫ID
    return caveMonster(id);
end

