#ifndef _CLAN_H_
#define _CLAN_H_

#include "Common/Stream.h"
#include "Common/TimeUtil.h"
#include "GObjectManager.h"
#include "ClanDynamicMsg.h"
#include "Server/ServerTypes.h"

class SysMsgItem;
namespace GObject
{

class Player;
class ClanTech;
class ClanBattle;

#define BASE_MEMBER_COUNT 15
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

struct ClanSkill
{
    UInt8 id;
	UInt8 level;	//如果 = 0， 表示此技能尚未被激活
};

struct ClanMember
{
	ClanMember(Player * pl = NULL, UInt8 c = 0, UInt32 jt = 0) : player(pl), cls(c), joinTime(jt)
	{
		enterCount = 0;
		proffer = 0;
	}
	std::multimap<UInt32, AllocItem> allocItems;
	Player * player;
	UInt8  cls;
	UInt32 joinTime;
    UInt32 proffer;
	UInt8  enterCount;
    std::map<UInt8, ClanSkill> clanSkill;
	std::map<UInt8, ClanPlayerPet> clanPet;
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
	Clan( UInt32 id, const std::string& name, UInt32 ft = 0 );
	~Clan();
	inline void patchMergedName() { patchMergedName(_id, _name); }
	static void patchMergedName(UInt32 id, std::string& name);
public:
	UInt16 getFavorItemId(UInt8 skilId);
	inline UInt32 getTechIdIndex(UInt8 skillId){return skillId - 7;}// 青龙7、白虎8、朱雀9、玄武10 fix??????

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
    inline void setMaxMemberCount(UInt8 count) { _maxMemberCount = BASE_MEMBER_COUNT + count; }
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
	bool apply(Player *, UInt32 = 0, bool = true);
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

    // 帮派职位
    bool setClanRank(Player* pl, UInt64 inviteeId, UInt8 cls);
    UInt8 getClanRank(Player* pl);
    UInt8 getClanRankCount(UInt8 cls);


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

// 帮派技能
    void addSkillFromDB(Player* pl, UInt8 skillId, UInt8 level);
    void addSkill(ClanMember* cm, UInt8 skillId);
    UInt8 getSkillLevel(Player* pl, UInt8 skillId);
    UInt8 skillLevelUp(Player* pl, UInt8 skillId);
	void makeSkillInfo(Stream&, Player*);
	void makeSkillInfo(Stream&, Player*, UInt8 skillId);
    void listSkills(Player * player);
    void showSkill(Player* player, UInt8 skillId);
    UInt32 getSkillHPEffect(Player* pl);
    UInt32 getSkillAtkEffect(Player* pl);
    UInt32 getSkillDefendEffect(Player* pl);
    UInt32 getSkillMagAtkEffect(Player* pl);
    UInt32 getSkillMagDefentEffect(Player* pl);

    void buildTechSkill(ClanMember* cm);

public:
	inline UInt8 getLev() { return _level; }
	ClanTech * getClanTech() { return _techs; }
	ClanBattle * getClanBattle() { return _clanBattle; }
	ClanDynamicMsg * getClanDynamicMsg() { return _clanDynamicMsg; }

	inline Player * getOwner()
	{
		if (_members.size() > 0)
			return (*_members.begin())->player;
		return NULL;
	}

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
	bool _deleted;
	std::vector<std::string> _keywords;

	ClanDynamicMsg * _clanDynamicMsg;
	ClanBattle * _clanBattle;

    UInt32 _funds;          // 帮派资金
	UInt64 _watchman;       // 帮派修炼地护法

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
	void search(Player *, std::string);
	void search2(Player *, std::string);
	void listAll(Player *, UInt16, UInt8);
private:
	void searchInternal(UInt8, std::vector<std::string>&, std::set<Clan *>&);
	void makeKeywordList(const std::string&, std::vector<std::string>&);
private:
	std::map<std::string, std::set<Clan *> > _keyLists[COUNTRY_MAX];
};

extern ClanCache clanCache;

}

#endif // _CLAN_H_
