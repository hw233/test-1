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
#include "DaysRank.h"
#include "ShuoShuo.h"
#include <stdio.h>

using namespace std;

namespace GObject
{
StrengthenMgr::StrengthenMgr(Player* p):_owner(p)
{

}
StrengthenMgr::~StrengthenMgr()
{
}

void StrengthenMgr::LoadFromDB(DBStrengthenData& data)
{
    _item.overTime = data.overTime;
    _item.souls = data.souls;
    _item.soulId = data.soulId;
    //解析flag
    StringTokenizer ntk(data.flags, "|");
    UInt32 size = std::min<UInt32>(ntk.count(), static_cast <UInt32>(SthMaxFlag));
    for(UInt32 i = 0 ; i < size; i ++)
    {
        _item.flag[i] = atoi(ntk[i].c_str());
    }
    //解析绿色宝箱开出物品id
    StringTokenizer ntkG(data.greenBox, "|");
    for(size_t idx = 0; idx < ntkG.count(); idx ++)
    {
        StringTokenizer tk(ntkG[idx].c_str(), ",");
        for(size_t i = 0; i < tk.count(); ++i)
        {
            _item.greenBox[idx][i] = atoi(tk[i].c_str());
        }
    }
    //解析蓝色宝箱开出物品id
    StringTokenizer ntkB(data.blueBox, "|");
    for(size_t idx = 0; idx < ntkB.count(); idx ++)
    {
        StringTokenizer tk(ntkB[idx].c_str(), ",");
        for(size_t i = 0; i < tk.count(); ++i)
        {
            _item.blueBox[idx][i] = atoi(tk[i].c_str());
        }
    }
    //解析紫色宝箱开出物品id
    StringTokenizer ntkP(data.purpleBox, "|");
    for(size_t idx = 0; idx < ntkP.count(); idx ++)
    {
        StringTokenizer tk(ntkP[idx].c_str(), ",");
        for(size_t i = 0; i < tk.count(); ++i)
        {
            _item.purpleBox[idx][i] = atoi(tk[i].c_str());
        }
    }
    //解析橙色宝箱开出物品id
    StringTokenizer ntkO(data.orangeBox, "|");
    for(size_t idx = 0 ; idx < ntkO.count(); idx ++)
    {
        StringTokenizer tk(ntkO[idx].c_str(), ",");
        for(size_t i = 0; i < tk.count(); ++i)
        {
            _item.orangeBox[idx][i] = atoi(tk[i].c_str());
        }
    }
}

bool StrengthenMgr::CheckTimeOver(UInt32 now)
{
    //UInt32 now = TimeUtil::Now();
    if(now < _item.overTime)
        return false;
    UInt32 over = TimeUtil::SharpDayT(1 , now);
    _item.Reset(over);
    UpdateToDB();
    return true;
}

UInt8 StrengthenMgr::GetFlag(UInt8 idx)
{
    if(idx >= SthMaxFlag)
        return 0xFF;
    else
        return _item.flag[idx];
}

void StrengthenMgr::AddSouls(UInt8 v)
{
    _item.souls += v;
    CheckeFixedSouls(v);
}

void StrengthenMgr::SendFlickerToClient()
{
    Stream st(REP::STRENGTHEN_LIST);
    st << static_cast<UInt8>(0x05) << GetSouls();
    st << Stream::eos;
    _owner->send(st);
}

void StrengthenMgr::CheckeFixedSouls(UInt8 v)
{
    UInt8 s = GetSouls();
    if(s - v < 25 && s >= 25)
    {
        _owner->activityUdpLog(1025, 9);    //布点统计
        SendFlickerToClient();
    }
    if(s - v < 50 && s >= 50)
    {
        _owner->activityUdpLog(1025, 10);
        SendFlickerToClient();
    }
    if(s - v < 75 && s >= 75) 
    {
        _owner->activityUdpLog(1025, 11);
        SendFlickerToClient();
    }
    if(s - v < 100 && s >= 100) 
    {
        _owner->activityUdpLog(1025, 12);
        SendFlickerToClient();
    }
}

void StrengthenMgr::UpdateFlag(UInt8 idx,  UInt8 v)
{
    if(idx >= SthMaxFlag)
        return ;
    _item.flag[idx] = v;
}

void StrengthenMgr::UpdateToDB()
{
    std::string strFlag;
    std::string green;
    std::string blue;
    std::string purple;
    std::string orange;

    for(UInt32 i = 0; i < SthMaxFlag; i ++)
    {
         strFlag += Itoa(_item.flag[i]);
         if(i != SthMaxFlag - 1)
             strFlag += "|";
    }
    for(UInt8 i = 0; i < STRONGTYPE_CNT; i ++)
    {
        for(UInt8 id = 0; id < STRONGBOX_CNT; ++id)
        {
            green += Itoa(_item.greenBox[i][id]);
            if(id < STRONGBOX_CNT - 1)
                green += ",";
        }
        if(i < STRONGTYPE_CNT - 1)
            green += "|";
    }
    for(UInt8 i = 0; i < STRONGTYPE_CNT; i ++)
    {
        for(UInt8 id = 0; id < STRONGBOX_CNT; ++id)
        {
            blue += Itoa(_item.blueBox[i][id]);
            if(id < STRONGBOX_CNT - 1)
                blue += ",";
        }
        if(i < STRONGTYPE_CNT - 1)
            blue += "|";
    }
    for(UInt8 i = 0; i < STRONGTYPE_CNT; i ++)
    {
        for(UInt8 id = 0; id < STRONGBOX_CNT; ++id)
        {
            purple += Itoa(_item.purpleBox[i][id]);
            if(id < STRONGBOX_CNT - 1)
                purple += ",";
        }
        if(i < STRONGTYPE_CNT - 1)
            purple += "|";
    }
    for(UInt8 i = 0; i < STRONGTYPE_CNT; i ++)
    {
        for(UInt8 id = 0; id < STRONGBOX_CNT; ++id)
        {
            orange += Itoa(_item.orangeBox[i][id]);
            if(id < STRONGBOX_CNT - 1)
                orange += ",";
        }
        if(i < STRONGTYPE_CNT - 1)
            orange += "|";
    }
    DB().PushUpdateData("REPLACE INTO `strengthenData` (`playerId`, `overTime`, `souls`, `soulId`, `flags`, `greenBox`, `blueBox`, `purpleBox`, `orangeBox`)VALUES (%"I64_FMT"u, %u, %u, %u, '%s', '%s', '%s', '%s', '%s')",
            this->_owner->getId(), _item.overTime, _item.souls, _item.soulId, strFlag.c_str(), green.c_str(), blue.c_str(), purple.c_str(), orange.c_str());
}

bool StrengthenMgr::CanOpenGreenBox(UInt8 type, UInt32 onlineTime)
{
    if(GetSouls() < 25)
        return false;
    UInt32 vipLevel = _owner->getVipLevel();
    if(type && vipLevel < 2)
        return false;
#define TIME 3600
    UInt8 cnt = 0;
    for(int i = 0; i < STRONGBOX_CNT; ++i){
        if(_item.greenBox[type][i] > 0)
            ++cnt;
    }
    if(onlineTime >= 1 * TIME && onlineTime < 2 * TIME)
        return cnt < 2;
    if(onlineTime >= 2 * TIME && onlineTime < 3 * TIME)
        return cnt < 3;
    if(onlineTime >= 3 * TIME && onlineTime < 4 * TIME)
        return cnt < 4;
    if(onlineTime >= 4 * TIME && onlineTime < 5 * TIME)
        return cnt < 5;
    if(onlineTime >= 5 * TIME)
        return cnt < 6;
    return cnt < 1;
}

bool StrengthenMgr::CanOpenBlueBox(UInt8 type, UInt32 useTael)
{
    if(GetSouls() < 50)
        return false;
    UInt32 vipLevel = _owner->getVipLevel();
    if(type && vipLevel < 4)
        return false;
#define TAEL 3000
    UInt8 cnt = 0;
    for(int i = 0; i < STRONGBOX_CNT; ++i){
        if(_item.blueBox[type][i] > 0)
            ++cnt;
    }
    if(useTael >= 1 * TAEL && useTael < 2 * TAEL)
        return cnt < 2;
    if(useTael >= 2 * TAEL && useTael < 3 * TAEL)
        return cnt < 3;
    if(useTael >= 3 * TAEL && useTael < 4 * TAEL)
        return cnt < 4;
    if(useTael >= 4 * TAEL && useTael < 5 * TAEL)
        return cnt < 5;
    if(useTael >= 5 * TAEL)
        return cnt < 6;
    return cnt < 1;
}

bool StrengthenMgr::CanOpenPurpleBox(UInt8 type, UInt32 useCoupon)
{
    if(GetSouls() < 75)
        return false;
    UInt32 vipLevel = _owner->getVipLevel();
    if(type && vipLevel < 6)
        return false;
#define COUPON 30
    UInt8 cnt = 0;
    for(int i = 0; i < STRONGBOX_CNT; ++i){
        if(_item.purpleBox[type][i] > 0)
            ++cnt;
    }
    if(useCoupon >= 1 * COUPON && useCoupon < 2 * COUPON)
        return cnt < 2;
    if(useCoupon >= 2 * COUPON && useCoupon < 3 * COUPON)
        return cnt < 3;
    if(useCoupon >= 3 * COUPON && useCoupon < 4 * COUPON)
        return cnt < 4;
    if(useCoupon >= 4 * COUPON && useCoupon < 5 * COUPON)
        return cnt < 5;
    if(useCoupon >= 5 * COUPON)
        return cnt < 6;
    return cnt < 1;
}

bool StrengthenMgr::CanOpenOrangeBox(UInt8 type, UInt32 useGold)
{
    if(GetSouls() < 100)
        return false;
    UInt32 vipLevel = _owner->getVipLevel();
    if(type && vipLevel < 8)
        return false;
#define GOLD 30
    UInt8 cnt = 0;
    for(int i = 0; i < STRONGBOX_CNT; ++i){
        if(_item.orangeBox[type][i] > 0)
            ++cnt;
    }
    if(useGold >= 1 * GOLD && useGold < 2 * GOLD)
        return cnt < 2;
    if(useGold >= 2 * GOLD && useGold < 3 * GOLD)
        return cnt < 3;
    if(useGold >= 3 * GOLD && useGold < 4 * GOLD)
        return cnt < 4;
    if(useGold >= 4 * GOLD && useGold < 5 * GOLD)
        return cnt < 5;
    if(useGold >= 5 * GOLD)
        return cnt < 6;
    return cnt < 1;
}

void StrengthenMgr::SendStrengthenInfo()
{
    UInt8 vipLevel = _owner->getVipLevel();
    Stream st(REP::STRENGTHEN_LIST);
    st << static_cast<UInt8>(0x01);
    //当日使用变强之吼功能的id
    st << _item.soulId;
    //刷新变强之魂数值
    st << _item.souls;
    //个数(35个)
    st << static_cast<UInt8>(SthMaxFlag);
    //各个项
    for(UInt8 idx = 0; idx < SthMaxFlag; ++idx)
    {
        UInt8 maxFlag = GameAction()->GetSthCheckFlag(idx);
        if(idx == SthHookSpeed) //挂机加速
            maxFlag = _owner->getMaxIcCount(vipLevel);
        if(idx == SthPSpeed) //修炼加速
            maxFlag = PracticePlace::_picCnt[vipLevel];
        if(idx == SthCopy || idx == SthDungeon || idx == SthFormation)
        {   //副本、决战之地、阵图
            UInt8 c1,m1 = 0;
            UInt8 c2,m2 = 0;
            UInt8 c3,m3 = 0;
            _owner->GetFuben(c1,m1,c2,m2,c3,m3);
            if(idx == SthCopy) //副本
                maxFlag = m1;
            if(idx == SthDungeon) //决战之地
                maxFlag = m2;
            if(idx == SthFormation) //阵图
                maxFlag = m3;
        }
        if(idx == SthGroupCopy)
        {
            Fighter* fgt = _owner->getMainFighter();
            if(fgt)
            {
                UInt8 lev = fgt->getLevel();
                maxFlag = 0;
                if(lev >= 30 && lev < 45)
                    maxFlag = 2;
                else if(lev >= 45 && lev < 60)
                    maxFlag = 4;
                else if(lev >= 60 && lev <70)
                    maxFlag = 6;
                else if(lev >= 70 && lev < 80)
                    maxFlag = 8;
                else if(lev >= 80 && lev < 90)
                    maxFlag = 10;
                else if(lev >= 90 && lev < 100)
                    maxFlag = 12;
                else if(lev >= 100)
                    maxFlag = 14;
            }
        }
        st << idx << _item.flag[idx] << maxFlag;
    }
    st << Stream::eos;
    _owner->send(st);
}

void StrengthenMgr::EveryDayRoar()
{
    if(GetSoulId())
        return;
    UInt8 soul = GameAction()->EveryDayRoarSoul();
    SetSoulId(soul);
    AddSouls(soul);
    UpdateToDB();
    Stream st(REP::STRENGTHEN_LIST);
    st << static_cast<UInt8>(0x04);
    st << _item.soulId;
    st << _item.souls;
    st << Stream::eos;
    _owner->send(st);
}

void StrengthenMgr::SendStrengthenRank()
{
    Stream st(REP::STRENGTHEN_LIST);
    st << static_cast<UInt8>(0x02);
    //需要获取10天的斗剑、等级、财富、PK、大神排名
    PlayerRankInfo* pInfo = GObject::DaysRank::instance().getDaysRank(_owner->getId());
    UInt8 cnt = 0;
    for(int j = 0; j < DAYS_RANK_COUNT; ++j)
    {
        if(pInfo && pInfo->rank[4][j] > 0)
            ++ cnt;
    }
    st << static_cast<UInt8>(cnt);
    if(pInfo)
    {
        for(int i = 0; i < DAYS_RANK_TYPE; ++i)
        {
            for(int j = DAYS_RANK_COUNT - cnt; j < DAYS_RANK_COUNT; ++j)
            {
                st << static_cast<UInt32>(pInfo->rank[i][j]);
            }
        }
    }
    st << Stream::eos;
    _owner->send(st);
}

void StrengthenMgr::SendOpenChestsInfo(UInt8 boxId, UInt8 index)
{
    bool toDB = false;
    Stream st(REP::STRENGTHEN_LIST);
    st << static_cast<UInt8>(0x03) << boxId;
    switch(boxId)
    {
        case 1: //绿箱
            {
                UInt32 time = _owner->GetOnlineTimeToday();
                st << time;
                if(index > 0 && index <= STRONGBOX_CNT)
                {
                    if(_item.greenBox[0][index-1])
                        return;
                    if(CanOpenGreenBox(0, time))
                    {
                        if(_owner->GetPackage()->GetRestPackageSize() < 1)
                        {
                            _owner->sendMsgCode(0, 1011);
                            return;
                        }
                        lua_tinker::table item = GameAction()->openGreenBoxStrong();
                        if(COUPON_ID == item.get<UInt32>(1))
                            _owner->getCoupon(item.get<UInt32>(2));
                        else
                            _owner->GetPackage()->Add(item.get<UInt32>(1), item.get<UInt32>(2), true, false, FromDailyStrengthen);
                        _item.greenBox[0][index-1] = item.get<UInt32>(1) * 100 + item.get<UInt32>(2);
                        toDB = true;
                        _owner->activityUdpLog(1025, 1);
                    }
                }
                for(int i = 0; i < STRONGBOX_CNT; ++i)
                {
                    st << static_cast<UInt32>(_item.greenBox[0][i]); 
                }
            }
            break;
        case 2: //蓝箱
            {
                UInt32 tael = _owner->GetVar(VAR_USETAEL_CNT);
                st << tael;
                if(index > 0 && index <= STRONGBOX_CNT)
                {
                    if(_item.blueBox[0][index-1])
                        return;
                    if(CanOpenBlueBox(0, tael))
                    {
                        if(_owner->GetPackage()->GetRestPackageSize() < 1)
                        {
                            _owner->sendMsgCode(0, 1011);
                            return;
                        }
                        lua_tinker::table item = GameAction()->openBlueBoxStrong();
                        if(COUPON_ID == item.get<UInt32>(1))
                            _owner->getCoupon(item.get<UInt32>(2));
                        else
                            _owner->GetPackage()->Add(item.get<UInt32>(1), item.get<UInt32>(2), true, false, FromDailyStrengthen);
                        _item.blueBox[0][index-1] = item.get<UInt32>(1) * 100 + item.get<UInt32>(2);
                        toDB = true;
                        _owner->activityUdpLog(1025, 2);
                    }
                }
                for(int i = 0; i < STRONGBOX_CNT; ++i)
                {
                    st << static_cast<UInt32>(_item.blueBox[0][i]); 
                }
            }
            break;
        case 3: //紫箱
            {
                UInt32 coupon = _owner->GetVar(VAR_USECOUPON_CNT);
                st << coupon;
                if(index > 0 && index <= STRONGBOX_CNT)
                {
                    if(_item.purpleBox[0][index-1])
                        return;
                    if(CanOpenPurpleBox(0, coupon))
                    {
                        if(_owner->GetPackage()->GetRestPackageSize() < 1)
                        {
                            _owner->sendMsgCode(0, 1011);
                            return;
                        }
                        lua_tinker::table item = GameAction()->openPurpleBoxStrong();
                        if(COUPON_ID == item.get<UInt32>(1))
                            _owner->getCoupon(item.get<UInt32>(2));
                        else
                            _owner->GetPackage()->Add(item.get<UInt32>(1), item.get<UInt32>(2), true, false, FromDailyStrengthen);
                        _item.purpleBox[0][index-1] = item.get<UInt32>(1) * 100 + item.get<UInt32>(2);
                        toDB = true;
                        //打开紫色变强秘宝
                        GameAction()->doStrong(_owner, SthOpenPurpleBox, 0, 0);
                        _owner->activityUdpLog(1025, 3);
                    }
                }
                for(int i = 0; i < STRONGBOX_CNT; ++i)
                {
                    st << static_cast<UInt32>(_item.purpleBox[0][i]); 
                }
            } 
            break;
        case 4: //橙箱
            {
                UInt32 gold = _owner->GetVar(VAR_USEGOLD_CNT);
                st << gold;
                if(index > 0 && index <= STRONGBOX_CNT)
                {
                    if(_item.orangeBox[0][index-1])
                        return;
                    if(CanOpenOrangeBox(0, gold))
                    {
                        if(_owner->GetPackage()->GetRestPackageSize() < 1)
                        {
                            _owner->sendMsgCode(0, 1011);
                            return;
                        }
                        lua_tinker::table item = GameAction()->openOrangeBoxStrong();
                        if(COUPON_ID == item.get<UInt32>(1))
                            _owner->getCoupon(item.get<UInt32>(2));
                        else
                            _owner->GetPackage()->Add(item.get<UInt32>(1), item.get<UInt32>(2), true, false, FromDailyStrengthen);
                        _item.orangeBox[0][index-1] = item.get<UInt32>(1) * 100 + item.get<UInt32>(2);
                        toDB = true;
                        _owner->OnShuoShuo(SS_STH100);
                        _owner->activityUdpLog(1025, 4);
                    }
                }
                for(int i = 0; i < STRONGBOX_CNT; ++i)
                {
                    st << static_cast<UInt32>(_item.orangeBox[0][i]); 
                }
            }
            break;
        case 5: //御剑2级,绿箱
            {
                UInt32 time = _owner->GetOnlineTimeToday();
                st << time;
                if(index > 0 && index <= STRONGBOX_CNT)
                {
                    if(_item.greenBox[1][index-1])
                        return;
                    if(CanOpenGreenBox(1, time))
                    {
                        if(_owner->GetPackage()->GetRestPackageSize() < 1)
                        {
                            _owner->sendMsgCode(0, 1011);
                            return;
                        }
                        lua_tinker::table item = GameAction()->openGreenBoxStrong();
                        if(COUPON_ID == item.get<UInt32>(1))
                            _owner->getCoupon(item.get<UInt32>(2));
                        else
                            _owner->GetPackage()->Add(item.get<UInt32>(1), item.get<UInt32>(2), true, false, FromDailyStrengthen);
                        _item.greenBox[1][index-1] = item.get<UInt32>(1) * 100 + item.get<UInt32>(2);
                        toDB = true;
                        _owner->activityUdpLog(1025, 5);
                    }
                }
                for(int i = 0; i < STRONGBOX_CNT; ++i)
                {
                    st << static_cast<UInt32>(_item.greenBox[1][i]); 
                }
            }
            break;
        case 6: //御剑4级,蓝箱
            {
                UInt32 tael = _owner->GetVar(VAR_USETAEL_CNT);
                st << tael;
                if(index > 0 && index <= STRONGBOX_CNT)
                {
                    if(_item.blueBox[1][index-1])
                        return;
                    if(CanOpenBlueBox(1, tael))
                    {
                        if(_owner->GetPackage()->GetRestPackageSize() < 1)
                        {
                            _owner->sendMsgCode(0, 1011);
                            return;
                        }
                        lua_tinker::table item = GameAction()->openBlueBoxStrong();
                        if(COUPON_ID == item.get<UInt32>(1))
                            _owner->getCoupon(item.get<UInt32>(2));
                        else
                            _owner->GetPackage()->Add(item.get<UInt32>(1), item.get<UInt32>(2), true, false, FromDailyStrengthen);
                        _item.blueBox[1][index-1] = item.get<UInt32>(1) * 100 + item.get<UInt32>(2);
                        toDB = true;
                        _owner->activityUdpLog(1025, 6);
                    }
                }
                for(int i = 0; i < STRONGBOX_CNT; ++i)
                {
                    st << static_cast<UInt32>(_item.blueBox[1][i]); 
                }
            }
            break;
        case 7: //御剑6级,紫箱
            {
                UInt32 coupon = _owner->GetVar(VAR_USECOUPON_CNT);
                st << coupon;
                if(index > 0 && index <= STRONGBOX_CNT)
                {
                    if(_item.purpleBox[1][index-1])
                        return;
                    if(CanOpenPurpleBox(1, coupon))
                    {
                        if(_owner->GetPackage()->GetRestPackageSize() < 1)
                        {
                            _owner->sendMsgCode(0, 1011);
                            return;
                        }
                        lua_tinker::table item = GameAction()->openPurpleBoxStrong();
                        if(COUPON_ID == item.get<UInt32>(1))
                            _owner->getCoupon(item.get<UInt32>(2));
                        else
                            _owner->GetPackage()->Add(item.get<UInt32>(1), item.get<UInt32>(2), true, false, FromDailyStrengthen);
                        _item.purpleBox[1][index-1] = item.get<UInt32>(1) * 100 + item.get<UInt32>(2);
                        toDB = true;
                        //打开紫色变强秘宝
                        GameAction()->doStrong(_owner, SthOpenPurpleBox, 0, 0);
                        _owner->activityUdpLog(1025, 7);
                    }
                }
                for(int i = 0; i < STRONGBOX_CNT; ++i)
                {
                    st << static_cast<UInt32>(_item.purpleBox[1][i]); 
                }
            }
            break;
        case 8: //御剑8级,橙箱
            {
                UInt32 gold = _owner->GetVar(VAR_USEGOLD_CNT);
                st << gold;
                if(index > 0 && index <= STRONGBOX_CNT)
                {
                    if(_item.orangeBox[1][index-1])
                        return;
                    if(CanOpenOrangeBox(1, gold))
                    {
                        if(_owner->GetPackage()->GetRestPackageSize() < 1)
                        {
                            _owner->sendMsgCode(0, 1011);
                            return;
                        }
                        lua_tinker::table item = GameAction()->openOrangeBoxStrong();
                        if(COUPON_ID == item.get<UInt32>(1))
                            _owner->getCoupon(item.get<UInt32>(2));
                        else
                            _owner->GetPackage()->Add(item.get<UInt32>(1), item.get<UInt32>(2), true, false, FromDailyStrengthen);
                        _item.orangeBox[1][index-1] = item.get<UInt32>(1) * 100 + item.get<UInt32>(2);
                        toDB = true;
                        _owner->OnShuoShuo(SS_STH100);
                        _owner->activityUdpLog(1025, 8);
                    }
                }
                for(int i = 0; i < STRONGBOX_CNT; ++i)
                {
                    st << static_cast<UInt32>(_item.orangeBox[1][i]); 
                }
            }
            break;
        default:
            return;
            break;
    }
    st << Stream::eos;
    _owner->send(st);
    if(toDB)
    {
        UpdateToDB();
    }
}

}
