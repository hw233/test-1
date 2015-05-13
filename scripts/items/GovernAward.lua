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
	[1] = { 1, 10000,10},    
	[2] = { 2, 8000, 50},
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
    [1] = {2200,4400,6600,7800,10000},
    [2] = {2200,4400,6600,7800,10000},
    [3] = {2200,4400,6600,7800,10000}
}

function RandMonster(group)
    local random = math.random(1,10000);
    index = 1
    for i=1,#monster_appear[group] do
       if random <= monster_appear[group][i]  then
           index = i
           break
       end
    end
    print(group..index)
    return Govern_monster[group][index];
end


function GetGovernDropMoney(res)
	return Govern_item[res][2]
end

function GetGovernDropItem(res)
    return Govern_item[res][3]
end
