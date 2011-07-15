---系数表
-- TODO: 

-- 属性成长不分主副将
--           儒     释      道
str_factor = { 2,    3,      4}  -- 力量
phy_factor = {10,   11,     12}  -- 耐力
agi_factor = { 2,    2,      4}  -- 敏捷
int_factor = { 5,    4,      2}  -- 智力
wil_factor = { 2,    4,      2}  -- 意志

hp_factor     = { 0,   0,   0}  -- 生命
atk_factor    = { 0,   0,   0}  -- 物功
def_factor    = { 0,   0,   0}  -- 物防
magatk_factor = { 0,   0,   0}  -- 法功
magdef_factor = { 0,   0,   0}  -- 法防

tough_factor       = { 0,  0,  0}  -- 坚韧
action_factor      = { 1,  1,  1}  -- 身法
hitrate_factor     = { 0,  0,  0}  -- 命中
evade_factor       = { 0,  0,  0}  -- 闪避
critical_factor    = { 0,  0,  0}  -- 暴击
criticaldmg_factor = { 0,  0,  0}  -- 暴击伤害
pierce_factor      = { 0,  0,  0}  -- 破击
counter_factor     = { 0,  0,  0}  -- 反击
magres_factor      = { 0,  0,  0}  -- 法术抵抗

str_atk_factor = 4.0            -- 力量影响的物理攻击系数
int_atk_factor = 4.0            -- 智力影响的法术攻击系数
agi_def_factor = 2.0            -- 敏捷影响的物理防御系数
phy_hp_factor  = 8.0            -- 耐力影响的生命值系数
wil_def_factor = 2.0            -- 意志影响的法术防御系数

str_cnt_factor = 0.02
agi_act_factor = 1.0
agi_evd_factor = 0.02
phy_def_factor = 2
int_hit_factor = 0.02


autobattle_tweak = 0.5
autobattle_A = 2.5

-- 辅助函数
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
  local cls = fgt:getClass()
  local lvl = fgt:getLevel() - 1
  local act = fgt:getBaseAction()
  return act + action_factor[cls] * lvl + fgt:getExtraAction();
end

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

function calcHitrate( fgt )
  if fgt == nil then
    return 0
  end
  local cls = fgt:getClass()
  local hitr = fgt:getBaseHitrate()
  return hitr + fgt:getExtraHitrate()
end

function calcEvade( fgt )
  if fgt == nil then
    return 0
  end
  local cls = fgt:getClass()
  local evd = fgt:getBaseEvade()
  return evd + fgt:getExtraEvade()
end

function calcCritical( fgt )
  if fgt == nil then
    return 0
  end
  local cls = fgt:getClass()
  local cri = fgt:getBaseCritical()
  return cri + fgt:getExtraCritical()
end

function calcCounter( fgt )
  if fgt == nil then
    return 0
  end
  local cls = fgt:getClass()
  local cnt = fgt:getBaseCounter()
  return cnt + fgt:getExtraCounter()
end

function calcPierce( fgt )
  if fgt == nil then
    return 0
  end
  local cls = fgt:getClass()
  local prc = fgt:getBasePierce()
  return prc + fgt:getExtraPierce()
end

function calcDamage( atk, def )
    if atk < def + 1 then
        return 1;
    end

  return atk - def
end

function calcCriticalDmg( fgt )
  if fgt == nil then
    return 0
  end
  local cls = fgt:getClass()
  local cri = fgt:getBaseCriticalDmg()
  return cri + fgt:getExtraCriticalDmg()
end

function calcMagRes( fgt )
    if fgt == nil then
        return 0
    end
    local cls = fgt:getClass()
    local magres = fgt:getBaseMagRes()
    return magres + fgt:getExtraMagRes()
end

function calcSoul( fgt )
    if fgt == nil then
        return 0
    end
    local cls = fgt:getClass()
    local soul = fgt:getBaseSoul()
    return soul + fgt:getExtraSoul()
end

function calcTough( fgt )
    if fgt == nil then
        return 0
    end
    local cls = fgt:getClass(0)
    local tough = fgt:getBaseTough()
    return tough + fgt:getExtraTough()
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
      local atk = ((fgt:getBaseAttack() + atk_factor * lvl) * (1 + str_atk_factor * str / 100)) * (40 + lvl) / 120
      local crt = fgt:getBaseCritical()
      local hit = fgt:getBaseHitrate() + int_hit_factor * intr
      local act = fgt:getBaseAction() / (1 + agi_act_factor * agi / 100)
      return atk * (1 + 0.005 * crt) * hit / act
    end
  end
  return calcAttack(fgt) * (1 + 0.005 * calcCritical(fgt)) * calcHitrate(fgt) / calcAction(fgt)
end

function calcAutoBattle( mybp, theirbp )
  if mybp >= theirbp then
    return (theirbp + (autobattle_A - 1) * mybp) / (autobattle_A * mybp)
  end
  return 1 + autobattle_tweak - autobattle_tweak * mybp / theirbp
end
