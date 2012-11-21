-- 第四职业——寻墨相关lua配置
local normalMonster = {
    [1] = {12051, 12052, 12053},
    [2] = {12051, 12052, 12053},
    [3] = {12051, 12052, 12053},
    [4] = {12051, 12052, 12053},
}

local bossMonster = {
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
        return bossMonster[id][math.random(1, #(bossMonster[id]))]
    else
        return 0
    end
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

