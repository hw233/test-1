evdlvl_factor = 10
evdlvl_addon_factor = 800
hitrlvl_factor = 10
hitrlvl_addon_factor = 800
crilvl_factor = 10
crilvl_addon_factor = 800
pirlvl_factor = 10
pirlvl_addon_factor = 800
toughlvl_factor = 10
toughlvl_addon_factor = 800
mreslvl_factor = 10
mreslvl_addon_factor = 800
counterlvl_factor = 10
counterlvl_addon_factor = 800

deflvl_factor = 250
deflvl_addon_factor = 1500

--暴击抗性
criticaldeflvl_factor = 30
criticaldeflvl_addon_factor = 9500
--破击抗性
piercedeflvl_factor = 30
piercedeflvl_addon_factor = 9500
--反击抗性
counterdeflvl_factor = 30
counterdeflvl_addon_factor = 9500
--攻击穿透
attackpiercelvl_factor = 30
attackpiercelvl_addon_factor = 9500

hitrate_max = 160
evade_max = 65
critical_max = 80
pierce_max = 80
tough_max = 80
counter_max = 65
magres_max = 75
critical_damage_max = 3


function getEvadeFactor()
    return evdlvl_factor
end

function getHitrateFactor()
    return hitrlvl_factor
end

function getCriticalFactor()
    return crilvl_factor
end

function getPierceFactor()
    return pirlvl_factor
end

function getToughFactor()
    return toughlvl_factor
end

function getHitrateMax()
    return hitrate_max
end

function getEvadeMax()
    return evade_max
end

function getCriticalMax()
    return critical_max
end

function getPierceMax()
    return pierce_max
end

function getToughMax()
    return tough_max
end

function getCounterMax()
    return counter_max
end 

function getMagResMax()
    return magres_max
end

function getCriticalDmgMax()
    return critical_damage_max
end
