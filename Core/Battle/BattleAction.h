
#ifndef BATTLEACTION_H_
#define BATTLEACTION_H_

#include "Config.h"
#include <math.h>
#include "Common/Stream.h"
#define FIELD_WIDTH 1440
#define FIELD_HIGH  9*60
#define STEP 60

#define MIN(x,y) x>y?y:x
#define MIN_3(x,y,z) MIN((MIN(x,y)),z)

namespace Battle
{
    class BattleFighter;
    class BattleObject;
    //行为动作
    struct BattleActionStream
    {
        UInt16 _curtime;
        BattleObject* _bo;
        UInt16 _prarm ;
        BattleActionStream(UInt16 curtime, BattleObject*  bo, UInt16 prarm):_curtime(curtime),_bo(bo),_prarm(prarm){ }
        UInt16 GetCurTime(){return _curtime;}
        BattleObject* GetBattleObject(){ return _bo;}
        UInt16 GetParam(){ return _prarm;}
    };
    class BattleAction
    {
        public:
            BattleAction(){};
            virtual  UInt32 GetAttack(){return 0;}
            virtual  UInt32 GetHit(){return 0;}
            virtual  UInt32 GetWreck() {return 0;}
            virtual  UInt32 GetCritical() {return 0;}
            virtual  BattleFighter * GetBattleFighter(){return NULL;}
            virtual  bool CanCounter() {return true;}
            virtual UInt16 GetHappenTime(){ return 0;}
            virtual UInt16 GetObjectSize(){return 0;}
            virtual BattleObject* GetObject(UInt16 index){return NULL;}
    };

    class ActionPackage  : public BattleAction
    {
        public:
            ActionPackage(BattleFighter * bf,UInt16 time/*,BattleObject* bo*/):_bf(bf),_time(time)//,_bo(bo)
        {
            vec_bo.clear();
        }
            ActionPackage(){}
            ~ActionPackage(){}
            UInt32 GetAttack(); //{if(!_bf) return 0; return _bf->GetAttack();}
            UInt32 GetHit(); //{ if(!_bf) return 0; return _bf->GetHit();}
            UInt32 GetWreck(); //{ if(!_bf) return 0; return _bf->GetWreck();}
            UInt32 GetCritical(); //{ if(!_bf) return 0; return _bf->GetCritical();}
            //UInt32 GetBattleObject() {return _bo;}
            BattleFighter * GetBattleFighter(); // {return _bf;}
            void PushObject(BattleObject* bo); // { vec_bo.push_back(bo);}
            UInt16 GetObjectSize(); //{return vec_bo.size();}
            UInt16 GetHappenTime(); //{return _time;}

            BattleObject* GetObject(UInt16 index) ;//{if(index > vec_bo.size())return NULL; return vec_bo[index];}

        private: 
            //UInt16 skillScopeId;
            std::vector<BattleObject *> vec_bo;
            BattleFighter * _bf; //攻击发起者
            UInt16 _time;
            //UInt8 _type;   //0 表示根据本包内容组织 1 表示根据攻击发起者进行组织(bf为NULL 或死亡，本次攻击失效)
    };

    class ImagePackage : public BattleAction
    {
        public:
            ImagePackage(UInt16 skillId,UInt32 attack , UInt32 critical, UInt32 wreck, UInt32 hit , BattleFighter * bf , UInt16 time):_skillId(skillId),_attack(attack),_critical(critical),_wreck(wreck),_hit(hit),_bf(bf),_time(time){}
            UInt32 GetAttack(){return _attack;}
            UInt32 GetHit(){return _hit;}
            UInt32 GetWreck() {return _wreck;}
            UInt32 GetCritical() {return _critical;}
            BattleFighter * GetBattleFighter(){return _bf;}
            bool CanCounter() {return true;}
            UInt16 GetSkillId(){ return _skillId;}
            UInt16 GetHappenTime(){ return _time;}
            UInt16 GetObjectSize(){return vec_bo.size();}
            BattleObject* GetObject(UInt16 index){if(index > vec_bo.size()) return NULL ; return vec_bo[index];}
            void PushObject(BattleObject* bo) { vec_bo.push_back(bo);}

        public:
            std::vector<BattleObject *> vec_bo;

        private:
            UInt16 _skillId;
            UInt32 _attack ;
            UInt32 _critical ;
            UInt32 _wreck ;
            UInt32 _hit;

            UInt16 skillScopeId;
            BattleFighter * _bf; //攻击发起者
            UInt16 _time;

    };
    class ObjectPackage : public BattleAction
    { 
        public:
            ObjectPackage(UInt16 skillId,UInt32 attack , UInt32 critical, UInt32 wreck, UInt32 hit , BattleFighter * bf , UInt16 time):_skillId(skillId),_attack(attack),_critical(critical),_wreck(wreck),_hit(hit),_bf(bf),_time(time),_x(0),_y(0),_xAdd(0),_yAdd(0),_flagX(0),_flagY(0),_count(0){}
            UInt32 GetAttack(){return _attack;}
            UInt32 GetHit(){return _hit;}
            UInt32 GetWreck() {return _wreck;}
            UInt32 GetCritical() {return _critical;}
            BattleFighter * GetBattleFighter(){return _bf;}
            bool CanCounter() {return true;}
            UInt16 GetSkillId(){ return _skillId;}
            UInt16 GetHappenTime(){ return _time;}

            UInt16 GetPosX() {return _x;}
            UInt16 GetPosY() {return _y;}

            void GoNext()
            { 
                if(_count)
                { 
                    --_count;
                    return;
                } 

                if(_xAdd)
                    _x += _xAdd * _flagX - _xAdd * !_flagX;
                if(_yAdd)
                    _y += _yAdd * _flagY - _yAdd * !_flagY;
            } 

            bool CanExit()
            { 
                if(_x > FIELD_WIDTH || _y > FIELD_HIGH)
                    return true;
                if(!_xAdd && !_yAdd && _count ==0)
                    return true;
                return false;
            } 

            UInt16 getDistance(UInt16 x, UInt16 y)  //获得飞行轨迹到目标的距离
            { 
                int x1 =static_cast<int>(_x);
                int y1 =static_cast<int>(_y);
                int x2 =static_cast<int>(_x + _xAdd);
                int y2 =static_cast<int>(_y + _yAdd);
                int x3 =static_cast<int>(x);
                int y3 =static_cast<int>(y);
                int res = (abs((y1-y2)*x3+(x2-x1)*y3-y1*x2+x1*y2)/sqrt(((y1-y2)*(y1-y2) + (x1-x2)*(x1-x2))));
                return static_cast<UInt16>(res);
            } 

            bool CanBeCounted(UInt16 x, UInt16 y)
            { 
                int x1 =static_cast<int>(_x);
                int y1 =static_cast<int>(_y);
                int x2 =static_cast<int>(_x + _xAdd);
                int y2 =static_cast<int>(_y + _yAdd);
                int x3 =static_cast<int>(x);
                int y3 =static_cast<int>(y);

                int res  = (x3-x1)*(x2-x3) + (y3 - y1)*(y2 - y3) ;
                if(res == 0)
                { 
                    if(x == _x || y == _y)
                        return false;
                    return true;
                } 
                return res < 0;
            }

            void setObjectDirection(UInt16 x, UInt16 y, UInt8 flagX,UInt8 flagY, UInt16 xAdd, UInt16 yAdd ,UInt16 rad)  //飞行系
            { 
                _x = x;
                _y = y;
                _xAdd = xAdd;
                _yAdd = yAdd;
                _flagX = flagX;
                _flagY = flagY;
                _rad = rad;
            } 

            void setObjectTime(UInt8 count ) { _count = count;}

            bool CheckFighterInSCope(BattleObject* bo);  //非指向性
            void InsertIntoPackage(UInt16 curtime , BattleObject* bo , UInt16 param)
            { 
                vec_struct.push_back(BattleActionStream(curtime, bo, param));
            } 
            UInt8 BuildStream(Stream& st);
        public:
            std::vector< BattleActionStream >  vec_struct;
        private:

            UInt16 _skillId;
            UInt32 _attack ;
            UInt32 _critical ;
            UInt32 _wreck ;
            UInt32 _hit;

            //            UInt16 skillScopeId;
            BattleFighter * _bf; //攻击发起者
            UInt16 _time;

            UInt16 _x ;
            UInt16 _y ;
            UInt16 _xAdd;
            UInt16 _yAdd;

            UInt8  _flagX;
            UInt8  _flagY;
            UInt16 _rad;

            UInt8 _count;

    };
}
#endif // BATTLEACTION_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

