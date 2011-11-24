
-- 帮会战buff配置，buff id从0开始 
local ClanBattleBuffs = { 
    -- 价格 属性(百分比)
    -- 价格 攻击%  防御%   生命%  身法%   命中%   闪避%   暴击%   暴击伤害% 破击%  反击% 法术抵抗% 入场灵气(值)
    {5,     5,     15,     10,    10,     5,      5,      5,      25,     5,    5,    5,   25},
    {5,     5,     15,     10,    10,     5,      5,      5,      25,     5,    5,    5,   25},    
    {5,     5,     15,     10,    10,     5,      5,      5,      25,     5,    5,    5,   25},
}


function getClanBattleBuffs()
    return ClanBattleBuffs
end

