def_single = {0, 1.0, false}
def_cross = {0, 1.0, false, -1, 0.5, true, 1, 1.0, true, -5, 1.0, false, 5, 1.0, false}
def_line = {0, 1.0, false, 5, 0.5, false, 10, 0.5, false, 15, 0.5, false, 20, 0.5, false}
def_row = {0, 1.0, false, 1, 1.0, true, 2, 1.0, true, 3, 1.0, true, 4, 1.0, true, -1, 1.0, true, -2, 1.0, true, -3, 1.0, true, -4, 1.0, true}
def_double = {0, 1.0, false, 5, 0.5, false}

addWeaponDef(weapon.Sword, "Sword", 2, 1, def_single)
addWeaponDef(weapon.Hammer, "Hammer", 1, 5, def_cross)
addWeaponDef(weapon.Spear, "Spear", 2, 5, def_line)
addWeaponDef(weapon.Whip, "Whip", 2, 9, def_row)
addWeaponDef(weapon.Bow, "Bow", 6, 1, def_single)
addWeaponDef(weapon.Throwing, "Throwing", 4, 2, def_double)
addWeaponDef(weapon.Fan, "Fan", 3, 1, def_single)
