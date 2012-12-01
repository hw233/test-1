-- 第四职业——寻墨相关lua配置
local normalMonster = {
    -- 普通怪物id
    [1] = {9532, 9533, 9534,9535,9536,9537,9538,9539,9540,9541,9542,9543,9544},
    [2] = {9547, 9548, 9549,9550,9551,9552,9553,9554,9555,9556,5557,9558,9559,9560,9561},
    [3] = {9564, 9565, 9566,9567,9568,9569,9570,9571,9572,9573,9574,9575,9576,9577,9578,9579},
    [4] = {9582, 9583, 9584,9585,9586,9587,9588,9589,9590,9591,9592,9593,9594,9595,9596,9597},
}

local bossMonster = {
    -- Boss id
    [1] = {9545,9546},
    [2] = {9562,9563},
    [3] = {9580,9581},
    [4] = {9598,9599},
}

local caveMonster = {
    -- 秘洞守卫 id
    [1] = 5211,
    [2] = 5211,
    [3] = 5211,
    [4] = 5211,
}


local stepAward = {
    {{1325, 1, 1}},
    {{1325, 1, 1}},
    {{1325, 1, 1}},
    {{1326, 4, 1}},
    {{1326, 4, 1}},
    {{1326, 2, 1}},
    {{1326, 2, 1}},
    {{1326, 2, 1}},
    {{1326, 2, 1}},
    {{1326, 2, 1}},
    {{1326, 1, 1}},
    {{1326, 1, 1}},
    {{1326, 1, 1}},
    {{1326, 1, 1}},
    {{1326, 1, 1}},
    {{1327, 2, 1}},
}

local treasure1 = {
    -- 副本一宝箱配置
    {{1327,1,1}},
    {{1326,1,1}},
    {{1525,1,1}},
    {{400,1,1}},
    {{1327,2,1}},
    {{1525,2,1}},
    {{400,2,1}},
	{{400,3,1}},
	{{1525,2,1},{1326,1,1}},
    {{400,2,1},{1326,1,1}},
	{{400,3,1},},
}

local treasure2 = {
    -- 副本二宝箱配置
    {{1327,1,1}},
    {{1326,1,1}},
    {{1525,1,1}},
    {{400,1,1}},
    {{1327,2,1}},
    {{1525,2,1}},
    {{400,2,1}},
	{{400,3,1}},
	{{1525,2,1},{1326,1,1}},
    {{400,2,1},{1326,1,1}},
	{{400,3,1},},
}

local treasure3 = {
    -- 副本三宝箱配置
    {{1327,1,1}},
    {{1326,1,1}},
    {{1525,1,1}},
    {{400,1,1}},
    {{1327,2,1}},
    {{1525,2,1}},
    {{400,2,1}},
	{{400,3,1}},
	{{1525,2,1},{1326,1,1}},
    {{400,2,1},{1326,1,1}},
	{{400,3,1},},
}

local treasure4 = {
    -- 副本四宝箱配置
    {{1327,1,1}},
    {{1326,1,1}},
    {{1525,1,1}},
    {{400,1,1}},
    {{1327,2,1}},
    {{1525,2,1}},
    {{400,2,1}},
	{{400,3,1}},
	{{1525,2,1},{1326,1,1}},
    {{400,2,1},{1326,1,1}},
	{{400,3,1},},
}

-- 宝箱掉落概率配置
local chance1 = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000,9000,10000}
local chance2 = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000,9000,10000}
local chance3 = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000,9000,10000}
local chance4 = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000,9000,10000}

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
        return bossMonster[id][math.random(1, #(bossMonster[id]))]
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
    local chanceType = {20, 75, 90, 95, 100}
    for i = 1, #chanceType do
        if prob <=chanceType[i] then
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
        if prob <= chanceTmp[i] then
            return treasure[id][i]
        end
    end
    print ("getTreasure error.")
end

function foundCave(id)
    -- 找到山洞后的的散仙ID
    return math.random(123,159)
end

function getStepAward(step)
    for i = 1, #stepAward do
        if i > step then
            return stepAward[i]
        end
    end
    return stepAward[#stepAward]
end

