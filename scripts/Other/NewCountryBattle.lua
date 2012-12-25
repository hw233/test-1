
-- 新阵营战(蜀山论剑)buff配置 
local NewCountryBattleBuffs = { 

--价格 攻击%  防御%   生命%  身法%   命中%   闪避%   暴击%   暴击伤害% 破击%  反击% 法术抵抗% 入场灵气(值) 成就id(effortId)
  {5,    0,   0,   0,   0,   10,  0,  0,   0,   0,   0,  0,   0,   7},
  {15,   0,   10,  0,   0,   0,   0,  0,   0,   0,   0,  0,   0,   9},
  {25,   0,   0,   10,  0,   0,   0,  0,   0,   0,   0,  0,   0,   11},
  {35,   0,   0,   0,   20,  0,   0,  0,   0,   0,   0,  0,   0,   13},
  {50,   0,   0,   20,  0,   0,   0,  0,   0,   0,   0,  0,   0,   15},
  {5,    0,   0,   0,   0,   0,   0,  0,   0,   30,  0,  0,   0,   8},
  {15,   0,   0,   0,   0,   0,   0,  30,  0,   0,   0,  0,   0,   10},
  {25,   0,   0,   0,   0,   0,   0,  0,   0,   0,   0,  0,   50,  12},
  {35,   0,   0,   0,   0,   0,   0,  0,   300, 0,   0,  0,   0,   14},
  {50,   100, 0,   0,   0,   0,   0,  0,   0,   0,   0,  0,   0,   16},

}

function getNewCountryBattleBuffs()
    return NewCountryBattleBuffs
end

local EffortIdValue = {
    [1] = { --普通 
        { {1, 10} }, --id, value
        { {2, 10} },
        { {3, 10} },
        { {4, 10} },
        { {5, 10} },
        { {6, 10} },
        { {7, 10}, {8, 10}, {9, 15}, {10, 15}, {11, 20}, {12, 20}, {13, 20}, {14, 20}, {15, 25}, {16, 25} },
    },
    [2] = { --中等
        { {17, 30} },
        { {18, 30} },
        { {19, 30} },
        { {20, 30} },
        { {21, 50} },
        { {22, 50} },
        { {23, 50} },
        { {24, 50} },
    },
    [3] = { --困难
        { {25, 80} },
        { {26, 80} },
        { {27, 80} },
        { {28, 100} },
        { {29, 100} },
        { {30, 100} },
    },
}

--新阵营战(蜀山论剑)成就id与value
function getNCBEffortIdValue()
    local result = {}
    local temp1 = EffortIdValue[1]
    local temp2 = EffortIdValue[2]
    --取第一个
    local r = math.random(1, #temp1)
    if #temp1[r] > 1 then
        local i = math.random(1, #temp1[r])
        result[1] = temp1[r][i]
    else
        result[1] = temp1[r][1]
    end
    
    --取第二个
    r = math.random(1, #temp1)
    if #temp1[r] > 1 then
        local i = math.random(1, #temp1[r])
        if temp1[r][i][1] ~= result[1][1] then
            result[2] = temp1[r][i]
        else
            if i < #temp1[r] then
                result[2] = temp1[r][i+1]
            else
                result[2] = temp1[r][i-1]
            end
        end
    else
        if temp1[r][1][1] ~= result[1][1] then
            result[2] = temp1[r][1]
        else
            if #temp1[r+1] > 1 then
                local i = math.random(1, #temp1[r+1])
                if temp1[r+1][i][1] ~= result[1][1] then
                    result[2] = temp1[r+1][i]
                else
                    if i < #temp1[r+1] then
                        result[2] = temp1[r+1][i+1]
                    else
                        result[2] = temp1[r+1][i-1]
                    end
                end
            else
                result[2] = temp1[r+1][1]
            end
        end
    end
    
    --取第三个
    r = math.random(1, #temp2)
    result[3] = temp2[r][1]
    
    --取第四个
    r = math.random(1, #temp2)
    if temp2[r][1][1] ~= result[3][1] then
        result[4] = temp2[r][1]
    else
        if r < #temp2 then
            result[4] = temp2[r+1][1]
        else
            result[4] = temp2[r-1][1]
        end
    end
    
    --取第五个
    r = math.random(1, #EffortIdValue[3])
    result[5] = EffortIdValue[3][r][1]
    return result
end

--新阵营战(蜀山论剑)荣誉等级奖励
local NCBAward = {
    [1] = {{1328, 4}, {29, 10}},
    [2] = {{1328, 1}, {1327, 1}, {29, 20}},
    [3] = {{1328, 2}, {1327, 1}, {29, 30}},
    [4] = {{1328, 2}, {1327, 2}, {29, 40}},
    [5] = {{1327, 3}, {29, 50}},
    [6] = {{1326, 1}, {29, 60}},
    [7] = {{1327, 1}, {1326, 1}, {29, 70}},
    [8] = {{1327, 2}, {1326, 1}, {29, 80}},
    [9] = {{1326, 2}, {30, 1}},
}

function getNewCountryBattleAward(lvl)
    local award = NCBAward[lvl]
    if award == nil or next(award) == nil then
        return {}
    end
    return award
end

