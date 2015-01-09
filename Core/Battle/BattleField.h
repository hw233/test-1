#ifndef _BATTLEFIELD_H_
#define _BATTLEFIELD_H_

//#include "BattleFighter.h"
#include "BattleAction.h"
#include "Common/Stream.h"
/* Battle Field grid indexes:
defender:	9 8 7 6 5
4 3 2 1 0

attacker:	0 1 2 3 4
5 6 7 8 9
*/

#define FIELD_WIDTH 36*20
#define FIELD_HIGH  11*20
#define STEP 20

namespace GObject
{
    class Player;
}

namespace Battle
{

    const int MAX_REIATSU = 100;

    enum fieldType
    {
        filed_none = 0,
        filed_flat = 1,
        filed_glass = 2,
        filed_desert = 3,
        filed_water = 4,
    };

    class BattleField
    {
        public:
            BattleField(UInt16 fieldDistance = 1, UInt16 timeActionLimit = 80);
            virtual ~BattleField();
            void clear();
            void reset();
            void setObject(UInt16, UInt16, BattleObject *, UInt16 = 0);
            bool setObjectXY(UInt16, UInt16, BattleObject *, bool = false, UInt16 = 0, UInt16 = 0);

            BattleObject * operator()(UInt16, UInt16);
            BattleObject * getObjectXY(UInt16, UInt16, UInt16);

            UInt16 getDistance(UInt16, UInt16, UInt16, UInt16);
            UInt16 getDistance(BattleObject * bf , BattleObject* bo);
            UInt16 getShootDistance(BattleObject * begin , BattleObject * end , BattleObject* target);

            int getSpecificTarget(UInt16 side, bool(*f)(BattleObject* bo));
            bool getSpecificTargets(UInt16 side, UInt16 pos, UInt16 val, std::vector<UInt16>& poslist, bool(*f)(BattleObject* bo, UInt16 targetPos, UInt16 maxLength));
            int getPossibleTarget(UInt16, UInt16); // return -1 for no found target, overload in Simulator

            UInt32 getAliveCount(UInt16);
            UInt32 getObjHp(UInt16 x, UInt16 y);

            /*
            inline BattleObject * getObject(UInt16 side, UInt16 idx)
            {
                if(side < 0 || side > 1 || idx < 0 || idx > 24)
                    return NULL;
                return _objs[side][idx];
            }
            inline void deleteObject(UInt16 side, UInt16 idx)
            {
                if(side < 0 || side > 1 || idx < 0 || idx > 24)
                    return;
                if(_objs[side][idx])
                {
                    //delete _objs[side][idx];
                    _objs[side][idx] = NULL;
                }
            }
            */

            BattleObject * GetTarget(UInt8 side , UInt16 posX , UInt16 posY,UInt16 skillScopeId = 0);
            void GetTargetList(UInt8 side ,std::vector<BattleObject *>& vec , UInt16 );

            inline UInt16 GetTimeActionLimit() { return _timeActionLimit;}
            inline UInt16 GetFieldDistance() { return _fieldDistance ;}
            inline UInt8 GetFirstSize() { return _fighters[0].size();}
        protected:
            bool anyObjectInRow(UInt16, UInt16);
            void updateStats(UInt16);
            void updateStats(UInt16, UInt16);
            
            

            void GetBSEnterInfo(Stream& st);
        protected:
            //BattleObject * _objs[FIELD_WIDTH][FIELD_HIGH];     //战场成员  [x][y] x 表示横坐标 y 表示纵坐标
            //fieldType [FIELD_HIGH][FIELD_WIDTH];    //场地信息
            
            std::vector<BattleFighter* > _fighters[2];
            UInt16 _fieldDistance;
            UInt16 _timeActionLimit ;
            std::map<UInt16,BattleAction> FieldBuff;  //延迟性buff (定时炸弹类型的行为)

    };

}

#endif // _BATTLEFIELD_H_
