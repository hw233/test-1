function _TryUseToRegen(fighter, itemId, bind, hpEach, maxHP)
  local currHP = fighter:getCurrentHP()
  if currHP == 0 then
    return true
  end
  local hpToFull = maxHP - currHP
  local package = GetPlayer():GetPackage()
  local num = package:GetItemNum(itemId, bind)
  if hpToFull < hpEach * num then
    num = math.floor((hpToFull + hpEach - 1) / hpEach)
  end
  fighter:regenHP(hpEach * num)
  package:DelItem(itemId, num, bind)
  if fighter:getCurrentHP() == 0 then
    return true
  end
  return false
end

function DoAutoRegen(fighter)
  local maxHP = fighter:getMaxHP()
  return _TryUseToRegen(fighter, 8900, 1, 300, maxHP) or _TryUseToRegen(fighter, 8900, 0, 300, maxHP) or _TryUseToRegen(fighter, 8901, 1, 1200, maxHP) or _TryUseToRegen(fighter, 8901, 0, 1200, maxHP) or _TryUseToRegen(fighter, 8902, 1, 1200, maxHP) or _TryUseToRegen(fighter, 8902, 0, 1200, maxHP) or _TryUseToRegen(fighter, 8903, 1, 2000, maxHP) or _TryUseToRegen(fighter, 8903, 0, 2000, maxHP)
end
