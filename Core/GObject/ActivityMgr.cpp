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
ActivityMgr::ActivityMgr(Player* p):_owner(p)
{

}
ActivityMgr::~ActivityMgr()
{
}

void ActivityMgr::LoadFromDB(DBActivityData& data)
{
    _item.overTime = data.overTime;
    _item.scores   = data.scores;
    _item.propsID  = data.propsID;
/**
    _item.awardID  = data.awardID;
    _item.point    = data.point;
    _item.award    = data.award;
    //解析flag
    StringTokenizer ntk(data.flags, "|");
    UInt32 size = std::min<UInt32>(ntk.count(), static_cast <UInt32>(AtyMaxFlag));

    for(UInt32 i = 0 ; i < size; i ++)
    {
        _item.flag[i] = atoi(ntk[i].c_str());
    }
 */
    //解析签到记录(两个月)
    StringTokenizer tk(data.signRecord, "|");
    UInt8 cnt = tk.count();
    for(UInt8 idx = 0; idx < cnt && idx < MONTH_COUNT; ++idx)
    {
        StringTokenizer ntk(tk[idx].c_str(), ",");
        UInt8 size = ntk.count();
        for(UInt8 i = 0; i < size && i < SIGNIN_RECORD; ++i)
        {
            _item.signRecord[idx][i] = atoi(ntk[i].c_str());
        }
        //SetOneDayRecord(i + 1, atoi(tk[i].c_str()));
    }
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
    /*
    UInt32 over = TimeUtil::SharpDayT(1 , now);
    _item.Reset( 0, over, _item.scores, _item.propsID);
    _onlineReward.clear() ;
    GetOnlineReward(GetOnlineRewardGetNum());
    */
    UInt32 over = TimeUtil::SharpMonth(1, now);
    _item.Reset(over, _item.scores, _item.propsID);
    UpdateToDB();
    return true;
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

UInt16 ActivityMgr::GetOneDayRecord(UInt8 day)
{
    if(day <= 0 || day > SIGNIN_RECORD)
        return 0;
    return _item.signRecord[1][day-1];
}

void ActivityMgr::SetOneDayRecord(UInt8 day, UInt16 score)
{
    if(score < 0)
        return;
    if(day <= 0 || day > SIGNIN_RECORD)
        return;
    _item.signRecord[1][day-1] = score;
}

/**
 * 更新数据库
 */
void ActivityMgr::UpdateToDB()
{
    /*
    std::string strFlag;
    for (UInt32 i = 0; i < AtyMaxFlag; i ++)
    {
         strFlag += Itoa(_item.flag[i]);
         if(i != AtyMaxFlag - 1)
             strFlag += "|";
    }
    */
    std::string strSign;
    for (int i = 0; i < MONTH_COUNT; i ++)
    {
        for (int j = 0; j < SIGNIN_RECORD; j ++)
        {
            strSign += Itoa(_item.signRecord[i][j]);
            if(j < SIGNIN_RECORD - 1)
                strSign += ",";
        }
        if(i < MONTH_COUNT - 1)
            strSign += "|";
    }

    //DB().PushUpdateData("REPLACE INTO `activityData` (`playerId`, `overTime`, `awardId`,`point`,`award`, `flags`, `scores`, `propsID`, `signRecord`) VALUES (%"I64_FMT"u, %u, %u, %u, %u, '%s', '%u', '%u', '%s')"  ,
    //        this->_owner->getId(), _item.overTime, _item.awardID, _item.point, _item.award, strFlag.c_str(), _item.scores, _item.propsID, strSign.c_str());
    DB().PushUpdateData("REPLACE INTO `activityData` (`playerId`, `overTime`, `scores`, `propsID`, `signRecord`) VALUES (%"I64_FMT"u, %u, %u, %u, '%s')"  ,
            this->_owner->getId(), _item.overTime, _item.scores, _item.propsID, strSign.c_str());
}

void ActivityMgr::SendActivityInfo()
{
    Stream st(REP::ACTIVITY_SIGNIN);
    st << static_cast<UInt8>(0x03);
    //刷新活跃度签到积分、待兑换道具id
    st << static_cast<UInt32>(_item.scores);
    lua_tinker::table props = GameAction()->GetExchangeProps(_item.propsID);
    st << static_cast<UInt16>(_item.propsID) << props.get<UInt8>(3) << props.get<UInt16>(2);
    UInt32 day = 1;
    UInt32 mon = 1;
    UInt32 year = 2012;
    TimeUtil::GetDMY(&day, &mon, &year);
    UInt8 dayCnt = TimeUtil::GetOneMonthDays();
    st << static_cast<UInt8>(dayCnt);
    for(UInt8 i = 0; i < dayCnt; ++i)
    {
        st << static_cast<UInt16>(GetOneDayRecord(i + 1));
        st << GetContinueSignInCnt(i + 1);
        lua_tinker::table award = GameAction()->GetdayExtraAward(mon, i + 1);
        UInt8 size = award.size();
        st << size;
        for(UInt8 j = 0; j < size; ++j)
        {
            lua_tinker::table a = award.get<lua_tinker::table>(j + 1);
            st << a.get<UInt16>(1) << a.get<UInt8>(2);
        }
    }
    st << Stream::eos;
    _owner->send(st);
}

UInt8 ActivityMgr::GetContinueSignInCnt(UInt8 day)
{   //最多连续签到7天
    UInt8 cnt = 0;
    if(day < 7)
    {
        for(UInt32 i = day; i > 0; i--)
        {
            if(GetOneDayRecord(i) <= 0)
                break;
            cnt++;
        }
        if(cnt == day)
        {
            UInt8 dayCnt = TimeUtil::GetOneMonthDays(TimeUtil::Now() - 7 * 86400);
            for(UInt8 j = dayCnt; j > dayCnt - 7 + day; --j)
            {
                if(_item.signRecord[0][j-1] <= 0)
                    break;
                cnt++;
            }
        }
    }
    else
    {
        for(UInt32 i = day, j = 0; i > 0 && j < 7; i--, j++)
        {
            if(GetOneDayRecord(i) <= 0)
                break;
            cnt++;
        }
    }
    return cnt;
}

//玩家每日签到接口
void ActivityMgr::ActivitySignIn()
{
    UInt32 day = 1;
    UInt32 mon = 1;
    UInt32 year = 2012;
    TimeUtil::GetDMY(&day, &mon, &year);
    if(GetOneDayRecord(day) > 0)
        return;
    lua_tinker::table award = GameAction()->GetdayExtraAward(mon, day);
    UInt32 size = award.size();
    if(0 != size)
    {
        if(_owner->GetPackage()->GetRestPackageSize() < size)
        {    //背包预留足够的位子,否则不能签到
            _owner->sendMsgCode(0, 1011);
            return;
        }
        for(UInt32 i = 0; i < size; ++i)
        {
            lua_tinker::table a = award.get<lua_tinker::table>(i + 1);
            if(499 == a.get<UInt32>(1))  //礼券
                _owner->getCoupon(a.get<UInt32>(2));
            else
                _owner->GetPackage()->Add(a.get<UInt32>(1), a.get<UInt32>(2), true, false, FromDailyActivity);
        }
    }
    UInt16 score = GameAction()->doAtySignIn(_owner);
    if(World::_wday == 7) //周日双倍
        score *= 2;
    SetOneDayRecord(day, score);
    UInt8 cnt = GetContinueSignInCnt(day);
    if(cnt == 2)
        score += 1;
    if(cnt == 3)
        score += 2;
    if(cnt == 4)
        score += 5;
    if(cnt == 5)
        score += 10;
    if(cnt == 6)
        score += 15;
    if(cnt >= 7)
        score += 20;
    AddScores(score);
    UpdateToDB();
    _owner->activityUdpLog(1025);
    Stream st(REP::ACTIVITY_SIGNIN);
    st << static_cast<UInt8>(0x00);
    st << static_cast<UInt32>(GetScores());
    st << static_cast<UInt16>(GetOneDayRecord(day));
    st << cnt << Stream::eos;
    _owner->send(st);
}

//刷新待兑换的道具
void ActivityMgr::RefreshProps()
{
    if(!_owner->hasChecked())
        return;
    if(_owner->getTael() < 100)
    {
        _owner->sendMsgCode(0, 1100);
        return;
    }
    ConsumeInfo ci(DailyActivity, 0, 0);
    _owner->useTael(100, &ci);
    UInt32 id = GameAction()->GetExchangePropsID();
    if(GetPropsID() == id)
    {
        switch(id)
        {
            case 29:
                id = 500;
                break;
            case 500:
                id = 29;
                break;
            default:
                id = 29;
                break;
        }
    }
    SetPropsID(id);
    UpdateToDB();
    _owner->activityUdpLog(1026);

    Stream st(REP::ACTIVITY_SIGNIN);
    st << static_cast<UInt8>(0x01);
    lua_tinker::table p = GameAction()->GetExchangeProps(id);
    st << static_cast<UInt16>(id) << p.get<UInt8>(3) << p.get<UInt16>(2);
    st << Stream::eos;
    _owner->send(st);
}

//积分兑换道具
void ActivityMgr::ExchangeProps()
{
    if(!_owner->hasChecked())
        return;
    lua_tinker::table props = GameAction()->GetExchangeProps(GetPropsID());
    if(5 != props.size())
        return;
    UInt32 score = props.get<UInt32>(2);
    if(GetScores() < score)
        return;
    if(_owner->GetPackage()->GetRestPackageSize() <= 0)
    {
        _owner->sendMsgCode(0, 1011);
        return;
    }
    SubScores(score);
    _owner->GetPackage()->Add(GetPropsID(), props.get<UInt8>(3), true, false, FromDailyActivity);
    _owner->activityUdpLog(1027, score);
    _owner->activityUdpLog(1028, score);
    //兑换后重新刷新一次
    UInt32 id = GameAction()->GetExchangePropsID();
    if(GetPropsID() == id)
    {
        switch(id)
        {
            case 29:
                id = 500;
                break;
            case 500:
                id = 29;
                break;
            default:
                id = 29;
                break;
        }
    }
    SetPropsID(id);
    UpdateToDB();
    lua_tinker::table p = GameAction()->GetExchangeProps(id);
    Stream st(REP::ACTIVITY_SIGNIN);
    st << static_cast<UInt8>(0x02);
    st << GetScores() << static_cast<UInt16>(id) << p.get<UInt8>(3) << p.get<UInt16>(2);
    st << Stream::eos;
    _owner->send(st);
}

/*
UInt32 ActivityMgr::GetRandomReward()
{
    UInt32 s = GetOnlineRewardNum();
    if(s == 0)
      return 1;
    UInt32 idx = uRand(s);
    return idx + 1;
}
*/
/**
 *  获取标志位
 *
UInt8 ActivityMgr::GetFlag(UInt32 idx )
{
    if( idx >= AtyMaxFlag)
        return 0xFF;
    else
        return _item.flag[idx];
}
 */
/**
 *  更新标志位
void ActivityMgr::UpdateFlag(UInt32 idx,  UInt8 v)
{
    if(idx >=  AtyMaxFlag)
        return ;

    _item.flag[idx] = v;
}
 */

/**
 * 添加活跃点数
 *
void ActivityMgr::AddPoint(UInt32 v)
{
    _item.point += v;
}
 */
/*
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
 */
/**
 * 改变上线奖励
 */
/**
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
 */
/**
 *  获取奖励
 */
/**
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
 */
/**
 *  刷新给客户端
 */
/**
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
 */


}
