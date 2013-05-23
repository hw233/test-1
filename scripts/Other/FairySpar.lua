local FairySparPara = {
    [1] = {
        [1] = 100,
        [2] = 200,
        [3] = 300,
        [4] = 400,
        [5] = 500,
        [6] = 600,
        [7] = 700,
        [8] = 800,
        [9] = 900,
    },
    [2] = {
        [1] = 100,
        [2] = 200,
        [3] = 300,
        [4] = 400,
        [5] = 500,
        [6] = 600,
        [7] = 700,
        [8] = 800,
        [9] = 900,
    },
    [3] = {
        [1] = 100,
        [2] = 200,
        [3] = 300,
        [4] = 400,
        [5] = 500,
        [6] = 600,
        [7] = 700,
        [8] = 800,
        [9] = 900,
    },
}

function GetFairySparParaMax(_type, count)
    local paraMax = 999
    count = count + 1
    if FairySparPara[_type] == nil then
        return paraMax;
    end
    if FairySparPara[_type][count] == nil then
        return paraMax;
    end
    return FairySparPara[_type][count]
end

--[[
    名字对应的索引如下：
    黄水晶碎片:1
    蓝水晶碎片:2
    红水晶碎片:3
    黄水晶    :4
    蓝水晶    :5
    红水晶    :6
    黄水晶精华:7
    蓝水晶精华:8
    红水晶精华:9
    紫玛瑙    :10
    绿玛瑙    :11
    白珍珠    :12
    黑珍珠    :13
]]
local elementPools = {
    [1] = {1,2,3,4,5,6,7,8,9,10,11,12,13},
    [2] = {21,22,23,24,25,26,27},
}
local elementProbs = {
    [1] = {2000,4000,6000,7000,8000,9000,9200,9400,9600,9700,9800,9900,10000},
    --[2] = {5500,7500,8000,8500,9000,9500,10000},
    [2] = {7000,8500,9500,10000,9000,9500,10000},
}
function GetFairySparElement(flag)
    local elements = {}
    local i = 1
    local prob
    local elementPool = elementPools[flag]
    local elementProb = elementProbs[flag]

    if elementPool == nil or elementProb == nil then
        return elements
    end

    for cnt = 1, 5 do
        prob = math.random(1,10000)
        for n = 1,#elementProb do
            if prob <= elementProb[n] then
                i = n
                break
            end
        end
        if elementPool[i] == nil then
            return elements
        end
        table.insert(elements, elementPool[i]);
    end

    return elements
end

