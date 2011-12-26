
-- 帮会战buff配置，buff id从0开始 
local ClanBattleBuffs = { 

 -- 价格 攻击%  防御%   生命%  身法%   命中%   闪避%   暴击%   暴击伤害% 破击%  反击% 法术抵抗% 入场灵气(值)
  {3,  5,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
  {3,  0,  15,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
  {3,  0,  0,  10,  0,  0,  0,  0,  0,  0,  0,  0,  0},
  {3,  0,  0,  0,  10,  0,  0,  0,  0,  0,  0,  0,  0},
  {3,  0,  0,  0,  0,  5,  0,  0,  0,  0,  0,  0,  0},
  {3,  0,  0,  0,  0,  0,  5,  0,  0,  0,  0,  0,  0},
  {3,  0,  0,  0,  0,  0,  0,  5,  0,  0,  0,  0,  0},
  {3,  0,  0,  0,  0,  0,  0,  0,  25,  0,  0,  0,  0},
  {3,  0,  0,  0,  0,  0,  0,  0,  0,  5,  0,  0,  0},
  {3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5,  0,  0},
  {3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  5,  0},
  {3,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  25},

}


function getClanBattleBuffs()
    return ClanBattleBuffs;
end



-- 帮战每日个人奖励
local ClanBattleDailyRewards = {
--  区间帮会积分上限  道具 数量
    { 40,       { 405,  1 } },
    { 120,      { 406,  1 } },
    { 200,      { 407,  1 } },
    { 300,      { 408,  1 } },
};

function getClanBattleDailyRewards()
    return ClanBattleDailyRewards;
end



-- 帮战周排名奖励
local ClanBattleWeekSortRewards = {
-- 区间排名上限   道具 数量
    { 1,        { 416,  5 } }, 
    { 3,        { 415,  5 } },
    { 6,        { 414,  5 } },
};

function getClanBattleWeekSortRewards()
    return ClanBattleWeekSortRewards;
end



-- 帮战周积分奖励
local ClanBattleWeekScoreRewards = {
--区间周积分上限  个人奖励                      帮派奖励
    { 300,      { {409, 2 } },                  { {409, 5 } } },
    { 700,      { {410, 2 }, {409, 1 } },       { {410, 5 }, {409, 5} } },
    { 1200,     { {411, 2 }, {410, 1 } },       { {411, 5 }, {410, 5} } },
    { 1600,     { {412, 1 }, {411, 2 } },       { {412, 5 }, {411, 5} } },
    { 3000,     { {412, 2 }, {411, 1 } },       { {413, 5 }, {412, 5}, {411, 5} } },
};

function getClanBattleWeekScoreRewards()
    return ClanBattleWeekScoreRewards;
end


