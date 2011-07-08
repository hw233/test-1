
local tripodaward = 
{
    -- 橙       紫      蓝      绿
    {1,         2,      3,      4}, -- 干材烈火
    {5,         6,      2,      3}, -- 貅目鑫火
    {1,         3,      2,      6}, -- 极地冽火
    {5,         2,      1,      6}, -- 盘木玄火
    {2,         2,      1,      1}, -- 炼狱冥火
    {3,         2,      5,      4}, -- 三昧真火
    {4,         2,      1,      6}  -- 九天离火
}

function GetTripodAward(fire)
    return tripodaward[fire]
end

