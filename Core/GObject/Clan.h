#ifndef _CLAN_H_
#define _CLAN_H_

#include <set>
#include "Common/Stream.h"
#include "Common/TimeUtil.h"
#include "GObjectManager.h"
#include "ClanDynamicMsg.h"
#include "Server/ServerTypes.h"
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
	UInt16 donateCount;
	UInt32 donateTime;

	MemberDonate() {}
	MemberDonate(const std::string& dn, UInt16 dc, UInt32 dt) : donateName(dn), donateCount(dc), donateTime(dt) {}
	bool operator<(const MemberDonate& md) const
	{
		return donateTime < md.donateTime;
	}
};
typedef std::multiset<MemberDonate> MemberDonates;

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
	void addClanDonateRecordFromDB(const std::string&, UInt8, UInt16, UInt32);
	void addClanDonateRecord(const std::string&, UInt8, UInt16, UInt32);

    // 帮派资金
    void setClanFunds(UInt32 funds) { _funds = funds; }
    void addClanFunds(UInt32 funds);
    void useClanFunds(UInt32 funds);
    UInt32 getClanFunds() { return _funds; }

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
	void listTechDonators(Player *, UInt8);

	void broadcastMemberInfo(ClanMember &, UInt8);
	void broadcastMemberInfo(Player *);
	void broadcastPendingMemberInfo(ClanPendingMember &);

	void broadcast(Stream&);
	void broadcast(const void *, size_t);
	void broadcast(SysMsgItem *);
    void broadcastCopyInfo();

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

	std::map<UInt8, MemberDonates> _memberDonates;

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

    std::string m_qqOpenid;

	Mutex _mutex;
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
