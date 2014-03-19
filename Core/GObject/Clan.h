﻿#ifndef _CLAN_H_
#define _CLAN_H_

#include <set>
#include "Common/Stream.h"
#include "Common/TimeUtil.h"
#include "GObjectManager.h"
#include "ClanDynamicMsg.h"
#include "Server/ServerTypes.h"
#include "Server/WorldServer.h"
#include "Common/BinaryReader.h"

class SysMsgItem;
namespace GObject
{

static const UInt16 COPY_RESET_LEVEL = 5;

class Player;
class ClanTech;
class ClanBattle;
class Clan;
class ClanStatue;
class ClanCopy;
struct ClanCopyLog;

#define BASE_MEMBER_COUNT 30
struct AllocItem
{
	UInt32 itemId;
	UInt8 itemNum;
};

struct AllocRecord
{
	UInt8 allocType;
	std::string playerName;
	std::vector<AllocItem> allocItems;
};

struct ClanPlayerPet
{
	UInt16 favorCount;
	UInt32 favorTime;
	UInt16 petFriendness;
	ClanPlayerPet(UInt16 count = 0,UInt32 tm = 0, UInt16 friendness = 0):favorCount(count), favorTime(tm), petFriendness(friendness){}
};


class ClanItemVisitor
{
public:
    virtual ~ClanItemVisitor(){}

    virtual bool operator()(UInt16 id, UInt32 num) = 0;
};


/**
 *@brief 帮派道具包裹
 */
struct ClanItemPkg
{
    typedef std::map<UInt16, UInt32>    ItemMap;

public:
    ClanItemPkg()
        :m_ClanId(0),m_PlayerId(0),m_MaxGrid(0), m_Grid(0){}
    ~ClanItemPkg(){}

public:
    void Init(UInt32 clanid, UInt64 playerId, UInt32 maxgrid)
    {
        m_ClanId = clanid;
        m_PlayerId = playerId;
        m_MaxGrid = maxgrid;
    }

    /**
     *@brief 设置获取最大格子数
     */
    void SetMaxGrid(UInt32 maxgrid){ if(maxgrid > m_MaxGrid) m_MaxGrid = maxgrid; }
    UInt32 GetMaxGrid() const { return m_MaxGrid; }

    /**
     *@brief 获取已使用格子数和剩余格子数
     */
    UInt32 GetGrid() const { return m_Grid; }
    UInt32 GetLeftGrid() const { return m_MaxGrid - m_Grid; }

    void LoadItem(UInt16 id, UInt32 num);

    /**
     *@brief 判断和添加道具
     */
    bool CheckAddItem(UInt16 id, UInt32 num);
    UInt32 AddItem(UInt16 id, UInt32 num);

    /**
     *@brief 获取道具数和移除道具
     */
    UInt32 GetItemNum(UInt16 id) const;
    void RemoveItem(UInt16 id, UInt32 num);

    /**
     *@brief 遍历道具列表
     */
    void VisitItems(ClanItemVisitor& visitor)
    {
        for(ItemMap::iterator iter = m_Items.begin(); iter != m_Items.end(); ++iter){
            if(!visitor(iter->first, iter->second)) return;
        }
    }

    void FillItems(Stream& stream);
    void GetItems(Player* player);

public:
    //帮会id
    UInt32      m_ClanId;
    //玩家id
    UInt64      m_PlayerId;
    //道具列表
    ItemMap     m_Items;
    //最大格子数
    UInt32      m_MaxGrid;
    //已经使用格子数
    UInt32      m_Grid;
};


/**
 * @brief 帮派道具奖励历史记录
 */
struct ClanItemHistory
{
    enum Type
    {
        CLANBATTLE = 0, //帮会战奖励
        ALLOCATED = 1,  //分配获得
        DRAWWEAL = 2,   //领取福利
        CLANBOSS = 3,   //末日之战
    };

    ClanItemHistory(UInt8 type, UInt32 time, UInt64 playerid, const std::string& itemstr)
        :m_Type(type), m_Time(time), m_PlayerId(playerid), m_ItemStr(itemstr){}

    UInt8       m_Type;
    UInt32      m_Time;
    UInt64      m_PlayerId;
    std::string m_ItemStr;
};


//个人帮会仓库大小
const static UInt32 PKGSIZE_PER_MEMBER = 20;


struct ClanMember
{
	ClanMember(Player * pl = NULL, UInt8 c = 0, UInt32 jt = 0) :
        player(pl), cls(c), joinTime(jt)
	{
		enterCount = 0;
		proffer = 0;
        signupRankBattleTime = 0;
        rankBattleField = 0;
        activepoint = 0;
        actpt_endtime = 0;
        last_actpt = 0;
	}


	std::multimap<UInt32, AllocItem> allocItems;
	Player * player;

    ClanItemPkg itemPkg;
	UInt8  cls;
	UInt32 joinTime;
    UInt32 proffer;
	UInt8  enterCount;
    UInt32 signupRankBattleTime;
    UInt32 rankBattleField;
    UInt32 activepoint;
    UInt32 actpt_endtime;
    UInt32 last_actpt;
	std::map<UInt8, ClanPlayerPet> clanPet;
};


typedef Visitor<ClanMember> ClanMemberVisitor;

struct ClanOpt
{
    UInt8 type;
    Clan* clan;
};

struct ClanPendingMember
{
	ClanPendingMember(Player * pl = NULL, UInt8 c = 0, UInt32 ot = 0): player(pl), cls(c), opTime(ot) {}
	Player * player;
	UInt8  cls;
	UInt32 opTime;
};

struct MemberDonate
{
	std::string donateName;
    UInt8 donateTo;
    UInt8 donateType;
	UInt32 donateCount;

	MemberDonate() {}
	MemberDonate(const std::string& dn, UInt8 dto, UInt8 dtype, UInt32 dc) : donateName(dn), donateTo(dto), donateType(dtype), donateCount(dc) {}
};
typedef std::multimap<UInt32, MemberDonate> MemberDonates;

struct DuoBaoLog
{
	UInt32 clanId;
	std::string name;
	UInt16 score;
    UInt32 itemId;
    UInt8 cnt;
    DuoBaoLog() : clanId(0), name(""), score(0), itemId(0), cnt(0) {}
};
typedef std::list<DuoBaoLog> DuoBaoLogs;

struct ClanSpiritTree
{
    UInt32 m_exp;
    UInt8 m_level;
    UInt32 m_endTime;
    UInt8 m_refreshTimes;
    UInt8 m_color;

    ClanSpiritTree(): m_exp(0), m_level(0), m_endTime(0), m_refreshTimes(0), m_color(0) {}
};

enum CLAN_ACTPT_FLAG
{
    e_clan_actpt_none = 0,
    e_clan_actpt_rank_battle = 1,
    e_clan_actpt_clanboss = 2,
    e_clan_actpt_clan_city = 3,
};

enum CLAN_DONATE_TO
{
    e_donate_to_build = 1,
    e_donate_to_tree = 2,
};

enum CLAN_DONATE_TYPE
{
    e_donate_type_tael = 1,
    e_donate_type_gold = 2,
};

class Clan:
	public GObjectBaseT<Clan>
{
public:
    //帮会排名战报名数下限
    const static UInt32 RANK_BATTLE_MIN_SIGNUP_NUM = 5;
    //帮会排名战单个战役人数
    const static UInt32 RANK_BATTLE_FIELD_PLAYERNUM = 15;

    //道具分配记录
    typedef std::list<ClanItemHistory> ItemHistoryList;

private:

	friend class ClanCache;
	friend class ClanBattle;
	friend class ClanCityBattle;
	friend class ClanRobBattle;

	struct MemberLess
	{
		bool operator()(const ClanMember * mem1, const ClanMember * mem2) const
		{
            if(mem1->cls == mem2->cls)
				return mem1->joinTime < mem2->joinTime;
            else
                return mem1->cls > mem2->cls;

#if 0
			if (mem1->cls == 4)
				return true;
			else if (mem2->cls == 4)
				return false;
			else if(mem1->proffer == mem2->proffer)
				return mem1->joinTime < mem2->joinTime;
			return mem1->proffer > mem2->proffer;
#endif
		}
	};
	typedef std::multiset<ClanMember *, MemberLess> Members;

    struct ScoreSort
    {
        GObject::Player* player;
        UInt16 score;
        UInt32 time;
        ScoreSort():player(NULL),score(0),time(0){}
    };

    struct lt_sort
    {
        bool operator()(const ScoreSort& a, const ScoreSort& b) const { return a.score > b.score || (a.score==b.score && a.time < b.time); }
    };

    struct ScoreSort32
    {
        GObject::Player* player;
        UInt32 score;
        UInt32 time;
        ScoreSort32():player(NULL),score(0),time(0){}
    };

    struct lt_sort32
    {
        bool operator()(const ScoreSort32& a, const ScoreSort32& b) const { return a.score > b.score || (a.score==b.score && a.time < b.time); }
    };

    typedef std::multiset<ScoreSort, lt_sort> ScoreSortType;
    typedef std::multiset<ScoreSort32, lt_sort32> ScoreSortType32;
public:
    ScoreSortType DuoBaoScoreSort;     //夺宝排名
    ScoreSortType32 TYSSScoreSort;     //天元神兽排名

public:
	Clan( UInt32 id, const std::string& name, UInt32 ft = 0, UInt8 lvl = 1 );
	~Clan();
#if 0
    inline void patchMergedName() { patchMergedName(_id, _name); }
	static void patchMergedName(UInt32 id, std::string& name);
#else
    inline void patchMergedName() { patchMergedName(_founder, _name); }
	static void patchMergedName(UInt64 id, std::string& name);
#endif
public:
	UInt16 getFavorItemId(UInt8 skilId);
	inline UInt32 getTechIdIndex(UInt8 skillId){return skillId - 7;}// ????7???׻?8????ȸ9??????10 fix??????

public:
	UInt16 getPetFriendness(ClanMember *mem, UInt8 skillId){return mem->clanPet[skillId].petFriendness;}

public:
	float getAutoBattleSpeed();
	UInt8 getAutoBattleEffect();

public:
	inline const std::string& getName() const { return _name; }
	inline void setName(const std::string& name) { _name = name; }
	inline UInt32 getCount() { return _members.size(); }
    inline UInt32 getMaxMemberCount() { return _maxMemberCount; }
    void setMaxMemberCount(UInt8 count);
	inline bool isFull() { return getCount() >= _maxMemberCount; }
	inline UInt32 getFoundTime() { return _foundTime; }
	inline UInt64 getFounder() { return _founder; }
	inline std::string getFounderName() { return _founderName; }
	void setLeaderId(UInt64, bool = true);
	bool setWatchmanId(UInt64, bool = true);
	inline UInt64 getWatchmanId() { return _watchman; }
	void fixLeaderId();
	inline UInt64 getLeaderId() { return _leader; }
	inline Player * getLeader() { return _members.empty() ?	NULL : (*(_members.begin()))->player; }
    inline UInt64 getConstruction() { return _construction; }
    void setConstruction(UInt64 cons, bool = true);
    void addConstruction(UInt64 cons, bool = true);
	bool alterLeader();
	UInt32 getDonateAchievement(Player *);
    UInt32 getMemberProffer(Player * player);
	void setFounder(UInt64);
	UInt8 getCountry();
	bool accept(Player *, UInt64);
	bool decline(Player*, UInt64);
	bool join(Player *, UInt8 = 0, UInt16 = 0, UInt32 = 0, UInt32 = 0, UInt32 = 0, UInt32 = 0);
	bool join(ClanMember *);	//Only called by db
	bool kick(Player *, UInt64);
	bool leave(Player *);
	bool handoverLeader(Player *, UInt64);
	bool invite(Player *, std::string);
	bool invite(Player *, UInt32 = 0, bool = true);
	UInt8 apply(Player *, UInt32 = 0, bool = true);
	bool declineInvite(Player *);
	void updateRank(ClanMember * = NULL, std::string = "");
	static UInt8 buildRank(UInt16);
	bool donate(Player *, UInt8, UInt16, UInt32);
	bool checkDonate(Player *, UInt8, UInt16, UInt32);
	bool GMDonate(Player * player, UInt8 skillId, UInt16 type, UInt32 count);
	inline const std::string& getContact() { return _contact; }
	void setContact(const std::string& c, bool = true);
	inline const std::string& getAnnounce() { return _announce; }
	void setAnnounce(const std::string& c, bool = true);
	inline UInt32 getProffer() { return _proffer; }
	void setProffer(UInt32, bool = true);
	inline const std::string& getPurpose() { return _purpose; }
	void setPurpose(const std::string& c, bool = true);
	UInt8 setRepoNum(UInt32, UInt8, bool = true);
	void addRepoNum(UInt32 itemId, UInt8 itemNum);
	UInt8 addRepo(UInt32, UInt8, UInt8);
	UInt8 getFromRepo(UInt32, UInt8);
	void sendRepoInfo(Player * = NULL);
	void allocateRepoAsReward(Player *, std::map<UInt32, UInt8>&);
	void sendPersonalRewardList(Player *);
	void sendPersonalRewardList(ClanMember&);
	void takeReward(Player *, UInt8, UInt32, UInt16);
	void takeRewardResult(Player *, bool, UInt32, const AllocItem&);
	void sendAllocRecords(Player *);
	void setPendingReward(Player *, UInt32, UInt32, UInt8);
	void addAllocRecord(UInt32, AllocRecord&, bool = true);
	void sendRepoMail(UInt8, UInt8);
	void initBuildClan();
	void disband(Player *);
	bool hasClanAuthority(Player *, UInt8);
    float getClanTechAddon();
    UInt8 getPracticeSlot();
    void sendPracticePlaceInfo(Player* pl);
    void broadcastPracticePlaceInfo();
    void setClanSpiritTreeBuff(UInt8 );

    // 帮派职位
    bool setClanRank(Player* pl, UInt64 inviteeId, UInt8 cls);
    UInt8 getClanRank(Player* pl);
    UInt8 getClanRankCount(UInt8 cls);

    void addMemberProffer(Player*pl, UInt32);

    /**
     *@brief 报名帮会排名战
     */
    UInt32 GetRankBattleField(Player* player, UInt32 now);
    bool SignupRankBattle(Player* player, UInt32 field, UInt32 now);
    bool SignoutRankBattle(Player* player, UInt32 now);
    UInt32 GetSignupRankBattleNum(UInt32 now);
    UInt32 AdjustRankBattleField(Player* player, UInt32 field, UInt32 now);

    /**
     *@brief 判断是否满足参赛资格，并获取报名列表
     *@return 0表示没报名 1表示报名了人数没够 2表示有资格
     */
    UInt32 CheckJoinRankBattle(UInt32 now, std::map<UInt32, std::vector<Player*> >& list);
    /**
     *@brief 战斗积分相关
     */
    void SetBattleScore(UInt32 score);
    UInt32 GetBattleScore() const { return m_BattleScore; }
    void LoadBattleScore(UInt32 score){ m_BattleScore = score;}

    void SetDailyBattleScore(UInt32 score);
    UInt32 GetDailyBattleScore() const { return m_DailyBattleScore; }
    void LoadDailyBattleScore(UInt32 score) { m_DailyBattleScore = score; }


    /**
     *@brief 帮会战排名相关
     */
    void SetLastBattleRanking(UInt32 ranking);
    UInt32 GetLastBattleRanking() const { return m_LastBattleRanking; }
    void LoadLastBattleRanking(UInt32 ranking){ m_LastBattleRanking = ranking;}

    void SetBattleRanking(UInt32 ranking){ m_BattleRanking = ranking; }
    UInt32 GetBattleRanking() const { return m_BattleRanking; }

    /**
     *@brief 广播帮会战相关数据
     */
    void BroadcastBattleData(UInt32 now);


    /**
     *@brief 加载仓库道具
     */
    void LoadItem(UInt64 playerid, UInt32 itemid, UInt32 num);
    void LoadItemHistory(UInt8 type, UInt32 time, UInt64 playerId, const std::string& itemstr);

    UInt32 AddItem(UInt32 itemid, UInt32 num);
    void AddItemHistory(UInt8 type, UInt32 time, UInt64 playerId, const std::string& itemstr);

    UInt32 GetGridNum() const { return _itemPkg.GetLeftGrid(); }

    /**
     *@brief 发送仓库信息
     */
    void SendPackageInfo(Player* player);
    void SendItemList(Player* player);
    void SendItemHistory(Player* player, UInt16 start, UInt8 count);
    void SendSelfItemList(Player* player);
    void ClearDueItemHistory();
    void SendDonateHistory(Player* player, UInt16 startIdx, UInt8 count);

    /**
     *@brief 分配奖励
     */
    void DistributeItem(Player* player, UInt64 memId, UInt16 itemId, UInt16 num);

    /**
     *@brief 获取福利和道具奖励
     */
    void GetWeal(Player* player);
    void GetItems(Player* player);
    void sendQQOpenid(Player* player);
    void setQQOpenid(Player* player,std::string openid); 
    void offQQOpenid(Player* player); 
    void setQQOpenid(std::string openid) {m_qqOpenid = openid;};
    void sendClanBattle(Player *player, Player* caller);
    void broadcastClanBattle(Player *caller);

public:
	inline bool alive() { return !_deleted; }

public:
	void notifyAllyClanInfo(Player * = NULL);
	void notifyAllyClanInfo(Clan *, UInt8, UInt8, Player * = NULL);

public:
	void retEnterPlsyersCount(UInt16 *count);

public:
	UInt32 getAllyClanFromDB() { return _allyClanId; }
	void addAllyClanFromDB(UInt32 ci) { _allyClanId = ci; }
	void addAllyClanFromDB(Clan * c) { _allyClan = c; }
	bool addAllyClan(Player *, Player *, Clan *);
	bool delAllyClan(Player *, Clan *);
	bool hasAllyClan() { return _allyClan != NULL; }
	bool hasAllyClan(Clan * c) { return c == _allyClan; }
	bool hasAllyClan(UInt32 ci) { return ci == _allyClanId; }
	Clan * getAllyClan() { return _allyClan; }
	void addEnemyClanFromDB(UInt32);
	void addEnemyClanFromDB(Clan *);
	bool addEnemyClan(Player *, Clan *);
	bool delEnemyClan(Player *, Clan *);
	std::vector<UInt32> getEnemyClanFromDB() { return _enemyClanId; }
	void setEnemyClanFromDB(std::vector<UInt32>& ids) { _enemyClanId = ids; }
	std::vector<Clan *> getEnemyClans() { return _enemyClan; }
	bool hasEnemyClan(UInt32);
	bool hasEnemyClan(Clan *);
	bool hasEnemyClanFull() { return _enemyClan.size() >= 2; }
	void updateEnemyClandb();

public:
	void sendClanDynamicMsg(Player *, UInt8, UInt16, UInt16);
    void sendClanList(Player *player, UInt8 type, UInt8 start, UInt8 cnt);

private:
	void addAllyClan(Clan *, bool = true);
	void delAllyClan(Clan *);

public:
    void addClanDonateRecordFromDB(const std::string& dn, UInt8 dto, UInt8 dtype, UInt32 dc, UInt32 dt);
    void addClanDonateRecord(const std::string& dn, UInt8 dto, UInt8 dtype, UInt32 dc, UInt32 dt);
    void checkClanDonateRecord();

    // 帮派资金
    void setClanFunds(UInt32 funds) { _funds = funds; }
    void addClanFunds(UInt32 funds);
    void useClanFunds(UInt32 funds);
    UInt32 getClanFunds() { return _funds; }

    void addClanGradeInAirBook(UInt32 grade);
    void updataClanGradeInAirBook(Player * pl= NULL);
    
    //天元神兽
    void SendClanMemberAward(UInt32 score, UInt8 flag ,std::string str);
    void LoadTYSSScore(Player* pl);
    void SetTYSSScore(Player * pl);
    void SendTYSSScore(Player* pl);
    void DelTYSSScore(Player* pl);
    void ClearTYSSScore();

    
    UInt32 getGradeInAirBook(){return  _gradeInAirbook;}
    void SendClanMemberGrade(Player* player);
    UInt8 skillLevelUp(Player* pl, UInt8 skillId);
public:
	inline UInt8 getLev() { return _level; }

	ClanTech * getClanTech() { return _techs; }
	ClanBattle * getClanBattle() { return _clanBattle; }
	ClanDynamicMsg * getClanDynamicMsg() { return _clanDynamicMsg; }

public:
    // 帮派副本，帮派神像

    void LoadStatue(UInt16 level, UInt32 exp, UInt32 expUpdateTime);
    void LoadCopy(UInt16 level, UInt32 levelUpdateTime, UInt16 maxLevel, UInt32 maxTime);
    void LoadCopyLog(UInt32 logTime, UInt8 logType, std::string playerName, UInt32 logVal);

    void updateStatueExp();
    void resetCopyLevel();

    bool copyLevelAvailable();
    void sendClanCopyInfo(Player * player, UInt8 val = 0);
    void notifyCopyCreated(Player * player);

    void   addStatueExp(UInt32 exp);
    void   subStatueExp(UInt32 exp);

    UInt16 getStatueLevel();
    UInt32 getStatueExp();
    UInt32 getStatueNextExp();
    UInt32 getStatueConsumeExp();
    UInt32 getStatueExHp();
    UInt32 getStatueExAttack();
    UInt32 getStatueExDefend();
    UInt32 getStatueExMagAtk();
    UInt32 getStatueExMagDef();
    UInt32 getStatueExAction();
    UInt32 getStatueExHitRate();

    void   notifyUpdateStatueAttr();

    UInt16 getCopyLevel();
    UInt32 getOutputExp();
    UInt32 getNextOutputExp();
    UInt8  getCopyStatus();
    UInt16 getCopyMeberCount();

    void   clanCopyTabOperate(Player * player, UInt8 command, UInt8 val = 0);
    void   clanCopyMemberOperate(Player * player, UInt8 command, BinaryReader& brd);
    void   clanCopyBattleOperate(Player * player, UInt8 command, BinaryReader & brd);

    void   addCopyLevel();
    void   addCopyWinLog(Player* player);

    void   setCopyLevel(UInt16 level);

    void   clearCopySnap();
    void   insertIntoCopySnap(Player *player, UInt8 spotId);
    UInt8  getCopyPlayerSnap(Player *player);

public:
    void LoadDuoBaoLog(const std::string& name, UInt16 score, UInt32 itemId, UInt8 cnt);
    void LoadDuoBaoScore(Player * pl);
    void DuoBaoInfo(Player * pl);
    void DuoBaoStart(Player * pl);
    void SendDuoBaoAward();
    void ClearDuoBaoData();

private:
    void SendDuoBaoLog(Stream & st);
    void SetDuoBaoScore(Player * pl);
    void DelDuoBaoScore(Player * pl);
    void SendDuoBaoScore(Stream & st);
    void DuoBaoLvlAward();
    void DuoBaoUpdate(const std::string& playerName, UInt16 score);
    void DuoBaoDel(UInt8 mark);
    void ClearDuoBaoLog();
    void BroadDuoBaoBegin(Player * player);
    void DuoBaoBroadcast(Stream& st);
public:
    void sendMemberBuf(UInt8 pos);

public:

    inline Player * getOwner()
    {
        if (_members.size() > 0)
            return (*_members.begin())->player;
        return NULL;
    }

    UInt8 getOnlineMembersCount();
    void VisitMembers(ClanMemberVisitor& visitor);
	void listMembers(Player *);
	void listPending(Player *);
	void sendInfo(Player *);
	void searchMatch(Player *);
	void appendListInfo(Stream&);

	void listTechs(Player *);
	void listDonators(Player *);

	void broadcastMemberInfo(ClanMember &, UInt8);
	void broadcastMemberInfo(Player *);
	void broadcastPendingMemberInfo(ClanPendingMember &);

	void broadcast(Stream&);
	void broadcast(const void *, size_t);
	void broadcast(SysMsgItem *);
    void broadcastCopyInfo();

    void setXianyun(UInt32 num) {_xianyun = num;}
    UInt32 getXianyun() {return _xianyun;}
    void addXianyun(int num) 
    {
        if (0 == num)
            return;
        if (num < 0 && (num+_xianyun) < 0)
            _xianyun = 0;
        else
            _xianyun += num;
	    DB5().PushUpdateData("UPDATE `clan` SET `xianyun` = %u WHERE `id` = %u", _xianyun, _id);
    }
    UInt8 getUrge(UInt8 t)
    {
        if (t >= 3)
            return 0;
        return _urge[t];
    }
    void setUrge(UInt8 t, UInt8 n, bool toDB = true)
    {
        if (t < 3)
        {
            _urge[t] = n;
            if (toDB)
                urgeToDB();
        }
    }
    void clearUrge()
    {
        memset(_urge, 0, sizeof(_urge));
        urgeToDB();
    }
    void addUrge(UInt8 t, UInt8 n)
    {
        if (t < 3)
        {
            _urge[t] += n;
            urgeToDB();
        }
    }
    void urgeToDB()
    {
         UInt32 t = _urge[0] + ((UInt32)_urge[1]<<8) + ((UInt32)_urge[2]<<16);
         DB5().PushUpdateData("UPDATE `clan` SET `urge` = %u WHERE `id` = %u", t, _id);
    }
    UInt32 getGongxian() {return _gongxian;}
    UInt32 addGongxian(int num)    {
        _gongxian += num;
	    DB5().PushUpdateData("UPDATE `clan` SET `gongxian` = %u WHERE `id` = %u", _gongxian, _id);
        return _gongxian;
    }
    void setGongxian(UInt32 num, bool toDB=false)
    {
        _gongxian = num;
        if (toDB)
            DB5().PushUpdateData("UPDATE `clan` SET `gongxian` = %u WHERE `id` = %u", _gongxian, _id);
    }

    void SetDuoBaoAward(UInt32 itemId) { _duoBaoAward = itemId; }
    UInt32 GetDuoBaoAward() {return _duoBaoAward;}
    
    void SetTYSSSum(UInt32 num,bool toDB=false) 
    {
        _tyssSum= num; 
        if (toDB)
            DB5().PushUpdateData("UPDATE `clan` SET `tyssSum` = %u WHERE `id` = %u", _tyssSum, _id);
    }
    UInt32 GetTYSSSum() {return _tyssSum;}
    void AddTYSSSum(UInt32 num)
    {
        _tyssSum += num; 
        DB5().PushUpdateData("UPDATE `clan` SET `tyssSum` = %u WHERE `id` = %u", _tyssSum, _id);
    }

public:
	ClanMember * getClanMember(Player *);
	bool existClanMember(Player *);
	bool existClanMember(UInt64);

private:
	Members::iterator find(Player *);
	Members::iterator find(UInt64);
	Members::iterator end();

	void purgePending();
	void purgeAlloc();
	void purgeAlloc(UInt32, ClanMember&);
	void getAllocBack(ClanMember&);
	void writeRepoNumToDB(std::map<UInt32, UInt8>::iterator);
	void writeAllocRecordToDB(UInt32, AllocRecord&);

private:
	static UInt8 _maxEnterCount;//
	std::string _name;
	Members _members;
    UInt8 _maxMemberCount;
	std::set<UInt32> _membersJoinTime;	// 保证每个成员加入的时间不一样
	std::vector<ClanPendingMember *> _pending;
	UInt8 _rank;
	UInt8 _level;
    UInt32  AirBookGrade;

    ClanItemPkg _itemPkg;
    ItemHistoryList _itemHistories;

    UInt32 _foundTime;
	UInt64 _founder;
	std::string _founderName;
	UInt64 _leader;
	UInt64 _construction;            // 帮派建设度
	UInt32 _nextPurgeTime;
	std::string _contact;
	std::string _announce;
	std::string _purpose;
	UInt32 _proffer;

    UInt32 m_BattleScore;   //战斗积分
    UInt32 m_DailyBattleScore;  //每日战斗积分
    UInt32 m_LastBattleRanking; //上周战斗名次
    UInt32 m_BattleRanking;  //本周战斗名次

	UInt32 _favorId[4];
	UInt32 _flushFavorTime;

	Clan * _allyClan;
	UInt32 _allyClanId;
	std::vector<Clan *> _enemyClan;
	std::vector<UInt32> _enemyClanId;

	//std::map<UInt8, MemberDonates> _memberDonates;
	MemberDonates _memberDonates;
    DuoBaoLogs _duobaoLogs;

	std::map<UInt32, UInt8> _repoNum;
	std::multimap<UInt32, AllocRecord> _allocRecords;

	ClanTech * _techs;
    ClanStatue * _statue;
    UInt16 _copyLevel;
    UInt32 _copyLevelUpdateTime;
    UInt16 _copyMaxLevel;
    UInt32 _copyMaxTime;

    UInt32 _lastCallTime;

    std::list<ClanCopyLog> _copyLog;
    std::map<Player *, UInt8> _copySpotSnap;

	bool _deleted;
	std::vector<std::string> _keywords;

	ClanDynamicMsg * _clanDynamicMsg;
	ClanBattle * _clanBattle;

    UInt32 _funds;          // 帮派资金
	UInt64 _watchman;       // 帮派修炼地护法

    UInt32 _gradeInAirbook; //天书奇缘帮派积分
    UInt32 _gradeInTYSS; //天元神兽帮派积分
    std::string m_qqOpenid;

	Mutex _mutex;

    UInt32 _xianyun;
    UInt32 _gongxian;
    UInt8 _urge[3];
    UInt32 _duoBaoAward;
    UInt32 _tyssSum;

    ClanSpiritTree m_spiritTree;
public:
    void raiseSpiritTree(Player* pl, UInt8 type);   // 培养 type(甘露:0 冰晶:1)
    void refreshColorAward();
    void getSpiritTreeAward(Player* pl, UInt8 idx); // 全部领取:idx=0xFF
    void sendSpiritTreeInfo(Player* pl, bool bc=false);
    void checkSpiritTree();
    void writeSptrToDB();
    void loadSptrFromDB(UInt32 exp, UInt8 lvl, UInt32 endtime, UInt8 times, UInt8 color);

    void addMemberActivePoint_nolock(Player* pl, UInt32 actpt, CLAN_ACTPT_FLAG f);
    void addMemberActivePoint(Player* pl, UInt32 actpt, CLAN_ACTPT_FLAG f);
    UInt32 getMemberActivePoint(Player* pl);
    void checkMemberActivePoint(ClanMember* mem);
    void listMembersActivePoint( Player * player );
    UInt32 getMemberActivePoint(ClanMember* mem);
};

typedef GGlobalObjectManagerT<Clan, UInt32> GlobalClans;
extern GlobalClans globalClans, globalClansByCountry[COUNTRY_MAX];
typedef GGlobalObjectManagerIStringT<Clan> GlobalNamedClans;
extern GlobalNamedClans globalGlobalNamedClans, globalNamedClans[COUNTRY_MAX];
extern GlobalNamedClans globalOwnedClans[COUNTRY_MAX];

class ClanCache
{
public:
	void push(Clan *, bool = true);
	void search(Player *, std::string, UInt8 flag);
	void search2(Player *, std::string);
	void listAll(Player *, UInt16, UInt8, UInt8);
private:
	void searchInternal(UInt8, std::vector<std::string>&, std::set<Clan *>&);
	void makeKeywordList(const std::string&, std::vector<std::string>&);
private:
	std::map<std::string, std::set<Clan *> > _keyLists[COUNTRY_MAX];
};

extern ClanCache clanCache;

}

#endif // _CLAN_H_
