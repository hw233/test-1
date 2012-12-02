
--某一项可以获得变强之魂的最大次数
local checkFlag = {
    [0] = 5, --SthShimenTask,  --师门任务
    [1] = 1, --SthTaskHook,  //挂机
    [2] = 1, --SthHookUse,  //使用挂机加速符
    [3] = 24, --SthHookSpeed,  //挂机加速
    [4] = 3, --SthDungeon,   //决战之地
    [5] = 2, --SthBoss,      //世界BOSS
    [6] = 1, --SthCountryWar,//阵营战
    [7] = 1, --SthPractice,  --修炼
    [8] = 1, --SthPUse,    //使用修炼加速符
    [9] = 12, --SthPSpeed,    //修炼加速
    [10] = 7, --SthCopy, //副本
    [11] = 1, --SthClanWar,   //帮派战
    [12] = 5, --SthYamenTask, //衙门任务
    [13] = 1, --SthAthletics1, //斗剑天梯
    [14] = 8, --SthAthletics2, //斗剑历练领取奖励
    [15] = 6, --SthOpenPurpleBox, //打开紫色变强秘宝
    [16] = 1, --SthTripodFire,--炉火
    [17] = 7, --SthFormation, //阵图
    [18] = 1, --SthTownDeamon, //封印锁妖塔
    [19] = 1, --SthHeroIsland,//英雄岛
    [20] = 14, --SthGroupCopy, //组队副本
    [21] = 1, --SthShuoShuo,   --领取发表说说奖励
    [22] = 1, --SthInvited,    --领取好友邀请奖励
    [23] = 1, --SthEnchant,   --装备强化
    [24] = 1, --SthSplit,     --装备分解
    [25] = 1, --SthForge,     --装备洗练
    [26] = 1, --SthOpenSocket, --装备打孔
    [27] = 1, --SthAttachGem, --宝石镶嵌
    [28] = 1, --SthMergeGem, --宝石合成
    [29] = 1, --SthDetachGem, --宝石拆卸
    [30] = 1, --SthTrumpEnchant,   --法宝强化
    [31] = 1, --SthTrumpLOrder,   --法宝升阶
    [32] = 1, --SthTrumpUpgrade,   --法宝熔炼
    [33] = 1, --SthCittaUpgrade,   --升级心法
    [34] = 1, --SthGenius,   --天赋洗炼
    [35] = 1, --SthPotential,   --潜力洗炼
    [36] = 1, --SthCapacity,   --资质洗炼
    [37] = 1, --SthCHTownDeamon,   --镇守锁妖塔
    [38] = 1, --SthActSignIn,   --活跃度签到
}

--增加的变强之魂
local addSouls = {
    [0] = 1, --SthShimenTask,  --师门任务
    [1] = 3, --SthTaskHook,  //挂机
    [2] = 1, --SthHookUse,  //使用挂机加速符
    [3] = 1, --SthHookSpeed,  //挂机加速
    [4] = 5, --SthDungeon,   //决战之地
    [5] = 5, --SthBoss,      //世界BOSS
    [6] = 3, --SthCountryWar,//阵营战
    [7] = 3, --SthPractice,  --修炼
    [8] = 1, --SthPUse,    //使用修炼加速符
    [9] = 1, --SthPSpeed,    //修炼加速
    [10] = 5, --SthCopy, //副本
    [11] = 3, --SthClanWar,   //帮派战
    [12] = 1, --SthYamenTask, //衙门任务
    [13] = 1, --SthAthletics1, //斗剑天梯
    [14] = 3, --SthAthletics2, //斗剑历练领取奖励
    [15] = 1, --SthOpenPurpleBox, //打开紫色变强秘宝
    [16] = 1, --SthTripodFire,--炉火
    [17] = 5, --SthFormation, //阵图
    [18] = 1, --SthTownDeamon, //锁妖塔
    [19] = 3, --SthHeroIsland,//英雄岛
    [20] = 1, --SthGroupCopy, //组队副本
    [21] = 1, --SthShuoShuo,   --领取发表说说奖励
    [22] = 1, --SthInvited,    --领取好友邀请奖励
    [23] = 1, --SthEnchant,   --装备强化
    [24] = 1, --SthSplit,     --装备分解
    [25] = 1, --SthForge,     --装备洗练
    [26] = 1, --SthOpenSocket, --装备打孔
    [27] = 1, --SthAttachGem, --宝石镶嵌
    [28] = 1, --SthMergeGem, --宝石合成
    [29] = 1, --SthDetachGem, --宝石拆卸
    [30] = 1, --SthTrumpEnchant,   --法宝强化
    [31] = 1, --SthTrumpLOrder,   --法宝升阶
    [32] = 1, --SthTrumpUpgrade,   --法宝熔炼
    [33] = 1, --SthCittaUpgrade,   --升级心法
    [34] = 1, --SthGenius,   --天赋洗炼
    [35] = 1, --SthPotential,   --潜力洗炼
    [36] = 1, --SthCapacity,   --资质洗炼
    [37] = 1, --SthCHTownDeamon,   --镇守锁妖塔
    [38] = 1, --SthActSignIn,   --活跃度签到
}

--某一项的最大值
function GetSthCheckFlag(idx)
    local flag = checkFlag[idx];
    if flag == nil then
        return 0;
    else
        return flag;
    end
end

--每日变强之吼
function EveryDayRoarSoul()
    local chance = { 2725, 4994, 6938, 8639, 10000 }
    local souls = { 5, 6, 7, 8, 10 }
    local r = math.random(1, 10000)
    local soul = 5
    for i = 1, #chance do
        if r <= chance[i] then
            soul = souls[i]
            break
        end
    end
    return soul
end

--增加变强之魂
function doStrong(player, id, param1, param2)
    local mgr = player:GetStrengthenMgr();
    local needflag = checkFlag[id];
    local as = addSouls[id];
    if as == nil then
        return;
    end
    if needflag == nil then
        return;
    end
    mgr:CheckTimeOver();
    --判断标志位
    local curflag = mgr:GetFlag(id);
    if curflag >= needflag then
         return;
    else
        mgr:UpdateFlag(id, curflag + 1);
    end
    mgr:AddSouls(as);
    mgr:UpdateToDB();
end

function openGreenBoxStrong()
    local items = 
    {
        {{55,  1}}, --初级挂机加速符
        {{502, 1}}, --太乙真金
        {{510, 1}}, --初级打孔石
        {{1328, 1}}, --绿色符文熔炼诀
        {{400,  1}}, --火凤果
        {{5001,1},{5011,1},{5021,1},{5031,1},{5041,1},{5051,1},{5061,1},{5071,1},{5081,1},{5091,1},{5101,1},{5111,1},{5121,1},{5131,1},{5141,1}}, --随机一级宝石
        {{9202, 1}}, --发髻笔谈书页
        {{9203, 1}}, --寻风记书页
    }
    local chance = { 1000, 2003, 3006, 5014, 7022, 9624, 9850, 10000 }
    local j = 1
    local r = math.random(1, 10000)
    for i = 1, #chance do
        if r <= chance[i] then
            j = i
            break
        end
    end
    if #items[j] > 1 then
        local g = math.random(1, #items[j])
        return items[j][g]
    end
    return items[j][1]
end

function openBlueBoxStrong()
    local items = 
    {
        {{57,  1}}, --修为加速符
        {{56,  1}}, --高级挂机加速符
        {{1327, 1}}, --蓝色符文熔炼诀
        {{35,  5}}, --碎银
        {{400, 3}}, --火凤果
        {{5002,1},{5012,1},{5022,1},{5032,1},{5042,1},{5052,1},{5062,1},{5072,1},{5082,1},{5092,1},{5102,1},{5112,1},{5122,1},{5132,1},{5142,1}}, --随机二级宝石
        {{9205, 1}}, --打结传书页
        {{9206, 1}}, --封神绑书页
    }
    local chance = { 1000, 2003, 3006, 5014, 7022, 9624, 9850, 10000 }
    local j = 1
    local r = math.random(1, 10000)
    for i = 1, #chance do
        if r <= chance[i] then
            j = i
            break
        end
    end
    if #items[j] > 1 then
        local g = math.random(1, #items[j])
        return items[j][g]
    end
    return items[j][1]
end

function openPurpleBoxStrong()
    local items =
    {
        {{503, 1}}, --太乙精金
        {{499, 10}}, --10礼券
        {{48,  1}}, --七星元木
        {{1326, 1}}, --紫色符文熔炼诀
        {{400, 5}}, --火凤果
        {{514, 1}}, --五行真金
        {{9208, 1}}, --竹草纲目书页
        {{9207, 1}}, --传说厨具的碎片
    }
    local chance = { 518, 2582, 4974, 7566, 9158, 9676, 9870, 10000 }
    local j = 1
    local r = math.random(1, 10000)
    for i = 1, #chance do
        if r <= chance[i] then
            j = i
            break
        end
    end
    if #items[j] > 1 then
        local g = math.random(1, #items[j])
        return items[j][g]
    end
    return items[j][1]
end

function openOrangeBoxStrong()
    local items = 
    {
        {{509, 1}}, --凝神易筋丹
        {{50,  1}}, --九龙神火
        {{30,  1}}, --中级道法金丹
        {{1325, 1}}, --技能符文熔炼诀
        {{49,   1}}, --乾坤净水
        {{515,  1}}, --五行精金
        {{134, 1}}, --法灵精金
        {{9204, 1}}, --补丁
        {{9201, 1}}, --桃木林地图碎片
    }
    local chance = { 505, 3031, 5557, 6399, 8925, 9178, 9684, 9874, 10000 }
    local j = 1
    local r = math.random(1, 10000)
    for i = 1, #chance do
        if r <= chance[i] then
            j = i
            break
        end
    end
    if #items[j] > 1 then
        local g = math.random(1, #items[j])
        return items[j][g]
    end
    return items[j][1]
end


