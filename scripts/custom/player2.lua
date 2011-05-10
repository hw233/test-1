for i = 1, 5 do
  fighter = obj[i]:getFighter()
  fighter:setName(tostring(i + 4))
  fighter:setStrength(math.random(50, 80))
  fighter:setAgility(math.random(50, 80))
  fighter:setLuck(math.random(50, 80))
  fighter:setIntelligence(math.random(50, 80))
  fighter:setCorporeity(math.random(50, 80))
  fighter:setClass(math.random(5))
  fighter:setWeapon(weaponManager[math.random(1, 7)])
end

for i = 1, 5 do
  repeat
    x = math.random(4)
    y = math.random(4)
  until battleField:getObject(curSide, x, y) == nil
  battleField:putObject(curSide, x, y, obj[i])
end
