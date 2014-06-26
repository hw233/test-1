#ifndef CLAN_BUILDING_H
#define CLAN_BUILDING_H

#include  "Config.h"
#include  "BuildingTmpl.h"
#include  "Common/BinaryReader.h"
#include  "Common/Stream.h"

namespace GObject
{
#define LEFTATTRMAX 6
    class Clan;
    class ClanBuildingOwner;
    class Player;
    struct ClanBuildBattleInfo
    {
        UInt8 leftId ; 
        std::string name;
        UInt8 type ;
        UInt8 res ;
        UInt32 battleId;
        UInt32 battleTime;
        UInt32 energy;
        ClanBuildBattleInfo():leftId(0),name(""),type(0),res(0),battleId(0),battleTime(0),energy(0){}
        ClanBuildBattleInfo(UInt8 leftId_ ,std::string  name_ ,UInt8 type_ , UInt8 res_ , UInt32 battleId_ ,UInt32 battleTime_,UInt32 energy_):leftId(leftId_),name(name_),type(type_) , res(res_),battleId(battleId_),battleTime(battleTime_),energy(energy_){}
    };
    struct LeftAttackLeader
    {
        Player * leader;
        UInt8 leftId;
        LeftAttackLeader(Player * leader_ , UInt8 leftId_):leader(leader_),leftId(leftId_){}
        LeftAttackLeader():leader(NULL),leftId(0){}
        bool operator < ( const  LeftAttackLeader& leftAttack)  const
        {
            return leftId < leftAttack.leftId;
        }
        bool operator == ( const  LeftAttackLeader& leftAttack) const
        {
            return leftId == leftAttack.leftId || leader == leftAttack.leader;
        }
    };

    enum ClanBuildingErrCode
    {
        eErrTypeWrong = 4,      // 建筑类型错误 
    };
    class ClanBuilding : public BuildingTmpl
    {
        public:
            enum ClanBuildingType
            {
                eClanBuildingUnknown = 0,
                eClanBuildingPhyAtk = 1,  // 物攻加成建筑
                eClanBuildingMagAtk = 2,  // 法攻加成建筑
                eClanBuildingAction = 3,  // 身法加成建筑
                eClanBuildingHP     = 4,  // 生命加成建筑
                eClanBuildingOracle = 5,  // 神谕塔（限制其他建筑等级）
                eClanBuildingMax,
            };
        public:
            ClanBuilding ();
            virtual ~ClanBuilding();

            bool init(UInt8 type, ClanBuildingOwner* owner);

            bool loadFromDB(UInt16 level, UInt32 updateTime);

            UInt8 getType() const;
            UInt8 getAttrValue() const;
            virtual UInt32 getTableLevelExp(UInt16 level) const;
            virtual UInt16 getTableLevel(UInt32 exp) const;
            virtual UInt16 getMaxLevel() const;

            UInt32 getAddVal() const;

            virtual bool saveToDB() const;

        private:
            UInt8 _type;    // 建筑类型
            ClanBuildingOwner *_owner;
    };

    class ClanBuildingOwner
    {
        public:
        enum OpType
        {
       //     opUnknown = 0,      // 未定义操作
            opGetInfo = 0,      // 获取帮派建筑信息
            opUpgrade = 1,      // 升级帮派建筑
            /*
            opLeftAddrInfo = 3,  //查看遗迹信息
            opCreateTeam = 4 ,  //创建征战小队            
            opEnterTeam = 5 ,  //加入征战小队
            opLeaveTeam = 6 ,  //离开征战小队
            opChangeTeamMember = 7 , //改变小队队形 
            */
        };
        public:
            ClanBuildingOwner(Clan *clan);
            virtual ~ClanBuildingOwner();

            bool loadFromDB( UInt32 fairylandEnergy, 
                    UInt16 phyAtkLevel, UInt16 magAtkLevel, UInt16 actionLevel, UInt16 hpLevel, UInt16 oracleLevel,
                    UInt16 updateTime);

            bool init();

            UInt64 getClanId() const;
            UInt16 getBuildingLevel(UInt8  type) const;
            UInt32 getEnergy() const;

            void   processFromBrd(Player *player, BinaryReader& brd);
            void   process(Player *player, std::vector<UInt8> vals);

            void   upgradeBuilding(Player *player, UInt8 type);

            UInt32 tryLevelUp(UInt8 type);

            void levelChange(Int32 iVal);

            void sendClanBuildingInfo(Player *player);
            void getClanBuildingStream(Stream& st);

            UInt16 getLevel(UInt8 type) const;
            UInt32 getAddVal(UInt8 type) const;
            void addEnergy(UInt32 energy)
            { 
                _energy+= energy ;
                if(_energy > 9999999)
                    _energy = 9999999;
                UpdateEnergy();
            }
            UInt32 getEnergy(){ return _energy;}
            void AddBattlesInfo(struct ClanBuildBattleInfo cbbi , UInt8 board = 0);
            void SendBattlesInfo(Stream & st);
            void CreateTeam(Player * leader , UInt8 leftId);
            void EnterTeam(Player * leader , Player * player);
            void ChangeTeamMember(Player * leader , UInt8 first , UInt8 sevond);
            void LeaveTeam(Player * leader , Player * player ,Player * opter ,UInt8 broad = 0);
            void AttackLeftAddr(UInt8 leftId ,Player * player);
            void LineUp(Player * player);
            void sendAttackTeamInfo(Player *player);
            void UpdateEnergy();
            UInt32 getLeftAttr(UInt8 type) const;
            void AddLeftAttr(UInt8 opt , UInt8 type , UInt32 value);
            void resetLeftAttr();
            void CheckInTheTeam(Player * pl);
            
        private:
            Clan * _clan;
            std::vector<ClanBuilding> _buildings;
            UInt32  _energy;        // 仙界元气
            std::deque<struct ClanBuildBattleInfo> battles_deque;
            std::map< LeftAttackLeader , std::vector<Player *> > leftAttackTeams;
            //1-物攻魔攻 2 -（） 3-物防 4-魔防 5-身法 6-HP
            UInt32 _leftAttr[LEFTATTRMAX];
    };
}
#endif // #ifndef CLAN_BUILDING_H

