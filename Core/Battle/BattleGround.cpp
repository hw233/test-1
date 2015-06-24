#include "BattleGround.h"
#include "BattleFighter.h" 
#include "GObject/Player.h"
#include "Battle/BattleSimulator.h"
#include "Battle/BattleReport.h"
#include "Common/StringTokenizer.h"
#include "GData/Map.h"
#include "Script/lua_tinker.h"
#include "Battle/Report2Id.h"
#include "Common/URandom.h"
#define MAX(x,y) x>y?x:y
#define ABS(x,y) x>y?x-y:y-x

namespace Battle
{
    void BattleGround::InitMapFight(UInt8 mapId)   
    { 

       //设置地图信息
       _mapId = mapId;
       GData::MapInfo* mapInfo = GData::mapTable.GetMapInfo(mapId);
       if( mapInfo == NULL )
           return;
        _x = mapInfo->GetWidth();
        _y = mapInfo->GetHeight();

        _mapGround = new UInt8[_x*_y];
        memset(_mapGround , 0 ,_x*_y*sizeof(UInt8));

        vecInfo tileInfo = mapInfo->GetTileInfo();
        for( UInt8 j = 0 ; j < tileInfo.size() ; ++j)
        {
            _mapGround[j] = tileInfo[j];
        }

        //设置地图上的阵营信息(跟上面的地图是对应的)
        vecInfo CampInfo = mapInfo->GetCampInfo();

        _mapCamp = new UInt8[_x*_y];
        memset(_mapCamp , 0 ,_x*_y*sizeof(UInt8));
        for( UInt8 j = 0 ; j < CampInfo.size() ; ++j )
        {
            _mapCamp[j] = CampInfo[j];
        }
        
        SetCampActId();
        SetEachPosNumber();
        _mapFighters = new BattleObject* [_x*_y];
        //_mapFlag = new UInt8[_x*_y];
        memset(_mapFighters,0,sizeof(BattleObject*)*_x*_y);
        //memset(_mapFlag,0,sizeof(UInt8)*_x*_y);
        
    }

     
    bool MoreFit(TargetInfo info1, TargetInfo info2)
    {
        if( info1.pri > info2.pri )
            return true;
        else if( info1.cost < info2.cost)
            return true;
        else
            return false;

    }

    bool BattleGround::InMyAttackHasEnemy(const Ascoord& p )
    {
        UInt8 x = currentBf->GetGroundX();
        UInt8 y = currentBf->GetGroundY();
        //判断我攻击范围内是不是有敌人呢
        UInt8 dis = currentBf->GetAttackRange();
        
        for( UInt8 j = ( y - dis > 0 ? y - dis : 0 ) ; j < ( y + dis+1 > _y ? _y : y+dis+1) ; ++j )
        {
            for( UInt8 i = (x-dis > 0 ? x -dis : 0 ) ; i < ( x + dis + 1 > _x ? _x : x+dis+1 ) ; ++i )
            {
                if( _mapGround[i+j*_x] != 0 && _mapFighters[i+j*_x] != NULL && _mapFighters[i+j*_x]->getHP() > 0 && _mapFighters[i+j*_x]->GetSide() != currentBf->GetSide() )
                {
                    return true;
                }
            }
        }
        return false;
    }

    void BattleGround::GetMovePos(std::list<Ascoord> path, Ascoord& move)
    {
        std::reverse(path.begin(),path.end());
        for( auto it = path.begin() ; it != path.end() ; ++it )
        {
            UInt8 x = (*it).x;
            UInt8 y = (*it).y;
            if(  (_mapFighters[x+y*_x] != NULL && _mapFighters[x+y*_x]->getHP() <= 0 ) || _mapFighters[x+y*_x] == NULL )
            {
                move = Ascoord(x,y);
                break;
            }
        }

    }


    void BattleGround::MoveAnalyse(std::list<Ascoord> path, Ascoord target, Ascoord& move)
    {
        //只走一个行动力
        std::list<Ascoord> movePath;
        Ascoord flag;
        std::reverse(path.begin(),path.end());
        int ride = currentBf->GetMovePower();
        if( path.size() < 2 )
        {
            return;
        }
        else if ( path.size() == 2 )
        {
            if( _mapGround[target.x+target.y+_x] == 0 )
            {
                move = _start;
            }
            else
            {
                move = target;
            }
        }
        else
        {
            auto begin = path.begin();
            begin++;
            for( auto it = begin; it != path.end() ;++it )
            {
                flag = *it;
                ride -=GetRideSub(flag.x,flag.y);
                if( InMyAttackHasEnemy(*it) )
                    break;
                if( ride <= 0 )
                    break;
            }
            for(auto it = path.begin();it != path.end(); ++it )
            {
                movePath.push_back(*it);
                if( *it == flag )
                    break;
            }
            GetMovePos(movePath,move);  //倒着走  找到第一个没有人的位置即可
        }
    }

    void BattleGround::MoveGetEnemy(std::vector<Ascoord>& vecAscoord)
    {
        UInt8 y = currentBf->GetGroundY();
        //先检查和他一条直线上
        for(UInt8 i = 0 ; i < _x ; ++i )
        {
            if( i > _x )
                break;
            if( _mapGround[i+y*_x] == 0 )
                continue;
            if( _mapFighters[i+y*_x] == NULL )
                continue;
            if( _mapFighters[i+y*_x]->getHP() <= 0 )
                continue;
            if( _mapFighters[i+y*_x]->GetSide() == currentBf->GetSide())
                continue;
            vecAscoord.push_back(Ascoord(i,y));
        }
        //然后是他上面的敌人 以及下面的敌人
        for(UInt8 j = 0 ; j < _y ;  ++ j)
        {
            if( j > _x )
                break;
            if( j == y )
                continue;
            for(UInt8 i = 0 ; i < _x ; ++i)
            {
                if( i > _x )
                    break;
                if( _mapGround[i+j*_x] == 0 )
                    continue;
                if( _mapFighters[i+j*_x] == NULL )
                    continue;
                if( _mapFighters[i+j*_x]->getHP() <= 0 )
                    continue;
                if( _mapFighters[i+j*_x]->GetSide() == currentBf->GetSide() )
                    continue;
                vecAscoord.push_back(Ascoord(i,j));
            }
        }
    }

    void BattleGround::GetMovePosition(Ascoord& move)
    {
        UInt8 x = currentBf->GetGroundX();
        UInt8 y = currentBf->GetGroundY();
        std::vector<Ascoord> vecAscoord;
        MoveGetEnemy(vecAscoord);
        if( vecAscoord.empty() )
            return;
        SetStart(Ascoord(x,y));
        Ascoord end = vecAscoord.front();
        SetEnd(end);
        ComputeRoute();
        std::list<Ascoord>path;
        GetRoute(path);
        MoveAnalyse(path,end,move);
    }

    UInt8 BattleGround::GetDistance(Ascoord & lp , Ascoord& rp )
    {
        Ascoord lh = ConvertConverstion(lp);
        Ascoord rh = ConvertConverstion(rp);
        UInt8 j = abs(lh.y -rh.y);
        UInt8 downI = lh.x - j;
        UInt8 upI = lh.x + j;
        if( rh.x >= downI && rh.x <= upI )
        {

            return j;
        }
        else if( rh.x < downI )
        {
            return j+(downI-rh.x)/2;
        }
        else
        {
            return j+(rh.x-upI)/2;
        }
    }

    void BattleGround::Move()
    {
        //选择对象
        if(!currentBf)
            return ;
        //写入当前战将信息
        GetTarget(currentBf->GetGroundX(),currentBf->GetGroundY(),currentBf->GetMovePower());
        if( _vecTarget.empty() )
        {

            UInt8 x = currentBf->GetGroundX();
            UInt8 y = currentBf->GetGroundY();
            
            Ascoord move = Ascoord(0xFF,0xFF);
            GetMovePosition(move); 
            UInt8 mx = 0;
            UInt8 my = 0;
            if( move.x == 0xFF && move.y == 0xFF)
            {
                mx = x;
                my = y;
            }
            else
            {
                mx = move.x;
                my = move.y;
                _mapFighters[x+y*_x] = NULL;
                _mapFighters[mx+my*_x] = currentBf;
            }
            currentBf->SetGroundX(mx);
            currentBf->SetGroundY(my);

            std::cout << "战将编号:      "  << static_cast<UInt32>(currentBf->GetBattleIndex());
            std::cout << "战将编号:      "  << static_cast<UInt32>(currentBf->GetBattleIndex());
            std::cout <<" 无方案" << " 从" << static_cast<UInt32>( x ) <<" , " << static_cast<UInt32>(y);
            std::cout <<" 移动到 "<<  static_cast<UInt32>(mx)<< " , " << static_cast<UInt32>(my) <<std::endl;
            currentBf->InsertFighterInfo(_pack);  //Stream

            UInt8 rand = uRand(20);
            _pack << static_cast<UInt8>(mx) << static_cast<UInt8>(my) << static_cast<UInt8>(rand)<<static_cast<UInt8>(0); //无战斗发生
            _oneRoundCostTime += rand;
        }
        else
        {
            std::sort(_vecTarget.begin(),_vecTarget.end(),MoreFit);
            TargetInfo target = _vecTarget.front();
            std::cout << std::endl;
            TestCoutBattleS(currentBf);
            //战斗
            std::cout <<"从     "<<static_cast<UInt32>(currentBf->GetGroundX())<<" , "<< static_cast<UInt32>(currentBf->GetGroundY())<<std::endl;
            std::cout <<"移动到  "<<static_cast<UInt32>(target.attack.x)<< "," << static_cast<UInt32>(target.attack.y) <<std::endl;
            std::cout << "攻击目标  " <<static_cast<UInt32>(target.goal.x)<<","<<static_cast<UInt32>(target.goal.y)<<std::endl;

            //Stream
            //
            //同步_mapFighters的坐标
            UInt8 x = currentBf->GetGroundX();
            UInt8 y = currentBf->GetGroundY();
            UInt8 ax = target.attack.x;
            UInt8 ay = target.attack.y;
            if( target.attack.x != x ||  target.attack.y != y )
            {
                _mapFighters[x+y*_x] = NULL;
                _mapFighters[ax+ay*_x] = currentBf;
            }
            currentBf->SetGroundX(ax);
            currentBf->SetGroundY(ay);
            
            UInt8 rand = uRand(20);
            currentBf->InsertFighterInfo(_pack);  //Stream
            _pack << static_cast<UInt8>(ax) << static_cast<UInt8>(ay);
            _pack << static_cast<UInt8>(rand);
            _pack << static_cast<UInt8>(1);

            _oneRoundCostTime +=rand; 

            //currentBf->InsertFighterInfo(_pack);
            target.bo->InsertFighterInfo(_pack);

            UInt8 win = 0;
            UInt32 reportId = 0;
            Fight(currentBf, target.bo, win, reportId);
             
            /*
            ////////////////////////////////////////////////////////////
            UInt8 gx = target.goal.x;
            UInt8 gy = target.goal.y;

            if(  win == 2 )   //己方赢
            {
                win = 0;
                _mapFighters[gx+gy*_x]->setHP(0);
            }
            */
            ////////////////////////////////////////////////////////
            /*
            if( win == 0 )
            {
                UInt8 side = target.bo->GetSide();
                for( auto it = fighters[side].begin(); it != fighters[side].end(); ++it )
                {
                    if( (*it) == target.bo)
                    {
                        fighters[side].remove(*it);
                        break;
                    }
                }
            }
            */
            if( win != 2 )
            {
                currentBf->setHP(100);
                (target.bo)->setHP(100);
                win = 2;
            }
            _pack << win << reportId;

            //cout
            TestCoutBattleS(target.bo);
            std::cout << std::endl;
        }
        _vecTarget.clear();
    }

    void BattleGround::GetNearEnemy(UInt8 x,UInt8 y,std::vector<Ascoord>& vecEnemy)
    {
        UInt8 attackDis = currentBf->GetAttackRange();   //获得攻击距离
        UInt8 ride = currentBf->GetMovePower();
        UInt8 range = attackDis+ride;
        UInt8 side  = currentBf->GetSide();

        for(UInt8 j = y > range ? y - range : 0 ; y <= ( y + range  >= _y ? _y-1 : y + range ) ; ++j)
        {
            if( j >= _y )
                break;
            for( UInt8 i = x > range ? x - range : 0 ; i <= ( x + range >= _x ? _x-1 : x +range) ; ++i)
            {
                if( i >= _x )
                    break;
                if( _mapGround[i+j*_x] == 0 )      //0代表不可放置的位置
                    continue;
                if( _mapFighters[i+j*_x] == NULL )
                    continue;
                //if( _mapFighters[i+j*_x]->getClass() > 100)
                //    continue;
                if( _mapFighters[i+j*_x]->GetSide() == side )  //属于同一个阵营  
                    continue;
                if( _mapFighters[i+j*_x]->getHP() <= 0 )    //已经死了
                    continue;
                if( _mapFighters[i+j*_x] == currentBf )     //不是自己
                    continue;
                vecEnemy.push_back(Ascoord(i,j));
            }
        }
    }

    Ascoord BattleGround::ConvertConverstion(const Ascoord& p)     //转换坐标系  
    {
        if ( p.y % 2 == 1 )
        {
            return Ascoord(((p.x+1)*2-p.y%2),p.y);
        }
        else
        {
            return Ascoord((p.x*2-p.y%2),p.y);
        }
    }

    bool BattleGround::IsInAttackZone(Ascoord stand, Ascoord target)   //站在某一点是不是可以攻击到目标呢
    {
        UInt8 dis = currentBf->GetAttackRange();
        Ascoord s  = ConvertConverstion(stand);
        Ascoord t  = ConvertConverstion(target);
        bool ret = false;
        if(   abs(t.y-s.y) > dis )
        {
            ret = false;
        }
        else
        {
            if( ( abs(t.x-s.x)+abs(t.y-s.y) )<= 2*dis )
            {
                ret = true;
            }
        }
        return ret;
    }

    bool BattleGround::EnemyCanAttack(Ascoord stand, Ascoord target)
    {
        UInt8 x = stand.x;
        UInt8 y = stand.y;
        if( _mapFighters[x+y*_x] == NULL)
            return false;
        UInt8 attackDis = static_cast<BattleFighter*>(_mapFighters[x+y*_x])->GetAttackRange();
        UInt8 dis = (stand.x-target.x)*(stand.x-target.x) + (stand.y-target.y)*(stand.y-target.y);
        if( dis <= attackDis*attackDis )
        {
            return true;
        }
        return false;

    }

    //获得与弓箭兵相邻的敌人数组
    void BattleGround::GetNeighbourEnemy(UInt8 x,UInt8 y,std::vector<Ascoord>& vecAscoord)
    {
        Ascoord pos(x,y);
        GetAround(pos,vecAscoord);
        CheckUp(vecAscoord);
        for( auto it = vecAscoord.begin() ; it != vecAscoord.end() ; )
        {
            UInt8 x = (*it).x;
            UInt8 y = (*it).y;
            if( _mapGround[x+y*_x] == 0)
            {
                //移除
                it = vecAscoord.erase(it);
            }
            else if( _mapFighters[x+y*_x] == NULL )
            {
                it = vecAscoord.erase(it);
            }
            else if( _mapFighters[x+y*_x] != NULL && _mapFighters[x+y*_x]->GetSide() == currentBf->GetSide())
            {
                it = vecAscoord.erase(it);
            }
            else if( _mapFighters[x+y*_x]->getHP() <= 0 )
            {
                it = vecAscoord.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }


    //获得一个行动力所能走到的范围
    void BattleGround::GetRideZone(std::vector<Ascoord>& vecAscoord)
    {
        UInt8 ride = currentBf->GetMovePower();
        UInt8 x = currentBf->GetGroundX();
        UInt8 y = currentBf->GetGroundY();
        for(UInt8 j = ( y > ride ? y -ride : 0 ) ; j < ( y + ride > _y ? _y : y + ride); ++j)
        {
            if( j >= _y )
               break;
            for(UInt8 i = (i > ride ? i -ride : 0 ) ; i < ( i +ride > _x ? _x : x+ride); ++i )
            {
                if( i >= _x )
                    break;
                if( _mapGround[i+j*_x] == 0 )
                    continue;
                if( _mapFighters[i+j*_x] != NULL && _mapFighters[i+j*_x]->getHP() > 0 )
                    continue;
                if( _mapFighters[i+j*_x] != NULL && _mapFighters[i+j*_x] == currentBf )
                    continue;
                vecAscoord.push_back(Ascoord(i,j));
            }
        }
    }



    void BattleGround::GetBackPosition(std::vector<Ascoord>& vecNearEnemy, std::vector<AttackInfo> vecFinal)
    {
        //后退的那个点 不能有人 不能是障碍 
        std::vector<Ascoord> vecAscoord;
        GetRideZone(vecAscoord);
        for( auto it = vecAscoord.begin(); it != vecAscoord.end() ; ++it )
        {
            for( auto iter = vecNearEnemy.begin(); iter != vecNearEnemy.end(); ++iter)
            {
               if( IsInAttackZone(*it,*iter) && ! EnemyCanAttack(*iter,*it) )
               {
                   vecFinal.push_back(AttackInfo(*it,*iter));
               }
            }
        }
    }


    void BattleGround::ShootCurrentPosAttack()
    {
        static UInt8 priority [4][4] = {
            {1,2,3,4},
            {1,2,3,4},
            {1,2,3,4},
            {1,2,3,4}
        };
        UInt8 x = currentBf->GetGroundX();
        UInt8 y = currentBf->GetGroundY();
        Ascoord pos = Ascoord(x,y);
        std::vector<Ascoord> vecEnemy;
        GetNearEnemy(x,y,vecEnemy);
        for( auto it = vecEnemy.begin() ; it != vecEnemy.end() ; ++it)
        {
            if( IsInAttackZone(pos,*it) )
            {
                UInt8 cost = 0;
                UInt8 pri = priority[currentBf->getClass()-1][_mapFighters[(*it).x+(*it).y*_x]->getClass()-1];
               _vecTarget.push_back(TargetInfo(static_cast<BattleFighter*>(_mapFighters[(*it).x+(*it).y*_x]),pos,*it,cost,pri));
            }
        }
        vecEnemy.clear();

    }

    void BattleGround::ShootBackAttack()
    {
        UInt8 x = currentBf->GetGroundX();
        UInt8 y = currentBf->GetGroundY();
        Ascoord pos = Ascoord(x,y);
        std::vector<Ascoord> vecEnemy;
        GetNearEnemy(x,y,vecEnemy);
        std::vector<AttackInfo> vecFinal;   //最终确立的那个要后退的一系列点
        GetBackPosition(vecEnemy,vecFinal);
        if( vecFinal.empty())   //后退没有位置 那就站原地攻击
        {
            ShootGetTarget(1);
        }
        else
        {
            SetStart(Ascoord(x,y));
            for( auto it = vecFinal.begin() ; it != vecFinal.begin(); ++it)
            {
                SetEnd((*it).attack);
                ComputeRoute();
                std::list<Ascoord>path;
                GetRoute(path);
                BowAnalyse(path,(*it).target);
            }
        }
        vecEnemy.clear();
        vecFinal.clear();
    }


    void BattleGround::ShootFrontAttack()
    {
        UInt8 x = currentBf->GetGroundX();
        UInt8 y = currentBf->GetGroundY();
        Ascoord pos = Ascoord(x,y);
        std::vector<Ascoord> vecEnemy;
        GetNearEnemy(x,y,vecEnemy);
        SetStart(Ascoord(x,y));
        for(auto it = vecEnemy.begin() ; it != vecEnemy.end() ; ++it )
        {
            SetEnd(*it);
            ComputeRoute();
            std::list<Ascoord>path;
            GetRoute(path);
            Analyse(path,(*it));
        }
        vecEnemy.clear();
    }

    void BattleGround::ShootGetTarget(UInt8 actionType)
    {
        switch(actionType)
        {
            case 0:
                return;
            case 1:
                ShootCurrentPosAttack();   //原地攻击
                break;
            case 2:
                ShootBackAttack();         //后退攻击
                break;
            case 3:
                ShootFrontAttack();        //前进攻击
                break;
            default:
                break;
        }
    }

    UInt8 BattleGround::GetShootActionType(UInt8 x ,UInt8 y, std::vector<Ascoord>& vecEnemy)
    {
        Ascoord target = Ascoord(0xFF,0xFF);
        Ascoord MyPos = Ascoord(x,y);
        for(auto it = vecEnemy.begin() ; it != vecEnemy.end() ; ++it)
        {
            if( IsInAttackZone(MyPos,*it))
            {
                target = *it;
                break;
            }
        }
        if( target.x == 0xFF && target.y == 0xFF )
        {
           return 3;
        }
        std::vector<Ascoord> vecNeighbourEnemy;
        GetNeighbourEnemy(x,y,vecNeighbourEnemy);
        if( vecNeighbourEnemy.size() == 6 || vecNeighbourEnemy.empty() ) //周围没有敌人  或者被敌人包围起来
        {
           return 1; //原地攻击
        }
        else
        {
           return 2; //后退攻击
        }
        return 0;

    }

    void BattleGround::GetTarget(UInt8 x,UInt8 y ,UInt8 ride)
    {
        std::vector<Ascoord> vecEnemy;
        GetNearEnemy(x,y,vecEnemy);
        if( vecEnemy.empty() )
        {

        }
        else
        {
            //弓箭兵另算
            if( currentBf->GetTypeId() == 3 )
            {
                //获得弓箭兵行为类型  1原地攻击  2后退攻击  3前进攻击
                UInt8 actionType = GetShootActionType(x,y, vecEnemy);
                ShootGetTarget(actionType);
            }
            else
            {
                SetStart(Ascoord(x,y));
                for(auto it = vecEnemy.begin() ; it != vecEnemy.end() ; ++it )
                {
                    SetEnd(*it);
                    ComputeRoute();
                    std::list<Ascoord>path;
                    GetRoute(path);
                    Analyse(path,(*it));
                }
            }
        }
    }
    

    void BattleGround::BowAnalyse(std::list<Ascoord> path,Ascoord target)
    {
        std::reverse(path.begin(),path.end());
        static UInt8 priority [4][4] = {
                          {1,2,3,4},
                          {1,2,3,4},
                          {1,2,3,4},
                          {1,2,3,4}
                      };
        //按照这个路径走一下  获得总的一个行动力消耗 走到可攻击点就行了
        UInt8 cost = 0;
        Ascoord attack;
        if( path.size() < 2 )
        {
            return ;
        }
        else
        {
            auto begin = path.begin();
            begin++;
            for( auto it = begin; it != path.end();++it )
            {
                    Ascoord p = *it;
                    cost+=GetRideSub(p.x,p.y);
            }
        }
        if( cost > currentBf->GetMovePower())
        {
            return;
        }
        else
        {
            UInt8 pri = priority[currentBf->getClass()-1][_mapFighters[target.x+target.y*_x]->getClass()-1];
            _vecTarget.push_back(TargetInfo(static_cast<BattleFighter*>(_mapFighters[target.x+target.y*_x]),attack,target,cost,pri));
        }
    }

    void BattleGround::Analyse(std::list<Ascoord> path,Ascoord& target)
    {
        static UInt8 priority [4][4] = {
                          {1,2,3,4},
                          {1,2,3,4},
                          {1,2,3,4},
                          {1,2,3,4}
                      };
        //按照这个路径走一下  获得总的一个行动力消耗 走到可攻击点就行了
        UInt8 cost = 0;
        Ascoord attack;
        std::reverse(path.begin(),path.end());
        if( path.size() < 2 )
        {
            return ;
        }
        if( path.size() == 2)
        {
            attack = _start;
        }
        else
        {
            for( auto it = path.begin(); it != path.end(); )
            {
                if( IsInAttackZone(*it,target))
                {
                    attack = *it;
                    break;
                }
                else
                {
                    ++it;
                    Ascoord p = *it;
                    cost+=GetRideSub(p.x,p.y);
                }

            }
        }
        UInt8 ride = GetRideSub(attack.x,attack.y);
        UInt8 movePower = currentBf->GetMovePower();
        if( cost > movePower )
        {
            if(IsInAround(attack,target) && (cost-movePower) <= (ride-1) )    //如果攻击点在目标点的附近
            {
                UInt8 pri = priority[currentBf->getClass()-1][_mapFighters[target.x+target.y*_x]->getClass()-1];
                _vecTarget.push_back(TargetInfo(static_cast<BattleFighter *>(_mapFighters[target.x+target.y*_x]),attack,target,cost,pri));
            }
            else
            {
                return;
            }
        }
        else
        {
            UInt8 pri = priority[currentBf->getClass()-1][_mapFighters[target.x+target.y*_x]->getClass()-1];
            _vecTarget.push_back(TargetInfo(static_cast<BattleFighter *>(_mapFighters[target.x+target.y*_x]),attack,target,cost,pri));
        }

    }
   
    //各兵种依次在grass forest town hill行动力消耗
    //
    UInt8 RideSub[3][4] = {
        { 1,1,2,3},  //hoser 骑兵
        { 1,2,2,3},  //步兵
        { 1,1,2,3}   //弓兵
    };


    UInt8 BattleGround::GetRideSub(const UInt8& posx ,const UInt8& posy)
    { 
         //TODO  返回消耗 (考虑周围敌军情况)
        //lua_State * L = lua_open();
        //luaOpen_base(L);
        //luaOpen_string(L);
        //luaOpen_table(L);
        //luaL_openlibs(L);
        // std::string path = cfg.scriptPath+"items/map.lua";
        // lua_tinker::dofile(L,path.c_str());
         UInt8 stype = currentBf->GetTypeId();
         UInt8 form = _mapGround[posx+posy*_x];
         //UInt8 rideSub = lua_tinker::call<UInt8>(L,"GetRideSub",stype,form); 
         UInt8 sub = RideSub[stype-1][form-1];
         return sub;
    }


    void BattleGround::Fight(BattleFighter *bf , BattleFighter * bo, UInt8& result, UInt32& BattleReport)
    { 
        //TODO   连接BattleSimulator
        //添加一个实际的攻击距离
        UInt8 distance = GetFactAttackDis();

        BattleSimulator bsim(bf,bo,distance);
        bsim.start(); 
        result = bsim.GetWin();
        BattleReport = bsim.getId();

        std::cout << "发生战斗  " << static_cast<UInt32>(bf->GetBattleIndex()) << " VS " << static_cast<UInt32>(bo->GetBattleIndex()) << "  战斗结果: " << static_cast<UInt32>(result) <<" 战报ID:" << BattleReport << std::endl;
        //result = 0;
        //BattleReport = 111;
        //bo->setHP(0);

    }

    UInt8 BattleGround::GetFactAttackDis()
    {
        return currentBf->GetAttackRange();       
    }


    BattleFighter* BattleGround::newFighter(UInt8 x,UInt8 y ,GObject::Fighter * fgt)
    {
        UInt8 typeId = fgt->GetTypeId();
        BattleFighter * bf = BattleSimulator::CreateFighter(typeId,NULL,fgt, x, y);
        if ( bf == NULL )
        {
            return NULL;
        }
        setObject(x, y, bf ,1);
        bf->SetEnterPos(x,y);
        UInt8 actId = GetBattleIndex(x,y);
        bf->SetBattleIndex(actId);
        //bf->InsertFighterInfo(_pack,1);
        std::cout << "入场战将编号 : " << static_cast<UInt32>(bf->GetBattleIndex()) << std::endl;
        return bf;
    } 

    void BattleGround::setObject(UInt8 x , UInt8 y ,BattleFighter * bf,UInt8 flag)
    { 
        if(x >= _x || y >= _y) 
            return ;
        UInt8 oldx = bf->GetGroundX();
        UInt8 oldy = bf->GetGroundY();
        if(!flag && _mapFighters[oldx+oldy*_x] != bf)
            return ;
        if(_mapFighters[x+y*_x])
            return ;
        _mapFighters[oldx+oldy*_x] = NULL;
        _mapFighters[x+y*_x] = bf;
        if(flag)
        {
            bf->SetEnterPos(x,y);
        }
    }

    BattleFighter* BattleGround::GetMinBattleIndexFighter(std::list<BattleFighter*> listFighter)
    {
        UInt8 actId = 0xFF;
        BattleFighter* bf = NULL;
        for(auto it = listFighter.begin(); it != listFighter.end(); ++it )
        {
            if((*it) != NULL &&  (*it)->getHP() > 0 && (*it)->GetBattleIndex() < actId )
            {
                bf = (*it);
                actId = (*it)->GetBattleIndex();
            }
        }
        return bf;
    }

    //一回合的战术
    void BattleGround::FightOneRound()
    {
        _oneRoundCostTime = 0;

        if( _isFirstRound )
        {
            preStart();  //
            _isFirstRound = false;
        }
        if( CheckIsStop() )
            return;
        TestCoutBattleS();
        std::map<UInt8,std::list<BattleFighter*>> camp2fighters_copy = camp2fighters;
        
        _pack.init(0x80);
        _pack << static_cast<UInt8>(_mapId);
        MakePreStartInfo();
        UInt16 actCount = 0;
        size_t offset = _pack.size();
        _pack << actCount;
        while(  /*flag < camp2fighters_copy.size()*/ camp2fighters_copy.size() > 0 )
        {
            if( CheckIsStop() )
                break;
            for(auto it = camp2fighters_copy.begin(); it != camp2fighters_copy.end(); ++it )
            {
                if( camp2fighters_copy.size() == 0 )
                {
                    break;
                }
                if( (it->second).empty())
                {
                    //++flag;
                   camp2fighters_copy.erase(it->first);
                   break;
                }
                else
                {
                    std::list<BattleFighter*> listFighter = camp2fighters_copy[it->first];
                    for( auto iter = listFighter.begin() ; iter != listFighter.end(); ++iter )
                    {
                        if( (*iter) != NULL && (*iter)->getHP() <= 0 )
                        {
                            listFighter.remove((*iter));
                        }
                    }
                    BattleFighter* bf =  GetMinBattleIndexFighter(listFighter);
                    if( bf == NULL )
                    {
                        break;
                    }
                    if( listFighter.empty() )
                    {
                        break;
                    }
                    currentBf = bf;
                    Move();
                    listFighter.remove(bf);
                    camp2fighters_copy[it->first] = listFighter;
                    ++actCount;
                }
            }
        }
        ++_actId;
        camp2fighters_copy.clear();

        std::cout<<"战术回合"<<static_cast<UInt32>(_actId)<<std::endl;
        _pack.data<UInt16>(offset) = actCount;
        UInt32 now = TimeUtil::Now();
        _pack << static_cast<UInt32>(now);
        _pack<<Stream::eos;
        UInt32 battleId = IDGenerator::gBattleOidGenerator0.ID();
        battleReport0.addReport(/*_battleNum*/ battleId,_pack);
        report2IdTable.Insert(_id-_mapId,_mapId,_actId,/*_battleNum*/ battleId,now);
        DB7().PushUpdateData("REPLACE INTO `report2id` value(%u,%u,%u,%u,%u)",_id-_mapId,_mapId,_actId, /*_battleNum*/battleId,now);


        Stream st(0xA6);
        st<< static_cast<UInt8>(1);
        st<< static_cast<UInt8>(_mapId);
        st<< static_cast<UInt32>(battleId/*_battleNum*/);
        st<< Stream::eos;

        std::set<GObject::Player*> setPlayer;
        GetJoinPlayers(setPlayer);
        for( auto it = setPlayer.begin(); it != setPlayer.end(); ++it )
        {
            (*it)->send(st);
        }
    }


    void BattleGround::GetJoinPlayers(std::set<GObject::Player*>& setPlayer)
    {
        for( auto it = map2fighter.begin(); it != map2fighter.end(); ++it )
        {
            for( auto iter = (it->second).begin(); iter != (it->second).end(); ++iter)
            {
                if( setPlayer.find((*iter)->owner)  == setPlayer.end() );
                {
                    setPlayer.insert((*iter)->owner);
                }
            
            }
        }
    }
   
    //检测某一阵营是不是已经死光了
    bool BattleGround::SomeCampIsAllDie(UInt8 campId)
    {
        std::list<BattleFighter*> listFighter = camp2fighters[campId];
        for( auto it = listFighter.begin(); it != listFighter.end(); ++it )
        {
            if( (*it) != NULL && (*it)->getHP() > 0 )
            {
                return false;
            }
        }
        return true;
    }
    
    bool BattleGround::CheckIsStop()
    {
        //检测是不是已经可以结束战斗了  
        UInt8 num = 0;
        for(auto it = camp2fighters.begin(); it != camp2fighters.end(); ++it )
        {
            UInt8 campId = it->first;
            bool res = SomeCampIsAllDie(campId);
            if( res )
            {
                ++num;
            }
        }
        UInt8 totalCamp = camp2fighters.size();
        if( num+1 >= totalCamp )
        {
            return true;
        }
        return false;
    }


    //一个回合一个战报  所以撒
    void BattleGround::MakePreStartInfo()
    {
        _pack<<static_cast<UInt8>(camp2fighters.size());
        for(auto it = camp2fighters.begin(); it != camp2fighters.end(); ++it )
        {
            UInt8 campId = it->first;
            _pack<<static_cast<UInt8>(campId);
            _pack<<static_cast<UInt8>((it->second).size());
            for(auto iter = (it->second).begin(); iter != (it->second).end(); ++iter)
            {
                if( (*iter) != NULL && (*iter)->getHP() > 0 )
                {
                    (*iter)->InsertFighterInfo(_pack,1);
                }
            }

        }

    }
    

    void BattleGround::start()
    {
        if( map2fighter.size() < 2 )  //只有一个阵营的话  就不开打
            return;
        preStart();  //
        //按照行动id的大小对每一个阵营进行排序

        while(true)
        {
            if( _actId > 5 )
                break;
            if(CheckIsStop())
            {
                break;
            }
            FightOneRound();
            //++_actId;
        }
    }


    /*
    void BattleGround::start()
    { 
        if( map2fighter.size() < 2 )  //只有一个阵营的话  就不开打
            return;
        preStart();  //
        //std::list<BattleFighter*> fighters_copy[PLAYERMAX];
        //for(UInt8 i=1; i <= map2fighter.size();++i)
        //{
        //   fighters_copy[i] = fighters[i];
        //}
        TestCoutBattleS();
        //UInt32 alive = 0;
        UInt16 actCount = 0;
        size_t offset = _pack.size();
        _pack << actCount ;
        UInt32 now = TimeUtil::Now();
        _pack << static_cast<UInt32>(now);

        std::map<UInt8,std::list<BattleFighter*>> camp2fighters_copy = camp2fighters;
        while( camp2fighters_copy.size() > 0 )
        {
            for(auto it = camp2fighters_copy.begin(); it != camp2fighters_copy.end(); ++it )
            {
                if( camp2fighters_copy.size() == 0 )
                {
                    break;
                }
                if( (it->second).empty())
                {
                   camp2fighters_copy.erase(it->first);
                }
                else
                {
                    std::cout<<"势力 "<<static_cast<UInt32>(it->first)<<"出手"<<std::endl;
                    std::list<BattleFighter*> listFighter = camp2fighters_copy[it->first];
                    for( auto iter = listFighter.begin() ; iter != listFighter.end(); ++iter )
                    {
                        if( (*iter) != NULL && (*iter)->getHP() <= 0 )
                        {
                            listFighter.remove((*iter));
                        }
                    }
                    BattleFighter* bf = GetMinBattleIndexFighter(listFighter);
                    if( bf == NULL )
                    {
                        break;
                    }
                    if( listFighter.empty() )
                    {
                        break;
                    }
                    currentBf = bf;
                    std::cout<<"轮到战将编号为  "<<static_cast<UInt32>(currentBf->GetBattleIndex())<<"进行攻击"<<std::endl;
                    Move();
                    listFighter.remove(bf);
                    camp2fighters_copy[it->first] = listFighter;
                    ++actCount;
                }
            }
        }
        */
        /*
        while(true)
        {

            if( flag >= map2fighter.size() )
                break;
            if( i > map2fighter.size() )
            {
                i = 1;
            }
            while(!fighters_copy[i].empty() && fighters_copy[i].back()->getHP() <= 0 )
            {
                 fighters_copy[i].pop_front();    
            }

            if( fighters_copy[i].size() == 0 )
            {
                ++flag;
                ++i;
                continue;
            }

            currentBf = fighters_copy[i].front();
            if( currentBf == NULL )
            {
                continue;
            }
            Move();
            ++actCount;
            if( !(alive)&(1<<i))
            {
                alive |= (1<<i);
                ++count;
            }
            fighters_copy[i].pop_front();
            ++i;
        }
        _pack.data<UInt16>(offset) = actCount;
        _pack<<Stream::eos;
        battleReport0.addReport(_battleNum,_pack);
        report2IdTable.Insert(_id-_mapId,_mapId,_battleNum);
        DB7().PushUpdateData("REPLACE INTO `report2id` value(%u,%u,%u)",_id-_mapId,_mapId,_battleNum);

    }
    */
    //战将进入战场
    void BattleGround::preStart()  //需要玩家手动操作
    { 
        //std::map<UInt8 ,std::vector<GObject::Player *> >::iterator it = map_player.begin();
        //_pack << static_cast<UInt8>(_mapId);
        //_pack << static_cast<UInt8>(map_player.size()); 
        //_pack << static_cast<UInt8>(map2fighter.size()); 
        if( map2fighter.size() <= 1 )
            return;
        for( auto it = map2fighter.begin(); it != map2fighter.end(); ++it )
        {
            UInt8 campId = it->first;
            std::cout<<"势力 "<<static_cast<UInt32>(campId);
            //_pack << static_cast<UInt8>(campId);
            std::vector<FighterInfo*> vecInfo = it->second;
            //_pack << static_cast<UInt8>(vecInfo.size());
            for( auto iter = vecInfo.begin(); iter != vecInfo.end(); ++iter)
            {
                GObject::Player* owner = (*iter)->owner;
                UInt16 fighterId = (*iter)->fighterId;
                UInt8  x = (*iter)->posx;
                UInt8  y = (*iter)->posy;
                GObject::Fighter* fgt = owner->findFighter(fighterId);
                if( camp2fighters[campId].empty() )
                {
                    std::list<BattleFighter*> listFighter;
                    BattleFighter * bft = newFighter(x,y,fgt);
                    if( bft == NULL )
                        continue;
                    listFighter.push_back(bft);
                    camp2fighters[campId] = listFighter;
                }
                else
                {

                    std::list<BattleFighter*> listFighter = camp2fighters[campId];
                    BattleFighter* bft = newFighter(x,y,fgt);
                    if( bft == NULL )
                    {
                        continue;
                    }
                    listFighter.push_back(bft);
                    camp2fighters[campId] = listFighter;
                }
            }
        }
        //放将的顺序是一个自己一个敌人
        /*
        for(;it != map_player.end(); ++it)
        {
            if(it->first >= PLAYERMAX)
                break;
            size_t offset = _pack.size();
            UInt8 count = 0;
            _pack << count ;
            std::vector<GObject::Player *> vec = it->second;

            std::cout << std::endl << "势力：" << static_cast<UInt32>(it->first) <<std::endl;
            std::vector<UInt8> vecforce =  GetSameCamp(it->first);
            for(UInt8 i = 0; i < vec.size(); ++i)
            {
                //for(UInt8 j = 0; j < 6 ; ++j)
                {
                    //军团成员放入战将
                    GObject::Fighter * fgt = vec[i]->getMainFighter();
                    if(!fgt)
                        continue;
                    UInt8 value = vecforce[i];
                    UInt8 x = value%_x;
                    UInt8 y = value/_x;
                    fighters[it->first].push_back(newFighter(x,y,fgt));
                    ++count;
                }
            }
            _pack.data<UInt8>(offset) = count;
            std::cout << "  人数"<< static_cast<UInt32>(count);
        }
        */
    } 

    void BattleGround::TestCoutBattleS(BattleFighter * bf)
    { 
        for(UInt8 j = 0 ; j < _y; ++j)   
        { 
            for(UInt8 i = 0; i < _x; ++i)  
            {
                if( _mapGround[i+j*_x] == 0 )
                    continue;
                if(_mapFighters[i+j*_x] != NULL )
                {
                    std::cout << "战将编号" << static_cast<UInt32>(_mapFighters[i+j*_x]->GetBattleIndex());
                    std::cout << "玩家Side:"<< static_cast<UInt32>(_mapFighters[i+j*_x]->GetSide());
                    std::cout << "  x:"<< static_cast<UInt32>(_mapFighters[i+j*_x]->GetGroundX());
                    std::cout << "  y:"<< static_cast<UInt32>(_mapFighters[i+j*_x]->GetGroundY());
                    std::cout << " 血量：" << static_cast<UInt32>(_mapFighters[i+j*_x]->getHP()) ;
                    std::cout << std::endl;
                }
            } 
        } 
    } 

    bool BattleGround::IsInAround(const Ascoord& p , const Ascoord& t)
    {
        std::vector<Ascoord> vecAscoord;
        GetAround(t,vecAscoord);
        CheckUp(vecAscoord);
        for( auto it = vecAscoord.begin() ; it != vecAscoord.end(); ++it )
        {
            if( (*it) == p )
            {
                return true;
            }
        }
        return false;
    }

    bool BattleGround::IsObstract(const Ascoord &p)
    {
        UInt8 x = p.x;
        UInt8 y = p.y;
        bool ret = false;

        if( _mapGround[x+y*_x] == 0 )
        {
            ret = true;
        }
        if(_mapFighters[x+y*_x] != NULL && _mapFighters[x+y*_x]->getHP() > 0 && IsInAround(p,_end))  //敌人附近的自己人也视为障碍物
        {
            ret = true;
        }
        if( Ascoord(x,y) != _end && _mapFighters[x+y*_x] != NULL && _mapFighters[x+y*_x]->GetSide() != currentBf->GetSide() && _mapFighters[x+y*_x]->getHP()> 0 )
        {
            ret = true;
        }
        return ret;
    }

    void BattleGround::GetAround(const Ascoord& p,std::vector<Ascoord>& vecAscoord)
    {
        vecAscoord.clear();
        UInt8 x = p.x;
        UInt8 y = p.y;
        if( y % 2 == 0 )
        {
            vecAscoord.push_back(Ascoord(x-1,y));
            vecAscoord.push_back(Ascoord(x+1,y));
            vecAscoord.push_back(Ascoord(x-1,y+1));
            vecAscoord.push_back(Ascoord(x,y+1));
            vecAscoord.push_back(Ascoord(x-1,y-1));
            vecAscoord.push_back(Ascoord(x,y-1));
        }
        else
        {
            vecAscoord.push_back(Ascoord(x,y-1));
            vecAscoord.push_back(Ascoord(x+1,y));
            vecAscoord.push_back(Ascoord(x+1,y-1));
            vecAscoord.push_back(Ascoord(x-1,y));
            vecAscoord.push_back(Ascoord(x,y+1));
            vecAscoord.push_back(Ascoord(x+1,y+1));
        }

    }

    bool BattleGround::IsInList(std::list<Node>& list, const Ascoord &p )
    {
        for( auto it = list.begin() ; it != list.end() ; ++it)
        {
            if( (*it).node == p )
                return true;
        }
        return false;
    }


    Node* BattleGround::FindFromList(std::list<Node>& list,const Ascoord& p)
    {
        for( auto it = list.begin() ; it != list.end(); ++it)
        {
            if( (*it).node == p )
            {
                return &(*it);
            }
        }
        return NULL;
    }

    void BattleGround::SetStart(const Ascoord& p)
    {
        _start = p;
    }

    void BattleGround::SetEnd(const Ascoord& p)
    {
        _end = p;
    }

    UInt8 BattleGround::GetHValue(const Ascoord& p)
    {
        const Ascoord& pos =  ConvertConverstion(p);
        const Ascoord& end =  ConvertConverstion(_end);
        return abs(pos.x-end.x)+abs(pos.y-pos.y);
    }

    UInt8 BattleGround::GetGValue(const Ascoord& p)
    {
        return GetRideSub(p.x,p.y)*2;
    }


    void BattleGround::CheckUp(std::vector<Ascoord>& vecAscoord)   //核查临近的6个方向的点是否越界  越界删掉
    {
        for(auto it = vecAscoord.begin(); it != vecAscoord.end() ; )
        {
            if( (*it).x >=  _x || (*it).y >=  _y )
            {
                it = vecAscoord.erase(it);
            }
            else
            {
                ++it;
            }
        }
    }


    bool BattleGround::ComputeRoute()
    {
        _openList.clear();
        _closeList.clear();
        Ascoord start = _start;
        Node node(_start);  //起点入openlist
        node.g = 0;
        node.h = GetHValue(_start);

        _openList.push_back(node);
        while( !_openList.empty() )
        {
            PopBestStep(&node);
            _closeList.push_back(node);
            Ascoord p = node.node;
            GetAround(p,_aroundAscoord);
            CheckUp(_aroundAscoord);
            for( auto it = _aroundAscoord.begin(); it != _aroundAscoord.end(); ++it)
            {
                UInt8 gAdd = GetGValue(*it);
                if( JudgeSurround((*it),p,node.g+gAdd))
                    return true;
            }
        }
        return false;
    }


    bool BattleGround::JudgeSurround(const Ascoord &cur,const Ascoord& parent,UInt8 g)
    {
        if( !IsInList(_closeList,cur) && !IsObstract(cur) )
        {
            Node* node = FindFromList(_openList,cur);
            if( node == NULL )
            {
                //新的节点放入到openList表中
                Node newNode(cur);
                newNode.g = g;
                newNode.h = GetHValue(cur);
                newNode.parent = parent;
                
                _openList.push_back(newNode);
                if( newNode.node == _end )
                    return true;
            }
            else
            {
                if( node->g > g  )   //比原来的节点路径还要短  用这个替换原来的
                {
                    node->g = g;
                    node->parent = parent;
                }
            }
        }
        return false;
    }

    bool BattleGround::PopBestStep(Node* node)
    {
        for(auto it = _openList.begin(); it != _openList.end();)
        {
            node->g = it->g;
            node->h = it->h;
            node->node = it->node;
            node->parent = it->parent;

            ++it;

            while( it != _openList.end() )
            {
                if( node->g + node->h > it->g + it->h )
                {
                    node->g = it->g;
                    node->h = it->h;
                    node->node = it->node;
                    node->parent = it->parent;
                }
                ++it;
            }

            it = _openList.begin();

            while( it->node != node->node )
            {
                ++it;
            }
            _openList.erase(it);
            return true;
        }
        return false;
    }

    bool BattleGround::GetRoute(std::list<Ascoord>& list)
    {
        list.clear();
        Node* Node = FindFromList(_openList,_end);
        if( Node != NULL )
        {
            list.push_back(Node->node);
            Node = FindFromList(_closeList,Node->parent);

            while( Node->node != _start )
            {
                list.push_back(Node->node);
                Node = FindFromList(_closeList,Node->parent);
            }
            list.push_back(Node->node);
            return true;
        }
        return false;
    }


    void BattleGround::PushFighter(GObject::Player* player,UInt16 fighterId ,UInt8 x,UInt8 y)
    {
        GObject::Clan* clan = player->GetClan();
        if( clan == NULL )
            return;
        UInt8 forceId = clan->GetBattleForceId();
        if( forceId == 0 )
            return;
        map2fighter[forceId].push_back(new FighterInfo(player,fighterId,x,y));

        player->SetBattleId(_id);
        player->SetBattleSide(forceId);
    }


    UInt8 BattleGround::GetSpecialDirection()
    {
        UInt8 dir = 0;
        if( map2fighter.size() == 4 )
        {
           dir = 1;
        }
        if( map2fighter.size() == 3 )
        {
            dir = 1;
        }
        if( map2fighter.size() == 2 )
        {
            dir = 1;
        }
        if( map2fighter.size() == 1 )
        {
            dir = 1;
        }
        return dir;

    }
    
    //设置阵营层面的行动Id
    void BattleGround::SetCampActId()
    {
        GData::MapInfo* mapInfo = GData::mapTable.GetMapInfo(_mapId);
        UInt8 forceNum = mapInfo->GetCampNum();  //获得阵营的数量
        if( forceNum < 2 )
        {
            return;
        }
        if( forceNum == 5 )  //如果有5个势力的这个比较特殊
        {
            forceNum = 4;
            //势力5特殊处理
            UInt8 dir = GetSpecialDirection();
            GetCampAttackOrder(5,dir);
        }
        //
        for(UInt8 i = 1 ; i <= forceNum ; ++i )
        {
            UInt8 dir = mapInfo->GetAttackDirect(i);
            //按照攻击方向获得实际的出手顺序
            GetCampAttackOrder(i,dir);
        }
    }
    //  我们这么约定 1 left 2 right 3 up 4 down(进攻方向)
    //

    void BattleGround::GetCampAttackOrder(UInt8 campId,UInt8 dir)
    {
        switch(dir)
        {
            case 1: //left  向左进攻
                SearchFromLeftToRight(campId);
                break;
            case 2: //right              //以x轴作为标准 y进行变化 4同2 
                SearchFromRightToLeft(campId);
                break;
            case 3: //up
                SearchFromUpToDown(campId);
                break;
            case 4: //down
                SearchFromDownToUp(campId);
                break;
        }
    }

    void BattleGround::SearchFromLeftToRight(UInt8 campId)
    {
        for(UInt8 i = 0 ; i < _x ; ++i)
        {
            for(UInt8 j = 0; j < _y ; ++j )
            {
                if( _mapCamp[i+j*_x] == campId )
                {
                    if( _camp2pos[campId].empty())
                    {
                        std::list<Ascoord> listAscoord;
                        listAscoord.push_back(Ascoord(i,j));
                        _camp2pos[campId] = listAscoord;
                    }
                    else
                    {
                        _camp2pos[campId].push_back(Ascoord(i,j));
                    }
                }
            }
        }

    }


    void BattleGround::SearchFromRightToLeft(UInt8 campId)
    {
        for( UInt8 i = _x-1; i >= 0 ; --i)
        {
            if( i > _x )
            {
                break;
            }
            for( UInt8 j = 0 ; j < _y ; ++j )
            {
                if( j > _y )
                {
                    break;
                }
                if( _mapCamp[i+j*_x] == campId )
                {
                    if( _camp2pos[campId].empty())
                    {
                        std::list<Ascoord> listAscoord;
                        listAscoord.push_back(Ascoord(i,j));
                        _camp2pos[campId] = listAscoord;
                    }
                    else
                    {
                        _camp2pos[campId].push_back(Ascoord(i,j));

                    }
                }
            }
        }
    }

    void BattleGround::SearchFromUpToDown(UInt8 campId)
    {
        for( UInt8 j = 0 ; j < _y ; ++j )
        {
            for( UInt8 i = 0; i < _x ; ++i )
            {
                if( _mapCamp[i+j*_x] == campId )
                {
                    if( _camp2pos[campId].empty())
                    {
                        std::list<Ascoord> listAscoord;
                        listAscoord.push_back(Ascoord(i,j));
                    }
                    else
                    {
                        _camp2pos[campId].push_back(Ascoord(i,j));
                    }

                }
            }
        }
    }

    void BattleGround::SearchFromDownToUp(UInt8 campId)
    {
        for(UInt8 j = _y-1; j >= 0 ; --j )
        {
            if( j > _y )
            {
                break;
            }
            for( UInt8 i = 0 ; i < _x ; ++i )
            {
                if( _mapCamp[i+j*_x] == campId )
                {

                    if( _camp2pos[campId].empty())
                    {
                        std::list<Ascoord> listAscoord;
                        listAscoord.push_back(Ascoord(i,j));
                        _camp2pos[campId] = listAscoord;
                    }
                    else
                    {
                        _camp2pos[campId].push_back(Ascoord(i,j));
                    }
                }
            }
        }

    }


    //对每一个位置进行编号(阵营位置)
    void BattleGround::SetEachPosNumber()
    {
        while( /*flag < _camp2pos.size()*/ _camp2pos.size() > 0 )
        {
            for( auto it = _camp2pos.begin(); it != _camp2pos.end(); ++it)
            {
                if( _camp2pos.size() == 0  )
                {
                    break;
                }
                if( (it->second).empty())
                {
                    _camp2pos.erase(it->first);
                    //++flag;
                    continue;
                }
                else
                {
                    Ascoord point = (it->second).front();
                    num2pos.insert(std::map<UInt8,Ascoord>::value_type (++_maxID,point));
                    (it->second).pop_front();
                }
            }
        }
    }


    UInt8 BattleGround::GetBattleIndex(UInt8 x,UInt8 y )
    {
        for( auto it = num2pos.begin() ; it != num2pos.end() ; ++it )
        {
            if( (it->second).x == x && (it->second).y == y )
            {
                return it->first;
            }
        }
        return -1;
    }
    

    //设置每个战将出手顺序
    /*
    void BattleGround::SetBattleIndex()
    {
        UInt8 actCampNum = map2fighter.size();  //实际阵营的数量
        std::list<BattleFighter *> fighters_copy[PLAYERMAX];   //阵营中的战将
        for(UInt8 i=1; i <= actCampNum;++i)
        {
            if( fighters[i].empty())
                continue;
            fighters_copy[i] = fighters[i];
        }
        
        std::map<UInt8, std::list<Ascoord>> camp2pos_copy = _camp2pos;

        UInt8 flag = 0 ;
        UInt8 i = 1;
        do
        {
            if( i > actCampNum )
            {
                i = 1;
            }
            if( fighters_copy[i].empty())
            {
                ++flag;
                ++i;
                continue;
            }
            if( camp2pos_copy[i].empty())
            {
                ++flag;
                ++i;
                continue;
            }
            Ascoord point =  camp2pos_copy[i].front();
            UInt8 x = point.x;
            UInt8 y = point.y;
            for( auto it = fighters_copy[i].begin(); it != fighters_copy[i].end(); ++it )
            {
                UInt8 posx = (*it)->GetEnterPosX();
                UInt8 posy = (*it)->GetEnterPosY();
                if( posx == x && posy == y )
                {
                    fighters_copy[i].remove(*it);
                    (*it)->SetBattleIndex(++_maxID);
                    break;
                }
            }
            camp2pos_copy[i].pop_front();
            ++i;
        }while(flag <= actCampNum );
    }
    */
}
