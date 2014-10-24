
#ifndef MONSTERKETTLE_H_
#define MONSTERKETTLE_H_
#include "Config.h"
#include "Common/Stream.h" 
#include "Player.h"
#include "GData/AttrExtra.h"

#define INSIDE_KETTLE_MAX 3
#define KETTLE_COUNT 15    //壶中界数
#define MONSTER_TYPE 7   //每层的怪物数量
#define MONSTER_KING 5   //每层的怪物星数
namespace GObject
{
    struct BattleInfo   //战报信息
    {
        UInt32 _battleId;
        UInt32 _npcId;
        UInt8 _res ;
        BattleInfo(UInt32 battleId , UInt32 npcId , UInt8 res):_battleId(battleId),_npcId(npcId),_res(res){ }
        BattleInfo():_battleId(0),_npcId(0),_res(0){}
    };
    struct MonsterKing
    {
        UInt32 npcId;
        UInt8 king;
        MonsterKing():npcId(0),king(0){}
    };

    class MonsterKettle   //壶中界
    {
        public:
            MonsterKettle()
            {
                memset(_npcIds , 0 , sizeof(_npcIds));
                memset(_hasWin , 0 , sizeof(_hasWin));
                memset(MonsterProcess , 0 , sizeof(MonsterProcess));
                
            }
            void SetMonster(UInt8 idx , UInt32 npcId , UInt8 king);
            MonsterKing GetMonster(UInt8 idx);
            UInt32 RandomMonster(Player * pl , UInt8 num , UInt8 type , UInt32 count = 1);

            void SetWin(UInt8 idx,UInt8 flag);
            UInt8 GetWin(UInt8 idx);

            void SetMonsterProcess(UInt8 idx , UInt8 king , UInt8 count);
            UInt8 GetMonsterProcess(UInt8 idx , UInt8 king);

            void GetMonsterInfo(Stream& st);
            void GetMonsterProcessInfo(Stream& st);
            bool AddMonsterProcess(UInt8 type , UInt8 king);

            UInt32 challenge(Player * pl,Stream& st,UInt64 & exp ,UInt8 idx , UInt8 flag = 0);

            //void SetKettleHistory(UInt8 type , UInt8 king , UInt8 value);
            //void SetKettleOccupy(UInt8 idx , UInt8 npcId , UInt8 win);
            UInt8 IsCompelete();

            void UpdateKettleToDB(UInt64 playerId , UInt8 num);
        private:
            MonsterKing _npcIds[INSIDE_KETTLE_MAX];   //壶中界的怪
            UInt8  _hasWin[INSIDE_KETTLE_MAX];   //壶中界的怪是否战胜
            UInt8 MonsterProcess[MONSTER_TYPE][MONSTER_KING];   //此层每个怪的星级进度
    };
    class MonsterKettleManager 
    {
        public:
            //static UInt32 monster[KETTLE_COUNT * MONSTER_TYPE * MONSTER_KING];
            MonsterKettleManager(Player * owner):m_player(owner)
            {
                memset(monsterKettle , 0 , sizeof(monsterKettle));
                //BattleInfoDeque.clear();
            }
            //void GetBattleStream(Stream& st);
            void GetMonsterKettleInfo(Stream& st);
           // void AddBattleInfo(BattleInfo bi);
            void SetKettleHistory(UInt8 idx,std::string history);
            void SetKettleOccupy(UInt8 idx,std::string occupy);
            void UpdateToDB(UInt8 idx);
            UInt32 RandomMonster(UInt8 idx , UInt8 type , UInt32 count );
            void BattleMonster(UInt8 idx , UInt8 num , UInt8 count = 0);
            void GetKettleAttr(GData::AttrExtra &ae);
            bool CheckKettleRight(UInt8 idx);
        private:
            MonsterKettle monsterKettle[KETTLE_COUNT];
            //std::deque<BattleInfo>  BattleInfoDeque;
            Player * m_player;
    };

}
#endif // MONSTERKETTLE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

