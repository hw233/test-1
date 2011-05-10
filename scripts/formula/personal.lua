---系数表
str_factor  = {2.50,      2.50,      1.50,      1.50,      2.50,       2.50 }
agi_factor  = {1.50,      1.50,      4.00,      4.00,      2.50,       2.50 }
int_factor  = {1.00,      1.00,      1.00,      1.00,      1.30,       1.30 }
phy_factor  = {4.00,      4.00,      2.50,      2.50,      2.70,       2.70 }

str_m_factor  = {2.75,      2.75,      2.00,      2.00,      3.00,       3.00 }
agi_m_factor  = {1.50,      1.50,      4.25,      4.25,      2.70,       2.70 }
int_m_factor  = {1.00,      1.00,      1.00,      1.00,      1.30,       1.30 }
phy_m_factor  = {4.50,      4.50,      2.50,      2.50,      3.00,       3.00 }

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
  if fgt:getId() < 7 then
    return (str + pot * str_m_factor[cls] * lvl) * (1 + fgt:getExtraStrengthPercent()) + fgt:getExtraStrength()
  end
  return (str + pot * str_factor[cls] * lvl) * (1 + fgt:getExtraStrengthPercent()) + fgt:getExtraStrength()
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