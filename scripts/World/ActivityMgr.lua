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
}

local addPoint = {
    [0] = 1,--AtyPractice,  --修炼
    [1] = 1,--AtyTripodFire,--炉火
    [2] = 1,--AtyBarRef,    --酒吧刷新
    [3] = 1,--AtyBookStore, --书店刷新
    [4] = 1,--AtyEnchant,   --装备强化
    [5] = 1,--AtySplit,     --装备分解
    [6] = 5,--AtyForge,     --装备洗练
    [7] = 1,--AtyBuy,       --购买物品
    [8] = 1,--AtyLongTime,  --在线时间足够长
    

    [1000] = 1, --AtyShimenTask = 1000,//师门任务
    [1001] = 1, --AtyYamenTask, //衙门任务
    [1002] = 1, --AtyClanTask,  //帮会任务
    [1003] = 5, --AtyDungeon,   //决战之地
    [1004] = 5, --AtyCopy,      //副本
    [1005] = 5, --AtyFormation, //阵图
    [1006] = 5, --AtyAthletics, //斗剑
    [1007] = 1, --AtyGroupCopy, //组队副本
    [1008] = 3, --AtyHeroIsland,//英雄岛
    [1009] = 3, --AtyBoss,      //世界BOSS
    [1010] = 3, --AtyCountryWar,//阵营战
    [1011] = 1, --AtyClanWar,   //帮派战
    [1012] = 1, --AtyTaskHook,  //挂机加速
    [1013] = 1, --AtyPSpeed,    //修炼加速
}

local actionTable = {
    [0] = AtyPractice,  --修炼
    [1] = AtyTripodFire,--炉火
    [2] = AtyBarRef,    --酒吧刷新
    [3] = AtyBookStore, --书店刷新
    [4] = AtyEnchant,   --装备强化
    [5] = AtySplit,     --装备分解
    [6] = AtyForge,     --装备洗练
    [7] = AtyBuy,       --购买物品
    [8] = AtyLongTime,  --在线时间足够长

}
local rewardNeedPoint = {
    [2] = 60,
    [4] = 80,
    [8] = 100,
    [16] = 120,
    [32] = 160,
}
local onlineReward = 
{
   1000,1001,1002,1003,1004,1005,1006,
}
function GetOnlineReward()
    return onlineReward;
end

function GetAtyReward(player, flag)
    local needPoint = rewardNeedPoint[flag];
    if needPoint == nil then
        return;
    end

    local mgr = player:GetActivityMgr();
    if mgr:GetPoint() < needPoint then
        return;
    end


    mgr:AddRewardFlag(flag);
end
function doAty(player, id, param1 ,  param2)

    print("ENTER");
    print(id);
    local mgr = player:GetActivityMgr();
    local needflag =  checkFlag[id];
    local ap = addPoint[id];

    if ap == nil then
        return;
    end
    print(ap);
    print("checkTimeOver");
    print(needflag);
    mgr:CheckTimeOver();
    --判断标志位
    if needflag ~= nil then
        local curflag = mgr:GetFlag(id);
        print(curflag);
        if curflag >= needflag then
             return;
        else
            mgr:UpdateFlag(id, curflag + 1);
        end
    end
    print("over");
    mgr:AddPoint(ap);
    mgr:UpdateToDB();

end
