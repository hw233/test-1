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
    无材料    :14
]]
local elementPool = {1,2,3,4,5,6,7,8,9,10,11,12,13,14}
local elementProb = {1000,2000,3000,4000,5000,6000,7000,8000,9000,10000,10000,10000,10000,10000}

function GetFairySparElement()
    local elements = {}
    local i = 1
    local prob

    for cnt = 1, 5 do
        prob = math.random(1,10000)
        for n = 1,#elementProb do
            if p <= elementProb[n] then
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


