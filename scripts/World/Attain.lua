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

--打通穴道
function OnAddAcupoint(player, param) --  打通穴道
    return MapFinder(player, param, AddAcupointT);
end

--增加元神力上限 param 当前最大上限
function OnAddSoulMax(player, param)
    if param >= 300 then
        AttempToUpdate(player, 10087, attained);
    end

    if param >= 999 then
        AttempToUpdate(player, 10088, attained);
    end
end

--增加修为  param 增加的值
function OnAddPExp(player,  param)
    if param > 9000000 then
        return;
    end

    local cur = player : GetVar(19); --VAR_PEXP_GET = 19,

    if cur > 9000000 then
        return ;
    end
    cur = cur + param;

    if cur >=100000 then
        AttempToUpdate(player,  10089, attained);
    end

    if cur >= 1000000 then
        AttempToUpdate(player,  10090, attained);
    end

    if cur >= 9000000 then
        AttempToUpdate(player,  10091, attained);
    end

    player : SetVar(19 ,  cur);
    
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
--添加散仙  param 颜色
function OnAddFighter(player, param)
    AttempToUpdate(player , 10101, attained);  

    if param >= 2 then 
         AttempToUpdate(player , 10103, attained);
    end
    if param >= 3 then
          AttempToUpdate(player ,10104, attained);
    end

end

function OnAddFighter4(player, param)
     AttempToUpdate(player , 10102, attained);
    
     if param >= 2 then 
        AttempToUpdate(player , 10105, attained);
     end
     if param >= 3 then
          AttempToUpdate(player ,10106, attained);
     end
end
function OnAddFighter9(player, param)
    if(param >= 3) then
        AttempToUpdate(player ,10108, attained);
    end
end
function OnRemoveFighter(player, param)
    if param  >= 2 then
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

--主角潜力增加  param 潜力
function  OnMainFighterFullPot(player, param)
    if param >= 2 then

         AttempToUpdate(player ,  10119, attained);
    end
end

--主角颜色变化 param 颜色
function  OnMainFighterColChange(player, param)
    if param >=3 then 
         AttempToUpdate(player,   10117,attained);
    end
    if param >= 4 then
        AttempToUpdate(player, 10118, attained);
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
   -- if param >= 2 then
     --    AttempToUpdate(player , 10119, attained);
   -- end

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
-- 主将资质练满
function   OnMainFighterCapFull(player, param)
    AttempToUpdate(player, 10112, attained);
end
--获得紫色装备
function   OnAddPEquip(player, param)
    local cur = player:GetVar(20);  -- VAR_PURPLE_EQUIP_NUM  20

    cur = cur + 1;

    if cur >=1 then
        ttempToUpdate(player ,   10151, attained);
    end

    if cur >= 10 then
        AttempToUpdate(player ,  10152, attained);
    end

    if cur >= 100 then
        AttempToUpdate(player ,  10153, attained);
    end

    if cur >= 1000 then
        AttempToUpdate(player ,  10154, attained);
    end

    player:SetVar(20 , cur);

end

--获得金色装备

function   OnAddYEquip(player, param)
    local cur = player:GetVar(21);  --  VAR_YELLOW_EQUIP_NUM = 21,

    cur = cur + 1;

    if cur >=1 then
        ttempToUpdate(player ,   10155, attained);
    end

    if cur >= 10 then
        AttempToUpdate(player ,  10156, attained);
    end

    if cur >= 100 then
        AttempToUpdate(player ,  10157, attained);
    end

    if cur >= 1000 then
        AttempToUpdate(player ,  10158, attained);
    end

    player:SetVar(21 , cur);

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
--一个散仙 全身装备8
function OneFighterEnchant8(player, param)
    AttempToUpdate(player, 10167, attained);
end
--5个散仙 全身装备 9
function FiveFighterEnchant9(player, param)
    AttempToUpdate(player, 10168, attained);
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
     if param >= 8 then 
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
    AttempToUpdate(player ,10211, attained);
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
    [1013] = 10264, --获得两仪微尘阵旗
    [1021] = 10263, --获得须弥九宫阵旗
    [1032] = 10262, --获得金刚伏魔阵旗
    [1041] = 10261, --获得紫薇太极阵旗
    [1053] = 10260, --获得五行灭绝阵旗
    [1060] = 10256, --获得四象元灵阵旗
    [1066] = 10258, -- 获得北斗七星阵旗
    [1075] = 10257, --获得颠倒八卦阵旗
    [1085] = 10259, -- 获得都天烈火阵旗
    [1097] = 10254, --获得天罡地煞阵旗
    [1108] = 10255, --获得奇门遁甲阵旗
    [1117] = 10253, --获得七绝锁云阵旗
}
function OnAddItem(player, param)  -- 增加道具
    return MapFinder(player, param, ItemId2AttainId);
end

--得到一个法宝  param 颜色
function OnAddTrump(player, param)
    AttempToUpdate(player, 10200, attained);
    if param == 4 then
        AttempToUpdate(player, 10202, attained);
    end
    if param == 5 then
        AttempToUpdate(player, 10203, attained);
    end
end

--得到累计多个法宝 
function  OnAddNTrump(player, param)
    
    local cur = player:GetVar(22) ; -- VAR_YELLOW_THRUMP_NUM=22,

    cur = cur + 1;

    if cur < 5 then
        player:SetVar(22, cur);
    end
    if cur == 5 then
        AttempToUpdate(player, 10201, attained);    
    end
       
    
end
--增加阵法个数 -- 成就ID
function OnAddFormation(player, param)
    if param  >=  5 then
        AttempToUpdate(player, 10251, attained);
    end

    if param >=  12 then 
        AttempToUpdate(player, 10252, attained);
    end
end

--升级高级阵法
function  OnAddSFormation(player, param)
    if param  >=  2 then
        AttempToUpdate(player, 10265, attained);
    end

    if param >= 6 then
         AttempToUpdate(player, 10266, attained);
    end
    if param >=  12 then 
        AttempToUpdate(player, 10267, attained);
    end
end
--PKNPC成功
local  pk_npc = {
    [5032] = 10351, --杨花
    [5033] = 10352, --四大金刚
    [5037] = 10353, --智通
    [5063] = 10356, --七魔许人龙
    [5065] = 10357, --六魔厉吼
    [5068] = 10358, --三魔钱青选
    [5071] = 10359, --魏枫娘
    [5214] = 10361, --钟敢
    [5213] = 10362, --戎敦
    [5212] = 10363, --穷奇
    [5211] = 10364, --无华氏
    [5229] = 10365, --三凤
    [5230] = 10366, --二凤
    [5231] = 10367, --初凤
    [5233] = 10368, --金须奴
    [5254] = 10370, --守灵侍卫
    [5255] = 10371, --烈焰侍者
    [5256] = 10372, --护卫神侍
    [5257] = 10373, --守灵真龙
    [5258] = 10374, --连山遗蜕
    [5270] = 10376, --阴四娘
    [5268] = 10377, --丙融
    [5271] = 10378, --毒手摩什
    [5267] = 10379, --圣姑遗蜕
    [5266] = 10380, --崔盈
}
function OnPKNpc(player, param)
       return MapFinder(player, param, pk_npc );   
end

--Pass 通天塔
local  pass_dungen={
    [1] = 10354, --魏家场
    [2] = 10355,--水火风雷阵
    [3] = 10360, -- 通关烈火魔阵
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
--装备强化失败
function OnFailEnchance (player, param)
    
    if param  >= 999 then
        AttempToUpdate(player, 10169,  attained);
    end

    player:SetVar(4, param);
end

--分解装备
function OnSplitEquip(player, param)
    if param  >= 10 then
        AttempToUpdate(player, 10176,  attained);
    end

    if param >= 1000 then
        AttempToUpdate(player, 10177,  attained);
    end
end
--分解特定颜色的装备
function OnSplitEquipColor(player, param)
    if param == 5 then
         AttempToUpdate(player, 10178,  attained);
    end
end

--分解法宝
function OnSplitThrump(player, param)
    if param >= 1 then
         AttempToUpdate(player, 10208,  attained);
    end
    if param >= 1000 then
         AttempToUpdate(player, 10209,  attained);
    end
end

function OnSplitThrumpColor(player, param)
    if param == 5 then
         AttempToUpdate(player, 10210,  attained);
    end
end
--选择阵营
function OnSelectCountry(player, param)
         AttempToUpdate(player, 10551,  attained);
end
-- 阵营战 累计获胜 param  累计获胜数量
function  OnCountryBattleWin(player,  param)

    if param >= 10 then
        AttempToUpdate(player, 10552, attained);

    end
    if param >= 100 then
        AttempToUpdate(player, 10553, attained);
    end
    if param >= 1000 then
        AttempToUpdate(player, 10554, attained);
    end
    player:SetVar(13,param);  -- VAR_COUNTRY_BATTLE_WIN
end

--阵营战中连续击杀 param 击杀数
function  OnCountryBattleKillStreak(player, param)
    if param >= 3 then
        AttempToUpdate(player, 10555, attained);
    end

    if param >= 6 then
        AttempToUpdate(player, 10556, attained);
    end
    if param >= 12 then
        AttempToUpdate(player, 10557, attained);
    end
end
--斗剑中累积胜利 param 累计获胜数量
function  OnAthleticWin(player, param)

    if param >= 100 then
        AttempToUpdate(player, 10558, attained);
    end

    if param >= 1000 then
        AttempToUpdate(player, 10559, attained);
    end

    player:SetVar(14, param); -- VAR_ATHLETICS_WIN
end
-- 战斗中闪避
function OnBattleMiss(player, param)

    local target = 0;
    if param >= 3 then
         AttempToUpdate(player, 10601,  attained);
         target = 9;
    end

    if param >= 9 then
         AttempToUpdate(player, 10605,  attained);
    end

    if target > 0 then
        player:SetVar(7, target); --VAR_BATTLE_MISS = 7,
    end
end
-- 战斗中重击
function OnBattleCS(player, param)

    local target = 0;
    if param >= 3 then
         AttempToUpdate(player, 10602,  attained);
         target = 9;
    end

    if param >= 9 then
         AttempToUpdate(player, 10606,  attained);
    end

    if target > 0 then
        player:SetVar(8, target); --VAR_BATTLE_MISS = 7,
    end
end
-- 战斗中破击
function OnBattlePR(player, param)

    local target = 0;
    if param >= 3 then
         AttempToUpdate(player, 10603,  attained);
         target = 9;
    end

    if param >= 9 then
         AttempToUpdate(player, 10607,  attained);
    end

    if target > 0 then
        player:SetVar(9, target); --VAR_BATTLE_MISS = 7,
    end
end

-- 战斗中反击
function OnBattleFJ(player, param)

    local target = 0;
    if param >= 3 then
         AttempToUpdate(player, 10604,  attained);
         target = 9;
    end

    if param >= 9 then
         AttempToUpdate(player, 10608,  attained);
    end

    if target > 0 then
        player:SetVar(10, target); --VAR_BATTLE_MISS = 7,
    end
end

--战斗中技能伤害
function  OnBattleSkillDmg(player, param)

    local target = 0;
    if param >= 300 then
        AttempToUpdate(player, 10609,  attained);
        target = 1000;
    end

    if param >= 1000 then
        AttempToUpdate(player, 10610,  attained);
        target = 5000;
    end

    if param >= 5000 then
        AttempToUpdate(player, 10611,  attained);
    end 

    print(target);
    if target  > 0 then
        player:SetVar(11 , target); --VAR_BATTLE_SKILL_DMG = 11,
    end
end
--战斗无双伤害
function  OnBattlePLDmg(player, param)

    local target = 0;
    if param >= 1000 then
        AttempToUpdate(player, 10612,  attained);
        target = 5000;
    end

    if param >= 5000 then
        AttempToUpdate(player, 10613,  attained);
        target = 10000;
    end

    if playeraram >= 10000 then
        AttempToUpdate(player, 10614,  attained);
    end 

    if target  > 0 then
        player:SetVar(12 , target); --VAR_BATTLE_PEERLESS_DMG = 12,
    end
end

--战斗中的最高灵气
function OnBattleMaxAura(player, param)
    if param >= 200 then
        AttempToUpdate(player, 10616, attained);
    end
end

--战斗中暴击比率
function OnBattleCSFactor(player, param)
     AttempToUpdate(player, 10167, attained);
end
--战斗中第一回合打出无双攻击
function OnFirstPeerLessAttack(player, param)
    AttempToUpdate(player, 10615, attained);
end
--完成任务数量  
function  OnSubmitTasks(player, param)
    
    local cur = player:GetVar(15);  -- vAR_TASK_SUBMITTED = 15

    cur = cur + 1;
    if cur >= 10  then
        AttempToUpdate(player, 10301,  attained);
    end

    if cur >= 100 then
         AttempToUpdate(player, 10302,  attained);
    end
    if cur >= 500 then
        AttempToUpdate(player, 10303,  attained);
    end
    if cur >= 1000 then
        AttempToUpdate(player, 10304,  attained);
    end

    player:SetVar(15 ,cur);
end
--特殊任务ID 完成了 给成就
local special_task = {
    [81] = 10305,
    [89] = 10306,
    [118] = 10307,
    [153] = 10308,

}
function OnSubmitSpecialTask(player, param)
         return MapFinder(player, param, special_task);
end

--完成衙门任务
function OnSubmitYamenTask(player, param)

    local cur = player:GetVar(16); --VAR_YAMEN_TASK_SUBMITTED = 16
    cur = cur +1;

    if cur >=100 then
        AttempToUpdate(player, 10313,  attained);
    end
    if cur >= 1000 then
        AttempToUpdate(player, 10314,  attained);
    end

    player:SetVar(16, cur);
end

--完成师门任务
function OnSubmitShimenTask(player, param)
    local cur = player:GetVar(17); --VAR_SHIMEN_TASK_SUBMITTED = 17,
    cur = cur +1;

 if cur >=100 then
        AttempToUpdate(player, 10315,  attained);
    end
    if cur >= 1000 then
        AttempToUpdate(player, 10316,  attained);
    end

    player:SetVar(17,cur);

end
--完成帮派任务
function OnSubmitClanTask(player, param)
    local cur = player:GetVar(18); --  VAR_CLAN_TASK_SUBMITTED - 18,
    cur = cur +1;

    if cur >=100 then
        AttempToUpdate(player, 10317,  attained);
    end
    if cur >= 1000 then
        AttempToUpdate(player, 10318,  attained);
    end

    player:SetVar(18 , cur);
   
end
--一天五次师门任务
function  OnShimen5Today(player,  param)
    AttempToUpdate(player,  11000, attained);
end

--一天5次衙门任务
function OnYamen5Today(player, param)
    AttempToUpdate(player,  11001, attained);
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
    [10087] = OnAddSoulMax,  --增加元神力
    [10089] =  OnAddPExp,  --增加修为
    [10101] = OnAddFighter,
    [10102] = OnAddFighter4,
    [10107] = OnRemoveFighter,
    [10108] = OnAddFighter9,  -- 另外招募10个散仙
    [10109] = OnFullPotential,
    [10110] = OnFullCapacity, 
    [10111] = OnAddCapacity,
    [10112] = OnMainFighterCapFull,
    [10115] = OnBothFull,
    [10117] = OnMainFighterColChange,
    [10119] = OnMainFighterFullPot,
    [10151] = OnAddPEquip,
    [10155] = OnAddYEquip,

    [10164] = OnEnchantEquip, --强化装备
    [10167] = OneFighterEnchant8,
    [10168] = FiveFighterEnchant9,
    [10169] = OnFailEnchance, --装备强化失败成就
    
    [10170] = OnAttachGem,     --镶嵌宝石
    [10173] = OnAttachFullGem,
    [10174] = OnAttachFUllGemFullGL,
    [10175] = OnForgeEquip,  --装备洗练
    [10176] = OnSplitEquip,  --分解装备
    [10178] = OnSplitEquipColor, --分解特定颜色的装备

    [10200] = OnAddTrump,  --增加法宝
    [10201] = OnAddNTrump, --累计增加N个法宝
    [10204] = OnEnchantTrump, --强化法宝
    [10207] = OnEnchantTrump9, -- 强化法宝到9级

    [10208] = OnSplitThrump,
    [10209] = OnSplitThrumpColor,

    [10211] = OnEquipThrump,   --装备法宝
    [10251] = OnAddFormation, -- 增加阵法
    [10253] = OnAddItem,       --得到特定道具
    [10265] =  OnAddSFormation, --升级阵法
    [10301] = OnSubmitTasks,
    [10305] = OnSubmitSpecialTask,
    [10313] = OnSubmitYamenTask,
    [10315] = OnSubmitShimenTask,
    [10317] = OnSubmitClanTask,

    [10351] = OnPKNpc,          --赢得固定NPC
    [10354] = OnPassDungen,       --通过通天塔
    [10451] = OnAddFriend,   --增加好友
    [10453] = OnFriendLev,  --好友等级上升
    [10454] = On5Friend50,
    [10456] = On5Friend80,
    [10501] = OnJoinClan,  --加入帮派
    [10502] =  OnClanAddMember,  -- 帮派增加成员
    [10504] =  OnClanLevUp,  -- 帮派等级增加

    [10551] = OnSelectCountry,
    [10552] =  OnCountryBattleWin,
    [10555] =  OnCountryBattleKillStreak,
    [10558] = OnAthleticWin,
    [10601] = OnBattleMiss, --战斗中闪避
    [10602] = OnBattleCS,   --战斗重击   
    [10603] = OnBattlePR,   --战斗破击
    [10604] = OnBattleFJ,   --战斗反击 
    [10609] = OnBattleSkillDmg,  -- 战斗技能攻击
    [10612] = OnBattlePLDmg, --战斗中无双技能
    [10615] = OnFirstPeerLessAttack, --战斗第一回合打出无双
    [10616] = OnBattleMaxAura, -- 战斗中最高灵气
    [10617] = OnBattleCSFactor, --战斗中打出暴击
    [11000] = OnShimen5Today, --一天5次师门任务
    [11001] = OnYamen5Today, -- 一天5次衙门任务
}
function doAttainment( player, attainId, param )
    local trigger = attain_table[attainId];
	if trigger == nil then
        print (param);
        print ("can not find")
    --    print (param .. " doAttainment: can not find it!")
		return;
	end

	return trigger(player, param);	
end
local attain_fin_table = {


}

function  giveReward(player, attainid,  itemId, itemNum)
    local package = player:GetPackage()
    if package:IsFull() then
        player:sendMsgCode(2, 1011, 0)
         return
     end

      SendMsg(player, 0x35, "恭喜你获得成就奖励！");
     player:GetAttainMgr():UpdateAttainment( attainid, attain_finish);
    package:AddItem(itemId, itemNum, true, false, 23 );
    
end
function finishAttainment( player, attainId )
--    player:sendMsgCode(2, 1011, 0);
    print ("finished")
   -- local package = player:GetPackage();
    -- package:AddItem(48, 10, true, false, 23 );
   --  attainId = 10004;

  --  SendMsg(player, 0x35, "恭喜您通过情人节乱世佳人活动获得50%额外奖励");
    if player:GetAttainMgr():HasAttained(attainId) then
    --   player:GetAttainMgr():UpdateAttainment( attainId, attain_finish);
        giveReward(player, attainId, 48, 1);
    end
     --[[
   if  player:GetAttainMgr():HasAttained(attainId) then
--      local trigger = attain_fin_table[attainId]
   --   if trigger == nil then
     --     return ;
     -- end
   --   return trigger(player)
        print ("getit")
        giveReward(player, attainid, 4999,10);
   end]]
end

