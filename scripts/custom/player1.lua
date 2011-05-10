names = {"0", "1", "2", "3", "4"}
for i = 1, 5 do
  fighter = obj[i]:getFighter()
  fighter:setName(names[i])
  fighter:setStrength(math.random(50, 80))
  fighter:setAgility(math.random(50, 80))
  fighter:setLuck(math.random(50, 80))
  fighter:setIntelligence(math.random(50, 80))
  fighter:setCorporeity(math.random(50, 80))
  fighter:setClass(math.random(5))
  fighter:setWeapon(weaponManager[math.random(1, 7)])
end

battleField:putObject(curSide, 1, 1, obj[1])
battleField:putObject(curSide, 3, 1, obj[2])
battleField:putObject(curSide, 2, 2, obj[3])
battleField:putObject(curSide, 1, 3, obj[4])
battleField:putObject(curSide, 3, 3, obj[5])
