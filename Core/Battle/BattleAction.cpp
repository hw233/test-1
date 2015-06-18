#include "BattleAction.h"
#include "BattleFighter.h"
namespace Battle
{
    UInt32 ActionPackage::GetAttack(){if(!_bf) return 0; return _bf->GetAttack();}
    UInt32 ActionPackage::GetHit(){ if(!_bf) return 0; return _bf->GetHit();}
    UInt32 ActionPackage::GetWreck(){ if(!_bf) return 0; return _bf->GetWreck();}
    UInt32 ActionPackage::GetCritical(){ if(!_bf) return 0; return _bf->GetCritical();}
    //UInt32 GetBattleObject() {return _bo;}
    BattleFighter * ActionPackage::GetBattleFighter() {return _bf;}
    void ActionPackage::PushObject(BattleObject* bo){ vec_bo.push_back(bo);}
    UInt16 ActionPackage::GetObjectSize(){return vec_bo.size();}
    UInt16 ActionPackage::GetHappenTime(){return _time;}
    float ActionPackage::GetHappenTime2(){return _time2;}

    BattleObject* ActionPackage::GetObject(UInt16 index){if(index > vec_bo.size())return NULL; return vec_bo[index]; }

    bool ObjectPackage::CheckFighterInSCope(BattleObject* bo)  //非指向性
    { 
        if(_bo != NULL && bo != _bo)
            return false;

        if(CanBeCounted(bo->getPosX(),bo->getPosY()))
            return false;

        UInt16 advance = getDistance(bo->getPosX(), bo->getPosY());

        if(advance < bo->GetRad()+_rad)
            return true;
        return false;
    } 
    UInt8 ObjectPackage::BuildStream(Stream& st)
    { 
        if(!GetBattleFighter())
            return 0;
        if(!vec_struct.size())
            return 0;
        //st << static_cast<UInt8>(GetHappenTime()); 
        st << static_cast<float>(GetHappenTime2()); 
        st << static_cast<UInt8>(GetBattleFighter()->GetBSNumber());
        st << static_cast<UInt8>(3);
        st << static_cast<UInt16>(GetSkillId());
        st << static_cast<UInt8>(vec_struct.size());

        std::cout << " 技能释放者编号: " << static_cast<UInt32>(GetBattleFighter()->GetBSNumber());
        std::cout << std::endl;
        std::cout << " 技能编号："  << static_cast<UInt32>(GetSkillId()) << " 前进 " << static_cast<UInt32>(GetPosX()) << " , " << static_cast<UInt32>(GetPosY());
        std::cout << std::endl;
        for(UInt8 i = 0; i < vec_struct.size(); ++i)
        { 
            if(!vec_struct[i].GetBattleObject())
                continue;
            std::cout << "被击回合数: " << static_cast<float>(vec_struct[i].GetCurTime2());
            std::cout << " 被击者：" << static_cast<UInt32>(vec_struct[i].GetBattleObject()->GetBSNumber()) << " 位置: " << static_cast<UInt32>(vec_struct[i].GetBattleObject()->getPosX()) <<" , "<< static_cast<UInt32>(vec_struct[i].GetBattleObject()->getPosY());
            std::cout << std::endl;

            st << static_cast<UInt8>(vec_struct[i].GetCurTime());
            st << static_cast<UInt16>(vec_struct[i].GetBattleObject()->GetBSNumber());
            st << static_cast<UInt16>(vec_struct[i].GetParam());
        } 
        return 1;
    } 
    void ObjectPackage::GoForTarget()
    { 
        UInt16 advance = static_cast<UInt16>(sqrt(_xAdd*_xAdd + _yAdd *_yAdd));
        UInt16 targetX = _bo->getPosX();
        UInt16 targetY = _bo->getPosY();
        UInt16 x = _x;
        UInt16 y = _y;
        UInt16 distanceX = x > targetX ? x - targetX:targetX -x;
        UInt16 distanceY = y > targetY ? y - targetY:targetY -y;

        if(!distanceX && !distanceY)
            return ;
        while(advance--)
        { 
            if(!distanceX && !distanceY)
                break;

            if( distanceX > distanceY)
            {
                if(x > targetX && x > 0) 
                    --x;
                else
                    ++x;
                --distanceX;
            }
            else
            {
                if(y > targetY && x > 0) 
                    --y;
                else
                    ++y;
                --distanceY;
            }
        } 
        _x = x;
        _y = y;
    } 
}

