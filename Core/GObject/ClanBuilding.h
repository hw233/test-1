#ifndef CLAN_BUILDING_H
#define CLAN_BUILDING_H

#include  "Config.h"
#include  "BuildingTmpl.h"
#include  "Common/BinaryReader.h"

namespace GObject
{
    class Clan;
    class ClanBuildingOwner;
    class Player;

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
            opUnknown = 0,      // 未定义操作
            opGetInfo = 1,      // 获取帮派建筑信息
            opUpgrade = 2,      // 升级帮派建筑
            
        };
        public:
            ClanBuildingOwner(Clan *clan);
            virtual ~ClanBuildingOwner();

            bool loadFromDB( UInt32 fairylandEnergy, 
                    UInt16 phyAtkLevel, UInt16 magAtkLevel, UInt16 actionLevel, UInt16 hpLevel, 
                    UInt16 updateTime);

            bool init();

            UInt64 getClanId() const;
            UInt16 getBuildingLevel(UInt8  type) const;
            UInt32 getEnergy() const;

            void   processFromBrd(Player *player, BinaryReader& brd);
            void   process(Player *player, UInt8 type, std::vector<UInt8> vals);

            void   upgradeBuilding(Player *player, UInt8 type);

            UInt32 tryLevelUp(UInt8 type);

            void levelChange(Int32 iVal);

            void sendClanBuildingInfo(Player *player);

            UInt16 getLevel(UInt8 type) const;
            UInt32 getAddVal(UInt8 type) const;
        private:
            Clan * _clan;
            std::vector<ClanBuilding> _buildings;
            UInt32  _energy;        // 仙界元气
    };
}
#endif // #ifndef CLAN_BUILDING_H

