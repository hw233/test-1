#ifndef	_ATHLETICSRANK_INC_
#define _ATHLETICSRANK_INC_

#include "Common/TimeUtil.h"

namespace GObject
{

const  UInt32  OrangeEquip50[] = {1521, 1522, 1523, 1524, 1525, 1526, 1527, 1528, 1529, 1530, 1531, 1532, 1533, 1534, 1541, 1542, 1543, 1544, 1545, 1546, 1547, 1548, 1549, 1550, 1551, 1552, 1553, 1554};
const  UInt32  OrangeEquip70[] = {1561, 1562, 1563, 1564, 1565, 1566, 1567, 1568, 1569, 1570, 1571, 1572, 1573, 1574, 1581, 1582, 1583, 1584, 1585, 1586, 1587, 1588, 1589, 1590, 1591, 1592, 1593, 1594};
const  UInt32  OrangeEquip80[] = {1601, 1602, 1603, 1604, 1605, 1606, 1607, 1608, 1609, 1610, 1611, 1612, 1613, 1614, 1621, 1622, 1623, 1624, 1625, 1626, 1627, 1628, 1629, 1630, 1631, 1632, 1633, 1634};

const UInt32 PurpleEquip40[] = {261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274,
281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 
301, 302, 303, 304, 305, 306, 307, 308, 309, 310, 311, 312, 313, 314, 
321, 322, 323, 324, 325, 326, 327, 328, 329, 330, 331, 332, 333, 334};
const  UInt32 PurpleEquip50[] = { 461, 462, 463, 464, 465, 466, 467, 468, 469, 470, 471, 472, 473, 474,
481, 482, 483, 484, 485, 486, 487, 488, 489, 490, 491, 492, 493, 494,
501, 502, 503, 504, 505, 506, 507, 508, 509, 510, 511, 512, 513, 514,
521, 522, 523, 524, 525, 526, 527, 528, 529, 530, 531, 532, 533, 534 };
const  UInt32 PurpleEquip60[] = { 661, 662, 663, 664, 665, 666, 667, 668, 669, 670, 671, 672, 673, 674,
681, 682, 683, 684, 685, 686, 687, 688, 689, 690, 691, 692, 693, 694,
701, 702, 703, 704, 705, 706, 707, 708, 709, 710, 711, 712, 713, 714,
721, 722, 723, 724, 725, 726, 727, 728, 729, 730, 731, 732, 733, 734 };
const  UInt32 PurpleEquip70[] = { 861, 862, 863, 864, 865, 866, 867, 868, 869, 870, 871, 872, 873, 874,
881, 882, 883, 884, 885, 886, 887, 888, 889, 890, 891, 892, 893, 894,
901, 902, 903, 904, 905, 906, 907, 908, 909, 910, 911, 912, 913, 914,
921, 922, 923, 924, 925, 926, 927, 928, 929, 930, 931, 932, 933, 934 };
const  UInt32 PurpleEquip80[] = {1061, 1062, 1063, 1064, 1065, 1066, 1067, 1068, 1069, 1070, 1071, 1072, 1073, 1074, 
1081, 1082, 1083, 1084, 1085, 1086, 1087, 1088, 1089, 1090, 1091, 1092, 1093, 1094, 
1101, 1102, 1103, 1104, 1105, 1106, 1107, 1108, 1109, 1110, 1111, 1112, 1113, 1114, 
1121, 1122, 1123, 1124, 1125, 1126, 1127, 1128, 1129, 1130, 1131, 1132, 1133, 1134};
const UInt32 PurpleEquip90[] = {1661, 1662, 1663, 1664, 1665, 1666, 1667, 1668, 1669, 1670, 1671, 1672, 1673, 1674};
const UInt32 PurpleEquip100[] = {1761, 1762, 1763, 1764, 1765, 1766, 1767, 1768, 1769, 1770, 1771, 1772, 1773, 1774};
const UInt16 OrangeFavor[] = {5820, 5821, 5822, 5823, 5824};
const UInt16 DietyFavor[] = {5825, 5826, 5827, 5828};

struct AthleticsRankData
{
	Player	* ranker;
	UInt8 row;
	UInt32	rank;
	UInt32	maxrank;
	UInt8	challengenum;
	UInt32	challengetime;
	//UInt32	boxflushtime;
	//UInt8   boxcolor;
	//UInt8	awardType;
	//UInt32	awardCount;
    UInt32  prestige;
    UInt32  tael;
	UInt16	winstreak;
    UInt16  bewinstreak;
    UInt16  failstreak;
    UInt16  befailstreak;
    UInt32  oldrank;
    UInt32  first4rank;
    UInt32  extrachallenge;
    UInt32  pageNum; //???
        //UInt32  first4rank;
        //// 0x1 第一次成为竞技场第一
        //// 0x2 第一次杀入竞技场二强
        //// 0x4 第一次杀入竞技场三强
        //// 0x8 第一次杀入竞技场10强
        //// 0x10 第一次杀入竞技场100强
        //// 0x20 第一次杀入竞技场200强
        //// 0x40 第一次杀入竞技场300强
        //// 0x80 第一次竞技场挑战
        //// 0x100 一天内提升200个排名
        //// 0x200 一天内提升100个排名
        //// 0x400 一天内提升50个排名
        //// 0x800 一天内提升20个排名
        //// 0x1000 失去10强排名
        //// 0x2000 夺得10强排名
        //// 0x4000 已特殊挑战
    AthleticsRankData() : pageNum(0)
    {
    }
};

struct AthleticsAward
{
	UInt32 athleticsid;
	//UInt8 color;
	UInt8 type;
	UInt8 side;
	bool win;
    UInt32 prestige;
    UInt32 tael;
	UInt32 count;
	Player *other;
    UInt32 itemId;
    UInt8  itemCount;
};

struct AthleticsEventData
{
	UInt32 id;
    Player* player1;
    Player* player2;
    UInt8  cond;
    UInt8  color;
    UInt16 value;
    UInt8  itemCount;
    UInt32 itemId;
	UInt32 time;

	AthleticsEventData() { memset(this, 0x00, sizeof(*this));}
	AthleticsEventData(UInt32 id_, Player* player1_, Player* player2_, UInt8 cond_, UInt8 color_, UInt16 value_, UInt8 itemCount_ = 0, UInt32 itemId_ = 0, UInt32 time_ = TimeUtil::Now())
		: id(id_), player1(player1_), player2(player2_), cond(cond_), color(color_), value(value_), itemCount(itemCount_), itemId(itemId_), time(time_) {}
};

struct AthleticsPay
{
    Int8  moneyEnough;
    UInt8 type;
};

static const UInt16 TimeOutbyColor[] = {2 * 60 * 60, 4 * 60 * 60, 6 * 60 * 60, 6 * 60 * 60};//??????ʱ?? ??ɫ:2Сʱ ��ɫ:4Сʱ ??ɫ:6Сʱ ??ɫ:6Сʱ
//static const UInt8 PLUND_RATE = 3;
static const UInt32 EXTRAREWARDTM = 19 * 3600;


class Player;
class AthleticsRank
{
	typedef std::list<AthleticsRankData *> AthleticsList;
	typedef AthleticsList::iterator Rank;
	typedef std::map<Player *, Rank> RankList;

	static const UInt32 ATHLETICS_BUFF_TIME = 15 * 60;

    public:
    enum
    {
        AthleticsPayForPaging,
        AthleticsPayForKillCD,
    };
public:
	AthleticsRank();
	~AthleticsRank();

public:
	void addAthleticsFromDB(UInt8, AthleticsRankData *);
	bool enterAthleticsReq(Player *, UInt8 row);
    void RequestPageNum(GObject::Player*); // client -> world
    void RequestKillCD(Player* player);
    void AddPageNum(Player* player, bool bMoneyEnough); // country -> world
    void KillCD(Player*, bool);
public:
	void requestAthleticsList(Player *, UInt16 type);
	void challenge(Player *, std::string&, UInt8 type = 0);
	void notifyAthletcisOver(Player *, Player *, UInt32, bool);
	void notifyAthletcisBoxFlushTime(Player *);

    void broadcastAthleticsEvent(UInt8 count);
    bool addAthleticsEventDataFromDB(UInt8 row, UInt32 id, Player* player1, Player* player2, UInt8 cond, UInt8 color, UInt16 value, UInt8 itemCount, UInt32 itemId, UInt32 time);
    UInt32 addAthleticsEventData(UInt8 row, Player* player1, Player* player2, UInt8 cond, UInt8 color, UInt16 value, UInt8 itemCount, UInt32 itemId);
    void requestAthleticsEvent(Stream&, Player * player);

	void challenge(Player *, UInt8 type);
	void challenge2(Player *, std::string&, UInt8 type);

protected:
	void getRandomEquip(UInt8 level, UInt16 rank, UInt16& EquipId, UInt16& cnt);
	void BuildNewBox(Rank &it_rank);

public:

	bool DistributeBox(Player *atker, Rank deferRank, UInt32& atkerAward);
	
	inline UInt16 GetOutTimebyColor(UInt8 boxcolor)
	{
		return TimeOutbyColor[boxcolor];
	}
	inline bool isTimeOut(UInt32 tm, UInt8 boxcolor);

	void GetBoxSourceReq(Player *owner);

	UInt8 getChallengeNum(Player *);
    UInt32 getAthleticsRank(Player*);
    UInt8 getAthleticsWinStreak(Player*);
    UInt8 getAthleticsBeWinStreak(Player*);
    UInt8 getAthleticsFailStreak(Player*);
    UInt8 getAthleticsBeFailStreak(Player*);

    UInt32 getAthleticsRankUpADay(Player* player);

    UInt32 getAthleticsExtraChallenge(Player* player);
    UInt32 setAthleticsExtraChallenge(Player* player, UInt32 extrachallenge);

    UInt32 getAthleticsFirst4Rank(Player*, UInt32 first4rank);
    UInt32 setAthleticsFirst4Rank(Player*, UInt32 first4rank);

    void updateAthleticsMartial(Player* pl);

    void giveAward(Player* pl);

    void notifyAthMartialOver(Player * atker, Player * defer, UInt32 id, UInt8 res);

public:
	void TmExtraAward();

protected:

	inline UInt8 getRankRow(UInt8 lev)
	{
		if (lev > 39)
			return 1;
		if (lev > 29)
			return 0;
		return 0xFF;
	}


	inline UInt32 getRankPos(UInt8 row, Rank rank)
	{
		return std::distance(_athleticses[row].begin(), rank) + 1;	//FIXME
	}
	inline Rank getRankBegin(UInt8 row)
	{
		return _athleticses[row].begin();
	}
	inline Rank getRankEnd(UInt8 row)
	{
		return _athleticses[row].end();
	}
	inline UInt32 getRankSize(UInt8 row)
	{
		return static_cast<UInt16>(_athleticses[row].size());
	}
	inline bool frontRankPos(UInt8 row, Rank rank)
	{
		return (*rank) == _athleticses[row].front();
	}
	inline bool backRankPos(UInt8 row, Rank rank)
	{
		return (*rank) == _athleticses[row].back();
	}

    void updateAthleticsRank(AthleticsRankData* data);
	void updateBatchRanker(UInt8, Rank, Rank);
	UInt8 updateChallengeNum(UInt8, UInt32);
    void  updatePageNum(Rank r);
	void getRankUpNeighbour(UInt8, Rank, Rank&);
	void getRankDownNeighbour(UInt8, Rank, Rank&);
	void getRankNeighbour(UInt8, Rank, Rank&, Rank&);

	bool updateBoxTimeoutAward(Rank, UInt8, UInt32 = TimeUtil::Now());

    UInt32 RunAthleticsEvent(UInt8 row, Rank atkRank, Rank defRank, UInt8 win);

public:	
	AthleticsList *getAthleticsRank()
	{
		return _athleticses;
	}

    static UInt8 GetMaxchallengenum(UInt32 vip);
private:	
	RankList		_ranks[2];
	AthleticsList	_athleticses[2];
	UInt32			_maxRank[2];	//???ڼ?¼????λ??????

	std::deque<AthleticsEventData *> _athleticsesEvent[2];
};

extern AthleticsRank gAthleticsRank;

}

#endif
