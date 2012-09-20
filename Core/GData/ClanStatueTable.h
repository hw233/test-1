#ifndef _CLAN_STATUE_TABLE_H_
#define _CLAN_STATUE_TABLE_H_

#include "Config.h"

namespace GData
{

struct ClanStatueTableData
{
    ClanStatueTableData( UInt16 level = 0, UInt32 needExp = 0, UInt32 consumeExp = 0,
            UInt32 exHp = 0, UInt32 exAttack = 0, UInt32 exDefend = 0,
            UInt32 exMagAtk = 0, UInt32 exMagDef = 0,
            UInt32 exAction = 0, UInt32 exHitRate =0)
        : level(level), needExp(needExp), consumeExp(consumeExp),
        exHp(exHp), exAttack(exAttack), exDefend(exDefend),
        exMagAtk(exMagAtk), exMagDef(exMagDef), exAction(exAction), exHitRate(exHitRate)
    {
    }

    ClanStatueTableData(const ClanStatueTableData& cstd)
    {
        level      = cstd.level;
        needExp    = cstd.needExp;
        consumeExp = cstd.consumeExp;
        exHp       = cstd.exHp;
        exAttack   = cstd.exAttack;
        exDefend   = cstd.exDefend;
        exMagAtk   = cstd.exMagAtk;
        exMagDef   = cstd.exMagDef;
        exAction   = cstd.exAction;
        exHitRate  = cstd.exHitRate;
    }

    ClanStatueTableData& operator= (const ClanStatueTableData& cstd)
    {
        level      = cstd.level;
        needExp    = cstd.needExp;
        consumeExp = cstd.consumeExp;
        exHp       = cstd.exHp;
        exAttack   = cstd.exAttack;
        exDefend   = cstd.exDefend;
        exMagAtk   = cstd.exMagAtk;
        exMagDef   = cstd.exMagDef;
        exAction   = cstd.exAction;
        exHitRate  = cstd.exHitRate;
        return *this;
    }

    // 帮派神像模板参数
    UInt16 level;       // 神像等级
    UInt32 needExp;     // 升级所需经验值
    UInt32 consumeExp;  // 每小时减少的经验值
    UInt32 exHp;        // 神像增加的hp
    UInt32 exAttack;    // 神像增加的物攻值
    UInt32 exDefend;    // 神像增加的物防值
    UInt32 exMagAtk;    // 神像增加的法攻值
    UInt32 exMagDef;    // 神像增加的法防值
    UInt32 exAction;    // 神像增加的身法值
    UInt32 exHitRate;   // 神像增加的命中值
};

typedef std::vector<ClanStatueTableData>  ClanStatueTable;

extern ClanStatueTable clanStatueTable;
}

#endif

