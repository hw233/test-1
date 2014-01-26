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
    [3] = {     --橙色仙宠
        [1] = 15000,
        [2] = 15000,
        [3] = 7,
    },
    [4] = {     --暗金仙宠(无)
        [1] = 0,
        [2] = 0,
        [3] = 0,
    },
}

greenPet = {501, 504, 507, 510}     --绿色仙宠id  【注:设置为全局变量】
bluePet = {502, 505, 508, 511}      --蓝色仙宠id  【注:设置为全局变量】
purplePet = {503, 506, 509, 512}    --紫色仙宠id  【注:设置为全局变量】
yellowPet = {513, 514, 515, 516}    --橙色仙宠id  【注:设置为全局变量】

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
    for _, val in pairs(yellowPet) do
        if petId == val then
            return 3
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
        ["shouhun"] = 0,    --兽魂
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
    if 5 == step then
        result.shouhun = math.random(50, 60)
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

function getConvertPetValue(petId)
    local result = {
        ["longyuan"] = 0,
        ["fengsui"] = 0,
        ["like"] = 0,
    }
    if nil == petId then
        return result
    end
    local color = getPetColorFromId(petId)
    if nil == color or nil == converts[color] then
        return result
    end
    result.longyuan = converts[color][1]
    result.fengsui = converts[color][2]
    result.like = converts[color][3]
    return result
end

function getYellowPetId(petId)
    if nil == petId then
        return 0
    end
    local idx = 0
    for k, id in pairs(purplePet) do
        if petId == id then
            idx = k
            break
        end
    end
    if nil == yellowPet[idx] then
        return 0
    end
    return yellowPet[idx]
end


--飞剑(坐骑)系统 坐骑id,散件id
local mountItems = {
    [9501] = { 9611, 9612, 9613, 9614, 9615, 9616, 9617, 9618 },
    [9502] = { 9621, 9622, 9623, 9624, 9625, 9626, 9627, 9628 },
    [9503] = { 9631, 9632, 9633, 9634, 9635, 9636, 9637, 9638 },
    [9504] = { 9641, 9642, 9643, 9644, 9645, 9646, 9647, 9648 },
    [9505] = { 9651, 9652, 9653, 9654, 9655, 9656, 9657, 9658 },
    [9506] = { 9661, 9662, 9663, 9664, 9665, 9666, 9667, 9668 },
    [9507] = { 9671, 9672, 9673, 9674, 9675, 9676, 9677, 9678 },
    [9508] = { 9681, 9682, 9683, 9684, 9685, 9686, 9687, 9678 },
    [9509] = { 9691, 9692, 9693, 9694, 9695, 9696, 9697, 9698 },
    [9510] = { 9701, 9702, 9703, 9704, 9705, 9706, 9707, 9708 },
    [9511] = { 9711, 9712, 9713, 9714, 9715, 9716, 9717, 9718 },
    [9512] = { 9721, 9722, 9723, 9724, 9725, 9726, 9727, 9728 },
}
function getMountChipByCangjian(itemId, floor)
    if nil == itemId or nil == floor or floor <= 0 or floor > 8 then
        return 0
    end
    if nil == mountItems[itemId] then
        return 0
    end
    return mountItems[itemId][floor]
end

