#ifndef _BATTLEFIELD_H_
#define _BATTLEFIELD_H_

//#include "BattleFighter.h"
#include "BattleAction.h"
#include "Common/Stream.h"
#include <vector>
/* Battle Field grid indexes:
defender:	9 8 7 6 5
4 3 2 1 0

attacker:	0 1 2 3 4
5 6 7 8 9
*/

#define FIELD_WIDTH 1520
#define STEP 36
#define FIELD_HIGH  10*STEP

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
            BattleField(UInt8 mapId1, UInt8 mapId2, UInt16 fieldDistance = 1, UInt16 timeActionLimit = 80);
            virtual ~BattleField();
            void clear();
            void reset();
            void setObject(UInt16, UInt16, BattleObject *, UInt16 = 0);
            bool setObjectXY(UInt16, UInt16, BattleObject *, bool = false, UInt16 = 0, UInt16 = 0);

            BattleObject * operator()(UInt16, UInt16);
            BattleObject * getObjectXY(UInt16, UInt16, UInt16);

            UInt16 getDistance(UInt16, UInt16, UInt16, UInt16);
            UInt16 getDistance(BattleObject * bf , BattleObject* bo);
            UInt16 getDistance(BattleObject * bf , UInt16 x2 , UInt16 y2 , UInt16 rad =0);
            UInt16 getShootDistance(BattleObject * begin , BattleObject * end , BattleObject* target);

            int getSpecificTarget(UInt16 side, bool(*f)(BattleObject* bo));
            bool getSpecificTargets(UInt16 side, UInt16 pos, UInt16 val, std::vector<UInt16>& poslist, bool(*f)(BattleObject* bo, UInt16 targetPos, UInt16 maxLength));
            int getPossibleTarget(UInt16, UInt16); // return -1 for no found target, overload in Simulator

            std::vector<UInt8> getAliveCount(UInt8 side);
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

            BattleObject * GetTarget(UInt8 side , UInt16 posX , UInt16 posY, BattleFighter* cur = NULL);
            BattleObject * GetTargetForRide(UInt8 side , UInt16 posX , UInt16 posY,UInt8 direction = 2);
            void GetTargetList(UInt8 side, BattleFighter* bf, std::vector<BattleObject *>& vec, UInt16 , UInt8 flag = 0);

            inline UInt16 GetTimeActionLimit() { return _timeActionLimit;}
            inline UInt16 GetFieldDistance() { return _fieldDistance ;}
            inline UInt8 GetFirstSize() { return _fighters[0].size();}

            void InsertTimeBattleAction(UInt16 time , ActionPackage);
            void InsertTimeBattleAction(UInt16 time , ImagePackage ip);
            void InsertObjectPackage(ObjectPackage ba);

            std::vector<ActionPackage> GetTimeBattleAction(UInt16 time);
            std::vector<ImagePackage> GetTimeBattleImage(UInt16 time);

            void InsertTimeBattleAction(float time , ActionPackage);
            std::vector<ActionPackage> GetTimeBattleAction(float& time);

            void InsertTimeBattleAction(float time , ImagePackage ip);
            void InsertBattlePre(float time, BattleFighter* fgt);

            std::vector<ImagePackage> GetTimeBattleImage(float& time);
            std::vector<BattleFighter*> GetBattlePre(float& time);

        protected:
            bool anyObjectInRow(UInt16, UInt16);
            void updateStats(UInt16);
            void updateStats(UInt16, UInt16);

            void GetBSEnterInfo(Stream& st);
            BattleFighter *getMyFighters(UInt8 side, UInt8 index);
            std::list<ObjectPackage>& GetObjectpackage();

            float GetMinTime();

            void DelBattleImage();
            void DelBattleAction();
            void DelBattlePre();

            void FieldPrint();

            void BattleActionPrintf(UInt8 index);
        protected:

            //BattleObject * _objs[FIELD_WIDTH][FIELD_HIGH];     //战场成员  [x][y] x 表示横坐标 y 表示纵坐标
            //fieldType [FIELD_HIGH][FIELD_WIDTH];    //场地信息
            
            std::vector<BattleFighter* > _fighters[2];
            UInt8 _mapId1;
            UInt8 _mapId2;
            UInt16 _fieldDistance;
            UInt16 _timeActionLimit ;

            /*
            std::map<UInt16,std::vector<ActionPackage> > FieldAttack;  //攻击

            std::map<UInt16,std::vector<ImagePackage> > FieldImage;  //延迟性buff (定时炸弹类型的行为)

            std::list< ObjectPackage > FieldObject;  //物体型攻击 (定时炸弹类型的行为)
            */

            std::map<float,std::vector<ActionPackage> > FieldAttack;  //攻击

            std::map<float,std::vector<ImagePackage> > FieldImage;  //延迟性buff (定时炸弹类型的行为)

            std::list< ObjectPackage > FieldObject;  //物体型攻击 (定时炸弹类型的行为)

            std::map<float,std::vector<BattleFighter*> > BattlePre;

    };

}

#endif // _BATTLEFIELD_H_
