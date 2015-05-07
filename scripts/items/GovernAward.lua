local Govern_item =
{
	{ 1, 10000,2000},    --¿¿  ¿¿100%¿¿¿   ¿20%¿¿¿¿¿¿¿
	{ 2, 8000, 1000},    --¿¿  ¿¿80%¿¿¿    ¿10%¿¿¿¿¿¿¿
    { 3, 6000,  0},      --¿¿  ¿¿60%¿¿¿    ¿¿¿¿¿
	{ 4, 4000,  0},      --¿¿  ¿¿40%¿¿¿    ¿¿¿¿¿
	{ 5, 0, 0}           --¿¿  ¿¿¿¿¿       ¿¿¿¿¿
}



local Govern_item =
{
	[1] = { 1, 10000,2000},    
	[2] = { 2, 8000, 1000},
    [3] = { 3, 6000,  0},
	[4] = { 4, 4000,  0},
	[5] = { 5, 0,     0}
}


local Govern_monster =
{
    [1] = {1,2,3,4,5},
    [2] = {6,7,8,9,10},
    [3] = {11,12,13,14,15},
}


local monster_appear =
{
    [1] = {2000,4000,6000,8000,1000},
    [2] = {2000,4000,6000,8000,1000},
    [3] = {2000,4000,6000,8000,1000}
}

function RandMonster(group)
    local random = math.random(0,10000);
    for i=1,#monster_appear do
       if random < monster_appear[group][i]  then
          return Govern_monster[group][i] 
       end
   end
end


function GetGovernDropMoney(res)
	return Govern_item[res][2]
end

function GetGovernDropItem(res)
    return Govern_item[res][3]
end
