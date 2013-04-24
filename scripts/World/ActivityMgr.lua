--某一项可以获得活跃度的最大次数
local checkFlag = {
    [0] = 1,--AtyPractice,  --修炼
    [1] = 1,--AtyTripodFire,--炉火
    [2] = 1,--AtyBarRef,    --酒吧刷新
    [3] = 1,--AtyBookStore, --书店刷新
    [4] = 1,--AtyEnchant,   --装备强化
    [5] = 1,--AtySplit,     --装备分解
    [6] = 1,--AtyForge,     --装备洗练
    [7] = 1,--AtyBuy,       --购买物品
    [8] = 1,--AtyLongTime,  --在线时间足够长
    [9] = 1,--AtyHeroIsland,//英雄岛
    [10] = 2,-- AtyBoss,      //世界BOSS
    [11] = 1,-- AtyCountryWar,//阵营战
    [12] = 5, -- AtyClanWar,   //帮派战
    [13] = 24,-- AtyAthletics, //斗剑
    [14]=  1, --AtySignIn,   //每日签到,做标记是否签到,不加活跃度点数

    --[110] = 1, --AtyShuoShuo, //领取发表说说奖励
    --[111] = 1, --AtyInvited, //领取好友邀请奖励
}
--增加的活跃度
local addPoint = {
    [0] = 1,--AtyPractice,  --修炼
    [1] = 1,--AtyTripodFire,--炉火
    [2] = 1,--AtyBarRef,    --酒吧刷新
    [3] = 1,--AtyBookStore, --书店刷新
    [4] = 1,--AtyEnchant,   --装备强化
    [5] = 1,--AtySplit,     --装备分解
    [6] = 5,--AtyForge,     --装备洗练
    [7] = 1,--AtyBuy,       --购买物品
    [8] = 5,--AtyLongTime,  --在线时间足够长
    [9] =  3, --AtyHeroIsland,//英雄岛
    [10]=  7, --AtyBoss,      //世界BOSS
    [11]=  3, --AtyCountryWar,//阵营战
    [12]=  1, --AtyClanWar,   //帮派战
    [13]=  1, --AtyAthletics, //斗剑

    [100] = 1, --AtyShimenTask = 1000,//师门任务
    [101] = 1, --AtyYamenTask, //衙门任务
    [102] = 1, --AtyClanTask,  //帮会任务
    [103] = 5, --AtyDungeon,   //决战之地
    [104] = 5, --AtyCopy,      //副本
    [105] = 5, --AtyFormation, //阵图
    [106] = 1, --AtyTaskHook,  //挂机加速
    [107] = 1, --AtyPSpeed,    //修炼加速
    [108] = 1, --AtyGroupCopy, //组队副本
    [109] = 1, --AtyTownDeamon, //锁妖塔
    [110] = 1, --AtyShuoShuo, //领取发表说说奖励
    [111] = 1, --AtyInvited, //领取好友邀请奖励
}

--某个奖励需要的活跃度
local rewardNeedPoint = {
    [4] = 60,
    [8] = 80,
    [16] = 100,
    [32] = 120,
    [64] = 140,
    [128] = 160,
    [256] = 180,
}
--一天活跃度的最大值
local maxActivity = {
    [0] = 109,
    [1] = 117,
    [2] = 127,
    [3] = 137,
    [4] = 155,
    [5] = 155,
    [6] = 165,
    [7] = 165,
    [8] = 167,
    [9] = 167,
    [10] = 167,
}

--上线奖励
onlineReward1 = {
    {{55,1},},
    {{510,1},},
    {{504,1},},
    {{502,1},},
    {{5001,1},{5011,1},{5021,1},{5031,1},{5041,1},{5051,1},{5061,1},{5071,1},{5081,1},{5091,1},{5101,1},{5111,1},{5121,1},{5131,1},{5141,1},},
    {{29,50},},
}
onlineReward2 = {
    {{56,1},},
    {{57,1},},
    {{510,1},},
    {{5002,1},{5012,1},{5022,1},{5032,1},{5042,1},{5052,1},{5062,1},{5072,1},{5082,1},{5092,1},{5102,1},{5112,1},{5122,1},{5132,1},{5142,1},},
    {{29,60},},
    {{51,1},},
}
onlineReward3 = {
    {{56,1},},
    {{57,1},},
    {{511,1},},
    {{500,1},},
    {{514,1},},
    {{48,1},},
}
onlineReward4 = {
    {{514,1},},
    {{506,1},},
    {{508,1},},
    {{49,1},},
    {{512,1},},
    {{503,1},},
}
onlineReward5 = {
    {{515,1},},
    {{507,1},},
    {{509,1},},
    {{50,1},},
    {{512,1},},
    {{503,1},},
}

onlineReward = {
    [0] = onlineReward2,
    [1] = onlineReward2,
    [2] = onlineReward3,
    [3] = onlineReward3,
    [4] = onlineReward4,
    [5] = onlineReward4,
    [6] = onlineReward5,
}

--获取上线奖励
function GetOnlineReward(cnt)
    if cnt > 6 then
        return onlineReward1;
    end

    local wday = os.date("%w", os.time())
    if wday == "0" then
        return onlineReward[cnt]
    end
    return onlineReward1
end

--某一项的最大值
function GetAtyCheckFlag(idx)
    local flag = checkFlag[idx];
    if flag == nil then
        return 0;
    else
        return flag;
    end
end
--VIP 得到一天获得的活跃度的最大值
function GetMaxActivity(vip)
    local ap = maxActivity [vip]
    if ap == nil then

        return 167;
    else
        return ap;
    end
end

--得到活跃度奖励
function GetAtyReward(player, flag)
    local needPoint = rewardNeedPoint[flag];
    if needPoint == nil then
        return;
    end

    local mgr = player:GetActivityMgr();
    if mgr:GetPoint() < needPoint then
       return;
    end

    local package = player:GetPackage();
    if package:IsFull() then
        player:sendMsgCode(2, 1011, 0);
        return;
    end

    local rand = math.random(1, 100);

    local isChristmas = getChristmas();
    if flag == 4 then -- 60
        local gemid =  getRandGem(1);
        package:AddItem(gemid, 1, true, false, 24 ); 
        if  isChristmas == true then
            package:AddItem(401, 1, true, false, 24 );
            Broadcast(0x27, msg_68.."[p:"..player:getCountry()..":"..player:getPName().."]"..msg_90.."[4:".. 401 .."]x1");
        end
    end

    if flag == 8 then -- 80

        player:getTael(500);
        package:AddItem(502, 1, true, false, 24 );

         if  isChristmas == true then
            package:AddItem(402, 1, true, false, 24 );
        end

    end

    if flag == 16 then -- 100
       player:getCoupon(20); 
       package:AddItem(503, 1, true, false, 24 );
       package:AddItem(50, 1, true, false, 24 );

        if  isChristmas == true then
            package:AddItem(401, 2, true, false, 24 );
            Broadcast(0x27, msg_68.."[p:"..player:getCountry()..":"..player:getPName().."]"..msg_90.."[4:".. 401 .."]x2");
        end
        player:OnShuoShuo(1)
    end

    if flag == 32 then -- 120
         package:AddItem(30, 1, true, false, 24 );
         if rand <= 20 then
             package:AddItem(514, 1, true, false, 24 );
         end
        if  isChristmas == true then
            package:AddItem(402, 1, true, false, 24 );
            package:AddItem(403, 1, true, false, 24 );
        end

    end

    if flag == 64 then -- 140
        package:Add(133, 1 , true, false , 24);
        if rand <= 20 then
            package:AddItem(514, 1, true, false, 24);
        end
    end

    if flag == 128 then -- 160
        package:AddItem(30, 2 , true, false , 24);
        package:AddItem(509, 1 , true, false , 24)
        if rand <= 20 then
            package:AddItem(515, 1, true, false, 24 );
        end
         if  isChristmas == true then
            package:AddItem(401, 3, true, false, 24 );
            Broadcast(0x27, msg_68.."[p:"..player:getCountry()..":"..player:getPName().."]"..msg_90.."[4:".. 401 .."]x3");
        end
    end

    if flag == 256 then -- 180
        package:AddItem(30, 2 , true, false , 24);
        package:AddItem(509, 1 , true, false , 24)
        if rand <= 20 then
            package:AddItem(515, 1, true, false, 24 );
        end
         if  isChristmas == true then
            package:AddItem(401, 3, true, false, 24 );
            Broadcast(0x27, msg_68.."[p:"..player:getCountry()..":"..player:getPName().."]"..msg_90.."[4:".. 401 .."]x3");
        end
    end

    mgr:AddRewardFlag(flag,true);
end

--获得活跃度
function doAty(player, id, param1 ,  param2)
--[[
    --print("ENTER");
    --print(id);
    local mgr = player:GetActivityMgr();
    local needflag =  checkFlag[id];
    local ap = addPoint[id];

    if ap == nil then
        return;
    end
    --print(ap);
    --print("checkTimeOver");
    --print(needflag);
    mgr:CheckTimeOver();
    if id == 13 then
        local VipLvl = player:getVipLevel()
        local count
        if VipLvl < 6 then
            count = 15
        else
            count = 20
        end
        if mgr:GetFlag(id) >= count then
            return
        end
    end
    --判断标志位
    if needflag ~= nil then
        local curflag = mgr:GetFlag(id);
        --print(curflag);
        if curflag >= needflag then
             return;
        else
            mgr:UpdateFlag(id, curflag + 1);
        end
    end
    --print("over");
    mgr:AddPoint(ap);
    mgr:UpdateToDB();
--]]
end

--处理玩家每日签到积分累加
function doAtySignIn(player)
    local chance = { 500, 1500, 4500, 7500, 8500, 9000, 9500, 9700, 9900, 10000 }
    local scores = { 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 }
    local rand = math.random(1, 10000)
    local score = 10
    for i = 1, #chance do
        if rand <= chance[i] then
            score = scores[i]
            break
        end
    end
    if score == 100 then
        Broadcast(0x27, "[p:"..player:getCountry()..":"..player:getPName().."]人品究极爆发，在活跃度中签到获得了100分！众生膜拜！")
    end
    return score
end

local exchangeProps = {
            --id,所需积分,数量,概率下限,概率上限(1~10000)
    [29]  = { 29,  10,  10,  1,    819   },
    [400] = { 400, 30,  3,   819,  1599  },
    [500] = { 500, 30,  1,   1599, 2389  },
    [510] = { 510, 30,  1,   2389, 3179  },
    [504] = { 504, 40,  1,   3179, 3919  },
    [503] = { 503, 30,  1,   3919, 4709  },
    [56]  = { 56,  120, 1,   4709, 5252  },
    [57]  = { 57,  130, 1,   5252, 5844  },
    [51]  = { 51,  40,  1,   5844, 6584  },
    [500] = { 500, 180, 1,   6584, 7078  },
    [514] = { 514, 160, 1,   7078, 7522  },
    [48]  = { 48,  80,  1,   7522, 8164  },
    [506] = { 506, 120, 1,   8164, 8707  },
    [508] = { 508, 180, 1,   8707, 9201  },
    [503] = { 503, 160, 1,   9201, 9645  },
    [507] = { 507, 330, 1,   9645, 9793  },
    [515] = { 515, 320, 1,   9793, 9842  },
    [509] = { 509, 300, 1,   9842, 10000 },
}

function GetExchangeProps(id) 
    if nil == id
    then
        return exchangeProps[29]
    end
    local props = exchangeProps[tonumber(id)]
    if nil == props 
    then
        return exchangeProps[29] 
    else
        return props
    end
end

function GetExchangePropsID()
    local ratio = math.random(1, 10000)
    for _, val in pairs(exchangeProps) do
       if val[4] <= ratio and ratio < val[5]
        then
            return val[1]
        end
    end
    return 29 
end

local dayExtraAward = {
    [1] = { --1月份
	    --日期   {id,数量},{id,数量}
		[1]  = { {509,1},{499,50} },
		[2]  = { {1325,1} },
		[3]  = { {134,1} },
		[5]  = { {503,1},{500,1} },
		[12] = { {516,1} },
		[19] = { {499,50} },
		[20] = { {517,1} },
		[27] = { {551,3} },
    },
    [2] = {         --2月份
    },
    [3] = {
		[9]  = { {9365,1} },
		[10]  = { {9365,1} },
		[11]  = { {9365,1} },
		[12]  = { {9365,1} },
		[13]  = { {9365,1} },
		[14]  = { {9365,1} },
		[15]  = { {9365,1} },
		[16]  = { {9365,1} },
		[17]  = { {9365,1} },
    },
    [4] = {
        [1]  = { {480,5} },
		[27] = { {10128,1} },
		[28] = { {10129,1} },
		[29] = { {10130,1} },
		[30] = { {10131,1} },
    },
    [5] = {
    },
    [6] = {
    },
    [7] = {         --7月份
    },
    [8] = { --8月份
		--日期   {id,数量},{id,数量}
		[1]  = { {500,5},{499,50} },
		[7]  = { {503,5},{499,50} },
		[13] = { {57,5},{499,50} },
		[17] = { {1325,2},{9094,5}},
		[18] = { {1325,2},{9094,5}},
		[19] = { {56,5},{499,50},{1325,2},{9094,5}},
		[20] = { {1325,2},{9094,5}},
		[21] = { {1325,2},{9094,5}},
		[22] = { {1325,2},{9094,5}},
		[23] = { {1325,2},{9094,5}},
		[25] = { {511,5},{499,50} },
		[31] = { {514,5},{499,50} },
    },
    [9] = {
        [6]  = { {503,5},{499,50} },
        [12] = { {57,5},{499,50} },
        [18] = { {56,5},{499,50} },
        [24] = { {511,5},{499,50} },
        [30] = { {514,5},{499,50} },
    },
    [10] = {
        [1]  = { {515,2},{499,20} },
        [2]  = { {503,5},{499,20} },
        [3]  = { {501,5},{499,20} },
        [4]  = { {503,5},{499,20} },
        [5]  = { {509,2},{499,20} },
        [6]  = { {134,2},{499,20} },
        [7]  = { {1325,2},{499,20} },
        [12] = { {57,5},{499,50} },
        [18] = { {56,5},{499,50} },
        [24] = { {511,5},{499,50} },
        [30] = { {514,5},{499,50} },	
    },
    [11] = {
		[1]  = { {499,50} },
        [2]  = { {503,1} },
        [3]  = { {509,1}},
        [4]  = { {517,1} },
        [5]  = { {507,1}},
        [6]  = { {499,30} },
        [7]  = { {1325,1} },
        [12] = { {57,5},{499,50} },
        [18] = { {56,5},{499,50} },
        [24] = { {511,5},{499,50} },
        [30] = { {514,5},{499,50} },
    },
    [12] = {
	    [1]  = { {9214,2},{499,50} },
		[2]  = { {503,1},{551,1} },
		[3]  = { {507,1} },
		[4]  = { {509,1} },
		[5]  = { {1325,1} },
		[6]  = { {134,1} },
        [7]  = { {9214,2},{499,50} },
        [13] = { {9214,2},{499,50} },
        [19] = { {9214,2},{499,50} },
        [25] = { {9214,2},{499,50} },
        [31] = { {9214,2},{499,50} },
	},
}

function GetdayExtraAward(month, day)
    if nil == month or nil == day
    then
        return {}
    end
    local mon_tbl = dayExtraAward[tonumber(month)]
    if nil == mon_tbl or nil == next(mon_tbl)
    then
        return {}
    end
    local award = mon_tbl[tonumber(day)]
    if nil == award or nil == next(award) 
    then 
        return {}
    end
    return award
end

