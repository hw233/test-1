#include "ClanCopy.h"
#include "MsgID.h"
#include "GData/ClanCopyTable.h"
#include "Player.h"
#include "Common/Stream.h"

namespace GObject
{

const static UInt32 COPY_ROUTE_COUNT = 3;  // 帮派副本的路径数目


ClanCopy::ClanCopy(Clan *c) : _clan(c), _copyLevel(0)
{
    _copyMonster.clear();
    _spotPlayer.clear();
    _spotList.clear();
    _copyProcessTime = 0;
    _deadPlayer.clear();
}


ClanCopy::~ClanCopy()
{
    _copyMonster.clear();
    _spotPlayer.clear();
    _spotList.clear();
    _copyProcessTime = 0;
    _deadPlayer.clear();
}

void ClanCopy::setInterval(UInt32 interval)
{
    // TODO: 设置游戏节奏时间
}

void ClanCopy::setStartTick(UInt32 tickCount)
{
    //  TODO: 设置游戏第一轮刷怪时间
}

void ClanCopy::process()
{
    // 定时器触发，每隔一秒更新一次状态
    ++_copyProcessTime;
}

void ClanCopy::createEnemy(UInt32 round)
{
    // TODO: 敌人老巢产生敌人
}

void ClanCopy::roundMove(UInt32 round)
{
    // 副本移动
    for (CopyMonster::iterator it = _copyMonster.begin(); it != _copyMonster.end(); ++it)
    {
        if ((it->second).hasMoved)
            continue;
        UInt8 curSpotId = it->first;

        if (curSpotId == Copy_Spot::Home)
        {
            // 敌人在主基地，不移动
            continue;
        }
        SpotList::iterator spotListIt = _spotList.find(curSpotId);
        if (spotListIt == _spotList.end())
            return;
        ClanCopyMonster clanCopyMonster = it->second;
        _copyMonster.erase(it);
        _copyMonster.insert(std::make_pair((spotListIt->second).nextSpotId, clanCopyMonster));
    }
}

void ClanCopy::roundCombat(UInt8 round)
{
    // 开始一轮战斗（FIXME: 战斗状态更新通知客户端）
    for (CopyMonster::iterator it = _copyMonster.begin(); it != _copyMonster.end(); ++it)
    {
        if (it->first == Copy_Spot::Enemy_Base) // 怪物老巢的怪物，不需要战斗
            continue;

        // 寻找该据点是否有玩家与之战斗
        SpotPlayer::iterator beg = _spotPlayer.lower_bound(it->first);
        SpotPlayer::iterator end = _spotPlayer.upper_bound(it->first);
        while (beg != end)
        {
            // 寻找是否有可以匹配的玩家与怪物战斗
            if ((beg->second).formalBattleRound < round)
            {
                // 找到可以匹配的玩家与怪物战斗
                GData::NpcGroups::iterator npcGroupsIt = GData::npcGroups.find((it->second).npcId);
                if(npcGroupsIt == GData::npcGroups.end())
                    return;

                GData::NpcGroup * ng = npcGroupsIt->second;
                Battle::BattleSimulator bsim(Battle::BS_COPY5, (beg->second).player, ng->getName(), ng->getLevel(), false);
                (beg->second).player->PutFighters( bsim, 0 );
                ng->putFighters( bsim );
                bsim.start();
                Stream& packet = bsim.getPacket();
                if (packet.size() <= 8)
                    return;

                bool res = bsim.getWinner() == 1;
                if (res)
                {
                    // 玩家防守成功
                    (beg->second).formalBattleRound = round; // 更新玩家战斗状态
                    _copyMonster.erase(it); // 怪物消失
                }
                else
                {
                    // 玩家防守失败，怪物可以走向下一个据点
                    (beg->second).player->regenAll();    // 生命值回满
                    _deadPlayer.insert (std::make_pair(beg->first, beg->second)); // 加入死亡名单
                    _spotPlayer.erase (beg); // 从据点玩家中剔除
                }
                Stream st(REP::ATTACK_NPC);
                st << static_cast<UInt8>(res) << static_cast<UInt8>(1) << static_cast<UInt8> (32) << static_cast<UInt8>(0) << static_cast<UInt8>(0);
                st.append(&packet[8], packet.size() - 8);
                st << Stream::eos;
                (beg->second).player->send(st);
                bsim.applyFighterHP(0, (beg->second).player, false);
                break;
            }
            ++beg;
        }

        // 怪物轮空
        if (beg == end && it->first == Home)
        {
            // 怪物在主基地轮空，攻击主基地
            attackHome(it->second);
            if (!_homeHp)
            {
                // 主基地被摧毁，副本挑战失败
                lose();
                return;
            }
        }
    }
}

void ClanCopy::attackHome(const ClanCopyMonster& clanCopyMonster)
{
    // 怪物攻击主基地
    if (_homeHp <= clanCopyMonster.npcValue)
    {
        // 主基地被摧毁
        _homeHp = 0;
        return;
    }
    else
    {
        _homeHp -= clanCopyMonster.npcValue;
    }
}

void ClanCopy::lose()
{
    // TODO: 帮派副本挑战失败

    for (SpotPlayer::iterator it = _spotPlayer.begin(); it != _spotPlayer.end(); ++ it)
    {
        (it->second).player->regenAll();
    }
}

void ClanCopy::win()
{
    // TODO: 帮派副本挑战成功
    for (SpotPlayer::iterator it = _spotPlayer.begin(); it != _spotPlayer.end(); ++ it)
    {
        (it->second).player->regenAll();
    }
}


}
