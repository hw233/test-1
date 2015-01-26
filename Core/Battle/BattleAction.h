
#ifndef BATTLEACTION_H_
#define BATTLEACTION_H_

#include "Config.h"

namespace Battle
{
    class BattleFighter;
    class BattleObject;
    //行为动作
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
            ActionPackage(BattleFighter * bf,UInt16 time):_bf(bf),_time(time)
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
            std::vector<BattleObject *> vec_bo;
            //UInt16 skillScopeId;
            BattleFighter * _bf; //攻击发起者
            UInt16 _time;
            //UInt8 _type;   //0 表示根据本包内容组织 1 表示根据攻击发起者进行组织(bf为NULL 或死亡，本次攻击失效)
    };
    class ImagePackage : public BattleAction
    {
        public:
            ImagePackage(UInt32 attack , UInt32 critical, UInt32 wreck, UInt32 hit , BattleFighter * bf , UInt16 time):_attack(attack),_critical(critical),_wreck(wreck),_hit(hit),_bf(bf),_time(time){}
            UInt32 GetAttack(){return _attack;}
            UInt32 GetHit(){return _hit;}
            UInt32 GetWreck() {return _wreck;}
            UInt32 GetCritical() {return _critical;}
            BattleFighter * GetBattleFighter(){return _bf;}
            bool CanCounter() {return true;}
            UInt16 GetHappenTime(){ return _time;}
            UInt16 GetObjectSize(){return vec_bo.size();}
            BattleObject* GetObject(UInt16 index){if(index > vec_bo.size()) return NULL ; return vec_bo[index];}

            void PushObject(BattleObject* bo) { vec_bo.push_back(bo);}
        private:
            UInt32 _attack ;
            UInt32 _critical ;
            UInt32 _wreck ;
            UInt32 _hit;

            std::vector<BattleObject *> vec_bo;
            UInt16 skillScopeId;
            BattleFighter * _bf; //攻击发起者
            UInt16 _time;

    };
}
#endif // BATTLEACTION_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

