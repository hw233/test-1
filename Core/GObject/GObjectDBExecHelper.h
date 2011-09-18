#ifndef _GOBJECTDBEXECHELPER_H_
#define _GOBJECTDBEXECHELPER_H_

#include "DB/DBExecHelper.h"
#include "ItemData.h"
#include "TaskData.h"
#include "DB/DBExecHelper.h"
#include "Map.h"
#include "Server/WorldServer.h"
#include "Player.h"

namespace GData
{

struct DBNpcGroup
{
	UInt32 id;
	std::string fighterId;
	UInt16 formationId;
	UInt32 experience;
	std::string lootId;
};

struct DBDungeon
{
	UInt32 id;
	std::string name;
	UInt16 location;
	UInt8 type;
	UInt8 lvlReq;
};

struct DBDungeonLevel
{
	UInt32 id;
	UInt8 level;
	UInt32 monsterSet;
	std::string lootSet;
};

struct DBDungeonMonster
{
	UInt32 id;
	UInt8 formated;
	std::string monsters;
	UInt32 experience;
};

struct DBClanAssistant
{
	UInt32 id;
	UInt8 formated;
	std::string assistants;
};

}

namespace GObject
{

struct DBMapSpot
{
	UInt16	id;
	std::string	name;
	UInt8	type;
	UInt8	countryBattle;
	UInt32	x;
	UInt32	y;
};

struct DBBoss
{
	UInt32 id;
	UInt16 location;
	UInt32 spawnTime;
	UInt32 fleeTime;
	UInt8 appearLevel;
};

struct DBMailPackageData
{
	UInt32		id;
	UInt32		itemId;
	UInt32		itemCount;
};

struct DBMailData
{
	UInt32		id;
	UInt64		playerId;
	std::string	sender;
	UInt32		recvTime;
	UInt8		flag;
	std::string	title;
	std::string	content;
	UInt32		additionalId;
};

struct DBPlayerData
{
	UInt64 id;
	PlayerData pdata;
	UInt8 bossLevel;
	std::string icCount;
	std::string lineup;
	std::string nextreward;
	std::string tavernId;
	std::string bookStore;
	std::string shimen;
	std::string fshimen;
	std::string yamen;
	std::string fyamen;
    std::string clantask;
    std::string formations;
};

struct DBPrepaid
{
	UInt64 id;
	UInt32 gold;
};

struct DBPlayerBuffData
{
	UInt64 id;
	UInt8 buffId;
	UInt32 data;
};

struct DBFighterBuffData
{
	UInt64 playerId;
	UInt32 fighterId;
	UInt8 buffId;
	UInt32 data;
};

// 将基础属性
struct DBFighter
{
    UInt32 id;                  // 将编号
    std::string name;           // 名字
    UInt8 cls;                  // 职业:1-儒 2-释 3-道
    UInt8 lvl;                  // 等级*
    UInt8 sex;                  // 性别:0-男 1-女 2-童
    float potential;            // 潜力*
    float capacity;             // 资质*
    std::string skill;          // 技能/初始技能*, 一些比较牛的将可能会有多个初始技能
    UInt32 npc_weapon;          // 初始装备
    Int16 strength;             // 力量
    Int16 physique;             // 耐力/体魄
    Int16 agility;              // 敏捷
    Int16 intelligence;         // 智力
    Int16 will;                 // 意志
    Int16 soul;                 // 元神力
    Int16 aura;                 // 灵气/士气
    Int16 auraMax;              // 最大灵气/士气
    Int16 tough;                // 坚韧
    Int16 attack;               // 物理攻击
    Int16 magatk;               // 法术攻击
    Int16 defend;               // 物理防御
    Int16 magdef;               // 法术防御
    Int32 hp;                   // 最大血槽*
    UInt16 action;              // 身法/行动值
    UInt16 talent;              // 天赋
    float hitrate;              // 命中
    float evade;                // 闪躲
    float critical;             // 暴击(等级): 暴击率=暴击等级/（暴击等级+200+目标等级*20）+修正
    float criticaldmg;          // 暴击伤害
    float pierce;               // 击破/护甲穿透
    float counter;              // 反击
    float magres;               // 法术抵抗
    std::string extraPos;
};

struct DBPracticePlace
{
    UInt8 id;           // 修为挂机点ID
    UInt64 ownerid;     // 主人ID
    UInt64 protid;      // 护法弟子ID - 某个玩家ID
    UInt16 maxslot;     // 最大修炼位
    UInt16 openslot;    // 金币开启的修炼位
    UInt16 protmoney;   // 8小时保护费用
    UInt16 slotmoney;   // 8小时收费
    UInt8 open;         // 是否开放
    UInt16 enemyCount;  // 来犯敌人总次数
    UInt16 winCount;    // 赢的次数
    UInt32 slotincoming; // 一周洞府总收入
    UInt32 protincoming; // 一周保护费总收入
};

struct DBPracticeData
{
    UInt64 id;          // 用户ID
    UInt8 place;        // 修炼地点
    UInt16 slot;        // 修炼位置
    UInt8 type;         // 修炼类型 0-8小时, 1-24小时
    UInt8 pricetype;    // 付费方式 0-金币， 1-银币
    UInt16 slotprice;   // 为洞府所付的钱
    UInt16 protprice;   // 为护法所付的钱
    UInt32 traintime;   // 修炼分钟数
    UInt32 checktime;   // 修炼分钟数
    UInt8 prot;         // 是否申请保护 0-没有任何保护, 1-护法弟子保护, 2-仙府禁法
    UInt32 cdend;       // 下次可修炼时间
    UInt64 winnerid;    // 挑战胜利者ID
    std::string fighters;
};

struct DBReRecharge
{
    UInt64 playerId;
    UInt16 id;
    UInt32 num;
    UInt8 status;
};

struct DBFightersFriendliness
{
	UInt64 playerId;
	UInt32 fighterId;
	UInt16 friendliness;
	UInt16 submitFavorCount;
	UInt32 submitFavorDay;
};

struct DBSecondPWD
{
	UInt64 playerId;
	std::string secondPWD;
	std::string questionForPWD;
	std::string answerForPWD;
};

struct DBBossHP
{
	UInt32 id;
	UInt8 level;
	UInt8 pos;
	UInt32 hp;
};

struct DBBossDamage
{
	UInt32 id;
	UInt64 playerId;
	UInt32 damage;
	UInt32 exp;
};

// 将可变属性
struct DBFighterObj
{
	UInt32 id;              // 将编号
	UInt64 playerId;        // 玩家ID
	float potential;        // 潜力
    float capacity;         // 资质
	UInt8 level;            // 等级
    UInt8 relvl;            // 转身后等级
	UInt64 experience;      // 经验槽
    UInt64 practiceExp;     // 修炼经验槽
	UInt32 hp;              // 血槽
	UInt32 weapon;          // 武器
	UInt32 armor1;          // 头盔
	UInt32 armor2;          // 护甲
	UInt32 armor3;          // 护肩
	UInt32 armor4;          // 腰带
	UInt32 armor5;          // 靴子
	UInt32 ring;            // 戒指
	UInt32 amulet;          // 项链
    UInt16 peerless;        // 无双技能
    UInt16 talent;          // 天赋
    std::string trump;      // 法宝
    std::string acupoints;  // 穴道,打通次数
    std::string skill;      // 装备的技能
    std::string citta;      // 装备的心法
    std::string skills;     // 学会的技能, ID1,ID2,...
    std::string cittas;     // 学会的心法, ID1,ID2,...
};

struct DBEquipment
{
	UInt32 id;
	UInt32 itemId;
	UInt8 enchant;
	UInt8 attrType1;
	Int16 attrValue1;
	UInt8 attrType2;
	Int16 attrValue2;
	UInt8 attrType3;
	Int16 attrValue3;
	UInt8 sockets;
	UInt32 socket1;
	UInt32 socket2;
	UInt32 socket3;
	UInt32 socket4;
	UInt32 socket5;
	UInt32 socket6;
	UInt8 bindType;
};

struct DBFriend
{
	UInt64 id;
	UInt8 type;
	UInt64 friendId;
};

struct DBClan
{
	UInt32 id;
	std::string name;
	UInt8  rank;
	UInt8  level;
    UInt32 funds;
	UInt32 foundTime;
	UInt64 founder;
	UInt64 leader;
	UInt64 watchman;
    UInt64 construction;
	std::string contact;
	std::string announce;
	std::string purpose;
	UInt32 proffer;
	UInt32 grabAchieve;
	UInt8 battleTime;
	UInt8 nextBattleTime;
	UInt32 allyClan;
	UInt32 enemyClan1;
	UInt32 enemyClan2;
	UInt32 battleThisDay;
	UInt8  battleStatus;
	UInt8  southEdurance;
	UInt8  northEdurance;
	UInt8  hallEdurance;
	UInt8  hasBattle;
};

struct DBClanRepo
{
	UInt32 id;
	UInt32 itemId;
	UInt32 itemNum;
};

struct DBClanPlayer
{
	UInt32 id;
	UInt64 playerId;
	UInt32 joinTime;
    UInt32 proffer;
	UInt8  cls;
	UInt8  enterCount;
	UInt32 thisDay;
	UInt16 petFriendness[4];
	UInt16 favorCount[4];
	UInt32 lastFavorTime[4];
};

struct DBClanTech
{
	UInt32 clanId;
	UInt8  techId;
	UInt8  level;
	UInt16  extra;
};

struct DBClanSkill
{
    UInt32 clanId;
    UInt64 playerId;
    UInt8  skillId;
    UInt8  level;
};

struct DBClanPendingPlayer
{
	UInt32 id;
	UInt64 playerId;
	UInt8 cls;
	UInt32 opTime;
};

struct DBClanDonateRecord
{
	UInt32 clanId;
	std::string doanteName;
	UInt8 techId;
	UInt16 donateCount;
	UInt32 donateTime;
};

struct DBClanBattleResult
{
	UInt32 id;
	UInt32 battleTime;
	UInt8 result;
};

struct DBClanBattler
{
	UInt32 id;
	UInt64 battler;
	UInt8  battlerLev;
	UInt32 battlerClanId;
	UInt32 battleClanTime;
	UInt16 battleHold;
	UInt8  battleStatus;
	UInt16 reliveNum;
	UInt16 wins;
	UInt16 serialWins;
	UInt16 maxSerialWins;
	UInt8  southEdurance;
	UInt8  northEdurance;
	UInt32 grabAchieve;
	UInt8  hasEnter;
	UInt8 hasAttack;
};

struct DBCBClanReward
{
	UInt32 id;
	UInt32 itemNum;
};

struct DBCBPlayerReward
{
	UInt64 id;
	UInt32 itemNum;
};

struct DBClanPendingReward
{
	UInt32 id;
	UInt32 timeAlloc;
	UInt64 playerId;
	UInt32 itemId;
	UInt32 itemNum;
};

struct DBClanRewardRecord
{
	UInt32 id;
	UInt8 type;
	UInt32 timeAlloc;
	std::string playerName;
	std::string items;
};

struct DBTaskData
{
	UInt64 m_ownerId;
	UInt32 m_TaskId;
	UInt32 m_AcceptTime;
	std::string m_TaskStep;
	UInt32 m_TimeBegin;
	UInt32 m_TimeEnd;
	UInt8  m_Completed;
	UInt8  m_Submit;
};

struct DBTaskCompleteData
{
	UInt64 m_ownerId;
	UInt32 m_TaskId;
    UInt32 m_TimeEnd;
};

struct DBDayTaskData
{
	UInt32 m_LoopTaskId;
	UInt64 m_ownerId;
	UInt16 m_Count;
	UInt16 m_MaxCount;
	UInt16 m_MaxFlushQualityCount;
	UInt32 m_PreTaskId;
	UInt32 m_PreFlushTime;
	UInt8  m_PreTaskQuality;
	UInt32 m_AutoCompletedTaksId;
	UInt32 m_AutoCompletedTaskAcceptTime;
	UInt32 m_CurrDay;
	UInt16 m_FlushCount;
};

typedef ItemData DBItemData;

struct DBDungeonPlayer
{
	UInt32 id;
	UInt64 playerId;
	UInt8 level;
	UInt8 count;
	UInt16 totalCount;
	UInt32 firstPass;
	UInt32 counterEnd;
    UInt8 justice;
    UInt8 justice_roar;
};


struct DBDungeonAuto
{
	UInt64 playerId;
	UInt32 dungeonId;
	UInt32 totalExp;
	UInt8 won;
};

struct DBTradeData
{
	UInt32 tradeId;
	UInt64 ownerId;
	UInt8  tradeSide;
	UInt8  tradeStatus;
	UInt64 tradeTo;
	std::string tradeTitle;
	UInt32 tradeTime;
	UInt32 coin;
	UInt32 gold;
	std::string items;
};

struct DBSaleData
{
	UInt32 saleId;
	UInt64 ownerId;
	UInt8  status;
	UInt32 saleTime;
	UInt8  priceType;
	UInt32 price;
	UInt32 itemId;
	UInt16 itemNum;
};

struct DBAutoBattle
{
	UInt64 playerId;
	UInt32 npcId;
	UInt16 count;
	UInt16 interval;
};

struct DBAutoCopy
{
	UInt64 playerId;
	UInt8 id;
};

struct DBExchangeTicket
{
	UInt64 playerId;
	UInt32 count;
};

struct DBAthleticsData
{
	UInt8  row;
	UInt32 rank;
	UInt64 ranker;
	UInt32 maxRank;
	UInt8  challengeNum;
	UInt32 challengeTime;
	//UInt8  boxcolor;
	//UInt8  boxtype;
	//UInt32 boxcount;
	//UInt32 boxFlushTime;
    UInt32  prestige;
	UInt16	winstreak;
    UInt16   bewinstreak;
    UInt16   failstreak;
    UInt16   befailstreak;
    UInt32  oldrank;
    UInt32  first4rank;
    UInt32  extrachallenge;
};

struct DBAthleticsEventData
{
	UInt32 id;
    UInt8  row;
    UInt64 player1;
    UInt64 player2;
    UInt8  cond;
    UInt8  color;
    UInt16 value;
    UInt8  itemCount;
    UInt32 itemId;
	UInt32 time;
};

struct DBAthleticsRecordData
{
	UInt32 id;
	UInt64 atker;
	UInt64 defer;
	UInt32 repid;
	UInt32 time;
	UInt8  winSide;
	//UInt8  awardType;
	//UInt32 awardAtkerCount;
};

struct DBSpecialAward
{
	UInt8 rank;
	UInt8 type;
	UInt64 playerid;
};

struct DBLuckyDraw
{
	UInt64 playerid;
	UInt32 cost;
};

struct DBFighterTrain
{
	UInt32 fighterId;
	UInt64 ownerId;
	UInt8  priceType;
	UInt32 price;
	UInt32 trainTime;
	UInt32 trainRemain;
	UInt32 checkTime;
	UInt32 accExp;
};


struct DBClanAssistant
{
	UInt32 id;
	UInt8 formated;
	std::string assistants;
};

struct DBVersion
{
	std::string version;
};

struct DBArenaBet
{
	UInt64 id;
	UInt8 round;
	UInt8 group;
	UInt8 pos;
	UInt8 tael;
};

struct DBTripod
{
    UInt64 id;
	UInt32 soul;
	UInt8 fire;
	UInt8 quality;
	UInt8 awdst;
    UInt16 itemId;
    UInt8 num;
};

struct DBCopyData
{
    UInt64 playerId;
    UInt8 id;
    UInt8 floor;
    UInt8 spot;
};

struct DBFrontMapData
{
    UInt64 playerId;
    UInt8 id;
    UInt8 spot;
    UInt8 count;
    UInt8 status;
};
struct DBAttainData
{
    UInt64 ownerId;
    UInt16 attainId;
    UInt32 status;
    UInt32 updatetime;
};
struct DBWorldBoss
{
    UInt32 npcId;
    UInt8 level;
    UInt16 loc;
    UInt8 count;
};

}

namespace DB {

SPECIALBEGIN(GObject::MapData)
SPECIALDEF(5)
	(
	UInt8, m_ID,
	std::string, m_Name,
	UInt8, m_Level,
	UInt8, m_Acquirable,
	UInt8, m_Country
	)
SPECIALEND()

SPECIALBEGIN(GObject::DBMapSpot)
SPECIALDEF(6)
	(
	UInt16, id,
	std::string, name,
	UInt8, type,
	UInt8, countryBattle,
	UInt32, x,
	UInt32, y
	)
SPECIALEND()

SPECIALBEGIN(GObject::DBBoss)
SPECIALDEF(5)
	(
	UInt32, id,
	UInt16, location,
	UInt32, spawnTime,
	UInt32, fleeTime,
	UInt8, appearLevel
	)
SPECIALEND()

SPECIALBEGIN(GObject::MOData)
SPECIALDEF(5)
	(
	UInt32,		m_ID,
	std::string,m_Name,
	UInt16,		m_Spot,
	UInt8,		m_Type,
	UInt32,		m_ActionType
	)
SPECIALEND()

SPECIALBEGIN(GObject::DBPlayerData)
SPECIALDEF(52)
	(
	UInt64, id,
	std::string, pdata.name,
	UInt32, pdata.gold,
	UInt32, pdata.coupon,
	UInt32, pdata.tael,
	UInt32, pdata.coin,
	UInt32, pdata.status,
	UInt8, pdata.country,
	UInt8, pdata.title,
	UInt32, pdata.achievement,
	UInt8, pdata.qqvipl,
	UInt8, pdata.qqvipyear,
    UInt8, pdata.qqawardgot,
    UInt32, pdata.qqawardEnd,
    UInt32, pdata.ydGemId,
	UInt16, pdata.location,
	UInt8, pdata.inCity,
	UInt32, pdata.lastOnline,
	UInt64, pdata.newGuild,
	UInt16, pdata.packSize,
	UInt8, pdata.mounts,
	std::string, icCount,
    UInt8,  pdata.picCount,
    UInt32, pdata.nextPIcReset,
	UInt16, pdata.formation,
	std::string, lineup,
	UInt8, bossLevel,
	UInt32, pdata.totalRecharge,
	std::string, nextreward,
	UInt32, pdata.nextExtraReward,
	UInt32, pdata.lastExp,
	UInt64, pdata.lastResource,
	std::string, tavernId,
	std::string, bookStore,
	std::string, shimen,
	std::string, fshimen,
	std::string, yamen,
	std::string, fyamen,
    std::string, clantask,
	UInt8, pdata.copyFreeCnt,
	UInt8, pdata.copyGoldCnt,
	UInt32, pdata.copyUpdate,
	UInt8, pdata.frontFreeCnt,
	UInt8, pdata.frontGoldCnt,
	UInt32, pdata.frontUpdate,
    std::string, formations,
	UInt8, pdata.gmLevel,
	UInt8, pdata.wallow,
    UInt8,  pdata.dungeonCnt,
    UInt32, pdata.dungeonEnd,
	UInt32, pdata.created,
	UInt32, pdata.lockExpireTime
    )
SPECIALEND()

SPECIALBEGIN(GObject::DBPrepaid)
SPECIALDEF(2)
	(
	UInt64, id,
	UInt32, gold
	)
SPECIALEND()

SPECIALBEGIN(GObject::DBPlayerBuffData)
SPECIALDEF(3)
	(
	UInt64, id,
	UInt8, buffId,
	UInt32, data
	)
SPECIALEND()

SPECIALBEGIN(GObject::DBFighterBuffData)
SPECIALDEF(4)
	(
	UInt64, playerId,
	UInt32, fighterId,
	UInt8, buffId,
	UInt32, data
	)
SPECIALEND()

SPECIALBEGIN(GObject::DBMailPackageData)
SPECIALDEF(3)
(
	UInt32,		id,
	UInt32,		itemId,
	UInt32,		itemCount
)
SPECIALEND()

SPECIALBEGIN(GObject::DBMailData)
SPECIALDEF(8)
(
	UInt32,		id,
	UInt64,		playerId,
	std::string,sender,
	UInt32,		recvTime,
	UInt8,		flag,
	std::string,title,
	std::string,content,
	UInt32,		additionalId
)
SPECIALEND()

SPECIALBEGIN(GObject::DBFighter)
SPECIALDEF(33)
(
	UInt32, id,
	std::string, name,
	UInt8, cls,
	UInt8, lvl,
	UInt8, sex,
	float, potential,
	float, capacity,
	std::string, skill,
	UInt32, npc_weapon,
	Int16, strength,
	Int16, physique,
	Int16, agility,
	Int16, intelligence,
	Int16, will,
	Int16, soul,
	Int16, aura,
	Int16, auraMax,
	Int16, tough,
	Int16, attack,
	Int16, magatk,
	Int16, defend,
	Int16, magdef,
	Int32, hp,
	UInt16, action,
	UInt16, talent,
	float, hitrate,
	float, evade,
	float, critical,
	float, criticaldmg,
	float, pierce,
	float, counter,
	float, magres,
	std::string, extraPos
)
SPECIALEND()

SPECIALBEGIN(GObject::DBPracticePlace)
SPECIALDEF(12)
	(
        UInt8, id,
        UInt64, ownerid,
        UInt64, protid,
        UInt16, maxslot,
        UInt16, openslot,
        UInt16, protmoney,
        UInt16, slotmoney,
        UInt8, open,
        UInt16, enemyCount,
        UInt16, winCount,
        UInt32, slotincoming,
        UInt32, protincoming
    )
SPECIALEND()

SPECIALBEGIN(GObject::DBPracticeData)
SPECIALDEF(13)
	(
        UInt64, id,
        UInt8, place,
        UInt16, slot,
        UInt8, type,
        UInt8, pricetype,
        UInt16, slotprice,
        UInt16, protprice,
        UInt32, traintime,
        UInt32, checktime,
        UInt8, prot,
        UInt32, cdend,
        UInt64, winnerid,
        std::string, fighters
    )
SPECIALEND()

SPECIALBEGIN(GObject::DBReRecharge)
SPECIALDEF(4)
	(
        UInt64, playerId,
        UInt16, id,
        UInt32, num,
        UInt8, status
    )
SPECIALEND()


SPECIALBEGIN(GObject::DBFightersFriendliness)
SPECIALDEF(5)
	(
	UInt64, playerId,
	UInt32, fighterId,
	UInt16, friendliness,
	UInt16, submitFavorCount,
	UInt32, submitFavorDay
	)
SPECIALEND()

SPECIALBEGIN(GObject::DBSecondPWD)
SPECIALDEF(4)
(
 UInt64, playerId,
 std::string, secondPWD,
 std::string, questionForPWD,
 std::string, answerForPWD
 )
 SPECIALEND()

SPECIALBEGIN(GObject::DBBossHP)
SPECIALDEF(4)
	(
	UInt32, id,
	UInt8, level,
	UInt8, pos,
	UInt32, hp
	)
SPECIALEND()

SPECIALBEGIN(GObject::DBBossDamage)
SPECIALDEF(4)
	(
	UInt32, id,
	UInt64, playerId,
	UInt32, damage,
	UInt32, exp
	)
SPECIALEND()

SPECIALBEGIN(GObject::DBFighterObj)
SPECIALDEF(25)
	(
	UInt32, id,
	UInt64, playerId,
	float, potential,
	float, capacity,
	UInt8, level,
    UInt8, relvl,
	UInt64, experience,
	UInt64, practiceExp,
	UInt32, hp,
	UInt32, weapon,
	UInt32, armor1,
	UInt32, armor2,
	UInt32, armor3,
	UInt32, armor4,
	UInt32, armor5,
	UInt32, ring,
	UInt32, amulet,
    UInt16, peerless,
    UInt16, talent,
    std::string, trump,
    std::string, acupoints,
    std::string, skill,
    std::string, citta,
    std::string, skills,
    std::string, cittas
	)
SPECIALEND()

SPECIALBEGIN(GData::DBNpcGroup)
SPECIALDEF(5)
(
	UInt32, id,
	std::string, fighterId,
	UInt16, formationId,
	UInt32, experience,
	std::string, lootId
)
SPECIALEND()

SPECIALBEGIN(GData::DBDungeon)
SPECIALDEF(5)
	(
	UInt32, id,
	std::string, name,
	UInt16, location,
	UInt8, type,
	UInt8, lvlReq
	)
SPECIALEND()

SPECIALBEGIN(GData::DBDungeonLevel)
SPECIALDEF(4)
	(
	UInt32, id,
	UInt8, level,
	UInt32, monsterSet,
	std::string, lootSet
	)
SPECIALEND()

SPECIALBEGIN(GData::DBDungeonMonster)
SPECIALDEF(4)
	(
	UInt32, id,
	UInt8, formated,
	std::string, monsters,
	UInt32, experience
	)
SPECIALEND()

SPECIALBEGIN(GObject::DBFriend)
SPECIALDEF(3)
(
	UInt64, id,
	UInt8, type,
	UInt64, friendId
)
SPECIALEND()

SPECIALBEGIN(GObject::DBClan)
SPECIALDEF(26)
(
	UInt32, id,
	std::string, name,
	UInt8, rank,
	UInt8, level,
	UInt32, funds,
	UInt32, foundTime,
	UInt64, founder,
	UInt64, leader,
	UInt64, watchman,
    UInt64, construction,
	std::string, contact,
	std::string, announce,
	std::string, purpose,
	UInt32, proffer,
	UInt32, grabAchieve,
	UInt8, battleTime,
	UInt8, nextBattleTime,
	UInt32, allyClan,
	UInt32, enemyClan1,
	UInt32, enemyClan2,
	UInt32, battleThisDay,
	UInt8,  battleStatus,
	UInt8,  southEdurance,
	UInt8,  northEdurance,
	UInt8,  hallEdurance,
	UInt8,  hasBattle
)
SPECIALEND()


SPECIALBEGIN(GObject::DBClanRepo)
SPECIALDEF(3)
(
	UInt32, id,
	UInt32, itemId,
	UInt32, itemNum
)
SPECIALEND()


SPECIALBEGIN(GObject::DBClanDonateRecord)
SPECIALDEF(5)
(
	UInt32, clanId,
	std::string, doanteName,
	UInt8, techId,
	UInt16, donateCount,
	UInt32, donateTime
)
SPECIALEND()


SPECIALBEGIN(GObject::DBClanPlayer)
SPECIALDEF(19)
(
	UInt32, id,
	UInt64, playerId,
	UInt32, joinTime,
    UInt32, proffer,
	UInt8,  cls,
	UInt8,  enterCount,
	UInt32, thisDay,
	UInt16, petFriendness[0],
	UInt16, petFriendness[1],
	UInt16, petFriendness[2],
	UInt16, petFriendness[3],
	UInt16, favorCount[0],
	UInt16, favorCount[1],
	UInt16, favorCount[2],
	UInt16, favorCount[3],
	UInt32, lastFavorTime[0],
	UInt32, lastFavorTime[1],
	UInt32, lastFavorTime[2],
	UInt32, lastFavorTime[3]
)
SPECIALEND()

SPECIALBEGIN(GObject::DBClanPendingPlayer)
SPECIALDEF(4)
(
	UInt32, id,
	UInt64, playerId,
	UInt8, cls,
	UInt32, opTime
)
SPECIALEND()

SPECIALBEGIN(GObject::DBCBClanReward)
SPECIALDEF(2)
(
	UInt32, id,
	UInt32, itemNum
)
SPECIALEND()

SPECIALBEGIN(GObject::DBCBPlayerReward)
SPECIALDEF(2)
(
	UInt64, id,
	UInt32, itemNum
)
SPECIALEND()

SPECIALBEGIN(GObject::DBClanPendingReward)
SPECIALDEF(5)
(
	UInt32, id,
	UInt32, timeAlloc,
	UInt64, playerId,
	UInt32, itemId,
	UInt32, itemNum
)
SPECIALEND()

SPECIALBEGIN(GObject::DBClanRewardRecord)
SPECIALDEF(5)
(
	UInt32, id,
	UInt8, type,
	UInt32, timeAlloc,
	std::string, playerName,
	std::string, items
)
SPECIALEND()

SPECIALBEGIN(GObject::DBClanBattleResult)
SPECIALDEF(3)
(
	UInt32, id,
	UInt32, battleTime,
	UInt8, result
)
SPECIALEND()


SPECIALBEGIN(GObject::DBTaskData)
SPECIALDEF(8)
(
	UInt64,	m_ownerId,
	UInt32,	m_TaskId,
	UInt32,	m_AcceptTime,
	std::string,m_TaskStep,
	UInt32,	m_TimeBegin,
	UInt32,	m_TimeEnd,
	UInt8,	m_Completed,
	UInt8,	m_Submit
)
SPECIALEND()

SPECIALBEGIN(GObject::DBTaskCompleteData)
SPECIALDEF(3)
(
	UInt64,	m_ownerId,
	UInt32,	m_TaskId,
	UInt32, m_TimeEnd
)
SPECIALEND()

SPECIALBEGIN(GObject::DBDayTaskData)
SPECIALDEF(12)
(
	UInt32, m_LoopTaskId,
	UInt64,	m_ownerId,
	UInt16, m_Count,
	UInt16, m_MaxCount,
	UInt16, m_MaxFlushQualityCount,
	UInt32, m_PreTaskId,
	UInt32, m_PreFlushTime,
	UInt8 , m_PreTaskQuality,
	UInt32, m_AutoCompletedTaksId,
	UInt32, m_AutoCompletedTaskAcceptTime,
	UInt32, m_CurrDay,
	UInt16, m_FlushCount
)
SPECIALEND()

SPECIALBEGIN(GObject::DBItemData)
SPECIALDEF(4)
(
	UInt32,	id,
	UInt64,	ownerId,
	UInt16,	itemNum,
	UInt8,  bindType
)
SPECIALEND()

SPECIALBEGIN(GObject::DBEquipment)
SPECIALDEF(17)
(
	UInt32, id,
	UInt32, itemId,
	UInt8, enchant,
	UInt8, attrType1,
	Int16, attrValue1,
	UInt8, attrType2,
	Int16, attrValue2,
	UInt8, attrType3,
	Int16, attrValue3,
	UInt8, sockets,
	UInt32, socket1,
	UInt32, socket2,
	UInt32, socket3,
	UInt32, socket4,
	UInt32, socket5,
	UInt32, socket6,
	UInt8, bindType
)
SPECIALEND()


SPECIALBEGIN(GObject::DBDungeonPlayer)
SPECIALDEF(9)
(
	UInt32, id,
	UInt64, playerId,
	UInt8, level,
	UInt8, count,
	UInt16, totalCount,
	UInt32, firstPass,
	UInt32, counterEnd,
	UInt8, justice,
    UInt8, justice_roar
)
SPECIALEND()

SPECIALBEGIN(GObject::DBDungeonAuto)
SPECIALDEF(4)
(
 UInt64, playerId,
 UInt32, dungeonId,
 UInt32, totalExp,
 UInt8, won
 )
 SPECIALEND()


SPECIALBEGIN(GObject::DBTradeData)
SPECIALDEF(10)
(
	UInt32, tradeId,
	UInt64, ownerId,
	UInt8,  tradeSide,
	UInt8,  tradeStatus,
	UInt64, tradeTo,
	std::string, tradeTitle,
	UInt32, tradeTime,
	UInt32, coin,
	UInt32, gold,
	std::string, items
)
SPECIALEND()

SPECIALBEGIN(GObject::DBSaleData)
SPECIALDEF(8)
(
	UInt32, saleId,
	UInt64, ownerId,
	UInt8,  status,
	UInt32, saleTime,
	UInt8,  priceType,
	UInt32, price,
	UInt32, itemId,
	UInt16, itemNum
)
SPECIALEND()

SPECIALBEGIN(GObject::DBAutoBattle)
SPECIALDEF(4)
(
	UInt64, playerId,
	UInt32, npcId,
	UInt16, count,
	UInt16, interval
)
SPECIALEND()

SPECIALBEGIN(GObject::DBAutoCopy)
SPECIALDEF(2)
(
	UInt64, playerId,
	UInt8, id
)
SPECIALEND()

SPECIALBEGIN(GObject::DBExchangeTicket)
SPECIALDEF(2)
(
	UInt64, playerId,
	UInt32, count
)
SPECIALEND()

SPECIALBEGIN(GObject::DBAthleticsData)
SPECIALDEF(14)
(
	UInt8,  row,
	UInt32, rank,
	UInt64, ranker,
	UInt32, maxRank,
	UInt8,  challengeNum,
	UInt32, challengeTime,
	//UInt8,  boxcolor,
	//UInt8,  boxtype,
	//UInt32, boxcount,
	//UInt32, boxFlushTime,
    UInt32, prestige,
	UInt16,  winstreak,
    UInt16,  bewinstreak,
    UInt16,  failstreak,
    UInt16,  befailstreak,
    UInt32, oldrank,
    UInt32, first4rank,
    UInt32, extrachallenge
)
SPECIALEND()

SPECIALBEGIN(GObject::DBAthleticsEventData)
SPECIALDEF(10)
(
	UInt32, id,
    UInt8,  row,
    UInt64, player1,
    UInt64, player2,
    UInt8,  cond,
    UInt8,  color,
    UInt16, value,
    UInt8,  itemCount,
    UInt32, itemId,
	UInt32, time
)
SPECIALEND()

SPECIALBEGIN(GObject::DBAthleticsRecordData)
SPECIALDEF(6)
(
	UInt32, id,
	UInt64, atker,
	UInt64, defer,
	UInt32, repid,
	UInt32, time,
	UInt8,	winSide
	//UInt8,  awardType,
	//UInt32, awardAtkerCount
)
SPECIALEND()

SPECIALBEGIN(GObject::DBSpecialAward)
SPECIALDEF(3)
(
	UInt8, rank,
	UInt8, type,
	UInt64, playerid
)
SPECIALEND()

SPECIALBEGIN(GObject::DBLuckyDraw)
SPECIALDEF(2)
(
 UInt64, playerid,
 UInt32, cost
 )
 SPECIALEND()

SPECIALBEGIN(GObject::DBFighterTrain)
SPECIALDEF(8)
(
	UInt32, fighterId,
	UInt64, ownerId,
	UInt8,  priceType,
	UInt32, price,
	UInt32, trainTime,
	UInt32, trainRemain,
	UInt32, checkTime,
	UInt32, accExp
)
SPECIALEND()


SPECIALBEGIN(GObject::DBClanTech)
SPECIALDEF(4)
(
	UInt32, clanId,
	UInt8,  techId,
	UInt8,  level,
	UInt16, extra
)
SPECIALEND()

SPECIALBEGIN(GObject::DBClanSkill)
SPECIALDEF(4)
(
	UInt32, clanId,
    UInt64, playerId,
	UInt8,  skillId,
	UInt8,  level
)
SPECIALEND()

SPECIALBEGIN(GObject::DBClanBattler)
SPECIALDEF(16)
(
	UInt32, id,
	UInt64, battler,
	UInt8,  battlerLev,
	UInt32, battlerClanId,
	UInt32, battleClanTime,
	UInt16, battleHold,
	UInt8,  battleStatus,
	UInt16, reliveNum,
	UInt16, wins,
	UInt16, serialWins,
	UInt16, maxSerialWins,
	UInt8,  southEdurance,
	UInt8,  northEdurance,
	UInt32, grabAchieve,
	UInt8,  hasEnter,
	UInt8,  hasAttack
)
SPECIALEND()


SPECIALBEGIN(GObject::DBClanAssistant)
SPECIALDEF(3)
(
	UInt32, id,
	UInt8, formated,
	std::string, assistants
)
SPECIALEND()

SPECIALBEGIN(GObject::DBArenaBet)
SPECIALDEF(5)
(
	UInt64, id,
	UInt8, round,
	UInt8, group,
	UInt8, pos,
	UInt8, tael
)
SPECIALEND()

SPECIALBEGIN(GObject::DBTripod)
SPECIALDEF(7)
(
    UInt64, id,
    UInt32, soul,
    UInt8, fire,
    UInt8, quality,
    UInt8, awdst,
    UInt16, itemId,
    UInt8, num
)
SPECIALEND()

SPECIALBEGIN(GObject::DBCopyData)
SPECIALDEF(4)
(
    UInt64, playerId,
    UInt8, id,
    UInt8, floor,
    UInt8, spot
)
SPECIALEND()

SPECIALBEGIN(GObject::DBFrontMapData)
SPECIALDEF(5)
(
    UInt64, playerId,
    UInt8, id,
    UInt8, spot,
    UInt8, count,
    UInt8, status
)
SPECIALEND()

SPECIALBEGIN(GObject::DBAttainData)
SPECIALDEF(4)
(
    UInt64, ownerId,
    UInt16, attainId,
    UInt32, status,
    UInt32, updatetime
)
SPECIALEND()

SPECIALBEGIN(GObject::DBWorldBoss)
SPECIALDEF(4)
(
    UInt32, npcId,
    UInt8, level,
    UInt16, loc,
    UInt8, count
)
SPECIALEND()

SPECIALBEGIN(GObject::DBVersion)
SPECIALDEF(1)
(
	std::string, version
)
SPECIALEND()

}

#endif // _GOBJECTDBEXECHELPER_H_
