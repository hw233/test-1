#ifndef _GAMEACTIONLUA_INC_
#define _GAMEACTIONLUA_INC_

#include "Script.h"

typedef lua_tinker::table Table;

#include "GData/GDataManager.h"
namespace GObject
{
    class Player;
    class Fighter;
    class StoreA;
}
using namespace GObject;

namespace Script
{
    extern 	void SysBroadcast(UInt8 type, const char * msg);

    class GameActionLua:
        public Script
    {
        public:
            GameActionLua(UInt8, const char *);
            ~GameActionLua();

        public:
            void init();
            void postInit();
            void RegisterActionInterface();
            //UInt16 GetGovernDropMoney(UInt8 res);
            //UInt16 GetGovernDropItem(UInt8 res);
            UInt32 RandMonster(UInt8 group);

        public:
            const char* GetItemName(UInt32 itemId);
            const char* GetGreatFighterName(UInt32 fgtId);
            UInt16 GetRandFighter();
            UInt8  GetSignForMouth(Player* player, UInt8 index, UInt8 vip, UInt8 flag);
            UInt8  UpgradeCost(Player* player ,UInt8 cls, UInt8 index, UInt8 lev);
            UInt8 UpgradeSoldierCost(Player* player ,UInt8 cls, UInt8 index, UInt8 lev);
            UInt8 EnchantSoldierCost(Player* player ,UInt8 cls, UInt8 index, UInt8 lev);
            bool loadItems(Player* player);
            bool loadPageItems(Player* player,UInt8 pageId);


        public:
            template <typename R> 
                inline R RunMO(Player* player, UInt32 npcId);

        public:
            template <typename R>
                inline R Run(Player* player, const std::string& script);
            template <typename R, typename T1>  
                inline R Run(Player* player, const std::string& script, const T1& t1);
            template <typename R, typename T1, typename T2> 
                inline R Run(Player* player, const std::string& script, const T1& t1,  const T2& t2);
            template <typename R, typename T1, typename T2, typename T3> 
                inline R Run(Player* player, const std::string& script, const T1& t1,  const T2& t2, const T3& t3);
            template <typename R, typename T1, typename T2, typename T3, typename T4> 
                inline R Run(Player* player, const std::string& script, const T1& t1,  const T2& t2, const T3& t3, const T4& t4);

        public:
            template<typename R>
                inline R Call(const std::string& name);
            template<typename R, typename T1>
                inline R Call(const std::string& name, const T1& t1);
            template<typename R, typename T1, typename T2>
                inline R Call(const std::string& name, const T1& t1, const T2& t2);
            template<typename R, typename T1, typename T2, typename T3>
                inline R Call(const std::string& name, const T1& t1, const T2& t2, const T3& t3);
            template<typename R, typename T1, typename T2, typename T3, typename T4>
                inline R Call(const std::string& name, const T1& t1, const T2& t2, const T3& t3, const T4& t4);
            template<typename R, typename T1, typename T2, typename T3, typename T4, typename T5>
                inline R Call(const std::string& name, const T1& t1, const T2& t2, const T3& t3, const T4& t4, const T5& t5);

        public:

            UInt32 GetSharpDay(UInt32);
            //inline Player * GetPlayerPtr(IDTYPE playerId);

        private:
            Player* _player1;	//?ű???????Ϊ????1
            Player* _player2;	//?ű???????Ϊ????2

        private:
            UInt8 _tid;	// ?????߳?ID
    };

#include "GameActionLuaImpl.inl"

}


#endif
