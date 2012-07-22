---系数表
-- TODO: 

-- 属性成长不分主副将
--           儒     释      道
str_factor = {4,   5,     7}  -- 力量
phy_factor = {8,   9,     10}  -- 耐力
agi_factor = {5,   4,     7}  -- 敏捷
int_factor = {8,   7,     4}  -- 智力
wil_factor = {5,   9,     5}  -- 意志

hp_factor     = { 0,   0,   0}  -- 生命
atk_factor    = { 0,   0,   0}  -- 物功
def_factor    = { 0,   0,   0}  -- 物防
magatk_factor = { 0,   0,   0}  -- 法功
magdef_factor = { 0,   0,   0}  -- 法防

tough_factor       = { 0,  0,  0}  -- 坚韧
action_factor      = {11, 12, 10}  -- 身法
hitrate_factor     = { 1,  1,  1}  -- 命中
evade_factor       = { 0,  0,  0}  -- 闪避
critical_factor    = { 0,  0,  0}  -- 暴击
criticaldmg_factor = { 0,  0,  0}  -- 暴击伤害
pierce_factor      = { 0,  0,  0}  -- 破击
counter_factor     = { 0,  0,  0}  -- 反击
magres_factor      = { 0,  0,  0}  -- 法术抵抗

str_atk_factor = 1.0            -- 力量影响的物理攻击系数
int_atk_factor = 1.0            -- 智力影响的法术攻击系数
agi_def_factor = 1.0            -- 敏捷影响的物理防御系数
phy_hp_factor  = 5.0            -- 耐力影响的生命值系数
wil_def_factor = 1.0            -- 意志影响的法术防御系数

str_cri_factor = 0.1            -- 力量影响的暴击等级系数
wil_mres_factor = 0.1           -- 意志影响的法术抵抗等级系数
agi_evd_factor = 0.1            -- 敏捷影响的闪避等级系数
int_pri_factor = 0.1            -- 智力影响的破击等级系数


str_cnt_factor = 0.02
agi_act_factor = 1.0
--agi_evd_factor = 0.02
phy_def_factor = 2
int_hit_factor = 0.02


autobattle_tweak = 0.5
autobattle_A = 2.5


-- 第二元神属性成长
--           1青龙 2朱雀 3玄武 4狂雷•青龙 5烈阳•青龙 6雨泽•青龙 7炽炎•朱雀 8羽焰•朱雀 9浴火•朱雀 10冰锋•玄武 11凛风•玄武 12水盾•玄武 
soul_str_factor = {2, 3, 5, 3, 5, 3, 4, 4, 4, 8, 6, 6}
soul_phy_factor = {6, 7, 8, 7, 7, 9, 8, 8, 10,9, 9, 11}
soul_agi_factor = {3, 2, 5, 4, 4, 4, 3, 3, 3, 6, 8, 6}
soul_int_factor = {6, 5, 2, 9, 7, 7, 8, 6, 6, 3, 3, 3}
soul_wil_factor = {3, 6, 3, 4, 4, 4, 7, 9, 7, 4, 4, 4}

-- 1角宿：攻击 2翼宿：身法 3斗宿：防御 4奎宿：生命
soul_xinxiu_attack = {3, 0, 0, 0}
soul_xinxiu_action = {0, 3, 0, 0}
soul_xinxiu_defend = {0, 0, 10,0}
soul_xinxiu_hp =     {0, 0, 0, 10}

-- 元神强度对应潜力表

soul_potential = {1,1.1,1.2,1.3,1.4,1.5,1.6,1.7,1.8,2}


function calcSoulStrength( ss )
  if ss == nil then
    return 0
  end

  local cls = ss:getClass()
  local stlvl = ss:getStateLevel()
  local lvl = ss:getPracticeLevel()
  return soul_potential[stlvl] * soul_str_factor[cls] * lvl
end

function calcSoulAgility( ss )
  if ss == nil then
    return 0
  end

  local cls = ss:getClass()
  local stlvl = ss:getStateLevel()
  local lvl = ss:getPracticeLevel()
  return soul_potential[stlvl] * soul_agi_factor[cls] * lvl
end

function calcSoulPhysique( ss )
  if ss == nil then
    return 0
  end

  local cls = ss:getClass()
  local stlvl = ss:getStateLevel()
  local lvl = ss:getPracticeLevel()
  return soul_potential[stlvl] * soul_phy_factor[cls] * lvl
end

function calcSoulIntelligence( ss )
  if ss == nil then
    return 0
  end

  local cls = ss:getClass()
  local stlvl = ss:getStateLevel()
  local lvl = ss:getPracticeLevel()
  return soul_potential[stlvl] * soul_int_factor[cls] * lvl
end

function calcSoulWill( ss )
  if ss == nil then
    return 0
  end

  local cls = ss:getClass()
  local stlvl = ss:getStateLevel()
  local lvl = ss:getPracticeLevel()
  return soul_potential[stlvl] * soul_wil_factor[cls] * lvl
end

function calcSoulXinxiuAttack( ss )
  if ss == nil then
    return 0
  end

  local xinxiu = ss:getXinxiu()
  if xinxiu == 0 then
      return 0
  end
  local stlvl = ss:getStateLevel()
  local lvl = ss:getPracticeLevel()
  return soul_potential[stlvl] * soul_xinxiu_attack[xinxiu] * lvl
end

function calcSoulXinxiuAction( ss )
  if ss == nil then
    return 0
  end

  local xinxiu = ss:getXinxiu()
  if xinxiu == 0 then
      return 0
  end
  local stlvl = ss:getStateLevel()
  local lvl = ss:getPracticeLevel()
  return soul_potential[stlvl] * soul_xinxiu_action[xinxiu] * lvl
end

function calcSoulXinxiuDefend( ss )
  if ss == nil then
    return 0
  end

  local xinxiu = ss:getXinxiu()
  if xinxiu == 0 then
      return 0
  end
  local stlvl = ss:getStateLevel()
  local lvl = ss:getPracticeLevel()
  return soul_potential[stlvl] * soul_xinxiu_defend[xinxiu] * lvl
end

function calcSoulXinxiuHp( ss )
  if ss == nil then
    return 0
  end

  local xinxiu = ss:getXinxiu()
  if xinxiu == 0 then
      return 0
  end
  local stlvl = ss:getStateLevel()
  local lvl = ss:getPracticeLevel()
  return soul_potential[stlvl] * soul_xinxiu_hp[xinxiu] * lvl
end




-- 辅助函数
-- 基础属性
function calcAura( fgt )
  if fgt == nil then
    return 0
  end
  local aura = fgt:getBaseAura()
  return aura * (1 + fgt:getExtraAuraPercent()) + fgt:getExtraAura()
end

function calcAuraMax( fgt )
  if fgt == nil then
    return 0
  end
  local maxAura = fgt:getBaseAuraMax()
  return maxAura * (1 + fgt:getExtraAuraMaxPercent()) + fgt:getExtraAuraMax()
end

function calcStrength( fgt )
  if fgt == nil then
    return 0
  end
  local str = fgt:getBaseStrength()
  if fgt:isNpc() then
      return str * (1 + fgt:getExtraStrengthPercent()) + fgt:getExtraStrength()
  end
  local cls = fgt:getClass()
  local pot = fgt:getPotential()
  local lvl = fgt:getLevel() - 1
  return (str + pot * str_factor[cls] * lvl) * (1 + fgt:getExtraStrengthPercent()) + fgt:getExtraStrength()
end

function calcPhysique( fgt )
  if fgt == nil then
    return 0
  end
  local phy = fgt:getBasePhysique()
  if fgt:isNpc() then
      return phy * (1 + fgt:getExtraPhysiquePercent()) + fgt:getExtraPhysique()
  end
  local cls = fgt:getClass()
  local pot = fgt:getPotential()
  local lvl = fgt:getLevel() - 1
  return (phy + pot * phy_factor[cls] * lvl) * (1 + fgt:getExtraPhysiquePercent()) + fgt:getExtraPhysique()
end

function calcAgility( fgt )
  if fgt == nil then
    return 0
  end
  local agi = fgt:getBaseAgility()
  if fgt:isNpc() then
      return agi * (1 + fgt:getExtraAgilityPercent()) + fgt:getExtraAgility()
  end
  local cls = fgt:getClass()
  local pot = fgt:getPotential()
  local lvl = fgt:getLevel() - 1
  return (agi + pot * agi_factor[cls] * lvl) * (1 + fgt:getExtraAgilityPercent()) + fgt:getExtraAgility()
end

function calcIntelligence( fgt )
  if fgt == nil then
    return 0
  end
  local int = fgt:getBaseIntelligence()
  if fgt:isNpc() then
      return int * (1 + fgt:getExtraIntelligencePercent()) + fgt:getExtraIntelligence()
  end
  local cls = fgt:getClass()
  local pot = fgt:getPotential()
  local lvl = fgt:getLevel() - 1
  return (int + pot * int_factor[cls] * lvl) * (1 + fgt:getExtraIntelligencePercent()) + fgt:getExtraIntelligence()
end

function calcWill( fgt )
  if fgt == nil then
    return 0
  end
  local wil = fgt:getBaseWill()
  if fgt:isNpc() then
      return wil * (1 + fgt:getExtraWillPercent()) + fgt:getExtraWill()
  end
  local cls = fgt:getClass()
  local pot = fgt:getPotential()
  local lvl = fgt:getLevel() - 1
  return (wil + pot * wil_factor[cls] * lvl) * (1 + fgt:getExtraWillPercent()) + fgt:getExtraWill()
end


function calcHP( fgt )
  if fgt == nil then
    return 0
  end
  local hp = fgt:getBaseHP()
  if fgt:isNpc() then
      return hp * (1 + fgt:getExtraHPPercent()) + fgt:getExtraHP()
  end
  local cls = fgt:getClass()
  local lvl = fgt:getLevel() - 1
  return (hp + hp_factor[cls] * lvl + phy_hp_factor * calcPhysique(fgt)) * (1 + fgt:getExtraHPPercent()) + fgt:getExtraHP()
end

function calcAction( fgt )
  if fgt == nil then
    return 0
  end
  local act = fgt:getBaseAction()
  if fgt:isNpc() then
      return act + fgt:getExtraAction()
  end
  local cls = fgt:getClass()
  local lvl = fgt:getLevel() - 1
  return (act + action_factor[cls] * lvl) * (1 + fgt:getExtraActionPercent()) + fgt:getExtraAction();
end


--暴击，破击，闪避，命中，反击，坚韧，法术抵抗等级
function calcCriticalLevel( fgt )
  if fgt == nil then
    return 0
  end
  local cri = fgt:getExtraCriticalLevel()
  return cri + str_cri_factor * calcStrength(fgt)
end

function calcPierceLevel( fgt )
    if fgt == nil then
        return 0
    end
    local pir = fgt:getExtraPierceLevel()
    return pir + int_pri_factor * calcIntelligence(fgt)
end

function calcEvadeLevel( fgt )
    if fgt == nil then
        return 0
    end
    local evd = fgt:getExtraEvadeLevel()
    return evd + agi_evd_factor * calcAgility(fgt)
end

function calcMagResLevel( fgt )
    if fgt == nil then
        return 0
    end
    local mres = fgt:getExtraMagResLevel()
    return mres + wil_mres_factor * calcWill(fgt)
end

function calcHitRateLevel( fgt )
    if fgt == nil then
        return 0
    end
    local hrate = fgt:getExtraHitrateLevel()
    local cls = fgt:getClass()
    local lvl = fgt:getLevel() - 1
    return hrate + hitrate_factor[cls] * lvl
end

function calcToughLevel( fgt )
    if fgt == nil then
        return 0
    end
    local tough = fgt:getExtraToughLevel()
    return tough
end

function calcCounterLevel( fgt )
    if fgt == nil then
        return 0
    end
    local counter = fgt:getExtraCounterLevel()
    return counter
end


--攻击，防御，概率
function calcAttack( fgt )
  if fgt == nil then
    return 0
  end
  local atk = fgt:getBaseAttack()
  if fgt:isNpc() then
      return atk * (1 + fgt:getExtraAttackPercent()) + fgt:getExtraAttack()
  end
  local cls = fgt:getClass()
  local lvl = fgt:getLevel() - 1
  return ((atk + atk_factor[cls] * lvl + str_atk_factor * calcStrength(fgt)) * (1 + fgt:getExtraAttackPercent()) + fgt:getExtraAttack())
end

function calcMagAttack( fgt )
  if fgt == nil then
    return 0
  end
  local magatk = fgt:getBaseMagAttack()
  if fgt:isNpc() then
      return magatk * (1 + fgt:getExtraMagAttackPercent()) + fgt:getExtraMagAttack()
  end
  local cls = fgt:getClass()
  local lvl = fgt:getLevel() - 1
  return ((magatk + magatk_factor[cls] * lvl + int_atk_factor * calcIntelligence(fgt)) * (1 + fgt:getExtraMagAttackPercent()) + fgt:getExtraMagAttack())
end

function calcDefend( fgt )
  if fgt == nil then
    return 0
  end
  local def = fgt:getBaseDefend()
  if fgt:isNpc() then
      return def * (1 + fgt:getExtraDefendPercent()) + fgt:getExtraDefend()
  end
  local cls = fgt:getClass()
  local lvl = fgt:getLevel() - 1
  return (def + def_factor[cls] * lvl + agi_def_factor * calcAgility(fgt)) * (1 + fgt:getExtraDefendPercent()) + fgt:getExtraDefend()
end

function calcMagDefend( fgt )
  if fgt == nil then
    return 0
  end
  local def = fgt:getBaseMagDefend()
  if fgt:isNpc() then
      return def * (1 + fgt:getExtraMagDefendPercent()) + fgt:getExtraMagDefend()
  end
  local cls = fgt:getClass()
  local lvl = fgt:getLevel() - 1
  return (def + magdef_factor[cls] * lvl + wil_def_factor * calcWill(fgt)) * (1 + fgt:getExtraMagDefendPercent()) + fgt:getExtraMagDefend()
end

function calcHitrate( fgt, defgt )
  if fgt == nil then
    return 0
  end
  local deflev = fgt:getLevel();
  if defgt ~= nil then
      deflev = defgt:getLevel();
  end
  local cls = fgt:getClass()
  local hitr = fgt:getBaseHitrate()
  local hitrlvl = calcHitRateLevel(fgt)
  return hitr + fgt:getExtraHitrate() + hitrlvl/(hitrlvl + hitrlvl_factor*deflev + hitrlvl_addon_factor)*100
end

function calcEvade( fgt, defgt )
  if fgt == nil then
    return 0
  end
  local deflev = fgt:getLevel();
  if defgt ~= nil then
      deflev = defgt:getLevel();
  end
  local cls = fgt:getClass()
  local evd = fgt:getBaseEvade()
  local evdlvl = calcEvadeLevel(fgt)
  return evd + fgt:getExtraEvade() + evdlvl/(evdlvl + evdlvl_factor*deflev + evdlvl_addon_factor)*100
end

function calcCritical( fgt, defgt )
  if fgt == nil then
    return 0
  end
  local deflev = fgt:getLevel();
  if defgt ~= nil then
      deflev = defgt:getLevel();
  end
  local cls = fgt:getClass()
  local cri = fgt:getBaseCritical()
  local crilvl = calcCriticalLevel(fgt)
  return cri + fgt:getExtraCritical() + crilvl/(crilvl + crilvl_factor*deflev + crilvl_addon_factor)*100
end

function calcCounter( fgt, defgt )
  if fgt == nil then
    return 0
  end
  local deflev = fgt:getLevel();
  if defgt ~= nil then
      deflev = defgt:getLevel();
  end
  local cls = fgt:getClass()
  local cnt = fgt:getBaseCounter()
  local cntlvl = calcCounterLevel(fgt)
  return cnt + fgt:getExtraCounter() + cntlvl/(cntlvl + counterlvl_factor*deflev + counterlvl_addon_factor)*100
end

function calcPierce( fgt, defgt )
  if fgt == nil then
    return 0
  end
  local deflev = fgt:getLevel();
  if defgt ~= nil then
      deflev = defgt:getLevel();
  end
  local cls = fgt:getClass()
  local prc = fgt:getBasePierce()
  local prclvl = calcPierceLevel(fgt)
  return prc + fgt:getExtraPierce() + prclvl/(prclvl + pirlvl_factor*deflev + pirlvl_addon_factor)*100
end

function calcDamage( atk, def, atklvl, toughFactor, dmgreduce )
    local dmgP = (1 - def/(def + deflvl_factor*atklvl + deflvl_addon_factor) * toughFactor - dmgreduce/100)
    if dmgP < 0.25 then
        dmgP = 0.25
    end
    return atk * dmgP
end

function calcCriticalDmg( fgt )
  if fgt == nil then
    return 0
  end
  local cls = fgt:getClass()
  local cri = fgt:getBaseCriticalDmg()
  return cri + fgt:getExtraCriticalDmg()
end

function calcMagRes( fgt, defgt )
    if fgt == nil then
        return 0
    end
    local deflev = fgt:getLevel();
    if defgt ~= nil then
        deflev = defgt:getLevel();
    end
    local cls = fgt:getClass()
    local magres = fgt:getBaseMagRes()
    local mreslvl = calcMagResLevel(fgt)
    return magres + fgt:getExtraMagRes() + mreslvl/(mreslvl + mreslvl_factor*deflev + mreslvl_addon_factor)*100
end

function calcSoul( fgt )
    if fgt == nil then
        return 0
    end
    local cls = fgt:getClass()
    local soul = fgt:getBaseSoul()
    return soul + fgt:getExtraSoul()
end

function calcTough( fgt, defgt )
    if fgt == nil then
        return 0
    end
    local deflev = fgt:getLevel();
    if defgt ~= nil then
        deflev = defgt:getLevel();
    end
    local cls = fgt:getClass(0)
    local tough = fgt:getBaseTough()
    local toughlvl = calcToughLevel(fgt)
    return tough + fgt:getExtraTough() + toughlvl/(toughlvl + toughlvl_factor*deflev + toughlvl_addon_factor)*100
end

function calcBattlePoint( fgt )
  if fgt == nil then
    return 0
  end
  if fgt:isNpc() then
    local lvl = fgt:getLevel() - 1
    if lvl >= 70 then
      local cls = 1
      local atk = fgt:getBaseAttack()
      local str = fgt:getBaseStrength() + str_factor[cls] * lvl
      local intr = fgt:getBaseIntelligence() + int_factor[cls] * lvl
      local agi = fgt:getBaseAgility() + agi_factor[cls] * lvl
      local atk = ((fgt:getBaseAttack() + atk_factor[cls] * lvl) * (1 + str_atk_factor * str / 100)) * (40 + lvl) / 120
      local crt = fgt:getBaseCritical()
      local hit = fgt:getBaseHitrate() + int_hit_factor * intr
      local act = fgt:getBaseAction() / (1 + agi_act_factor * agi / 100)
      return atk * (1 + 0.005 * crt) * hit / act
    end
  end
  return calcAttack(fgt) * (1 + 0.005 * calcCritical(fgt, nil)) * calcHitrate(fgt, nil) / calcAction(fgt)
end

function calcAutoBattle( mybp, theirbp )
  if mybp >= theirbp then
    return (theirbp + (autobattle_A - 1) * mybp) / (autobattle_A * mybp)
  end
  return 1 + autobattle_tweak - autobattle_tweak * mybp / theirbp
end

