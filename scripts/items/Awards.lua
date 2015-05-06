local ColorChance = {
    8500,9500,10000 
}
local ColorFighter = {
    {1003,1004,1005,1007,1010,1012,1014,1015,1016,1017},
    {1001,1006,1008,1009,1013},
    {1002},
    --{1,2,3,4,5},
    --{6,7,8,9,10},
    --{11,12,13,14,15},
    --{16,17,18,19,20},
    --{21,22,23,24,25},
}
function GetRandFighter()
    print("lua开始筛选将领")
    local r = math.rand(1,10000)
    local index = 1
    for i = 1,#ColorChance do
        if r < ColorChance[i] then
            index = r
            break
        end
    end
    if index > #ColorChance then
        return 0
    end
    
    return ColorFighter[index][r%(#ColorFighter[index])+1]
end

