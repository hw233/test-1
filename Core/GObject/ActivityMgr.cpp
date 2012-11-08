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
#include "PracticePlace.h"
#include "AthleticsRank.h"
#include "GObject/Package.h"
#include "TeamCopy.h"
#include <stdio.h>
using namespace std;

namespace GObject
{
#define SIGNIN_RECORD 7
ActivityMgr::ActivityMgr(Player* p):_owner(p)
{

}
ActivityMgr::~ActivityMgr()
{
}

void ActivityMgr::LoadFromDB(DBActivityData& data)
{
    _item.overTime = data.overTime;
    _item.awardID  = data.awardID;
    _item.point    = data.point;
    _item.award    = data.award;
    _item.scores   = data.scores;
    _item.propsID  = data.propsID;
    {
        //解析flag
        StringTokenizer ntk(data.flags, "|");
        UInt32 size = std::min<UInt32>(ntk.count(), static_cast <UInt32>(AtyMaxFlag));

        for(UInt32 i = 0 ; i < size; i ++)
        {
            _item.flag[i] = atoi(ntk[i].c_str());
        }
    }
    {
        //解析签到记录(7天)
        StringTokenizer ntk(data.signRecord, "|");
        UInt8 size = ntk.count();
        for(UInt8 i = 0; i < size && i < SIGNIN_RECORD; ++i)
        {
            StringTokenizer tk(ntk[i].c_str(), ",");
            Sign s;
            s.day = atoi(tk[0].c_str());
            s.time = atoi(tk[1].c_str());
            _signRecord.push_back(s);
        }
    }
}
UInt32 ActivityMgr::GetRandomReward()
{
    UInt32 s = GetOnlineRewardNum();
    if(s == 0)
      return 1;
    UInt32 idx = uRand(s);
    return idx + 1;
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
    _item.Reset( 0  , over, _item.scores, _item.propsID);
    _onlineReward.clear() ;
    GetOnlineReward(GetOnlineRewardGetNum());
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
 * 添加每日签到积分
 *
*/
void ActivityMgr::AddScores(UInt32 v)
{
    if(v <= 0) return;
    _item.scores += v;
}

void ActivityMgr::SetPropsID(UInt32 id)
{
    if(id <= 0) return;
    _item.propsID = id;
}

/*
 *  兑换道具减少签到积分
 */
void ActivityMgr::SubScores(UInt32 v)
{
    if(v <= 0) return;
    _item.scores -= v;
    if(_item.scores < 0)
        _item.scores = 0;
}

/**
 * 更新数据库
 */
void ActivityMgr::UpdateToDB()
{
    std::string strFlag;
    std::string strSign;

    for (UInt32 i = 0; i < AtyMaxFlag; i ++)
    {
         strFlag += Itoa(_item.flag[i]);
         if(i != AtyMaxFlag - 1)
             strFlag += "|";
    }
    std::vector<Sign>::iterator it = _signRecord.begin();
    while(it != _signRecord.end())
    {
        strSign += Itoa((*it).day);
        strSign += ",";
        strSign += Itoa((*it).time);
        strSign += "|";
        it ++;
    }

    DB().PushUpdateData("REPLACE INTO `activityData` (`playerId`, `overTime`, `awardId`,`point`,`award`, `flags`, `scores`, `propsID`, `signRecord`) VALUES (%"I64_FMT"u, %u, %u, %u, %u, '%s', '%u', '%u', '%s')"  ,
            this->_owner->getId(), _item.overTime, _item.awardID, _item.point, _item.award, strFlag.c_str(), _item.scores, _item.propsID, strSign.c_str());
}

UInt8 ActivityMgr::GetOnlineRewardGetNum()
{
    UInt8 cnt = 0;
    for (UInt8 i = 0; i < 7; ++i)
    {
        if (_owner->GetVar(VAR_ATYITEM_1+i))
            ++cnt;
    }
    return cnt;
}

void ActivityMgr::GetOnlineReward(UInt8 cnt)
{
    lua_tinker::table t = GameAction()->GetOnlineReward(cnt);

    UInt32 size = t.size();
    for(UInt32 i = 0; i < size; i++)
    {
        RtyRewards v;
        lua_tinker::table t2 = t.get<lua_tinker::table>(i + 1);
        UInt32 s = t2.size();
        for(UInt32 j = 0; j < s; j++)
        {
            lua_tinker::table tt = t2.get<lua_tinker::table>(j + 1);
            if(tt.size() == 2)
            {
                stRtyReward r;
                r.num = tt.get<UInt8>(2);
                r.id = tt.get<UInt32>(1);
                v.push_back(r);
            }
        }

        if(s == 1)
            _onlineReward.push_back(v[0]);
        if(s >1)
        {
            UInt32 idx = uRand(s);
            if(idx < s)
                _onlineReward.push_back(v[idx]);
        }
    }
}

UInt32  ActivityMgr::GetOnlineRewardNum()
{
    UInt32 s = _onlineReward.size();
    if(s == 0)
    {
        GetOnlineReward(GetOnlineRewardGetNum());
        s = _onlineReward.size();
    }
     return s;
}
/**
 * 改变上线奖励
 */
void ActivityMgr::ChangeOnlineReward()
{
    // XXX: 取消这个功能 yangyoufa@ 01/04/12 14:11:39
    return;

    CheckTimeOver();
    //已经领取过了
    if(_item.award & AtyOnlineReward)
        return;

    if(_item.award & AtyOnlineFree)
    {
        //已经用去了免费
        if(_owner->getTael() < 200 )
        {
            _owner->sendMsgCode(0, 1100);
            return;
        }

        ConsumeInfo ci(DailyActivity,0,0);
        _owner->useTael(200, &ci);

    }
    else
    {
         AddRewardFlag( AtyOnlineFree , false);
    }
    //随机一个奖励
    _item.awardID = GetRandomReward();
    UpdateToDB();
    // to do 返回消息
    //
     Stream st(REP::ACTIVITY_REWARD);
     st<<static_cast<UInt8>( _item.awardID);
     st<<Stream::eos;
     _owner->send(st);
}
/**
 *  获取奖励
 */
void ActivityMgr::GetReward(UInt32 flag)
{
    CheckTimeOver();

    if(flag != AtyOnlineReward && _item.award & flag)
        return;

    if(flag == AtyOnlineReward)
    {
        if (_owner->GetPackage()->IsFull())
        {
            _owner->sendMsgCode(0, 1011);
            return;
        }

        if (_owner->GetVar(VAR_ATYITEM_1+World::_wday-1))
            return;

        UInt32 s =  _onlineReward.size();
        if(s == 0)
            return;

        UInt32 idx = GetRandomReward() - 1;
        if(idx >= s)
            idx = 0;
       //给奖励
        _owner->GetPackage()->Add(_onlineReward[idx].id, _onlineReward[idx].num, true, false, FromDailyActivity);
        _owner->SetVar(VAR_ATYITEM_1+World::_wday-1, _onlineReward[idx].id<<16|_onlineReward[idx].num);

        // AddRewardFlag(flag);
        ActivityList(0x01);
    }
    else
    {
        //交给脚本来做
        GameAction()->GetAtyReward(_owner, flag);
    }

    ActivityList(0x02);
}

void  ActivityMgr::AddRewardFlag(UInt32 flag, bool db)
{
    if(_item.award & flag)
        return;
    _item.award = _item.award | flag;
    if(db)
        UpdateToDB();
}

void ActivityMgr::SendOnlineReward(Stream& s)
{
    _onlineReward.clear();
    GetOnlineReward(GetOnlineRewardGetNum());

    for (UInt8 i = 0; i < 7; ++i)
    {
        UInt32 val = _owner->GetVar(VAR_ATYITEM_1+i);
        s << static_cast<UInt8>(val & 0xFFFF) << static_cast<UInt16>((val>>16) & 0xFFFF);
    }

    s << World::_wday;
    if (!_owner->GetVar(VAR_ATYITEM_1+World::_wday-1))
    {
        s << static_cast<UInt8>(_onlineReward.size());
        for(UInt32 i = 0 ; i < _onlineReward.size(); i ++ )
        {
            s << static_cast<UInt8>(_onlineReward[i].num) << static_cast<UInt32>(_onlineReward[i].id);
        }
    }
    else
    {
        s << static_cast<UInt8>(0);
    }
}
inline UInt8 GetAtyIDInClient(UInt32 item_enum)
{
    return  static_cast<UInt8>(item_enum + 1);
}

void ActivityMgr::SendActivityInfo(Stream& s)
{
    UInt8 c1, m1 = 0;
    UInt8 c2,  m2 = 0;
    UInt8 c3,  m3 = 0;

    UInt32 vipLevel = _owner->getVipLevel();
    //刷新活跃度
    c1 =static_cast<UInt8>( GameAction()->GetMaxActivity(vipLevel));
    s<< static_cast<UInt8>(_item.point) << c1;
    //刷新活跃度签到积分、待兑换道具id
    s<< static_cast<UInt32>(_item.scores) << static_cast<UInt8>(_item.flag[AtySignIn]);
    lua_tinker::table props = GameAction()->GetExchangeProps(_item.propsID);
    s<< static_cast<UInt16>(_item.propsID) << props.get<UInt8>(3) << props.get<UInt16>(2);
    //个数
    c1 = static_cast<UInt8>(AtyEnd - AtyBegin - 1 + AtyMaxFlag - 1); //跳过签到
    s<< c1;
   //printf("个数 %u  \n" ,c1);

    //各个各个项
    _owner->GetDailyTask(c1,m1,c2,m2,c3,m3);
    //printf("日常：%u,%u,||%u,%u,||%u,%u\n",c1,m1,c2,m2,c3,m3);
    s<< GetAtyIDInClient( AtyShimenTask) << c1 << m1;
    //printf("师门任务, %u  %u\n",c1 ,m1);

    s<< GetAtyIDInClient( AtyYamenTask) << c2 << m2;
    //printf("衙门, %u  %u\n",c1 ,m1);

    s<< GetAtyIDInClient( AtyClanTask) << c3 << m3;
    //printf("帮拍  %u  %u\n" , c3 ,m3);


    _owner-> GetFuben(c1,m1,c2,m2,c3,m3);
    //printf("副本：%u,%u,||%u,%u,||%u,%u\n",c1,m1,c2,m2,c3,m3);
    s<< GetAtyIDInClient( AtyCopy ) << c1 << m1;
    //printf("副本  %u  %u\n" , c1 ,m1);

    s<< GetAtyIDInClient( AtyDungeon)  << c2 << m2;
    //printf("通天塔  %u  %u\n" , c2 ,m2);

    s<< GetAtyIDInClient( AtyFormation ) << c3 << m3;
    //printf("阵图  %u  %u\n" , c3 ,m3);

    c1 = _owner->getIcCount();
    m1 =  Player::getMaxIcCount(vipLevel);
    s<< GetAtyIDInClient( AtyTaskHook ) << c1 << m1;
    //printf("挂机加速  %u  %u\n" , c1 ,m1);

    //c1 = _owner->getPlayerData().picCount;//_owner->getPIcCount();
    c1 = _owner->getPIcCount();
    m1 = PracticePlace::_picCnt[vipLevel];

    if(c1 <= m1)
        c1 = m1 - c1;
    s<< GetAtyIDInClient( AtyPSpeed ) << c1 << m1;
    //printf("修炼加速  %u  %u\n" , c1 ,m1);

    c1 = 0;
    m1 = 0;
    TeamCopyPlayerInfo* pInfo = _owner->getTeamCopyPlayerInfo();
    if(pInfo)
    {
        c1 = pInfo->getPassNum();
    }
    Fighter* fgt = _owner->getMainFighter();
    if(fgt)
    {
        UInt8 lev = fgt->getLevel();
        if(lev >= 35 && lev < 45)
            m1 = 2;
        else if(lev >= 45 && lev < 60)
            m1 = 4;
        else if(lev >= 60 && lev <70)
            m1 = 6;
        else if(lev >= 70 && lev < 80)
            m1 = 8;
        else if(lev >= 80 && lev < 90)
            m1 = 10;
        else if(lev >= 90 && lev < 100)
            m1 = 12;
        else if(lev >= 100)
            m1 = 14;
    }
    s<< GetAtyIDInClient( AtyGroupCopy) << c1 << m1;
    s << GetAtyIDInClient(AtyTownDeamon) << static_cast<UInt8>(_owner->GetVar(VAR_TOWNDEAMON)) << static_cast<UInt8>(1);
    s << GetAtyIDInClient(AtyShuoShuo) << static_cast<UInt8>(_owner->GetVar(VAR_SHUOSHUO)) << static_cast<UInt8>(1);
    s << GetAtyIDInClient(AtyInvited) << static_cast<UInt8>(_owner->GetVar(VAR_INVITED)) << static_cast<UInt8>(1);

    for(UInt32 i = 0 ; i <  AtyMaxFlag; i ++  )
    {
        c1 = static_cast<UInt8>(_item.flag[i]);
        if( i == AtyAthletics )
        {
            m1 = AthleticsRank::GetMaxchallengenum(vipLevel);
        }
#if 0
        else if( i == AtyBoss)
        {
            UInt8 lev =  worldBoss.getLevel();
            if(lev >= 40)
            {
                m1 = (lev - 40)/10 + 1;
            }
            else
                m1 = 0;
        }
#endif
        else
            m1 = static_cast<UInt8>(GameAction()->GetAtyCheckFlag(i));
        if( i == AtySignIn)  //跳过签到不发送
            continue;
        s<< GetAtyIDInClient(i ) << c1 << m1;
        //printf("项目%d : %u  %u\n" , i, c1 ,m1);
    }
    //奖励
    s<<static_cast<UInt16>(_item.award);
}

static const UInt32 ATY_ONLINE_TIME = 4*60*60;
/**
 *  刷新给客户端
 */
void ActivityMgr::ActivityList(UInt8 type)
{
    //自我刷新一下
    CheckTimeOver();

    UInt32 time = 0;
    //上线时间检查
    if(_item.flag[AtyLongTime]  == 0)
    {
        time = _owner->GetOnlineTimeToday();
        if(time >= ATY_ONLINE_TIME)
        {
            GameAction()->doAty(this->_owner, AtyLongTime, 0 , 0);
        }
    }
    Stream st(REP::ACTIVITY_LIST);
    st<<static_cast<UInt8> (type);
    if(type & 0x01)
    {
        SendOnlineReward(st);
    }
    if(type & 0x02)
    {
        SendActivityInfo(st);
    }

    if(type & 0x04)
    {
        if(time == 0)
            time = _owner->GetOnlineTimeToday();
        st<< static_cast<UInt32>(time);
    }
    st << Stream::eos;
    _owner->send(st);
}

void ActivityMgr::AddSignTime(UInt8 day)
{
   UInt32 now = TimeUtil::Now();
   if(_signRecord.size() >= SIGNIN_RECORD){
       std::vector<Sign>::iterator it = _signRecord.begin();
       while(it != _signRecord.end())
       {
            if(0 != day && (*it).day == day)
                return;
            it ++;
       }
       _signRecord.erase(_signRecord.begin());
   }
   Sign s;
   s.day = day;
   s.time = now;
   _signRecord.push_back(s);
}

}
