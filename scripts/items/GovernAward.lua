Govern_item =
{
	[1] = { 1, 10000,2000},    
	[2] = { 2, 8000, 1000},
    [3] = { 3, 6000,  0},
	[4] = { 4, 4000,  0},
	[5] = { 5, 0,     0},
}


function GetGovernDropMoney(res)
	return Govern_item[res][2]
end

function GetGovernDropItem(res)
    return Govern_item[res][3]
end
