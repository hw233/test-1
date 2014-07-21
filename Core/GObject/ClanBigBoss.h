#ifndef CLANBigBOSS_H_
#define CLANBigBOSS_H_

#include "Config.h"
#include "GData/NpcGroup.h"
#include "Battle/BattleSimulator.h"
#include "GObject/Clan.h"
#include "Common/Stream.h"
#include <map>
#include <set>
#include "GObjectDBExecHelper.h"

typedef Battle::BattleSimulator BattleSimulator;

namespace GObject
{

    enum EStatus
    {
        CLAN_BIGBOSS_NOINIT = 0,//未预约 
        CLAN_BIGBOSS_AWAIT,//等待 
        CLAN_BIGBOSS_BEGIN,//开始BOSS 
        CLAN_BIGBOSS_OVER,//结束 
    };

    class Player;
    class DBClanBigBoss;
   
    struct PlayerDamage
    {
        Player * player;
        UInt32 damage;
        UInt32 time;
        PlayerDamage() : player(NULL), damage(0), time(0) {}
    };

    struct lt_sort1
    {
        bool operator() (const PlayerDamage& a, const PlayerDamage& b) const { return (a.damage > b.damage) || (a.damage == b.damage && a.time < b.time); }
    };
        
    //Boss未激活血量 
    #define MAXHP 0xFFFFFFFF
    #define CD 20 

    class ClanBigBoss
    {
        typedef std::multiset<PlayerDamage, lt_sort1> PlayerDamageSort;
        
    public:
                
        ClanBigBoss(Clan*); 
        ~ClanBigBoss(); 
        
        void ReqBossAppointment(UInt8 app_time,std::string str);
        void checkReset();

        void LoadFromDB(DBClanBigBoss*);
        void resetBossInfo();
        void attack(Player* pl);
        bool attackBoss(Player* pl);
        bool attackBossFinal(Player* pl);
        void reward(Player* pl,UInt8 type ,UInt8 idx = 0);
        void insertDamage(Player* pl,UInt32 damage);
        void broadcastStream(UInt8 flag);
        void updateInfo();
        void ReturnBossInfoPl(Player* pl);
        void ReturnBossInfo(Player* pl,UInt8 status);
        
        void offsetException();//异常补偿
        void closeBossInfo(UInt8 flag = 0/* 0 - 正常关闭 1 - 非正常关闭*/);//关闭Boss
        void addLevel();
        void setNpcBoss();
        
        inline UInt32 getHP(){ return m_BossHP; }
        void process(UInt32 now);
        void GMAttackBoss(UInt32 attack_boss);
        void SetDirty(Player* player,bool _iscbbbuf);
            
        inline bool setHP(UInt32 hp)
        {
            if (hp != MAXHP)
            {
                if(hp < m_BossHP)//保证正常打BOSS扣血
                    return false;
            }
            m_BossHP = hp;
            return true;
        }

        inline EStatus getStatus() { return _status; }
        void setStatus(EStatus eTmp);
        
        inline UInt16 getLastTime() const { return _lastTime; }
        inline UInt32 getLastHP() const { return _lastHp; }
        inline UInt32 getLastAtk() const { return _lastAtk; }
        inline UInt32 getLastMAtk() const { return _lastMAtk; }
        inline bool getFlag() { return _flag; }
        inline void setFlag(bool flag) { _flag = flag; }

    private:
		//GData::NpcGroup s_ng;
		GData::NpcGroup* _ng;
		GData::NpcGroup* _ng1;
        UInt32 m_BossHP;//Boss当前血量
        UInt8 m_spirit;//当前元神数
        std::multiset<PlayerDamage, lt_sort1> s_pds;
        UInt32 appointment_time;
        UInt32 lastAttack_time;
        Clan* _clan;
        EStatus _status;
        bool m_final;
        UInt8 _percent;

        UInt16 _lastTime;//打boss的时间
        UInt32 _lastHp;
        Int32 _lastAtk;
        Int32 _lastMAtk;
        bool _flag;
        UInt8 _buffer;
    };

    class ClanBigBossMgr : public Singleton <ClanBigBossMgr>
    {
        typedef std::map<UInt32 ,ClanBigBoss*> MClanBB;

        public:
        ClanBigBossMgr();
        ~ClanBigBossMgr();

        void process(UInt32 now);
        void closeAllBossInfo();//每天重置所有的,补偿宕机BOSS超时
        void delClanBigBossInstance(ClanBigBoss* cbb);
       
        void playerReqStart(Player * pl,Clan* clan,UInt8 hour);//预约Boss时间 
        void insertMap(UInt32 clanid,ClanBigBoss* cbb);

        private:
        MClanBB _clanBB;
        UInt32 lastReset_time;

    };



}








#endif
