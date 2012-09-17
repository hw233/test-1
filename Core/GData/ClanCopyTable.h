#ifndef _CLAN_COPY_TABLE_
#define _CLAN_COPY_TABLE_

namespace GData
{

#if 0 // 这里我想多了
static UInt8 spotMapType[] =
{
			/*0123456789ABCDEF */
    /*0x00*/ "????????????????"\
    /*0x10*/ "????????????????"\
    /*0x20*/ "????????????????"\
    /*0x30*/ "????????????????"\
    /*0x40*/ "????????????????"\
    /*0x50*/ "????????????????"\
    /*0x60*/ "????????????????"\
    /*0x70*/ "????????????????"\
    /*0x80*/ "????????????????"\
    /*0x90*/ "????????????????"\
    /*0xA0*/ "????????????????"\
    /*0xB0*/ "????????????????"\
    /*0xC0*/ "????????????????"\
    /*0xD0*/ "????????????????"\
    /*0xE0*/ "????????????????"\
    /*0xF0*/ "????????????????"\
			/*0123456789ABCDEF */
};

#endif


struct ClanCopySpotData
{
    UInt8  bufferType;     // 防守方的据点增益效果种类
    UInt8  maxCount;       // 防守方的最大人数
    std::vector<float> bufferValue; // 防守方增益效果值（和人数相关）
    ClanCopySpotData(UInt8 bufferType)
        : bufferType(bufferType), maxCount(20)
    {
        bufferValue.clear();
    }
    ClanCopySpotData()
        : bufferType(0), maxCount(20)
    {
    }
};
typedef std::map<UInt8, ClanCopySpotData> ClanCopySpotMap;

extern ClanCopySpotMap clanCopySpotMap;


struct ClanCopyData
{
    UInt16 level;               // 帮派副本等级
    UInt32 expOutput;           // 该等级给予神像每小时的经验值
    UInt16 maxWaveCount;        // 该等级刷怪的最大波数（包括不出怪的轮空波数）
    UInt16 minPlayer;           // 该等级副本参与最少人数
    UInt16 maxPlayer;           // 该等级副本参与最多人数
    UInt8  spotMaxPlayer;       // 该等级副本每个据点最大人数
    UInt32 homeHp;              // 该等级主基地的的生命值
    UInt32 maxReward;           // 该等级获胜给予奖励经验值的最大值

    ClanCopyData(UInt16 level, UInt32 expOutput, UInt16 maxWaveCount,
            UInt16 minPlayer, UInt16 maxPlayer, UInt8 spotMaxPlayer, UInt32 homeHp, UInt32 maxReward)
        : level(level), expOutput(expOutput), maxWaveCount(maxWaveCount), 
        minPlayer(minPlayer), maxPlayer(maxPlayer), spotMaxPlayer(spotMaxPlayer),
        homeHp(homeHp), maxReward(maxReward)
    {
    }
    ClanCopyData():level(0), expOutput(0), maxWaveCount(0), minPlayer(0), maxPlayer(0), spotMaxPlayer(0), homeHp(0), maxReward(0)
    {
    }
    
};

typedef std::vector<ClanCopyData> ClanCopyTable;

struct ClanCopyMonsterData
{
    ClanCopyMonsterData(UInt16 level, UInt16 appearRound, UInt32 npcId,
            UInt8 npcCount, UInt8 npcRouteIndex, UInt16 npcValue, UInt8 monsterType)
        : level(level), appearRound(appearRound), npcId(npcId),
        npcCount(npcCount), npcRouteIndex(npcRouteIndex), npcValue(npcValue), monsterType(monsterType)
    {
    }
    UInt16 level;              // 副本等级
    UInt16 appearRound;        // 生产该怪的波数
    UInt32 npcId;              // 怪物npcGroupId
    UInt8  npcCount;           // 每一路怪物的数量
    UInt8  npcRouteIndex;      // 出现的路数 
    UInt16 npcValue;           // 怪物对主基地的破坏值
    UInt8  monsterType;        // 怪物类型（普通怪 ，精英怪，boss）
};

// key值为 ((level << 16 | appearRound) << 8 | npcRouteIndex)
typedef std::multimap<UInt32, ClanCopyMonsterData> ClanCopyMonsterMap;

extern ClanCopyTable clanCopyTable;
extern ClanCopyMonsterMap clanCopyMonsterMap;

}

#endif

