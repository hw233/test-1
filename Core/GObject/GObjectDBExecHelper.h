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

struct DBTownDeamonMonster
{
	UInt16 level;
    UInt32 npcId;
    UInt32 itemId;
    UInt32 itemNum;
};

struct DBNpcGroup
{
	UInt32 id;
	std::string fighterId;
	UInt16 formationId;
	UInt8 type;
	UInt32 experience;
	std::string lootId[4];
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

struct DBTeamCopy
{
    UInt8 id;
    UInt8 type;
    UInt16 location;
    std::string npcgroups;
};

struct DBClanAssistant
{
	UInt32 id;
	UInt8 formated;
	std::string assistants;
};

struct DBLBSkill
{
    UInt16 id;
    std::string name;
    UInt8 lbtype;
    UInt8 level;
    UInt8 target;
    float  prob;
    UInt16 cond;
    UInt16 cond2;
    UInt8 area;
    std::string factor;
    Int16 last;
    UInt16 cd;

    UInt16 ef_type;             
    UInt16 minFactor;          
    UInt16 ef_value;           
    UInt16 battlepoint;
};

}

namespace GObject
{

struct DBTownDeamonPlayer
{
    UInt16 deamonLevel;
    UInt16 curLevel;
    UInt16 maxLevel;
    UInt64 playerId;
    UInt32 startTime;
    UInt32 accTime;
    UInt32 accLen;
    UInt32 accAwards;
    UInt32 vitalityTime;
    UInt16 vitality;
    UInt16 spirit;
    UInt32 challengeTime;
    UInt32 itemId;
    UInt32 itemNum;
    UInt16 quitLevel;
    UInt64 attacker;
};

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

struct DBTeamCopyPlayer
{
    UInt64 playerId;
    UInt8 copyId;
    UInt8 type;
    UInt8 pass;
    UInt8 passTimes;
    UInt32 vTime;
};

struct DBTeamCopyPlayerAward
{
    UInt64 playerId;
    UInt8  rollId;
    UInt8  roll;
    UInt32 awardId;
    UInt32 awardCnt;
};

struct DBPlayerData
{
	UInt64 id;
	PlayerData pdata;
	UInt8 bossLevel;
    std::string titleAll;
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
    std::string atohicfg;
    std::string openid;
};

struct DBHoneyFall
{
    UInt64 playerId;
    UInt8 type;
    UInt32 value;
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

struct DBPlayerVar
{
    UInt64 playerId;
    UInt16 id;
    UInt32 data;
    UInt32 overTime;
};

struct DBGVar
{
    UInt32 id;
    UInt32 data;
    UInt32 overTime;
};

struct DBSS
{
    UInt32 id;
    UInt64 playerId;
    UInt16 skillid;
    UInt16 father;
    UInt32 maxVal;
    UInt32 curVal;
    UInt8 lvl;
    UInt8 maxLvl;
};

struct DBQixi
{
    UInt64 playerId;
    UInt64 lover;
    UInt8 bind;
    UInt8 pos;
    UInt8 event;
    UInt32 score;
};
struct DBSnow
{
    UInt64 playerId;
    UInt64 lover;
    UInt8 bind;
    UInt32 score;
};

struct DBArenaExtraBoard
{
    UInt8 week;
    std::string name[5];
    UInt8 heroId[5];
    UInt32 sufferTotal;
    UInt32 sufferCnt[5];
    UInt32 lasttime[5];
};

struct DBJobHunter
{
    UInt64 playerId;
    std::string fighterList;
    std::string mapInfo;
    UInt8 progress;
    UInt8 posX;
    UInt8 posY;
    UInt8 earlyPosX;
    UInt8 earlyPosY;
    UInt32 stepCount;
    UInt8 slotVal1;
    UInt8 slotVal2;
    UInt8 slotVal3;
};

struct DBCopyFrontWin
{
    UInt64 playerId;
    UInt8 posOrig;
    UInt8 posPut;
    UInt32 itemId;
    UInt16 ratio;
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
    Int32 attack;               // 物理攻击
    Int32 magatk;               // 法术攻击
    Int32 defend;               // 物理防御
    Int32 magdef;               // 法术防御
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

struct DBSecondSoul
{
	UInt32 fighterId;
	UInt64 playerId;
	UInt8 cls;
	UInt8 xinxiu;
	UInt8 practiceLevel;
	UInt8 stateLevel;
	UInt32 stateExp;
    std::string skills;
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
struct DBFighter2
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
	UInt32 halo;            // 光环
	UInt32 fashion;         // 时装
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
    UInt8 attrType1;
    UInt16 attrValue1;
    UInt8 attrType2;
    UInt16 attrValue2;
    UInt8 attrType3;
    UInt16 attrValue3;
	UInt32 fighterId;
	UInt8 cls;
	UInt8 xinxiu;
	UInt8 practiceLevel;
	UInt8 stateLevel;
	UInt32 stateExp;
    std::string skills_2nd;
    Int32 strength;
    Int32 physique;
    Int32 agility;
    Int32 intelligence;
    Int32 will;
    Int32 soul;
    Int32 attack;
    Int32 defend;
    Int32 critical;
    Int32 pierce;
    Int32 evade;
    Int32 counter;
    Int32 tough;
    Int32 action;
    UInt8 hideFashion;
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
    UInt8 attrType1;
    UInt16 attrValue1;
    UInt8 attrType2;
    UInt16 attrValue2;
    UInt8 attrType3;
    UInt16 attrValue3;
};

struct DBEquipment
{
	UInt32 id;
	UInt32 itemId;
	UInt8 enchant;
    UInt8 tRank;
    UInt8 maxTRank;
    UInt32 trumpExp;
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

struct DBEquipmentSpirit
{
	UInt32 id;
	Int16 splev1;
	Int16 splev2;
	Int16 splev3;
	Int16 splev4;
	UInt8 spform1;
	UInt8 spform2;
	UInt8 spform3;
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
    UInt32 battleScore;
    UInt32 dailyBattleScore;
    UInt32 battleRanking;
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
    UInt32 signupRankBattleTime;
    UInt32 rankBattleField;
};

struct DBClanItem
{
    UInt32 clanid;
    UInt64 playerid;
    UInt32 itemid;
    UInt32 itemnum;
};

struct DBClanItemHistory
{
    UInt32 id;
    UInt32 clanid;
    UInt8  type;
    UInt32 time;
    UInt64 playerid;
    std::string itemstr;
};

struct DBClanTech
{
	UInt32 clanId;
	UInt8  techId;
	UInt8  level;
	UInt32  extra;
};

struct DBClanSkill
{
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

struct DBClanStatue
{
    UInt32 clanId;
    UInt16 level;
    UInt32 exp;
    UInt32 expUpdateTime;
};

struct DBClanCopy
{
    UInt32 clanId;
    UInt16 level;
    UInt32 levelUpdateTime;
    UInt16 maxLevel;
    UInt32 maxLevelTime;
};

struct DBClanCopyLog
{
    UInt32 clanId;
    UInt32 logTime;
    UInt8  logType;
    std::string playerName;
    UInt32 logVal;
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

struct DBAutoFrontMap
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
    UInt32  tael;
	UInt16	winstreak;
    UInt16   bewinstreak;
    UInt16   failstreak;
    UInt16   befailstreak;
    UInt32  oldrank;
    UInt32  first4rank;
    UInt32  extrachallenge;
    UInt8   pageNum;
    UInt32 eChallengeTime;
    UInt8 ePhysical;
    UInt8 eSelectIndex;
    UInt32 eCombine[5];
    UInt64 eRival[5];
    UInt8 eCanAttack[5];
    UInt8 eRivalType[5];
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
	UInt8 state;
	UInt8 group;
	UInt8 recieved;
	UInt16 pos;
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

struct DBMoneyLog
{
    Int32 time;
    Int32 type;
    Int32 gold;
    Int32 coupon;
    Int32 tael;
    Int32 achievement;
    Int32 prestige;
};

struct DBCopyData
{
    UInt64 playerId;
    UInt8 id;
    UInt8 floor;
    UInt8 spot;
    UInt8 lootlvl;
};

struct DBFrontMapData
{
    UInt64 playerId;
    UInt8 id;
    UInt8 spot;
    UInt8 count;
    UInt8 status;
    UInt8 lootlvl;
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
struct DBWBoss
{
    UInt8 idx;
    UInt32 last;
    UInt32 hp;
    Int32 atk;
    Int32 matk;
};
struct DBDiscount
{
    UInt16 itemID;
    UInt32 beginTime;
    UInt32 endTime;
    UInt16 priceOriginal;
    UInt16 priceDiscount;
    UInt8 type;
    UInt32 count;
};
struct DBLuckyLog
{
    std::string name;
    std::string items;
};

struct DBRealItemAward
{
    UInt32 id;
    UInt32 cd;
    std::string card_no;
    std::string card_psw;
};

struct DBNewRelation
{
    UInt64 playerId;
    UInt8 mood;
    std::string sign;
};

struct DBRNR
{
    UInt64 id;
    std::string record;
};

struct DBActivityData
{
    UInt64 playerId;
    UInt32 overTime;
    /*
    UInt32 awardID;
    UInt32 point;
    UInt32 award;
    std::string flags;
    */
    UInt32 scores;
    UInt32 propsID;
    std::string signRecord;
};
struct DBHeroMemo
{
    UInt64 playerId;
	std::string awards;
	std::string memos;
};
struct DBShuoShuo
{
    UInt64 playerId;
    UInt32 update;
	std::string ss;
};
struct DBCFriendAwards
{
    UInt64 playerId;
    UInt64 invitedId;
	std::string awards;
};
struct DBBigLock
{
    UInt32 playerId;
    UInt32 lockExpireTime;
};

struct DBStrengthenData
{
    UInt64 playerId;
    UInt32 overTime;
    UInt8  souls;
    UInt8  soulId;
    std::string flags;
    std::string greenBox;
    std::string blueBox;
    std::string purpleBox;
    std::string orangeBox;
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


SPECIALBEGIN(GObject::DBTownDeamonPlayer)
SPECIALDEF(16)
    (
    UInt16, deamonLevel,
    UInt16, curLevel,
    UInt16, maxLevel,
    UInt64, playerId,
    UInt32, startTime,
    UInt32, accTime,
    UInt32, accLen,
    UInt32, accAwards,
    UInt32, vitalityTime,
    UInt16, vitality,
    UInt16, spirit,
    UInt32, challengeTime,
    UInt32, itemId,
    UInt32, itemNum,
    UInt16, quitLevel,
    UInt64, attacker
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
SPECIALDEF(57)
	(
	UInt64, id,
	std::string, pdata.name,
	UInt32, pdata.gold,
	UInt32, pdata.coupon,
	UInt32, pdata.tael,
	UInt32, pdata.coin,
	UInt32, pdata.prestige,
	UInt32, pdata.status,
	UInt8, pdata.country,
	UInt8, pdata.title,
    std::string, titleAll,
	UInt32, pdata.achievement,
    UInt32, pdata.attainment,
	UInt8, pdata.qqvipl,
	UInt8, pdata.qqvipyear,
    UInt32, pdata.qqawardgot,
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
    std::string, atohicfg,
	UInt8, pdata.gmLevel,
	UInt8, pdata.wallow,
    UInt8,  pdata.dungeonCnt,
    UInt32, pdata.dungeonEnd,
	UInt32, pdata.created,
	UInt32, pdata.lockExpireTime,
    std::string, openid
    )
SPECIALEND()

SPECIALBEGIN(GObject::DBHoneyFall)
SPECIALDEF(3)
	(
    UInt64, playerId,
    UInt8,  type,
    UInt32, value
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

SPECIALBEGIN(GObject::DBPlayerVar)
SPECIALDEF(4)
    (
    UInt64, playerId,
    UInt16, id,
    UInt32, data,
    UInt32, overTime
    )
SPECIALEND()

SPECIALBEGIN(GObject::DBGVar)
SPECIALDEF(3)
    (
    UInt32, id,
    UInt32, data,
    UInt32, overTime
    )
SPECIALEND()

SPECIALBEGIN(GObject::DBSS)
SPECIALDEF(8)
    (
    UInt32, id,
    UInt64, playerId,
    UInt16, skillid,
    UInt16, father,
    UInt32, maxVal,
    UInt32, curVal,
    UInt8, lvl,
    UInt8, maxLvl
    )
SPECIALEND()

SPECIALBEGIN(GObject::DBQixi)
SPECIALDEF(6)
    (
    UInt64, playerId,
    UInt64, lover,
    UInt8, bind,
    UInt8, pos,
    UInt8, event,
    UInt32, score
    )
SPECIALEND()

SPECIALBEGIN(GObject::DBSnow)
SPECIALDEF(4)
    (
    UInt64, playerId,
    UInt64, lover,
    UInt8, bind,
    UInt32, score
    )
SPECIALEND()

SPECIALBEGIN(GObject::DBArenaExtraBoard)
SPECIALDEF(22)
    (
    UInt8, week,
    std::string, name[0],
    std::string, name[1],
    std::string, name[2],
    std::string, name[3],
    std::string, name[4],
    UInt8, heroId[0],
    UInt8, heroId[1],
    UInt8, heroId[2],
    UInt8, heroId[3],
    UInt8, heroId[4],
    UInt32, sufferTotal,
    UInt32, sufferCnt[0],
    UInt32, sufferCnt[1],
    UInt32, sufferCnt[2],
    UInt32, sufferCnt[3],
    UInt32, sufferCnt[4],
    UInt32, lasttime[0],
    UInt32, lasttime[1],
    UInt32, lasttime[2],
    UInt32, lasttime[3],
    UInt32, lasttime[4]
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

SPECIALBEGIN(GObject::DBTeamCopyPlayer)
SPECIALDEF(6)
(
    UInt64, playerId,
    UInt8,  copyId,
    UInt8,  type,
    UInt8,  pass,
    UInt8,  passTimes,
    UInt32, vTime
)
SPECIALEND()

SPECIALBEGIN(GObject::DBTeamCopyPlayerAward)
SPECIALDEF(5)
(
    UInt64, playerId,
    UInt8,  rollId,
    UInt8,  roll,
    UInt32, awardId,
    UInt32, awardCnt
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
	Int32, attack,
	Int32, magatk,
	Int32, defend,
	Int32, magdef,
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

SPECIALBEGIN(GObject::DBSecondSoul)
SPECIALDEF(8)
(
	UInt32, fighterId,
    UInt64, playerId,
	UInt8, cls,
	UInt8, xinxiu,
	UInt8, practiceLevel,
	UInt8, stateLevel,
	UInt32, stateExp,
    std::string, skills
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

SPECIALBEGIN(GObject::DBFighter2)
SPECIALDEF(55)
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
	UInt32, halo,
	UInt32, fashion,
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
    std::string, cittas,
    UInt8, attrType1,
    UInt16, attrValue1,
    UInt8, attrType2,
    UInt16, attrValue2,
    UInt8, attrType3,
    UInt16, attrValue3,
	UInt32, fighterId,
	UInt8, cls,
	UInt8, xinxiu,
	UInt8, practiceLevel,
	UInt8, stateLevel,
	UInt32, stateExp,
    std::string, skills_2nd,
    Int32, strength,
    Int32, physique,
    Int32, agility,
    Int32, intelligence,
    Int32, will,
    Int32, soul,
    Int32, attack,
    Int32, defend,
    Int32, critical,
    Int32, pierce,
    Int32, evade,
    Int32, counter,
    Int32, tough,
    Int32, action,
    UInt8, hideFashion
	)
SPECIALEND()


SPECIALBEGIN(GObject::DBFighterObj)
SPECIALDEF(31)
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
    std::string, cittas,
    UInt8, attrType1,
    UInt16, attrValue1,
    UInt8, attrType2,
    UInt16, attrValue2,
    UInt8, attrType3,
    UInt16, attrValue3
	)
SPECIALEND()


SPECIALBEGIN(GData::DBTownDeamonMonster)
SPECIALDEF(4)
(
	UInt16, level,
    UInt32, npcId,
    UInt32, itemId,
    UInt32, itemNum
)
SPECIALEND()

SPECIALBEGIN(GData::DBNpcGroup)
SPECIALDEF(9)
(
	UInt32, id,
	std::string, fighterId,
	UInt16, formationId,
	UInt8, type,
	UInt32, experience,
	std::string, lootId[0],
	std::string, lootId[1],
	std::string, lootId[2],
	std::string, lootId[3]
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

SPECIALBEGIN(GData::DBTeamCopy)
SPECIALDEF(4)
	(
	UInt8, id,
	UInt8, type,
	UInt16, location,
	std::string, npcgroups
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
SPECIALDEF(29)
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
	UInt8,  hasBattle,
    UInt32, battleScore,
    UInt32, dailyBattleScore,
    UInt32, battleRanking
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
SPECIALDEF(21)
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
	UInt32, lastFavorTime[3],
    UInt32, signupRankBattleTime,
    UInt32, rankBattleField
)
SPECIALEND()

SPECIALBEGIN(GObject::DBClanItem)
SPECIALDEF(4)
(
    UInt32, clanid,
    UInt64, playerid,
    UInt32, itemid,
    UInt32, itemnum
)
SPECIALEND()

SPECIALBEGIN(GObject::DBClanItemHistory)
SPECIALDEF(6)
(
    UInt32, id,
    UInt32, clanid,
    UInt8,  type,
    UInt32, time,
    UInt64, playerid,
    std::string, itemstr
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

SPECIALBEGIN(GObject::DBClanStatue)
SPECIALDEF(4)
(
    UInt32, clanId,
    UInt16, level,
    UInt32, exp,
    UInt32, expUpdateTime
)
SPECIALEND()

SPECIALBEGIN(GObject::DBClanCopy)
SPECIALDEF(5)
(
    UInt32, clanId,
    UInt16, level,
    UInt32, levelUpdateTime,
    UInt16, maxLevel,
    UInt32, maxLevelTime
)
SPECIALEND()

SPECIALBEGIN(GObject::DBClanCopyLog)
SPECIALDEF(5)
(
    UInt32, clanId,
    UInt32, logTime,
    UInt8,  logType,
    std::string, playerName,
    UInt32, logVal
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
SPECIALDEF(20)
(
	UInt32, id,
	UInt32, itemId,
	UInt8, enchant,
    UInt8, tRank,
    UInt8, maxTRank,
    UInt32, trumpExp,
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

SPECIALBEGIN(GObject::DBEquipmentSpirit)
SPECIALDEF(8)
(
	UInt32, id,
	Int16, splev1,
	Int16, splev2,
	Int16, splev3,
	Int16, splev4,
	UInt8, spform1,
	UInt8, spform2,
	UInt8, spform3
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

SPECIALBEGIN(GObject::DBAutoFrontMap)
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
SPECIALDEF(39)
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
    UInt32, tael,
	UInt16,  winstreak,
    UInt16,  bewinstreak,
    UInt16,  failstreak,
    UInt16,  befailstreak,
    UInt32, oldrank,
    UInt32, first4rank,
    UInt32, extrachallenge,
    UInt8,  pageNum,
    UInt32, eChallengeTime,
    UInt8, ePhysical,
    UInt8, eSelectIndex,
    UInt32, eCombine[0],
    UInt32, eCombine[1],
    UInt32, eCombine[2],
    UInt32, eCombine[3],
    UInt32, eCombine[4],
    UInt64, eRival[0],
    UInt64, eRival[1],
    UInt64, eRival[2],
    UInt64, eRival[3],
    UInt64, eRival[4],
    UInt8, eCanAttack[0],
    UInt8, eCanAttack[1],
    UInt8, eCanAttack[2],
    UInt8, eCanAttack[3],
    UInt8, eCanAttack[4],
    UInt8, eRivalType[0],
    UInt8, eRivalType[1],
    UInt8, eRivalType[2],
    UInt8, eRivalType[3],
    UInt8, eRivalType[4]
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
	UInt32, extra
)
SPECIALEND()

SPECIALBEGIN(GObject::DBClanSkill)
SPECIALDEF(3)
(
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
SPECIALDEF(7)
(
	UInt64, id,
	UInt8, round,
	UInt8, state,
	UInt8, group,
	UInt8, recieved,
	UInt16, pos,
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

SPECIALBEGIN(GObject::DBMoneyLog)
SPECIALDEF(7)
(
    Int32, time,
    Int32, type,
    Int32, gold,
    Int32, coupon,
    Int32, tael,
    Int32, achievement,
    Int32, prestige
)
SPECIALEND()

SPECIALBEGIN(GObject::DBCopyData)
SPECIALDEF(5)
(
    UInt64, playerId,
    UInt8, id,
    UInt8, floor,
    UInt8, spot,
    UInt8, lootlvl
)
SPECIALEND()

SPECIALBEGIN(GObject::DBFrontMapData)
SPECIALDEF(6)
(
    UInt64, playerId,
    UInt8, id,
    UInt8, spot,
    UInt8, count,
    UInt8, status,
    UInt8, lootlvl
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

SPECIALBEGIN(GObject::DBActivityData)
SPECIALDEF(5)
(
    UInt64, playerId,
    UInt32, overTime,
    /*
    UInt32, awardID,
    UInt32, point,
    UInt32, award,
    std::string, flags,
    */
    UInt32, scores,
    UInt32, propsID,
    std::string, signRecord
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

SPECIALBEGIN(GObject::DBWBoss)
SPECIALDEF(5)
(
    UInt8, idx,
    UInt32, last,
    UInt32, hp,
    Int32, atk,
    Int32, matk
)
SPECIALEND()

SPECIALBEGIN(GObject::DBDiscount)
SPECIALDEF(7)
(
    UInt16, itemID,
    UInt32, beginTime,
    UInt32, endTime,
    UInt16, priceOriginal,
    UInt16, priceDiscount,
    UInt8, type,
    UInt32, count
)
SPECIALEND()

SPECIALBEGIN(GObject::DBLuckyLog)
SPECIALDEF(2)
(
    std::string, name,
    std::string, items
)
SPECIALEND()

SPECIALBEGIN(GObject::DBRealItemAward)
SPECIALDEF(4)
(
    UInt32, id,
    UInt32, cd,
    std::string, card_no,
    std::string, card_psw
)
SPECIALEND()

SPECIALBEGIN(GObject::DBNewRelation)
SPECIALDEF(3)
(
    UInt64, playerId,
    UInt8, mood,
    std::string, sign
)
SPECIALEND()

SPECIALBEGIN(GObject::DBRNR)
SPECIALDEF(2)
(
    UInt64, id,
    std::string, record
)
SPECIALEND()

SPECIALBEGIN(GObject::DBHeroMemo)
SPECIALDEF(3)
(
    UInt64, playerId,
    std::string, awards,
    std::string, memos
)
SPECIALEND()

SPECIALBEGIN(GObject::DBShuoShuo)
SPECIALDEF(3)
(
    UInt64, playerId,
    UInt32, update,
    std::string, ss
)
SPECIALEND()

SPECIALBEGIN(GObject::DBCFriendAwards)
SPECIALDEF(3)
(
    UInt64, playerId,
    UInt64, invitedId,
    std::string, awards
)
SPECIALEND()

SPECIALBEGIN(GObject::DBVersion)
SPECIALDEF(1)
(
	std::string, version
)
SPECIALEND()

SPECIALBEGIN(GObject::DBBigLock)
SPECIALDEF(2)
(
	UInt32, playerId,
    UInt32, lockExpireTime
)
SPECIALEND()

SPECIALBEGIN(GObject::DBStrengthenData)
SPECIALDEF(9)
(
    UInt64, playerId,
    UInt32, overTime,
    UInt8, souls,
    UInt8, soulId,
    std::string, flags,
    std::string, greenBox,
    std::string, blueBox,
    std::string, purpleBox,
    std::string, orangeBox
)
SPECIALEND()

SPECIALBEGIN(GObject::DBJobHunter)
SPECIALDEF(12)
(
	UInt64, playerId,
	std::string, fighterList,
	std::string, mapInfo,
	UInt8, progress,
	UInt8, posX,
	UInt8, posY,
	UInt8, earlyPosX,
	UInt8, earlyPosY,
	UInt32, stepCount,
    UInt8, slotVal1,
    UInt8, slotVal2,
    UInt8, slotVal3
)
SPECIALEND()

SPECIALBEGIN(GObject::DBCopyFrontWin)
SPECIALDEF(5)
(
    UInt64, playerId,
    UInt8, posOrig,
    UInt8, posPut,
    UInt32, itemId,
    UInt16, ratio
)
SPECIALEND()

SPECIALBEGIN(GData::DBLBSkill)
SPECIALDEF(16)
(
    UInt16, id,
    std::string, name,
    UInt8, lbtype,
    UInt8, level,
    UInt8, target,
    float,  prob,
    UInt16, cond,
    UInt16, cond2,
    UInt8, area,
    std::string, factor,
    Int16, last,
    UInt16, cd,

    UInt16, ef_type,
    UInt16, minFactor,
    UInt16, ef_value,
    UInt16, battlepoint
)
SPECIALEND()

}

#endif // _GOBJECTDBEXECHELPER_H_
