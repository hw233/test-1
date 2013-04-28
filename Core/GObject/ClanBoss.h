#ifndef CLANBOSS_H_
#define CLANBOSS_H_

#include <map>
#include <set>
#include "Config.h"
#include "GData/NpcGroup.h"
#include "GObject/WBossMgr.h"
#include "GObject/Fighter.h"
#include "GObject/Clan.h"
#include "Common/Stream.h"
#include "Common/Mutex.h"
#include "MsgHandler/MsgTypes.h"
#include "MsgHandler/MsgFunc.h"
#include "Common/BinaryReader.h"
#include "Battle/BattleSimulator.h"

using namespace std;

#define MIN(a,b)  ((a)<(b)? (a):(b))
typedef Battle::BattleSimulator BattleSimulator;
const UInt8 g_clanSkillsNax = 5;
namespace GObject
{
    class Map;
    class Player;
    struct ClanSkills
    {
        ClanSkills()
        {memset(this, 0, sizeof(ClanSkills));};
        UInt32 buffEffect[g_clanSkillsNax];
        UInt32 buffCD[g_clanSkillsNax];
    };
 
    class ClanBoss 
    {
        typedef multimap<UInt32, Clan*, std::greater<int> > TSortMap;
   public:
		static ClanBoss& instance()
		{
		    static ClanBoss s;
			return s;
		}
    public:
		ClanBoss();
        bool isOpening() {return m_isOpening;}
        void clear();
		void init();
		bool LoadFromDB();
        void setCanOpened(bool v) {_canOpened = v;};
        bool getCanOpened() {return _canOpened;}
       
        void onClanBossReq(GameMsgHdr&, const void*);
        void makePlayerInfo(Player* pl);

        void sendStatus(Player* pl, UInt8 t = 0);
		void process(UInt32 now);
        void start();
        void refresh();
        void notify(Player* pl = NULL);
        void close();
        void caclPlayerBuff(Player* pl, bool isAttackBoss=false);
        void addBoss();

        void setBossHp(int maxHp) {_setBossHp = maxHp;};
        void addEmpower(UInt8 t, UInt32 em)
        {
            if (t <= 5)
            {
                _empower[t] += em;
                if (_empower[t] >= 4000)
                {
                    _empower[t] = 0;
                    EmpowerRelease(NULL, (t+1));
                }
            }
        }
        void setPowerType(UInt8 t) {if (t >0 && t <= 6) m_powerType=t;}

		bool attack(Player* pl);
        void membersAction(Clan* cl, Player* pl, bool b, UInt8 t);
        void sendPickStream(Player* pl);
        void sendClanPickerStream(Player* pl, UInt64 clanId);
        void sendEmpowerStream(Player* pl, UInt8 t);
        void sendClanEmpowerStream(Player* pl, UInt8 t, UInt64 clanId);
        void sendEmpowerPercentStream(Player* pl);
        void sendMyClanInfo(Player* pl);
        void broadClanMembersResume(Clan* cl);

        Player* getLeader(Clan* cl);
        UInt32 getClanGongxian(Clan* cl);
        UInt32 getPlayerGongxian(Player* pl); 
        UInt8 getMembers(Clan* cl, UInt8 t);
        UInt8 getMyStatus(Player* pl, UInt8& buffTime);

        void pickXianyun(Player* pl, UInt64 other = 0);
        void pickComplete(Player* pl, Player* other=NULL, bool timeFinish=0, bool stopBySelf=0);
        bool toBeCrazy(Player* pl);
        bool isCrazy(Player* pl);
        void sendMyStatus(Player* pl);
        
        void Empowerment(Player* pl, UInt8 t, UInt64 other);
        void EmpowerComplete(Player* pl, UInt8 t, Player* other=NULL, bool timeFinish=0, bool stopBySelf=0);
        void EmpowerRelease(Player* pl,UInt8 t);
        void addGongXian(Player* pl, Clan* cl, UInt32 num = 0, bool pend=false);
        void insertToGxSort(Clan* cl, UInt32 oldGx, UInt32 newGx, bool needBroad=false);
        void broadEmpower(UInt8, UInt8 );
        void Change2HPStatus();
        void Change2PowerStatus();
        void BossDead(Player* pl);

        void Urge(Player* pl, UInt8 t);
        ClanSkills* getClanSkills(Clan* cl);
        bool hasClanSkills(Clan* cl, UInt8 t);
        void UseClanSkill(Player* pl, UInt8 t);
        bool IsSkillInCD(Clan* cl , UInt8 t);
        void ClanMembersResume(Clan* cl);
        bool makeClanSkillsStream(Clan* cl, Stream& st);
        void sendClanSkillsInfo(Player* pl);
        void broadClanSkillsInfo(Clan* cl);
        void broadClanStream(Clan* cl, Stream* st, Player* pl=NULL);
        void sendClanBossMsg(char* str, Player* pl=NULL, Clan* cl=NULL);
        void reward();

        void broadClanStatus(Clan* cl);
        void broadClanUrge(Clan* cl);
        void broadClanInfo(Clan* cl, Player* pl = NULL);
        void broadClanRank();
    private:
		GData::NpcGroup* _ng;

        bool _statusChanged;
        UInt8 m_notifyRate;
        bool m_isOpening;
        UInt8 _totalPercent;
        UInt8 _minutes;
        UInt32 _seconds;

        UInt32 _setBossHp;

        bool _canOpened;
        UInt32 m_openTime;
        UInt32 _hp;
        UInt32 m_bossMaxHp;
        UInt32 m_power;
        UInt32 m_maxPower;
        UInt8 m_powerType;   //深渊之力共有六种
        UInt8 m_bossStatus;  //1:深渊之力 2:真身
        UInt8 _powerPercent;
        UInt8 _bossPercent;
        AtkInfoType m_atkinfo;
        //充能
        UInt16 _empower[6];
        map<Clan*, set<Player*> > _emClan[6];
        UInt16 _emPlayers[6];
        set<Player*> _pickPlayer;
        UInt16 _pickPlayers;
        /*0:等待 1:充能 2:采集 3:疗伤*/
        map<Clan*, set<Player*> > _membersClan[4];
        map<Player*, UInt8> _playerStatus;
        map<Player*, UInt8> _playerEmPos;
        //指挥
        map<Clan*, Player*> _leaderClan;
        map<Clan*, ClanSkills> _skillsClan;
        //贡献
//        map<Clan*, UInt32> _gxClan;
//        map<Player*, UInt32> _gxPlayer;
        TSortMap _gxSort;
        //狂暴的玩家,只可抢夺,且无CD
        map<Player*, UInt32> _crazyPlayer;
    };
}
#endif

