---系数表
-- TODO: 

-- 属性成长不分主副将
--           儒     释      道
str_factor = {0,    0,      3}  -- 力量
phy_factor = {1.5,  1.8,    2}  -- 耐力
agi_factor = {0,    0.8,    1}  -- 敏捷
int_factor = {3.5,  2.5,    0}  -- 智力
wil_factor = {1,    1,      0}  -- 意志

str_atk_factor = 1.0
str_cnt_factor = 0.02
agi_act_factor = 1.0
agi_evd_factor = 0.02
phy_hp_factor  = 1.0
phy_def_factor = 2
int_hit_factor = 0.02

atk_factor = 6
hp_factor  = 15
def_factor = 4000

autobattle_tweak = 0.5
autobattle_A = 2.5

-- 辅助函数
function calcStrength( fgt )
  if fgt == nil then
    return 0
  end
  local cls = fgt:getClass()
  local str = fgt:getBaseStrength()
  local pot = fgt:getPotential()
  local lvl = fgt:getLevel()
  return str + str_factor[cls] * pot;
end

function calcPhysique( fgt )
  if fgt == nil then
    return 0
  end
  local cls = fgt:getClass()
  local phy = fgt:getBasePhysique()
  local pot = fgt:getPotential()
  local lvl = fgt:getLevel()
  if fgt:getId() < 7 then
    return (phy + pot * phy_m_factor[cls] * lvl) * (1 + fgt:getExtraPhysiquePercent()) + fgt:getExtraPhysique()
  end
  return (phy + pot * phy_factor[cls] * lvl) * (1 + fgt:getExtraPhysiquePercent()) + fgt:getExtraPhysique()
end

function calcAgility( fgt )
  if fgt == nil then
    return 0
  end
  local cls = fgt:getClass()
  local agi = fgt:getBaseAgility()
  local pot = fgt:getPotential()
  local lvl = fgt:getLevel()
  if fgt:getId() < 7 then
    return (agi + pot * agi_m_factor[cls] * lvl) * (1 + fgt:getExtraAgilityPercent()) + fgt:getExtraAgility()
  end
  return (agi + pot * agi_factor[cls] * lvl) * (1 + fgt:getExtraAgilityPercent()) + fgt:getExtraAgility()
end

function calcIntelligence( fgt )
  if fgt == nil then
    return 0
  end
  local cls = fgt:getClass()
  local int = fgt:getBaseIntelligence()
  local pot = fgt:getPotential()
  local lvl = fgt:getLevel()
  if fgt:getId() < 7 then
    return (int + pot * int_m_factor[cls] * lvl) * (1 + fgt:getExtraIntelligencePercent()) + fgt:getExtraIntelligence()
  end
  return (int + pot * int_factor[cls] * lvl) * (1 + fgt:getExtraIntelligencePercent()) + fgt:getExtraIntelligence()
end

function calcHP( fgt )
  if fgt == nil then
    return 0
  end
  local cls = fgt:getClass()
  local hp = fgt:getBaseHP()
  local lvl = fgt:getLevel()
  return (hp + hp_factor * lvl) * (1 + phy_hp_factor * calcPhysique(fgt) / 100) * (1 + fgt:getExtraHPPercent()) + fgt:getExtraHP()
end

function calcAction( fgt )
  if fgt == nil then
    return 0
  end
  local cls = fgt:getClass()
  local act = fgt:getBaseAction()
  return act / (1 + agi_act_factor * calcAgility(fgt) / 100) / (1 + fgt:getExtraAction())
end

function calcAttack( fgt )
  if fgt == nil then
    return 0
  end
  local cls = fgt:getClass()
  local atk = fgt:getBaseAttack()
  local lvl = fgt:getLevel()
  if fgt:isNpc() then
    return ((atk + atk_factor * lvl) * (1 + str_atk_factor * calcStrength(fgt) / 100) * (1 + fgt:getExtraAttackPercent()) + fgt:getExtraAttack()) * (40 + lvl) / 120
  end
  return (atk + atk_factor * lvl) * (1 + str_atk_factor * calcStrength(fgt) / 100) * (1 + fgt:getExtraAttackPercent()) + fgt:getExtraAttack()
end

function calcDefend( fgt )
  if fgt == nil then
    return 0
  end
  local cls = fgt:getClass()
  local def = fgt:getBaseDefend()
  return (def + phy_def_factor * calcPhysique(fgt)) * (1 + fgt:getExtraDefendPercent()) + fgt:getExtraDefend()
end

function calcHitrate( fgt )
  if fgt == nil then
    return 0
  end
  local cls = fgt:getClass()
  local hitr = fgt:getBaseHitrate()
  return hitr + int_hit_factor * calcIntelligence(fgt) + fgt:getExtraHitrate()
end

function calcEvade( fgt )
  if fgt == nil then
    return 0
  end
  local cls = fgt:getClass()
  local evd = fgt:getBaseEvade()
  return evd + agi_evd_factor * calcAgility(fgt) + fgt:getExtraEvade()
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
  return cnt + str_cnt_factor * calcStrength(fgt) + fgt:getExtraCounter()
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
  return atk * def_factor / (def + def_factor)
end

function calcBattlePoint( fgt )
  if fgt == nil then
    return 0
  end
  if fgt:isNpc() then
    local lvl = fgt:getLevel()
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
