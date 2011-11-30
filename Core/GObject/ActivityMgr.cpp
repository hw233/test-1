#include "Config.h"
#include "ActivityMgr.h"
#include "Player.h"
#include "Common/StringTokenizer.h"
#include "Common/Itoa.h"
#include "GObjectDBExecHelper.h"
#include "Common/Random.h"
#include "MsgHandler/CountryMsgStruct.h"
#include "Country.h"
#include "Script/GameActionLua.h"



namespace GObject
{
ActivityMgr::ActivityMgr(Player* p):_owner(p)
{

}
ActivityMgr::~ActivityMgr()
{
}

void ActivityMgr::LoadFromDB(DBActivityData& data)
{
    _item.overTime = data.overTime;
    _item.awardID = data.awardID;
    _item.point = data.point;
    _item.award = data.award;

    //解析flag
    StringTokenizer ntk(data.flags, "|");
    UInt32 size = std::min<UInt32>(ntk.count(), static_cast <UInt32>(AtyMaxFlag));

    for(UInt32 i = 0 ; i < size; i ++)
    {
        _item.flag[i] = atoi(ntk[i].c_str());
    }
}
UInt32 ActivityMgr::GetRandomReward()
{
    std::vector<UInt32> v = GetOnlineReward();
    UInt32 s = v.size();
    UInt32 idx = uRand(s);
    return idx;
    /*
    if(idx < s )
    {
         return v[idx];
    }
    return 0;*/
}
/**
 *   @return  是否到期 需要重置数据库
 *
 */
bool ActivityMgr::CheckTimeOver()
{
    UInt32 now = TimeUtil::Now();
    if(now < _item.overTime)
        return false;
    UInt32 over = TimeUtil::SharpDayT(1 , now);
    UInt32 rewardID = GetRandomReward();
     _item.Reset(rewardID , over);
    UpdateToDB();
    return true;
}
/**
 *  获取标志位
 *
 */
UInt8 ActivityMgr::GetFlag(UInt32 idx )
{
    if( idx >= AtyMaxFlag)
        return 0xFF;
    else
        return _item.flag[idx];
}
/** 
 *  更新标志位
 */
void ActivityMgr::UpdateFlag(UInt32 idx,  UInt8 v)
{
    if(idx >=  AtyMaxFlag)
        return ;

    _item.flag[idx] = v;
}

/**
 * 添加活跃点数
 *
 */
void ActivityMgr::AddPoint(UInt32 v)
{
    _item.point += v;
}

/**
 * 更新数据库
 */
void ActivityMgr::UpdateToDB()
{
    std::string strFlag;

    for (UInt32 i = 0; i < AtyMaxFlag; i ++)
    {
         strFlag += Itoa(_item.flag[i]);
         if(i != AtyMaxFlag - 1)
             strFlag += "|";
    }

    DB().PushUpdateData("REPLACE INTO `activityData` (`playerId`, `overTime`, `awardId`,`point`,`award`, `flags`) VALUES (%"I64_FMT"u, %u, %u, %u, %u, '%s')"  ,
            this->_owner->getId(), _item.overTime,    _item.awardID,    _item.point, _item.award, strFlag.c_str());
}

std::vector<UInt32> ActivityMgr::GetOnlineReward()
{
    lua_tinker::table t = GameAction()->GetOnlineReward();

    std::vector<UInt32> v;
    UInt32 size = t.size();
    for(UInt32 i = 0; i< size; i++)
    {
        v.push_back(t.get<UInt32>(i+1));
    }
    return v;
}
/**
 * 改变上线奖励
 */
void ActivityMgr::ChangeOnlineReward()
{
    if(_item.award & AtyOnlineReward)
        return;

    if(_owner->getTael() < 200 )
    {
        _owner->sendMsgCode(0, 1100);
        return ;
    }
    _item.awardID = GetRandomReward();
    UpdateToDB();
}
/**
 *  获取奖励 
 */
void ActivityMgr::GetReward(UInt32 flag)
{
    if(_item.award & flag)
        return;

    if(flag == AtyOnlineReward)
    {
        std::vector<UInt32> v = GetOnlineReward() ;
        UInt32 s = v.size();
        if(s == 0)
            return;

        UInt32 idx = _item.awardID;
        if(idx >= s)
            idx = 0;

        //给奖励
        AddRewardFlag(flag);
    }
    else
    {
        //交给脚本来做
        GameAction()->GetAtyReward(_owner, flag);
    }
}

void  ActivityMgr::AddRewardFlag(UInt32 flag)
{
    if(_item.award & flag)
        return;
    _item.award = _item.award | flag;
    UpdateToDB();
}
}
