
--某一项可以获得变强之魂的最大次数
local checkFlag = {
    [0] = 5, --SthShimenTask,  --师门任务
    [1] = 1, --SthTaskHook,  //挂机
    [2] = 1, --SthHookUse,  //使用挂机加速符
    [3] = 24, --SthHookSpeed,  //挂机加速
    [4] = 6, --SthDungeon,   //决战之地
    [5] = 2, --SthBoss,      //世界BOSS
    [6] = 1, --SthCountryWar,//阵营战
    [7] = 1, --SthPractice,  --修炼
    [8] = 1, --SthPUse,    //使用修炼加速符
    [9] = 12, --SthPSpeed,    //修炼加速
    [10] = 7, --SthCopy, //副本
    [11] = 1, --SthClanWar,   //帮派战
    [12] = 5, --SthYamenTask, //衙门任务
    [13] = 10, --SthAthletics1, //斗剑天梯变强1次  天书奇缘10次
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
    [39] = 1, --SthActSignIn,   --切磋斗剑
    [40] = 1,--   SthMSCG ,     //墨守成规
    [41] = 5,--   SthClanTask,  //帮派任务
    [42] = 1,--  SthLastDay，  //末日之战
    [43] = 1,--    SthClanSpirit, //神魔之树
    [44] = 1,--    SthPrayTree ,  //许愿树
    [45] = 1,--    SthSerachMo ,    //寻墨
    [46] = 1,--  SthFuwenJIe ,   //符文解封
    [47] = 1,--    SthFuwenRong,  //符文熔炼
    [48] = 1,--   SthSkillUp ,   //个人技能提升
    [49] = 1,--    SthStoreFix , //晶石融合
    [50] = 1,--   SthXingchen , //星辰描绘
    [51] = 1,--    SthPinJieUp , //品阶进阶
    [52] = 1,--    SthGenguUp , //根骨提升
    [53] = 1,--    SthSpiritEat , //元神吞噬
    [54] = 1,--    SthSpiritUp , //元神升级
    [55] = 1,--    SthNeiDanUp, //内丹升级
    [56] = 1,--   SthJingPoFix,//精魄合成
    [57] = 1,--   SthEquipLH ， //装备炼化
    [58] = 1,--    SthEquipSpirit , //装备注灵
    [59] = 1,--   SthGuJiSpirit , //古籍唤灵
    [60] = 1,--    SthBaoJuSpirit , //宝具通灵
    [61] = 1, --SthZixiaozhidian, //紫霄之巅
    [62] = 5, --SthXuanjiFront, //璇玑阵法
    [63] = 1, --SthMoBao,  //墨家墨宝
    [64] = 1, --SthDuoBao, //夺宝奇兵
    [65] = 1, --SthPetCopy, //宠物副本
    [66] = 1, --SthCangjiya, //藏剑崖
    [67] = 1, --SthArenaLeft, //仙界遗迹
    [68] = 1, --SthAcupointGold, //凝结金丹
    [69] = 1, --SthModifyMount, //人物飞剑
    [70] = 1, --SthSetZhenYuan,  //阵元嵌入
    [71] = 1, --SthClanBuild,  //帮派建筑
    [72] = 1, --SthMarryCopy, //夫妻副本
    [73] = 1, --SthMarryFish, //水中垂钓
    [74] = 1, --SthPetSanHun, //宠物三魂
    [75] = 1, --SthPetQiPo,   //宠物七魄
    [76] = 1, --SthXinMo, //人物心魔
    [77] = 1, --SthSkillUpgrade, //技能升阶
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
    [39] = 1, --SthActSignIn,   --切磋斗剑
    [40] = 5, --SthMSCG ,     //墨守成规
    [41] = 1, --SthClanTask,  //帮派任务
    [42] = 5, --SthLastDay，  //末日之战
    [43] = 1, --SthClanSpirit, //神魔之树
    [44] = 1, --SthPrayTree ,  //许愿树
    [45] = 3, --SthSerachMo ,    //寻墨
    [46] = 1, --SthFuwenJIe ,   //符文解封
    [47] = 1, --SthFuwenRong,  //符文熔炼
    [48] = 1, --SthSkillUp ,   //个人技能提升
    [49] = 1, --SthStoreFix , //晶石融合
    [50] = 1, --SthXingchen , //星辰描绘
    [51] = 1, --SthPinJieUp , //品阶进阶
    [52] = 1, --SthGenguUp , //根骨提升
    [53] = 1, --SthSpiritEat , //元神吞噬
    [54] = 1, --SthSpiritUp , //元神升级
    [55] = 1, --SthNeiDanUp, //内丹升级
    [56] = 1, --SthJingPoFix,//精魄合成
    [57] = 1, --SthEquipLH ， //装备炼化
    [58] = 1, --SthEquipSpirit , //装备注灵
    [59] = 1, --SthGuJiSpirit , //古籍唤灵
    [60] = 1, --SthBaoJuSpirit , //宝具通灵
    [61] = 5, --SthZixiaozhidian, //紫霄之巅
    [62] = 3, --SthXuanjiFront, //璇玑阵法
    [63] = 1, --SthMoBao,  //墨家墨宝
    [64] = 1, --SthDuoBao, //夺宝奇兵
    [65] = 3, --SthPetCopy, //宠物副本
    [66] = 1, --SthCangjiya, //藏剑崖
    [67] = 5, --SthArenaLeft, //仙界遗迹
    [68] = 1, --SthAcupointGold, //凝结金丹
    [69] = 1, --SthModifyMount, //人物飞剑
    [70] = 1, --SthSetZhenYuan,  //阵元嵌入
    [71] = 1, --SthClanBuild,  //帮派建筑
    [72] = 1, --SthMarryCopy, //夫妻副本
    [73] = 1, --SthMarryFish, //水中垂钓
    [74] = 1, --SthPetSanHun, //宠物三魂
    [75] = 1, --SthPetQiPo,   //宠物七魄
    [76] = 1, --SthXinMo, //人物心魔
    [77] = 1, --SthSkillUpgrade, //技能升阶
}
local grade11 = {
--[[ [1] = {{10,7},{14 ,3},{ 17,5},{ 19,1},{ 42,1},{ 40,1}},
    [2] = {{17,7},{ 5,2},{ 37,1},{ 13,10},{ 42,1},{ 40,1}},
    [3] = {{ 4,4},{ 20,6},{11,1},{ 6,1},{ 14,3},{ 10,5}},
    [4] = {{10 ,5},{ 17,5},{ 4,4},{ 5,2},{ 6,1},{ 14,3}},
    [5] = {{20 ,6},{ 14,3},{ 16,3},{ 19,1},{ 6,1},{ 13,10}},
    [6] = {{ 13,10},{ 37,1},{ 41,1},{ 11,1},{10 ,5},{ 4,4}},
    [7] = {{ 10,5},{ 13,10},{ 14,3},{ 4,6},{ 19,1},{ 17,5}},
    [8] = {{ 10,7},{ 5,2},{ 20,6},{ 4,4},{ 42,1},{ 40,1}},
    [9] = {{ 18,1},{ 12,1},{ 17,7},{ 42,1},{ 40,1},{ 11,1}},
    [10] = {{ 14,3},{ 20,6},{ 37,1},{ 16,3},{ 13,10},{ 5,2}},
    [11] = {{ 14,3},{ 17,5},{ 10,5},{ 6,1},{ 20,6},{12,1}},
    ]]--
    [1] = {{10,5},{17,5},{4,4},{19,1},{13,10},{5,1}}, --ok
    [2] = {{10,5},{17,5},{4,4},{37,1},{5,1},{19,1}}, --ok
    [3] = {{10,5},{17,5},{4,4},{16,3},{12,5},{14,3}},  --ok
    [4] = {{10,5},{17,5},{4,4},{19,1},{13,10},{5,1}}, --ok
    [5] = {{10,5},{17,5},{4,6},{18,5},{5,1},{19,1}}, --ok
---华丽的分割线
    [6] = {{ 13,10},{ 37,1},{ 41,1},{ 11,1},{10 ,5},{ 4,4}},
    [7] = {{ 10,5},{ 13,10},{ 14,3},{ 4,6},{ 19,1},{ 17,5}},
    [8] = {{ 10,7},{ 5,2},{ 20,6},{ 4,4},{ 42,1},{ 40,1}},
    [9] = {{ 18,1},{ 12,1},{ 17,7},{ 42,1},{ 40,1},{ 11,1}},
    [10] = {{ 14,3},{ 20,6},{ 37,1},{ 16,3},{ 13,10},{ 5,2}},
    [11] = {{ 14,3},{ 17,5},{ 10,5},{ 6,1},{ 20,6},{12,1}},
}

--某一项的最大值
function GetSthCheckFlag(idx)
    local flag = checkFlag[idx];
    if flag == nil then
        if idx == 1 then
            print("nil:0")
        end
        return 0;
    else
        if idx == 1 then
            print("return : ".. flag)
        end
        return flag;
    end
end
function GetGradeCheckFlag(idx,day)
    --local num = get11TimeNum();
    if day > 5 then 
        return 0
    end
    local max = grade11[day];   --lb
    if max == nil then
        return 0
    end
    for i = 1, #max do
        if idx == max[i][1] then
            return max[i][2]
        end
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
    local num = get11TimeNum();
    if num > 0 and num < 6 then
        -- XXX: 这是一个不应该出现的BUG
        if id == 16 then
            checkFlag[id] = 3
        end
        if id == 37 then
            checkFlag[id] = 2
        end
        if id == 18 then
            checkFlag[id] = 5
        end
        if id == 12 then
            checkFlag[id] = 5
        end
        do11Grade(player, id, num, param2);
    else
        if id == 16 then
            checkFlag[id] = 1
        end
        if id == 37 then
            checkFlag[id] = 1
        end
        if id == 18 then
            checkFlag[id] = 1
        end
        if id == 12 then
            checkFlag[id] = 1
        end
    end
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
    if id ==13 and curflag > 1 then
        mgr:UpdateToDB();
        return ;
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


function do11Grade(player, id, param1, param2)
    local mgr = player:GetStrengthenMgr();
    if param1 > 5 or param1==0 then 
        return ;
    end
    local dayTask = grade11[param1];
    local as = addSouls[id];
    if as == nil then
        return;
    end
    if dayTask == nil then
        return;
    end
    mgr:CheckTimeOver();
    --判断标志位
    local curflag = mgr:GetFlag(id);
    for i = 1, #dayTask do
        if id == dayTask[i][1]  then
            if curflag >= dayTask[i][2] or curflag >=checkFlag[id] then
                return ;
            end
            player:Add11grade(10);
            break
        end
    end
end
