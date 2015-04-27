local ColorChance = {
    6000,8000,9000,9500,10000 
}
local ColorFighter = {
    {1,2,3,4,5},
    {6,7,8,9,10},
    {11,12,13,14,15},
    {16,17,18,19,20},
    {21,22,23,24,25},
}
function GetRandFighter()
    local r = math.rand(1,10000)
    local index = 0
    for i = 1,#ColorChance do
        if r < ColorChance[i] then
            index = r
            break
        end
    end
    
    return ColorFighter[r%5+1]
end

