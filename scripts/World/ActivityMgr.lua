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
    [10] = 7,-- AtyBoss,      //世界BOSS
    [11] = 1,-- AtyCountryWar,//阵营战
    [12] = 5, -- AtyClanWar,   //帮派战
    [13] = 24,-- AtyAthletics, //斗剑
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
    [10]=  3, --AtyBoss,      //世界BOSS
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
}

--某个奖励需要的活跃度
local rewardNeedPoint = {
    [4] = 60,
    [8] = 80,
    [16] = 100,
    [32] = 120,
    [64] = 160,
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
local onlineReward = 
{
    { {502,1}},
    { {55,1}},
    { {29,20}},
    { {504, 1}},
    { {5001,1} ,  {5011, 1} , {5021, 1}, {5031, 1}, {5041, 1} } ,
    { {5061,1} ,  {5071, 1} , {5081, 1}, {5091, 1}, {5101, 1}, {5111 , 1 }, {5121, 1} } , 
    { {510,1}},
}
local christmasReward = {
    { { 403,1} },
    { { 403,2} },
    { { 403,3} },
    { { 402,1}},
    { { 402,2}},
    { { 402,3}},
    { { 401,1}},
}
local newYearReward = {
    { { 433,1}},
    { { 434,1}},
    { { 435,1}},
    { { 436,1}},
    { { 437,1}},
    { { 433,1}},
    { { 433,1}},
}
local valentineReward = 
{
    { {502,1}},
    { {55,1}},
    { {29,20}},
    { {504, 1}},
    { {438,1}},
    { {439,1}},
    { {510,1}},
}
--获取上线奖励
function GetOnlineReward()
    if getNewYear() then
        return newYearReward;
    end
    if getChristmas() then
        return christmasReward;
    end
    if getValentineDay() then
        return valentineReward;
    end
    return onlineReward;
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

    local rand = math.random(100);

    local isChristmas = getChristmas();
    if flag == 4 then
        local gemid =  getRandGem(1);
        package:AddItem(gemid, 1, true, false, 24 ); 
        if  isChristmas == true then
            package:AddItem(401, 1, true, false, 24 );
            Broadcast(0x27, "恭喜[p:"..player:getCountry()..":"..player:getPName().."]通过活跃奖励，获得了[4:".. 401 .."]x1");
        end
    end

    if flag == 8 then

        player:getTael(500);
        package:AddItem(502, 1, true, false, 24 );

         if  isChristmas == true then
            package:AddItem(402, 1, true, false, 24 );
        end

    end

    if flag == 16 then
       player:getCoupon(20); 
       package:AddItem(503, 1, true, false, 24 );

        if  isChristmas == true then
            package:AddItem(401, 2, true, false, 24 );
            Broadcast(0x27, "恭喜[p:"..player:getCountry()..":"..player:getPName().."]通过活跃奖励，获得了[4:".. 401 .."]x2");
        end

    end

    if flag == 32 then
         package:AddItem(30, 1, true, false, 24 );
         if rand <= 20 then
             package:AddItem(514, 1, true, false, 24 );
         end
        if  isChristmas == true then
            package:AddItem(402, 1, true, false, 24 );
            package:AddItem(403, 1, true, false, 24 );
        end

    end
    if flag == 64 then
        package:AddItem(30, 2 , true, false , 24);
        package:AddItem(509, 1 , true, false , 24)
        if rand <= 20 then
            package:AddItem(515, 1, true, false, 24 );
        end
         if  isChristmas == true then
            package:AddItem(401, 3, true, false, 24 );
            Broadcast(0x27, "恭喜[p:"..player:getCountry()..":"..player:getPName().."]通过活跃奖励，获得了[4:".. 401 .."]x3");
        end

    end
    mgr:AddRewardFlag(flag,true);
end

--获得活跃度
function doAty(player, id, param1 ,  param2)

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

end
