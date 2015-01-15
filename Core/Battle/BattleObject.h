#ifndef _BATTLEOBJECT_H_
#define _BATTLEOBJECT_H_

#include "BattleAction.h"
#include "Common/Stream.h"
namespace Battle
{
    enum 
    {
        Rider = 1 ,   //骑将
        Walker = 2 ,  //步将
        shooter = 3 , //弓将
        adviser = 4 , //谋将

        eMain  = 10,

        eTower = 15,   //箭塔
        eCar   = 16,   //投石车
        eAirship = 17, //飞艇(孔明灯)

        eFighter = 100,

        Stone = 253,
        Tree = 254,
        Water = 255
    };
    class BattleObject
    {
        public:
            BattleObject(UInt8 c, UInt8 px, UInt8 py/*, BattleField * field*/): _cls(c), _hp(0), _pointX(px), _pointY(py)/*,_field(field) */{}
            virtual ~BattleObject() {};

            //inline void setSideAndPos(UInt8 s, UInt8 p) { _side = s; _pos = p; }

            inline void setHP(UInt32 u) {_hp = u;}

            inline UInt8 getClass() {return _cls;}
            inline UInt32 getHP() {return _hp;}

            virtual void makeDamage(UInt32 u)
            {
                if(_hp < u)
                    _hp = 0;
                else
                    _hp -= u;
            }

            inline bool isChar() { return _cls < eFighter; }

            inline void setPos (UInt16 x , UInt16 y) 
            { 
                _pointX = x;
                _pointY = y;
            }

            UInt16 getPosX(){return _pointX;} 
            UInt16 getPosY(){return _pointY;} 

            //inline bool isHide() { return _hide; }
            //inline void setHide(bool hide) { _hide = hide; }

            virtual UInt16 BeActed(BattleAction battleAction){ makeDamage(battleAction.GetAttack()); return 0;}   //用于非战斗对象
            //virtual BattleField * GetField(){ return _field;}
            UInt32 GetDefendNear() {return defend_near;}
            UInt32 GetDefendDistance(){ return defend_distance;}
            UInt8 AppendFighterStream(Stream & st){if(!_st.size()) return 0; st << _st; _st.reset(); return 1;}
            virtual UInt16 GetRad(){return 1;}
            virtual UInt8 GetSide(){return 0;}
            virtual UInt16 GetBattleIndex(){return 0;}
            virtual UInt8 GetGroundX() {return 0;}
            virtual UInt8 GetGroundY() {return 0;}
            virtual void InsertFighterInfo(Stream& st,UInt8 flag = 0){}
            //virtual UInt8 GetBSNumber(){return 0;}
        protected:
            UInt8 _cls;
            UInt32 _hp;
            UInt16 _pointX ;
            UInt16 _pointY;
            //bool _hide;
            UInt32 defend_near;
            UInt32 defend_distance;
            //BattleField * _field;

            //流
            Stream _st;
    };

}

#endif // _BATTLEOBJECT_H_
