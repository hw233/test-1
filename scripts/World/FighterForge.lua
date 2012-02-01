
-- 洗到某一属性的概率
local attrTypeChance =
{
    -- 力量 耐力 敏捷 智力 意志 坚韧 身法 命中值 闪避值 暴击值 暴击伤害 破击值 反击 法术抵抗
    714,1428,2142,2856,3570,4284,4998,5712,6426,7140,7854,8568,9282,10000
}

-- 属性区间的概率
local attrChance = {3950, 6450, 7950, 8950, 9450, 9850, 9950, 10000}
-- 各区间最大值(百分比)
local attrMaxValProp = {20, 25, 50, 60, 70, 80, 90, 96, 100}
-- 各属性的最大值(百分比)
local attrMaxVal = {25,30,30,25,30,4,15,4,4,4,6,4,4,4}

function loadFighterForge()
    for k,v in pairs(attrTypeChance)
    do
        setFFTypeChance(v)
    end
    for k,v in pairs(attrChance)
    do
        setFFAttrChance(v);
    end
    for k,v in pairs(attrMaxValProp)
    do
        setFFAttrMaxValProp(v);
    end
    for k,v in pairs(attrMaxVal)
    do
        setFFAttrMaxVal(v);
    end
end

loadFighterForge()

