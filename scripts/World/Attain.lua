local attain_finish = -1    --0xffffffff
local attained = -2         --0xfffffffe

function AttempToUpdate(player , id, flag)
    if player:GetAttainMgr():CanAttain(id) then
        player:GetAttainMgr():UpdateAttainment( id, flag)
        return true;
    end
    return false;
end
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

---------------------------------------------
function MapFinder(player, param, table)
    local it = table[param];
    if it == nil then
        return false;
    end

    if player:GetAttainMgr():CanAttain(it) then
        player:GetAttainMgr():UpdateAttainment( it, attained);
        return true;
    end
    return false;
end
--穴道ID ->成就ID
local  AddAcupointT = {
    [0] = 10051,
    [3] = 10052,
    [6] = 10053,
    [9] = 10054,
    [14] = 10055
}
function OnAddAcupoint(player, param) --  打通穴道
    return MapFinder(player, param, AddAcupointT);
end

--学会心法 param 个数
function OnLearnCitta(player, param)
    if param == 3 then
        AttempToUpdate(player, 10061, attained);
    end
    if param == 6 then
         AttempToUpdate(player, 10062, attained);
    end
end
--学习某个阶数的心法 param 阶数
function OnLearnCittaLevel(player, param)
    if(param == 9) then 
        AttempToUpdate(player, 10074, param);
    end
end
--提升心法等级到9级  param 心法阶数
function OnCittaLevup9(player, param)
    AttempToUpdate(player, 10071, attained);
    if param >= 3 then
         AttempToUpdate(player, 10072, attained);
    end
    if param >= 6 then
         AttempToUpdate(player, 10083, attained);
    end
    if param >= 9 then
         AttempToUpdate(player, 10085, attained);
    end
end
--提升心法到9级 param 个数
function OnCittaLevup9ByNum(player, param)
   if param >= 6 then
         AttempToUpdate(player, 10073, attained);
   end
end

--提升8阶心法到9级 param 个数
function  On8CittaLevup9ByNum(player, param)
    if param >= 3 then
         AttempToUpdate(player, 10086, attained);
    end
end
--装备心法 param 装备数量
function OnUpCitta(player, param)
    if param == 6 then
         AttempToUpdate(player, 10084, attained);
    end
end
--删除心法
function OnDelCitta(player, param)
    AttempToUpdate(player, 10081, attained);
end
--添加散仙
function OnAddFighter(player, param)
    AttempToUpdate(player , 10101, attained);
    if(param  == 10) then
        return;
    end
    if param > 2 then 
         AttempToUpdate(player , 10103, attained);
    end
    if param > 3 then
          AttempToUpdate(player ,10104, attained);
    end

end

function OnAddFighter4(player, param)
     AttempToUpdate(player , 10102, attained);
     if(param  == 10) then
        return;
     end
     if param > 2 then 
        AttempToUpdate(player , 10105, attained);
     end
     if param > 3 then
          AttempToUpdate(player ,10106, attained);
     end
end
function OnAddFighter5(player, param)
    if(param > 3) then
        AttempToUpdate(player ,10108, attained);
    end
end
function OnRemoveFighter(player, param)
    if param  == 3 then
         AttempToUpdate(player ,10107, attained);
    end
end

--潜力练满
function OnFullPotential(player, param)
    if param > 0 then
       AttempToUpdate(player ,10109, attained);
    end
    if param > 4 then
        AttempToUpdate(player ,10113, attained);
    end

end
--资质练满 param 人数
function OnFullCapacity(player, param)
    if param > 0 then
       AttempToUpdate(player ,  10110, attained);
    end

    if param > 4 then
       AttempToUpdate(player ,  10114, attained);
    end
end

--资质 潜力都满 param 人数
function  OnBothFull(player, param)
    if param > 9 then
        AttempToUpdate(player ,  10115, attained);
    end
end
--提升资质
function   OnAddCapacity(player, param)
    if param >= 2 then
         AttempToUpdate(player , 10119, attained);
    end

    if param >= 7 then
         AttempToUpdate(player ,  10111, attained);
    end
    if param >= 8 then
         AttempToUpdate(player ,  10120, attained);
    end
    if param >= 9 then
         AttempToUpdate(player ,  10121, attained);

    end
end
--强化装备 param +数
function OnEnchantEquip(player, param)

    if param > 2 then
       AttempToUpdate(player ,10164, attained);    
    end
    if param > 5 then
       AttempToUpdate(player ,10165, attained);
    end
    if param > 8 then
        AttempToUpdate(player ,10166, attained);
    end
end
--装备洗练
function OnForgeEquip(player, param)
    AttempToUpdate(player ,10175, attained);
end
--强化法宝 param + 数
function OnEnchantTrump(player, param)
     if param > 2 then
        re = AttempToUpdate(player ,10204, attained);
     end

     if param > 5 then
        AttempToUpdate(player ,10205, attained);
     end
     if param > 8 then 
        AttempToUpdate(player ,10206, attained);
     end
end
--强化法宝 param 颜色
function OnEnchantTrump9(player, param)
    if param == 5 then
         AttempToUpdate(player ,10207, attained);
    end
end
--穿法宝
function  OnEquipThrump(player, param)
    AttempToUpdate(palyer ,10211, attained);
end
--镶嵌宝石
function OnAttachGem(player, param)
    AttempToUpdate(player ,10170, attained);

    if(param > 4) then
        AttempToUpdate(player ,10171, attained);
    end


    if(param  == 10) then
        AttempToUpdate(player ,10172, attained);
    end
end
--镶嵌满
function OnAttachFullGem(player, param)
     AttempToUpdate(player ,10173, attained);
end

--满宝石 满等级
function OnAttachFUllGemFullGL(player, param)
    AttempToUpdate(player ,10174, attained);
end
--道具ID  ->成就ID
local  ItemId2AttainId = {
    [1117] = 10253,
    [1108] = 10255
}
function OnAddItem(player, param)  -- 增加道具
    return MapFinder(player, param, ItemId2AttainId);
end

--得到法宝  param 颜色
function OnAddTrump(player, param)
    AttempToUpdate(player, 10200, attained);
    if param == 4 then
        AttempToUpdate(player, 10202, attained);
    end
    if param == 5 then
        AttempToUpdate(player, 10203, attained);
    end
end
--增加阵法个数 -- 成就ID
function OnAddFormation(player, param)
    if param > 4 then
        if player:GetAttainMgr():CanAttain(10251) then
             player:GetAttainMgr():UpdateAttainment( 10251, attained);
             return true;
        end
    end

    if param > 11 then 
        if player:GetAttainMgr():CanAttain(10252) then
            player:GetAttainMgr():UpdateAttainment( 10252, attained);
            return true;
        end
    end
    return false;
end
--PKNPC成功
local  pk_npc = {
    
}
function OnPKNpc(player, param)
       return MapFinder(player, param, pk_npc );   
end

--Pass 通天塔
local  pass_dungen={

}
function OnPassDungen(player, param)
    return MapFinder(player, param,pass_dungen );
end

--增加好友   param 数量
function OnAddFriend(player, param)
    if param  >= 1 then
         AttempToUpdate(player, 10451,   attained);
    end

    if param >= 20 then
         AttempToUpdate(player, 10452,   attained);
    end
end
--好友等级上升 param 等级
function OnFriendLev(player, param)

    if param >= 50 then
         AttempToUpdate(player, 10453,   attained);
    end
    if param >= 80 then
         AttempToUpdate(player, 10455,  attained);
    end
    if param >= 100 then
        AttempToUpdate(player, 10457,  attained);
    end
end

--5个好友50级
function  On5Friend50(player, param)

    AttempToUpdate(player, 10454,  attained);
end

--5个好友80级
function  On5Friend80(player, param)
    AttempToUpdate(player, 10456,  attained);
end 
--加入帮派
function  OnJoinClan(player, param)
     AttempToUpdate(player, 10501,  attained);
end
--帮派增加成员 param 人数
function  OnClanAddMember(player, param)

    if param >= 20 then
         AttempToUpdate(player, 10502,  attained);
    end

    if param >= 35 then
         AttempToUpdate(player, 10503,  attained);
    end
end

--帮派等级增加 param 帮派等级
function OnClanLevUp(player, param)
    if param >= 5 then
         AttempToUpdate(player, 10504,  attained);
    end

    if param >= 10 then
         AttempToUpdate(player, 10505,  attained);
    end
end
--成就处理映射表
local attain_table = {
    [10001] = OnLevelUpAttain,        -- 1000 到 1006 为升级成就
    [10008] = OnIncarnationAttain,
    [10051] = OnAddAcupoint,
    [10061] = OnLearnCitta,  --学会心法
    [10071] = OnCittaLevup9,
    [10073] = OnCittaLevup9ByNum,
    [10074] = OnLearnCittaLevel,
    [10081] = OnDelCitta,  --删除心法
    [10084] = OnUpCitta,   --增加心法
    [10086] = On8CittaLevup9ByNum, --增加心法
    [10101] = OnAddFighter,
    [10102] = OnAddFighter4,
    [10107] = OnRemoveFighter,
    [10108] = OnAddFighter10,  -- 另外招募10个散仙
    [10109] = OnFullPotential,
    [10110] = OnFullCapacity, 
    [10115] = OnBothFull,
    [10119] = OnAddCapacity,
    [10164] = OnEnchantEquip, --强化装备
    [10170] = OnAttachGem,     --镶嵌宝石
    [10173] = OnAttachFullGem,
    [10174] = OnAttachFUllGemFullGL,
    [10175] = OnForgeEquip,  --装备洗练
    [10200] = OnAddTrump,  --增加法宝]
    [10204] = OnEnchantTrump, --强化法宝
    [10207] = OnEnchantTrump9, -- 强化法宝到9级
    [10211] = OnEquipThrump,   --装备法宝
    [10251] = OnAddFormation, -- 增加阵法
    [10253] = OnAddItem,       --得到特定道具
    [10351] = OnPKNpc,          --赢得固定NPC
    [10354] = OnPassDungen,       --通过通天塔
    [10451] = OnAddFriend,   --增加好友
    [10453] = OnFriendLev,  --好友等级上升
    [10454] = On5Friend50,
    [10456] = On5Friend80,
    [10501] = OnJoinClan,  --加入帮派
    [10502] =  OnClanAddMember,  -- 帮派增加成员
    [10504] =  OnClanLevUp,  -- 帮派等级增加
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

