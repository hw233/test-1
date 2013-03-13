--放生仙宠对应的龙元凤髓数值
local converts = {
    [0] = {     --绿色仙宠
        [1] = 200,     --龙元
        [2] = 200,     --凤髓
        [3] = 0,        --好感度
    },
    [1] = {     --蓝色仙宠
        [1] = 1200,
        [2] = 1200,
        [3] = 0,
    },
    [2] = {     --紫色仙宠
        [1] = 7500,
        [2] = 7500,
        [3] = 7,
    },
    [3] = {     --橙色仙宠(无)
        [1] = 0,
        [2] = 0,
        [3] = 0,
    },
    [4] = {     --暗金仙宠(无)
        [1] = 0,
        [2] = 0,
        [3] = 0,
    },
}

local greenPet = {501, 504, 507, 510}   --绿色仙宠id
local purplePet = {503, 506, 509, 512}  --紫色仙宠id
bluePet = {502, 505, 508, 511}          --蓝色仙宠id  【注:设置为全局变量】

function exchangPurplePet(player)
    if nil == player then
        return 0
    end
    local like = player:GetVar(300) - 15
    if like < 0 then
        return 0
    end
    local id = purplePet[math.random(1, #purplePet)]
    player:SetVar(300, like)
    return id
end

function getPetColorFromId(petId)
    for _, val in pairs(greenPet) do
        if petId == val then
            return 0
        end
    end
    for _, val in pairs(bluePet) do
        if petId == val then
            return 1
        end
    end
    for _, val in pairs(purplePet) do
        if petId == val then
            return 2
        end
    end
    return 10
end

--仙宠
function onSeekFairypetAwardAndSucceed(step, isConvert)
    local longYuan = {
        [1] = {25, 75},
        [2] = {100, 200},
        [3] = {200, 300},
        [4] = {300, 400},
        [5] = {400, 500},
    }
    local fengSui = {
        [1] = {25, 75},
        [2] = {100, 200},
        [3] = {200, 300},
        [4] = {300, 400},
        [5] = {400, 500},
    }
    local petProb = {
        [1] = 0,
        [2] = 1667,
        [3] = 417,
        [4] = {1000, 1834},
        [5] = 3000,
    }
    local result =  --返回结果
    {
        ["longyuan"] = 0,   --龙元数
        ["fengsui"] = 0,    --凤髓数
        ["greenId"] = 0,    --绿色仙宠id
        ["blueId"] = 0,     --蓝色仙宠id
        ["like"] = 0,       --好感度(绿色)
        ["succeed"] = 0,    --是否成功晋级下一游历池
        ["convert1"] = 0,   --放生仙宠获得龙元数
        ["convert2"] = 0,   --放生仙宠获得凤髓数
    }
    if nil == step or step < 1 or step > 5 then
        step = 1
    end
    result.longyuan = math.random(longYuan[step][1], longYuan[step][2])
    result.fengsui = math.random(fengSui[step][1], fengSui[step][2])
    local r = math.random(1, 10000)
    if 2 == step and r <= petProb[step] then
        result.greenId = greenPet[math.random(1, #greenPet)]
        if isConvert > 0 then
            result.convert1 = converts[0][1]
            result.convert2 = converts[0][2]
        end
    end
    if 3 == step and r <= petProb[step] then
        result.blueId = bluePet[math.random(1, #bluePet)]
        if isConvert > 0 then
            result.convert1 = converts[1][1]
            result.convert2 = converts[1][2]
        end
    end
    if 4 == step then
        if r <= petProb[step][1] then
            result.like = 1
        end
        if r > petProb[step][1] and r <= petProb[step][2] then
            result.blueId = bluePet[math.random(1, #bluePet)]
            if isConvert > 0 then
                result.convert1 = converts[1][1]
                result.convert2 = converts[1][2]
            end
        end
    end
    if 5 == step and r <= petProb[step] then
        result.like = 1
    end
    --是否成功晋级下一游历池
    local prob = {7500, 6666, 5000, 5000, 10000}
    r = math.random(1, 10000)
    if r <= prob[step] then
        result.succeed = 1
    else
        result.succeed = 0
    end
    return result
end

function getConvertPetValue(color)
    local result = {
        ["longyuan"] = 0,
        ["fengsui"] = 0,
        ["like"] = 0,
    }
    if nil == color or nil == converts[color] then
        return result
    end
    result.longyuan = converts[color][1]
    result.fengsui = converts[color][2]
    result.like = converts[color][3]
    return result
end

