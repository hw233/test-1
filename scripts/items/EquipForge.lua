
--装备强化费用
local Enchant_cost = 10
--宝石合成费用
local Merge_cost = 10
--宝石拆卸费用
local Detach_cost = 10
--装备分解费用
local Split_cost = 10
--装备锻造费用
local Forge_cost = 10

--装备打孔失败概率
local Socket_chance = {0, 0, 15, 15, 45, 45}

local Split_chance = {
    {30, 0},            --绿色装备分解概率
    {40, 0},            --蓝色装备分解概率
    {95, 5},            --紫色装备分解概率
    {80, 20}            --橙色装备分解概率
}

--强化概率
local Enchant_chance = {
    {1000,  900,  600,  400,  200,  100,  50,   10,   2,    1,    1,    1},    -- 装备强化概率
    {1000, 1000, 1000,  900,  800,  700,  600,  500,    0,    0,    0,    0},    -- 绿色法宝强化概率
    {1000, 1000,  900,  800,  700,  600,  450,  300,    0,    0,    0,    0},    -- 蓝色法宝强化概率
    {1000,  900,  750,  600,  450,  300,  150,   50,    0,    0,    0,    0},    -- 紫色法宝强化概率
    { 650,  500,  350,  200,   50,   10,    7,    5,    0,    0,    0,    0},    -- 橙色法宝强化概率
}

local Enchant_max = {7, 7, 7, 7, 8, 10, 10, 10, 10, 11, 12}

--2级至10级宝石的合成概率
local Merge_chance = {100, 85, 85, 75, 75, 65, 65, 55, 55}



--蓝色装备随机属性概率
local Blue_attr_chance0 = {0, 0, 0, 0, 0, 0, 0, 0, 0}
local Blue_attr_chance1 = {15, 50, 35, 0, 0, 0, 0, 0, 0}
local Blue_attr_chance2 = {5,  60, 35, 0, 0, 0, 0, 0, 0}
local Blue_attr_chance3 = {25, 50, 25, 0, 0, 0, 0, 0, 0}
local Blue_attr_chance4 = {5, 65,30, 0, 0, 0, 0, 0, 0}

--紫色装备随机属性概率
local Purple_attr_chance0 = {0, 0, 0, 0, 0, 0, 0, 0, 0}
local Purple_attr_chance1 = {15, 50, 35, 0, 0, 0, 0, 0, 0}

--橙色装备随机属性概率
local Orange_attr_chance0 = {0, 0, 0, 0, 0, 0, 0, 0, 0}
local Orange_attr_chance1 = {15, 44, 20, 10, 5, 3, 2, 1, 0}
local Orange_attr_chance2 = {15, 70, 10, 5, 0, 0, 0, 0, 0}

--0级装备随机属性概率
local Attr_chance_lvl0  = {Blue_attr_chance1, Purple_attr_chance0, Orange_attr_chance0}
local Attr_chance_lvl10 = {Blue_attr_chance1, Purple_attr_chance0, Orange_attr_chance0}
local Attr_chance_lvl20 = {Blue_attr_chance2, Purple_attr_chance0, Orange_attr_chance0}
local Attr_chance_lvl30 = {Blue_attr_chance3, Purple_attr_chance0, Orange_attr_chance0}
local Attr_chance_lvl40 = {Blue_attr_chance4, Purple_attr_chance1, Orange_attr_chance1}
local Attr_chance_lvl50 = {Blue_attr_chance4, Purple_attr_chance1, Orange_attr_chance1}
local Attr_chance_lvl60 = {Blue_attr_chance4, Purple_attr_chance1, Orange_attr_chance2}
local Attr_chance_lvl70 = {Blue_attr_chance4, Purple_attr_chance1, Orange_attr_chance1}
local Attr_chance_lvl80 = {Blue_attr_chance4, Purple_attr_chance1, Orange_attr_chance1}
local Attr_chance_lvl90 = {Blue_attr_chance0, Purple_attr_chance1, Orange_attr_chance1}
local Attr_chance_lvl100 = {Blue_attr_chance0, Purple_attr_chance1, Orange_attr_chance1}


--装备随机属性概率
local AttrChances = {
    Attr_chance_lvl0,
    Attr_chance_lvl10,
    Attr_chance_lvl20,
    Attr_chance_lvl30,
    Attr_chance_lvl40,
    Attr_chance_lvl50,
    Attr_chance_lvl60,
    Attr_chance_lvl70,
    Attr_chance_lvl80,
    Attr_chance_lvl90,
    Attr_chance_lvl100
}

--生命值属性数值
local HP_value0 = {0, 0, 0, 0, 0, 0, 0, 0}
local Blue_HP_value1 = {10, 15, 20, 0, 0, 0, 0, 0}
local Blue_HP_value2 = {20, 25, 30, 0, 0, 0, 0, 0}
local Blue_HP_value3 = {40, 45, 50, 0, 0, 0, 0, 0}
local Blue_HP_value4 = {50, 55, 60, 0, 0, 0, 0, 0}
local Blue_HP_value5 = {60, 70, 80, 0, 0, 0, 0, 0}
local Blue_HP_value6 = {80, 90, 100, 0, 0, 0, 0, 0}
local Blue_HP_value7 = {100, 110, 120, 0, 0, 0, 0, 0}
local Blue_HP_value8 = {120, 130, 140, 0, 0, 0, 0, 0}
local Purple_HP_value1 = {60, 70, 80, 0, 0, 0, 0, 0}
local Purple_HP_value2 = {80, 90, 100, 0, 0, 0, 0, 0}
local Purple_HP_value3 = {100, 110, 120, 0, 0, 0, 0, 0}
local Purple_HP_value4 = {120, 130, 140, 0, 0, 0, 0, 0}
local Purple_HP_value5 = {140, 155, 170, 0, 0, 0, 0, 0}
local Purple_HP_value6 = {175, 200, 225, 0, 0, 0, 0, 0}
local Orange_HP_value1 = {80, 85, 90, 95, 100, 105, 110, 120} 
local Orange_HP_value2 = {120, 125, 130, 135, 140, 150, 160, 180}
local Orange_HP_value3 = {120, 140, 160, 180, 0, 0, 0, 0}
local Orange_HP_value4 = {140, 145, 150, 160, 170, 180, 200, 220}
local Orange_HP_value5 = {170, 180, 190, 200, 210, 220, 240, 260}
local Orange_HP_value6 = {200, 220, 240, 260, 280, 300, 320, 350}
local Orange_HP_value7 = {250, 275, 300, 325, 350, 375, 400, 450}

--命中值属性数值
local Hitrate_value0 = {0, 0, 0, 0, 0, 0, 0, 0}
local Blue_Hitrate_value1 = {40, 50, 60, 0, 0, 0, 0, 0}
local Blue_Hitrate_value2 = {50, 60, 70, 0, 0, 0, 0, 0}
local Purple_Hitrate_value1 = {60, 70, 80, 0, 0, 0, 0, 0}
local Purple_Hitrate_value2 = {70, 80, 90, 0, 0, 0, 0, 0}
local Orange_Hitrate_value1 = {90, 100, 110, 120, 130, 140, 150, 160}
local Orange_Hitrate_value2 = {90, 110, 130, 160, 0, 0, 0, 0}

--闪避值属性数值
local Evad_value0 = {0, 0, 0, 0, 0, 0, 0, 0}
local Blue_Evad_value1 = {40, 50, 60, 0, 0, 0, 0, 0}
local Purple_Evad_value1 = {60, 70, 80, 0, 0, 0, 0, 0}
local Orange_Evad_value1 = {80, 90, 100, 110, 120, 130, 140, 150}
local Orange_Evad_value2 = {80, 100, 120, 150, 0, 0, 0, 0}

--暴击值属性数值
local Critical_value0 = {0, 0, 0, 0, 0, 0, 0, 0}
local Blue_Critical_value1 = {50, 80, 100, 0, 0, 0, 0, 0}
local Blue_Critical_value2 = {60, 90, 120, 0, 0, 0, 0, 0}
local Purple_Critical_value1 = {100, 120, 150, 0, 0, 0, 0, 0}
local Purple_Critical_value2 = {120, 150, 180, 0, 0, 0, 0, 0}
local Orange_Critical_value1 = {180, 190, 200, 220, 240, 260, 280, 300}
local Orange_Critical_value2 = {180, 220, 260, 300, 0, 0, 0, 0}

--破击值属性数值
local Pierce_value0 = {0, 0, 0, 0, 0, 0, 0, 0}
local Blue_Pierce_value1 = {50, 80, 100, 0, 0, 0, 0, 0}
local Blue_Pierce_value2 = {60, 90, 120, 0, 0, 0, 0, 0}
local Purple_Pierce_value1 = {100, 120, 150, 0, 0, 0, 0, 0}
local Purple_Pierce_value2 = {120, 150, 180, 0, 0, 0, 0, 0}
local Orange_Pierce_value1 = {180, 190, 200, 210, 230, 250, 260, 280}
local Orange_Pierce_value2 = {180, 220, 250, 280, 0, 0, 0, 0}

--身法值属性数值
local Action_value0 = {0, 0, 0, 0, 0, 0, 0, 0}
local Blue_Action_value1 = {40, 50, 60, 0, 0, 0, 0, 0}
local Purple_Action_value1 = {60, 70, 80, 0, 0, 0, 0, 0}
local Orange_Action_value1 = {80, 90, 100, 110, 120, 130, 140, 150}
local Orange_Action_value2 = {80, 100, 120, 150, 0, 0, 0, 0}

--反击值属性数值
local Counter_value0 = {0, 0, 0, 0, 0, 0, 0, 0}
local Blue_Counter_value1 = {40, 50, 60, 0, 0, 0, 0, 0}
local Purple_Counter_value1 = {60, 70, 80, 0, 0, 0, 0, 0}
local Orange_Counter_value1 = {80, 90, 100, 110, 120, 130, 140, 150}
local Orange_Counter_value2 = {80, 90, 110, 130, 0, 0, 0, 0}

--法术抵抗值属性数值
local MagRes_value0 = {0, 0, 0, 0, 0, 0, 0, 0}
local Blue_MagRes_value1 = {40, 50, 60, 0, 0, 0, 0, 0}
local Purple_MagRes_value1 = {60, 70, 80, 0, 0, 0, 0, 0}
local Orange_MagRes_value1 = {80, 90, 100, 110, 120, 130, 140, 150}
local Orange_MagRes_value2 = {80, 100, 120, 150, 0, 0, 0, 0}


--0级装备属性数值
local AttrInfo_HP_lvl0 =       {Blue_HP_value1,       HP_value0,       HP_value0}
local AttrInfo_Hitrate_lvl0 =  {Blue_Hitrate_value1,  Hitrate_value0,  Hitrate_value0}
local AttrInfo_Evade_lvl0 =    {Blue_Evad_value1,     Evad_value0,     Evad_value0}
local AttrInfo_Critical_lvl0 = {Blue_Critical_value1, Critical_value0, Critical_value0}
local AttrInfo_Pierce_lvl0 =   {Blue_Pierce_value1,   Pierce_value0,   Pierce_value0}
local AttrInfo_Action_lvl0 =   {Blue_Action_value1,   Action_value0,   Action_value0}
local AttrInfo_Counter_lvl0 =  {Blue_Counter_value1,  Counter_value0,  Counter_value0}
local AttrInfo_MagRes_lvl0 =   {Blue_MagRes_value1,   MagRes_value0,   MagRes_value0}

local AttrInfo_lvl0 = {
    AttrInfo_HP_lvl0,
    AttrInfo_Action_lvl0,
    AttrInfo_Hitrate_lvl0,
    AttrInfo_Evade_lvl0,
    AttrInfo_Critical_lvl0,
    AttrInfo_Pierce_lvl0,
    AttrInfo_Counter_lvl0,
    AttrInfo_MagRes_lvl0
}



--10级装备属性数值
local AttrInfo_HP_lvl10 =       {Blue_HP_value1,       HP_value0,       HP_value0}
local AttrInfo_Hitrate_lvl10 =  {Blue_Hitrate_value1,  Hitrate_value0,  Hitrate_value0}
local AttrInfo_Evade_lvl10 =    {Blue_Evad_value1,     Evad_value0,     Evad_value0}
local AttrInfo_Critical_lvl10 = {Blue_Critical_value1, Critical_value0, Critical_value0}
local AttrInfo_Pierce_lvl10 =   {Blue_Pierce_value1,   Pierce_value0,   Pierce_value0}
local AttrInfo_Action_lvl10 =   {Blue_Action_value1,   Action_value0,   Action_value0}
local AttrInfo_Counter_lvl10 =  {Blue_Counter_value1,  Counter_value0,  Counter_value0}
local AttrInfo_MagRes_lvl10 =   {Blue_MagRes_value1,   MagRes_value0,   MagRes_value0}

local AttrInfo_lvl10 = {
    AttrInfo_HP_lvl10,
    AttrInfo_Action_lvl10,
    AttrInfo_Hitrate_lvl10,
    AttrInfo_Evade_lvl10,
    AttrInfo_Critical_lvl10,
    AttrInfo_Pierce_lvl10,
    AttrInfo_Counter_lvl10,
    AttrInfo_MagRes_lvl10
}



--20级装备属性数值
local AttrInfo_HP_lvl20 =       {Blue_HP_value2,       HP_value0,       HP_value0}
local AttrInfo_Hitrate_lvl20 =  {Blue_Hitrate_value1,  Hitrate_value0,  Hitrate_value0}
local AttrInfo_Evade_lvl20 =    {Blue_Evad_value1,     Evad_value0,     Evad_value0}
local AttrInfo_Critical_lvl20 = {Blue_Critical_value1, Critical_value0, Critical_value0}
local AttrInfo_Pierce_lvl20 =   {Blue_Pierce_value1,   Pierce_value0,   Pierce_value0}
local AttrInfo_Action_lvl20 =   {Blue_Action_value1,   Action_value0,   Action_value0}
local AttrInfo_Counter_lvl20 =  {Blue_Counter_value1,  Counter_value0,  Counter_value0}
local AttrInfo_MagRes_lvl20 =   {Blue_MagRes_value1,   MagRes_value0,   MagRes_value0}

local AttrInfo_lvl20 = {
    AttrInfo_HP_lvl20,
    AttrInfo_Action_lvl20,
    AttrInfo_Hitrate_lvl20,
    AttrInfo_Evade_lvl20,
    AttrInfo_Critical_lvl20,
    AttrInfo_Pierce_lvl20,
    AttrInfo_Counter_lvl20,
    AttrInfo_MagRes_lvl20
}



--30级装备属性数值
local AttrInfo_HP_lvl30 =       {Blue_HP_value3,       HP_value0,       HP_value0}
local AttrInfo_Hitrate_lvl30 =  {Blue_Hitrate_value1,  Hitrate_value0,  Hitrate_value0}
local AttrInfo_Evade_lvl30 =    {Blue_Evad_value1,     Evad_value0,     Evad_value0}
local AttrInfo_Critical_lvl30 = {Blue_Critical_value1, Critical_value0, Critical_value0}
local AttrInfo_Pierce_lvl30 =   {Blue_Pierce_value1,   Pierce_value0,   Pierce_value0}
local AttrInfo_Action_lvl30 =   {Blue_Action_value1,   Action_value0,   Action_value0}
local AttrInfo_Counter_lvl30 =  {Blue_Counter_value1,  Counter_value0,  Counter_value0}
local AttrInfo_MagRes_lvl30 =   {Blue_MagRes_value1,   MagRes_value0,   MagRes_value0}

local AttrInfo_lvl30 = {
    AttrInfo_HP_lvl30,
    AttrInfo_Action_lvl30,
    AttrInfo_Hitrate_lvl30,
    AttrInfo_Evade_lvl30,
    AttrInfo_Critical_lvl30,
    AttrInfo_Pierce_lvl30,
    AttrInfo_Counter_lvl30,
    AttrInfo_MagRes_lvl30
}



--40级装备属性数值
local AttrInfo_HP_lvl40 =       {Blue_HP_value4,       Purple_HP_value1,       Orange_HP_value1}
local AttrInfo_Hitrate_lvl40 =  {Blue_Hitrate_value1,  Purple_Hitrate_value1,  Orange_Hitrate_value1}
local AttrInfo_Evade_lvl40 =    {Blue_Evad_value1,     Purple_Evad_value1,     Orange_Evad_value1}
local AttrInfo_Critical_lvl40 = {Blue_Critical_value1, Purple_Critical_value1, Orange_Critical_value1}
local AttrInfo_Pierce_lvl40 =   {Blue_Pierce_value1,   Purple_Pierce_value1,   Orange_Pierce_value1}
local AttrInfo_Action_lvl40 =   {Blue_Action_value1,   Purple_Action_value1,   Orange_Action_value1}
local AttrInfo_Counter_lvl40 =  {Blue_Counter_value1,  Purple_Counter_value1,  Orange_Counter_value1}
local AttrInfo_MagRes_lvl40 =   {Blue_MagRes_value1,   Purple_MagRes_value1,   Orange_MagRes_value1}

local AttrInfo_lvl40 = {
    AttrInfo_HP_lvl40,
    AttrInfo_Action_lvl40,
    AttrInfo_Hitrate_lvl40,
    AttrInfo_Evade_lvl40,
    AttrInfo_Critical_lvl40,
    AttrInfo_Pierce_lvl40,
    AttrInfo_Counter_lvl40,
    AttrInfo_MagRes_lvl40}



--50级装备属性数值
local AttrInfo_HP_lvl50 =       {Blue_HP_value5,       Purple_HP_value2,       Orange_HP_value2}
local AttrInfo_Hitrate_lvl50 =  {Blue_Hitrate_value1,  Purple_Hitrate_value1,  Orange_Hitrate_value1}
local AttrInfo_Evade_lvl50 =    {Blue_Evad_value1,     Purple_Evad_value1,     Orange_Evad_value1}
local AttrInfo_Critical_lvl50 = {Blue_Critical_value1, Purple_Critical_value1, Orange_Critical_value1}
local AttrInfo_Pierce_lvl50 =   {Blue_Pierce_value1,   Purple_Pierce_value1,   Orange_Pierce_value1}
local AttrInfo_Action_lvl50 =   {Blue_Action_value1,   Purple_Action_value1,   Orange_Action_value1}
local AttrInfo_Counter_lvl50 =  {Blue_Counter_value1,  Purple_Counter_value1,  Orange_Counter_value1}
local AttrInfo_MagRes_lvl50 =   {Blue_MagRes_value1,   Purple_MagRes_value1,   Orange_MagRes_value1}

local AttrInfo_lvl50 = {
    AttrInfo_HP_lvl50,
    AttrInfo_Action_lvl50,
    AttrInfo_Hitrate_lvl50,
    AttrInfo_Evade_lvl50,
    AttrInfo_Critical_lvl50,
    AttrInfo_Pierce_lvl50,
    AttrInfo_Counter_lvl50,
    AttrInfo_MagRes_lvl50
}



--60级装备属性数值
local AttrInfo_HP_lvl60 =       {Blue_HP_value6,       Purple_HP_value3,       Orange_HP_value3}
local AttrInfo_Hitrate_lvl60 =  {Blue_Hitrate_value2,  Purple_Hitrate_value2,  Orange_Hitrate_value2}
local AttrInfo_Evade_lvl60 =    {Blue_Evad_value1,     Purple_Evad_value1,     Orange_Evad_value2}
local AttrInfo_Critical_lvl60 = {Blue_Critical_value2, Purple_Critical_value2, Orange_Critical_value2}
local AttrInfo_Pierce_lvl60 =   {Blue_Pierce_value2,   Purple_Pierce_value2,   Orange_Pierce_value2}
local AttrInfo_Action_lvl60 =   {Blue_Action_value1,   Purple_Action_value1,   Orange_Action_value2}
local AttrInfo_Counter_lvl60 =  {Blue_Counter_value1,  Purple_Counter_value1,  Orange_Counter_value2}
local AttrInfo_MagRes_lvl60 =   {Blue_MagRes_value1,   Purple_MagRes_value1,   Orange_MagRes_value2}

local AttrInfo_lvl60 = {
    AttrInfo_HP_lvl60,
    AttrInfo_Action_lvl60,
    AttrInfo_Hitrate_lvl60,
    AttrInfo_Evade_lvl60,
    AttrInfo_Critical_lvl60,
    AttrInfo_Pierce_lvl60,
    AttrInfo_Counter_lvl60,
    AttrInfo_MagRes_lvl60
}



--70级装备属性数值
local AttrInfo_HP_lvl70 =       {Blue_HP_value7,       Purple_HP_value4,       Orange_HP_value4}
local AttrInfo_Hitrate_lvl70 =  {Blue_Hitrate_value2,  Purple_Hitrate_value2,  Orange_Hitrate_value1}
local AttrInfo_Evade_lvl70 =    {Blue_Evad_value1,     Purple_Evad_value1,     Orange_Evad_value1}
local AttrInfo_Critical_lvl70 = {Blue_Critical_value2, Purple_Critical_value2, Orange_Critical_value1}
local AttrInfo_Pierce_lvl70 =   {Blue_Pierce_value2,   Purple_Pierce_value2,   Orange_Pierce_value1}
local AttrInfo_Action_lvl70 =   {Blue_Action_value1,   Purple_Action_value1,   Orange_Action_value1}
local AttrInfo_Counter_lvl70 =  {Blue_Counter_value1,  Purple_Counter_value1,  Orange_Counter_value1}
local AttrInfo_MagRes_lvl70 =   {Blue_MagRes_value1,   Purple_MagRes_value1,   Orange_MagRes_value1}

local AttrInfo_lvl70 = {
    AttrInfo_HP_lvl70,
    AttrInfo_Action_lvl70,
    AttrInfo_Hitrate_lvl70,
    AttrInfo_Evade_lvl70,
    AttrInfo_Critical_lvl70,
    AttrInfo_Pierce_lvl70,
    AttrInfo_Counter_lvl70,
    AttrInfo_MagRes_lvl70
}



--80级装备属性数值
local AttrInfo_HP_lvl80 =       {Blue_HP_value8,       Purple_HP_value5,       Orange_HP_value5}
local AttrInfo_Hitrate_lvl80 =  {Blue_Hitrate_value2,  Purple_Hitrate_value2,  Orange_Hitrate_value1}
local AttrInfo_Evade_lvl80 =    {Blue_Evad_value1,     Purple_Evad_value1,     Orange_Evad_value1}
local AttrInfo_Critical_lvl80 = {Blue_Critical_value2, Purple_Critical_value2, Orange_Critical_value1}
local AttrInfo_Pierce_lvl80 =   {Blue_Pierce_value2,   Purple_Pierce_value2,   Orange_Pierce_value1}
local AttrInfo_Action_lvl80 =   {Blue_Action_value1,   Purple_Action_value1,   Orange_Action_value1}
local AttrInfo_Counter_lvl80 =  {Blue_Counter_value1,  Purple_Counter_value1,  Orange_Counter_value1}
local AttrInfo_MagRes_lvl80 =   {Blue_MagRes_value1,   Purple_MagRes_value1,   Orange_MagRes_value1}

local AttrInfo_lvl80 = {
    AttrInfo_HP_lvl80,
    AttrInfo_Action_lvl80,
    AttrInfo_Hitrate_lvl80,
    AttrInfo_Evade_lvl80,
    AttrInfo_Critical_lvl80,
    AttrInfo_Pierce_lvl80,
    AttrInfo_Counter_lvl80,
    AttrInfo_MagRes_lvl80
}



--90级装备属性数值
local AttrInfo_HP_lvl90 =       {Blue_HP_value8,       Purple_HP_value6,       Orange_HP_value6}
local AttrInfo_Hitrate_lvl90 =  {Blue_Hitrate_value2,  Purple_Hitrate_value2,  Orange_Hitrate_value1}
local AttrInfo_Evade_lvl90 =    {Blue_Evad_value1,     Purple_Evad_value1,     Orange_Evad_value1}
local AttrInfo_Critical_lvl90 = {Blue_Critical_value2, Purple_Critical_value2, Orange_Critical_value1}
local AttrInfo_Pierce_lvl90 =   {Blue_Pierce_value2,   Purple_Pierce_value2,   Orange_Pierce_value1}
local AttrInfo_Action_lvl90 =   {Blue_Action_value1,   Purple_Action_value1,   Orange_Action_value1}
local AttrInfo_Counter_lvl90 =  {Blue_Counter_value1,  Purple_Counter_value1,  Orange_Counter_value1}
local AttrInfo_MagRes_lvl90 =   {Blue_MagRes_value1,   Purple_MagRes_value1,   Orange_MagRes_value1}

local AttrInfo_lvl90 = {
    AttrInfo_HP_lvl90,
    AttrInfo_Action_lvl90,
    AttrInfo_Hitrate_lvl90,
    AttrInfo_Evade_lvl90,
    AttrInfo_Critical_lvl90,
    AttrInfo_Pierce_lvl90,
    AttrInfo_Counter_lvl90,
    AttrInfo_MagRes_lvl90
}



--100级装备属性数值
local AttrInfo_HP_lvl100 =       {Blue_HP_value8,       Purple_HP_value6,       Orange_HP_value7}
local AttrInfo_Hitrate_lvl100 =  {Blue_Hitrate_value2,  Purple_Hitrate_value2,  Orange_Hitrate_value1}
local AttrInfo_Evade_lvl100 =    {Blue_Evad_value1,     Purple_Evad_value1,     Orange_Evad_value1}
local AttrInfo_Critical_lvl100 = {Blue_Critical_value2, Purple_Critical_value2, Orange_Critical_value1}
local AttrInfo_Pierce_lvl100 =   {Blue_Pierce_value2,   Purple_Pierce_value2,   Orange_Pierce_value1}
local AttrInfo_Action_lvl100 =   {Blue_Action_value1,   Purple_Action_value1,   Orange_Action_value1}
local AttrInfo_Counter_lvl100 =  {Blue_Counter_value1,  Purple_Counter_value1,  Orange_Counter_value1}
local AttrInfo_MagRes_lvl100 =   {Blue_MagRes_value1,   Purple_MagRes_value1,   Orange_MagRes_value1}

local AttrInfo_lvl100 = {
    AttrInfo_HP_lvl100,
    AttrInfo_Action_lvl100,
    AttrInfo_Hitrate_lvl100,
    AttrInfo_Evade_lvl100,
    AttrInfo_Critical_lvl100,
    AttrInfo_Pierce_lvl100,
    AttrInfo_Counter_lvl100,
    AttrInfo_MagRes_lvl100
}




--装备属性数值
local AttrInfo = {
    AttrInfo_lvl0,
    AttrInfo_lvl10,
    AttrInfo_lvl20,
    AttrInfo_lvl30,
    AttrInfo_lvl40,
    AttrInfo_lvl50,
    AttrInfo_lvl60,
    AttrInfo_lvl70,
    AttrInfo_lvl80,
    AttrInfo_lvl90,
    AttrInfo_lvl100
}








function getEnchantCost()
    return Enchant_cost
end

function getMergeCost()
    return Merge_cost
end

function getDetachCost()
    return Detach_cost
end

function getSplitCost()
    return Split_cost
end

function getForgeCost()
    return Forge_cost
end

function getSplitChance(q)
    return Split_chance[q]
end

function getMergeChance()
    return Merge_chance
end

function getEnchantChance(q)
    return Enchant_chance[q]
end

function getEnchantMax()
    return Enchant_max
end

function getAttrChance(l, q)
    return AttrChances[l][q]
end

function getAttrInfo(l, t, q)
    return AttrInfo[l][t][q]
end

function getSocketChance()
    return Split_chance
end





