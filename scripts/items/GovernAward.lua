local Govern_item =
{
	{ 1, 10000,2000},    --¿¿  ¿¿100%¿¿¿   ¿20%¿¿¿¿¿¿¿
	{ 2, 8000, 1000},    --¿¿  ¿¿80%¿¿¿    ¿10%¿¿¿¿¿¿¿
    { 3, 6000,  0},      --¿¿  ¿¿60%¿¿¿    ¿¿¿¿¿
	{ 4, 4000,  0},      --¿¿  ¿¿40%¿¿¿    ¿¿¿¿¿
	{ 5, 0, 0}           --¿¿  ¿¿¿¿¿       ¿¿¿¿¿
}


function GetGovernDropMoney(res)
	return Govern_item[res][2]
end

function GetGovernDropItem(res)
    return Govern_item[res][3]
end
