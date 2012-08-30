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
}


ClanCopy::~ClanCopy()
{
    _copyMonster.clear();
    _spotPlayer.clear();
    _spotList.clear();
    _copyProcessTime = 0;
}

void ClanCopy::createEnemy(UInt32 round)
{
    // TODO: 敌人老巢产生敌人
}

void ClanCopy::roundMove(UInt32 round)
{
    // TODO: 副本移动
    for (CopyMonster::iterator it = _copyMonster.begin(); it != _copyMonster.end(); ++it)
    {
        if ((it->second).hasMoved)
            continue;
        UInt8 curSpotId = it->first;

        if (curSpotId == Copy_Spot::Home)
        {
            // 敌人在主基地未打死不移动
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
                    (beg->second).formalBattleRound = round;
                    _copyMonster.erase(it);
                }
                else
                {
                    // 玩家防守失败，怪物可以走向下一个据点
                    _spotPlayer.erase (beg);
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
    }
}

void ClanCopy::lose()
{
    // TODO: 帮派副本挑战失败
}

void ClanCopy::win()
{
    // TODO: 帮派副本挑战成功
}


}
