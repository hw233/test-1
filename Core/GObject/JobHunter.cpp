#include "JobHunter.h"

#include "Common/Stream.h"
#include "Common/StringTokenizer.h"
#include "Player.h"
#include "Fighter.h"
#include "MsgID.h"

//************************************************
// 用于第四职业招募的——寻墨
//************************************************

namespace GObject
{

JobHunter::JobHunter(Player * player)
    : _owner(player), _equipProb(0), _cittaProb(0), _trumpProb(0)
{
}

JobHunter::~JobHunter()
{
}

bool JobHunter::list2string(std::string& str)
{
    char buf[1024] = {0};
    char* pbuf = buf;
    char* pend = &buf[sizeof(buf)-1];
    for (std::set<UInt16>::iterator it = _fighterList.begin(); it != _fighterList.end(); ++it)
    {
        pbuf += snprintf(pbuf, pend - pbuf, "%u", *it);
        pbuf += snprintf(pbuf, pend - pbuf, ",");
    }

    if (pbuf != buf)
        str = buf;
    return true;
}

void JobHunter::LoadFighterList(std::string& list)
{
    // 加载玩家的待招列表
    StringTokenizer tokenizer(list, ",");
    for (UInt32 i = 0; i < tokenizer.count(); ++ i)
    {
        UInt16 id = atoi(tokenizer[i].c_str());
        if (id)
            _fighterList.insert(id);
    }
}

void JobHunter::AddToFighterList(UInt16 id)
{
    // 增加至待招列表
    if (_owner->GetLev() < 70)
        return;
    Fighter * fighter = globalFighters[id];
    if (!fighter)
        return;
    if (fighter->getClass() != 4)
        return;

    if (_fighterList.find(id) != _fighterList.end())
        return;
    _fighterList.insert(id);

    std::string list;
    list2string(list);

    DB2().PushUpdateData("REPLACE INTO `ExJob` (`playeyId`, `list`) VALUES (%"I64_FMT"u, '%s')", 
            _owner->getId(), list.c_str());

}

void JobHunter::SendFighterList()
{
    // 发送待招散仙列表
    if (!_owner)
        return;
    Stream st(REP::EXJOB);
    st << static_cast<UInt8>(0x01);
    st << static_cast<UInt8>(_fighterList.size());
    for (std::set<UInt16>::iterator it = _fighterList.begin(); it != _fighterList.end(); ++ it)
    {
        st << static_cast<UInt16>(*it);
    }
    st << Stream::eos;
    _owner->send(st);
}

void JobHunter::SendGameInfo()
{
    // TODO: 发送寻墨游戏的具体内容
}
} 

