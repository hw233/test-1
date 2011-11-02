local attain_finish = -1    --0xffffffff
local attained = -2         --0xfffffffe


-- 等级
function Level40( player )
    if player:GetAttainMgr():CanAttain(10001) then
        player:GetAttainMgr():UpdateAttainment( 10001, attained)
    end
end

function Level50( player )
    Level40( player )
    if player:GetAttainMgr():CanAttain(10002) then
        player:GetAttainMgr():UpdateAttainment( 10002, attained)
    end
end

function Level60( player )
    Level40( player )
    Level50( player )
    if player:GetAttainMgr():CanAttain(10003) then
        player:GetAttainMgr():UpdateAttainment( 10003, attained)
    end
end

function Level70( player )
    Level40( player )
    Level50( player )
    Level60( player )
    if player:GetAttainMgr():CanAttain(10004) then
        player:GetAttainMgr():UpdateAttainment( 10004, attained)
    end
end

function Level80( player )
    Level40( player )
    Level50( player )
    Level60( player )
    Level70( player )
    if player:GetAttainMgr():CanAttain(10005) then
        player:GetAttainMgr():UpdateAttainment( 10005, attained)
    end
end

function Level90( player )
    Level40( player )
    Level50( player )
    Level60( player )
    Level70( player )
    Level80( player )
    if player:GetAttainMgr():CanAttain(10006) then
        player:GetAttainMgr():UpdateAttainment( 10006, attained)
    end
end

function Level100( player )
    Level40( player )
    Level50( player )
    Level60( player )
    Level70( player )
    Level80( player )
    Level90( player )
    if player:GetAttainMgr():CanAttain(10007) then
        player:GetAttainMgr():UpdateAttainment( 10007, attained)
    end
end

local level_table = {
    [40] = Level40,
    [41] = Level40,
    [42] = Level40,
    [43] = Level40,
    [44] = Level40,
    [45] = Level40,
    [46] = Level40,
    [47] = Level40,
    [48] = Level40,
    [49] = Level40,
    [50] = Level50,
    [51] = Level50,
    [52] = Level50,
    [53] = Level50,
    [54] = Level50,
    [55] = Level50,
    [56] = Level50,
    [57] = Level50,
    [58] = Level50,
    [59] = Level50,
    [60] = Level60,
    [61] = Level60,
    [62] = Level60,
    [63] = Level60,
    [64] = Level60,
    [65] = Level60,
    [66] = Level60,
    [67] = Level60,
    [68] = Level60,
    [69] = Level60,
    [70] = Level70,
    [71] = Level70,
    [72] = Level70,
    [73] = Level70,
    [74] = Level70,
    [75] = Level70,
    [76] = Level70,
    [77] = Level70,
    [78] = Level70,
    [79] = Level70,
    [80] = Level80,
    [81] = Level80,
    [82] = Level80,
    [83] = Level80,
    [84] = Level80,
    [85] = Level80,
    [86] = Level80,
    [87] = Level80,
    [88] = Level80,
    [89] = Level80,
    [90] = Level90,
    [91] = Level90,
    [92] = Level90,
    [93] = Level90,
    [94] = Level90,
    [95] = Level90,
    [96] = Level90,
    [97] = Level90,
    [98] = Level90,
    [99] = Level90,
    [100] = Level100
}

function OnLevelUpAttain( player, param )
	local trigger = level_table[param];
	if trigger == nil then
		return false;
	end
	return trigger(player);	
end

function OnIncarnationAttain( player, param )
end




local attain_table = {
    [10001] = OnLevelUpAttain,        -- 1000 到 1006 为升级成就
    [10008] = OnIncarnationAttain
}

local attain_fin_table = {
}


function doAttainment( player, attainId, param )
	local trigger = attain_table[attainId];
	if trigger == nil then
		return false;
	end
	return trigger(player, param);	
end

function finishAttainment( player, attainId )
    local trigger = attain_fin_table[attainId]
    if trigger == nil then
        return false
    end
    return trigger(player)
end

