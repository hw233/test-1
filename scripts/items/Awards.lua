local ColorChance = {
    6000,9000,10000 
}
local ColorFighter = {
    --{1003,1004,1005,1007,1010,1012,1014,1015,1016,1017},
    --{1001,1006,1008,1009,1013},
    --{1002},
    --{1,2,3,4,5},
    --{6,7,8,9,10},
    --{11,12,13,14,15},
    --{16,17,18,19,20},
    --{21,22,23,24,25},
    {1003,1014,1015,1017},
    {1001,1006,1013} ,
    {1002},
}
function GetRandFighter()
    print("lua开始筛选将领")
    local r = math.random(1,10000)
    local index = 1
    for i = 1,#ColorChance do
        if r < ColorChance[i] then
            index = i
            break
        end
    end
    print("index:" .. index.. "  r:".. r)
    if index > #ColorChance then
        return 0
    end
    
    return ColorFighter[index][r%(#ColorFighter[index])+1]
end

local AwardForMouth = {
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
    {1,1,1},
}

function GetSignForMouth(player, index , vip ,flag)  --flag == 0 表示补签
    if index > #AwardForMouth then
        return 0
    end
    local count = 1; 

    if flag == 0 and vip < AwardForMouth[index][1] then
        return 0
    end

    if flag == 1 and vip >= AwardForMouth[index][1] then
        count = 2
    end

    local package = player:GetPackage()

    package:AddItem(AwardForMouth[index][2], AwardForMouth[index][3] * count)
    return count
end

