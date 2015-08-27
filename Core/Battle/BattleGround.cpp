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
#include "Battle/ClanBattleDistribute.h"
#include "Battle/ClanBattleCityStatus.h"
#include "Battle/Report2Id.h"

#define MAX(x,y) x>y?x:y
#define ABS(x,y) x>y?x-y:y-x
#include<math.h>
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
        //
        Battle::Report2Id* report2id = Battle::report2IdTable.GetReport2Id(_id-_mapId,_mapId);
        if( report2id == NULL )
        {
            _actId = 0;
        }
        else
        {
            Battle::ReportOneRound* roundReport =  report2id->GetNewestReport();
            _actId = roundReport->GetActId();
        }

        std::cout<<" 接上一个回合的 id 为  "<<static_cast<UInt32>(_actId)<<std::endl;
        
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
    
    bool BowMoreFit(TargetInfo info1,TargetInfo info2)
    {
        if( info1.cost < info2.cost)
            return true;
        else if( info1.pri > info2.pri )
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
                if( InMyAttackHasEnemy(*it) || ride <= 0 )
                    break;
                flag = *it;
                ride -=GetRideSub(flag.x,flag.y);
                //UInt8 dis = GetDistance(flag,target);
                if( IsNearbyHaveEnemy(*it) )
                {
                    ride -= 1;
                }
            }
        }
        for(auto it = path.begin();it != path.end(); ++it )
        {
            movePath.push_back(*it);
            if( *it == flag )
                break;
        }
        GetMovePos(movePath,move);  //倒着走  找到第一个没有人的位置即可
    }

    void BattleGround::MoveGetEnemy(std::vector<Ascoord>& vecAscoord)
    {
        UInt8 y = currentBf->GetGroundY();
        if( y > _y )
            return;
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
            return j-1;
        }
        else if( rh.x < downI )
        {
            return j+(downI-rh.x)/2-1;
        }
        else
        {
            return j+(rh.x-upI)/2-1;
        }
    }

    static UInt8 priority [4][4] = {

        {1,2,3,4},
        {1,2,3,4},
        {1,2,3,4},
        {1,2,3,4}
    };


    void BattleGround::SyncHp(Battle::BattleFighter* bft,UInt8 x,UInt8 y)
    {
        if( bft == NULL || bft->GetOwner() == NULL )
            return;
        Battle::battleDistribute.UpdateMainFighterHP(_id-_mapId,_mapId,x,y,bft->getHP());
        std::vector<UInt32> vecHP;
        for( UInt8 i = 0 ; i < 10 ; ++i )
        {
            UInt32 hp = bft->GetSoldierHp(i);
            vecHP.push_back(hp);
        }
        Battle::battleDistribute.UpdateSoldiersHP(_id-_mapId,_mapId,x,y,vecHP);
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
            UInt8 dis = 0;
            if( move.x == 0xFF && move.y == 0xFF)
            {
                mx = x;
                my = y;
            }
            else
            {
                mx = move.x;
                my = move.y;
                Ascoord p = Ascoord(x,y);
                dis = GetDistance(p,move);
                _mapFighters[x+y*_x] = NULL;
                _mapFighters[mx+my*_x] = currentBf;
                if( currentBf->GetOwner() != NULL )  //NPC的血量和位置不需要同步到数据库
                {
                    Battle::battleDistribute.MoveFighter(_mapId,currentBf->GetOwner(),x,y,mx,my,1);
                    SyncHp(currentBf,mx,my);
                }
                else //npc
                {
                    if( _mapCamp[mx+my*_x] != 5 )
                    {
                        Battle::battleDistribute.MoveNpc(_id-_mapId,_mapId,currentBf->GetId(),x,y,mx,my);
                        SyncHp(currentBf,mx,my);
                    }
                }
                //排布信息同步
                /*
                Battle::battleDistribute.UpdateMainFighterHP(_mapId,currentBf->GetOwner(),mx,my,currentBf->getHP());
                std::vector<UInt32> vecHP;
                for( UInt8 i = 0 ; i < 10 ; ++i )
                {
                   UInt32 hp = currentBf->GetSoldierHp(i);
                   vecHP.push_back(hp);
                }
                Battle::battleDistribute.UpdateSoldiersHP(_mapId,currentBf->GetOwner(),mx,my,vecHP);
                */
            }
            currentBf->SetGroundX(mx);
            currentBf->SetGroundY(my);
            std::cout << "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"<<std::endl;
            std::cout << "战将编号:      "  << static_cast<UInt32>(currentBf->GetBattleIndex());
            std::cout <<" 无方案" << " 从" << static_cast<UInt32>( x ) <<" , " << static_cast<UInt32>(y);
            std::cout <<" 移动到 "<<  static_cast<UInt32>(mx)<< " , " << static_cast<UInt32>(my) <<std::endl;
            std::cout << " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"<<std::endl;
            currentBf->InsertFighterInfo(_pack);  //Stream

            //UInt8 rand = uRand(255);
            _pack << static_cast<UInt8>(mx);
            _pack << static_cast<UInt8>(my);
            _pack << static_cast<UInt8>(ceil((dis+1)*0.5));
            std::cout<<"移动用时  " <<static_cast<UInt32>(ceil((dis+1)*0.5))<<std::endl;
            _pack << static_cast<UInt8>(0); //无战斗发生
            _oneRoundCostTime += ceil(dis*0.5)/*currentBf->GetNowTime()/100*/;
             
        }
        else
        {
            //if( currentBf->GetTypeId() == 3 )
            //{
            //    std::sort(_vecTarget.begin(),_vecTarget.end(),BowMoreFit);
            //}
            //else
            //{
                std::sort(_vecTarget.begin(),_vecTarget.end(),MoreFit);
            //}
            TargetInfo target = _vecTarget.front();
            std::cout << std::endl;
            TestCoutBattleS(currentBf);


            //战斗
            
            std::cout << " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"<<std::endl;
            std::cout <<"从     "<<static_cast<UInt32>(currentBf->GetGroundX())<<" , "<< static_cast<UInt32>(currentBf->GetGroundY())<<std::endl;
            std::cout <<"移动到  "<<static_cast<UInt32>(target.attack.x)<< "," << static_cast<UInt32>(target.attack.y) <<std::endl;
            std::cout << "攻击目标  " <<static_cast<UInt32>((target.bo)->GetGroundX())<<","<<static_cast<UInt32>((target.bo)->GetGroundY())<<std::endl;
            std::cout << " XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"<<std::endl;

            //Stream
            //
            //同步_mapFighters的坐标
            UInt32 timeCost = 1;
            UInt8 x = currentBf->GetGroundX();
            UInt8 y = currentBf->GetGroundY();
            UInt8 ax = target.attack.x;
            UInt8 ay = target.attack.y;
            UInt8 dis = 1;
            if( target.attack.x != x ||  target.attack.y != y )
            {
                Ascoord p = Ascoord(x,y);
                dis = GetDistance(p,target.attack);
                _mapFighters[x+y*_x] = NULL;
                _mapFighters[ax+ay*_x] = currentBf;
                if( currentBf->GetOwner() != NULL )
                {
                    Battle::battleDistribute.MoveFighter(_mapId,currentBf->GetOwner(),x,y,ax,ay,1);
                    SyncHp(currentBf,ax,ay);
                }
                else
                {
                    if( _mapCamp[ax+ay*_x] != 5 )
                    {
                        Battle::battleDistribute.MoveNpc(_id-_mapId,_mapId,currentBf->GetId(),x,y,ax,ay);
                        SyncHp(currentBf,ax,ay);
                    }
                }
                /*
                Battle::battleDistribute.UpdateMainFighterHP(_mapId,currentBf->GetOwner(),ax,ay,currentBf->getHP());
                std::vector<UInt32> vecHP;
                for( UInt8 i = 0 ; i < 10 ; ++i )
                {
                   UInt32 hp = currentBf->GetSoldierHp(i);
                   vecHP.push_back(hp);
                }
                Battle::battleDistribute.UpdateSoldiersHP(_mapId,currentBf->GetOwner(),ax,ay,vecHP);
                */
            }
            timeCost += ceil(dis*0.5);
            currentBf->SetGroundX(ax);
            currentBf->SetGroundY(ay);
            
            //UInt8 rand = uRand(255);
            currentBf->InsertFighterInfo(_pack);  //Stream
            _pack << static_cast<UInt8>(ax) << static_cast<UInt8>(ay);

            UInt8 win = 0;
            UInt32 reportId = 0;
            UInt16 fightCostTime = 0 ;
            Fight(currentBf, target.bo, win, reportId,fightCostTime);

            std::cout<<"--------------------------------------------------------------------------------------------------------------------------------"<<endl;
            std::cout<<" the time from fight is " << static_cast<UInt32>(fightCostTime)<<endl;
            std::cout<<"--------------------------------------------------------------------------------------------------------------------------------"<<endl;
            timeCost += ceil(static_cast<float>(fightCostTime/100.0))+3;
            _pack << static_cast<UInt8>(timeCost);
            _pack << static_cast<UInt8>(1);
            std::cout<<" 此次战斗用时  "<< static_cast<UInt32>(timeCost)<<"秒"<<endl; 

            //currentBf->InsertFighterInfo(_pack);
            target.bo->InsertFighterInfo(_pack);
            

#if 0
            if( win == 1 || win == 0 )
            {
                //两边对死
                currentBf->setHP(100);
                (target.bo)->setHP(100);
                win = 2 ;
            }
#endif
            _pack << win << reportId;

            //cout
            std::cout<<" after fight the fighter hp info ..............................................................................."<<std::endl;
            TestCoutBattleS(target.bo);
            std::cout << std::endl;


            _oneRoundCostTime += timeCost;
            //增加击杀人数
            if( currentBf->GetOwner() != NULL )
            {
                currentBf->GetOwner()->AddKillFighterNum(currentBf->GetKillCount1());
                currentBf->GetOwner()->AddKillSoldiersNum(currentBf->GetKillCount2());
                std::cout<<" 杀死小兵的数量 "<<static_cast<UInt32>(currentBf->GetKillCount2())<<std::endl;
                currentBf->GetOwner()->AddKillFighter(currentBf->GetId(),_mapId,currentBf->GetKillCount1());
                currentBf->GetOwner()->AddKillSoldier(currentBf->GetId(),_mapId,currentBf->GetKillCount2());
            }
            if( (target.bo)->GetOwner() != NULL )
            {
                (target.bo)->GetOwner()->AddKillFighterNum((target.bo)->GetKillCount1());
                (target.bo)->GetOwner()->AddKillSoldiersNum((target.bo)->GetKillCount2());
                std::cout<<" 杀死小兵的数量 "<<static_cast<UInt32>((target.bo)->GetKillCount2())<<std::endl;
                (target.bo)->GetOwner()->AddKillFighter((target.bo)->GetId(),_mapId,(target.bo)->GetKillCount1());
                (target.bo)->GetOwner()->AddKillSoldier((target.bo)->GetId(),_mapId,(target.bo)->GetKillCount2());
            }

            //往排布那边同步战将数据
            //自己
            UInt8 cx = currentBf->GetGroundX();
            UInt8 cy = currentBf->GetGroundY();
            if( currentBf->getHP() <= 0 )
            {
                if( currentBf->GetOwner() != NULL )
                {
                    Battle::battleDistribute.RemoveFighter(_mapId,currentBf->GetOwner(),currentBf->GetId(),cx,cy);
                    UInt32 constantKill = (currentBf->GetOwner())->GetConstantlyKill(currentBf->GetId());
                    if( constantKill >= 1 )
                    {
                        if( (target.bo)->GetOwner() != NULL )
                        {
                            ((target.bo)->GetOwner())->AddEndConstantlyKill((target.bo)->GetId(),currentBf->GetOwner(),currentBf->GetId(),constantKill,true);
                        }
                    }
                    if( (target.bo)->GetOwner() != NULL )
                    {
                        ((target.bo)->GetOwner())->AddConstantlyKill((target.bo)->GetId(),1,true);
                    }
                    currentBf->GetOwner()->AddBeKilledFighterNum();
                    currentBf->GetOwner()->AddLoseInfo(currentBf->GetId(),(target.bo)->GetOwner(),(target.bo)->GetId());                  
                }
                else //npc
                {
                    if( _mapCamp[cx+cy*_x] != 5 )
                    {
                        Battle::battleDistribute.RemoveNpc(_id-_mapId,_mapId,x,y);
                    }
                }
            }
            else
            {
                if( _mapCamp[cx+cy*_x] != 5 )
                {
                    SyncHp(currentBf,cx,cy);
                }
            }
            //对手
            UInt8 bx = (target.bo)->GetGroundX();
            UInt8 by = (target.bo)->GetGroundY();
            if( (target.bo)->getHP() <= 0 )
            {

                if( (target.bo)->GetOwner() != NULL )
                {
                    Battle::battleDistribute.RemoveFighter(_mapId,(target.bo)->GetOwner(),(target.bo)->GetId(),bx,by);
                    UInt32 constantKill = ((target.bo)->GetOwner())->GetConstantlyKill((target.bo)->GetId());
                    if( constantKill >= 1 )
                    {
                        if( currentBf->GetOwner() != NULL )
                        {
                            (currentBf->GetOwner())->AddEndConstantlyKill(currentBf->GetId(),(target.bo)->GetOwner(),(target.bo)->GetId(),constantKill,true);
                        }
                    }
                    if( currentBf->GetOwner() != NULL )
                    {
                        (currentBf->GetOwner())->AddConstantlyKill(currentBf->GetId(),1,true);
                    }
                    (target.bo)->GetOwner()->AddBeKilledFighterNum();
                    (target.bo)->GetOwner()->AddLoseInfo((target.bo)->GetId(),currentBf->GetOwner(),currentBf->GetId());                  
                }
                else  //npc
                {
                    if( _mapCamp[bx+by*_x] != 5 )
                    {
                        Battle::battleDistribute.RemoveNpc(_id-_mapId,_mapId,bx,by);
                    }
                }

            }
            else
            {
                if( _mapCamp[bx+by*_x] != 5 )
                {
                    SyncHp((target.bo),bx,by);
                }
            }
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


    //获得行动力所能走到的范围
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



    void BattleGround::GetBackPosition(std::vector<Ascoord>& vecNearEnemy, std::vector<AttackInfo>& vecFinal)
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
               _vecTarget.push_back(TargetInfo(static_cast<BattleFighter*>(_mapFighters[(*it).x+(*it).y*_x]),pos,cost,pri));
            }
        }
        vecEnemy.clear();

    }

    void BattleGround::ShootBackAttack()
    {
        UInt8 x = currentBf->GetGroundX();
        UInt8 y = currentBf->GetGroundY();
        //Ascoord pos = Ascoord(x,y);
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
            for( auto it = vecFinal.begin() ; it != vecFinal.end(); ++it)
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
        //Ascoord pos = Ascoord(x,y);
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
    
     
    //弓箭兵呢  肯定选的是离敌人最远的点( 后退攻击 )
    void BattleGround::BowAnalyse(std::list<Ascoord> path,Ascoord target)
    {
        std::reverse(path.begin(),path.end());
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
                if( IsInAttackZone(p,target))
                {
                    if( _mapFighters[(*it).x+(*it).y*_x] == NULL || ( _mapFighters[(*it).x+(*it).y*_x] != NULL && _mapFighters[(*it).x+(*it).y*_x]->getHP() <= 0 ))
                    {
                        attack = p;
                        break;
                    }
                    else
                    {
                        return;
                    }
                }
                UInt8 dis = GetDistance(p,target);
                if( IsNearbyHaveEnemy(p) && dis > 0  )
                {
                    std::cout<<" 由于周围有敌人的干扰  行动力消耗+1 "<<std::endl;
                    cost+=1;
                }
            }
        }
        if( cost > currentBf->GetMovePower())
        {
            return;
        }
        else
        {
            UInt8 pri = priority[currentBf->getClass()-1][_mapFighters[target.x+target.y*_x]->getClass()-1];
            _vecTarget.push_back(TargetInfo(static_cast<BattleFighter*>(_mapFighters[target.x+target.y*_x]),attack,cost,pri));
        }
    }


    bool BattleGround::IsNearbyHaveEnemy(Ascoord& p)
    {
        std::vector<Ascoord> vecAscoord;
        GetAround(p,vecAscoord);
        CheckUp(vecAscoord);
        for( auto it = vecAscoord.begin(); it != vecAscoord.end(); ++it )
        {
           UInt8 x = (*it).x;  
           UInt8 y = (*it).y;
           if( _mapGround[x+y*_x] == 0 )
               continue;
           if( _mapFighters[x+y*_x] == NULL )
               continue;
           if( _mapFighters[x+y*_x]->GetSide() == currentBf->GetSide() )
               continue;
           if( _mapFighters[x+y*_x]->getHP() <= 0 )
               continue;
           if( _mapFighters[x+y*_x] == currentBf )
               continue;
           return true;
        }
        return false;
    }

    //按照这个路径走一下  获得总的一个行动力消耗 走到可攻击点就行了
    void BattleGround::Analyse(std::list<Ascoord> path,Ascoord& target)
    {
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
            std::cout<<" ||||||||||||||||||||||||||||||目标点  " << static_cast<UInt32>( target.x ) << "     " << static_cast<UInt32>( target.y) <<std::endl;
            for( auto it = path.begin(); it != path.end(); )
            {
                if( IsInAttackZone(*it,target))
                {
                    if( _mapFighters[(*it).x+(*it).y*_x] == NULL || ( _mapFighters[(*it).x+(*it).y*_x] != NULL && _mapFighters[(*it).x+(*it).y*_x]->getHP() <= 0 ))
                    {
                        attack = *it;
                        break;
                    }
                    else  //在可攻击的点还是有人 就return
                    {
                        return;
                    }
                }
                else
                {
                    ++it;
                    Ascoord p = *it;
                    cost+=GetRideSub(p.x,p.y);
                    std::cout<<" 在  "<<static_cast<UInt32>(p.x)<<"    " << static_cast<UInt32>( p.y ) << " 消耗行动力 " << static_cast<UInt32>(GetRideSub(p.x,p.y))<<std::endl;

                    UInt8 dis = GetDistance(p,target);
                    if( IsNearbyHaveEnemy(p) && dis > 0  )
                    {
                        std::cout<<" 由于周围有敌人的干扰  行动力消耗+1 "<<std::endl;
                        cost+=1;
                    }
                }
            }
        }
        UInt8 ride = GetRideSub(attack.x,attack.y);
        UInt8 movePower = currentBf->GetMovePower();
        if( cost > movePower )
        {
            //cost-movePower 为额外需要的行动力   
            //自身行动虽然不够但是  还是有剩余的
            UInt8 toReverseSecondCost = cost - ride ;   //  从起始点到倒数第二个点的行动力消耗
            //然后再跟自己本身就有的行动力做比较
            if( toReverseSecondCost >= movePower ) //到倒数第一个点的时候已经没有行动力剩余  直接return
            {
                return ;
            }
            else
            {
                if(IsInAround(attack,target))    //如果攻击点在目标点的附近
                {
                    UInt8 pri = priority[currentBf->getClass()-1][_mapFighters[target.x+target.y*_x]->getClass()-1];
                    _vecTarget.push_back(TargetInfo(static_cast<BattleFighter *>(_mapFighters[target.x+target.y*_x]),attack,cost,pri));
                }
                else
                {
                    return;
                }
            }
        }
        else
        {
            UInt8 pri = priority[currentBf->getClass()-1][_mapFighters[target.x+target.y*_x]->getClass()-1];
            _vecTarget.push_back(TargetInfo(static_cast<BattleFighter *>(_mapFighters[target.x+target.y*_x]),attack,cost,pri));
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
         UInt8 stype = currentBf->GetTypeId();
         UInt8 form = _mapGround[posx+posy*_x];
         UInt8 sub = RideSub[stype-1][form-1];
         return sub;
    }


     //1  草地

     //2  森林

     //3  城镇

     //4  山地  


    //地形和战斗背景的关系   依次为 草地 树林 城镇 山地
    static UInt8 land2FightGround[4]  = {1,2,4,3};

    void BattleGround::Fight(BattleFighter *bf , BattleFighter * bo, UInt8& result, UInt32& BattleReport,UInt16& fightTimeCost)
    { 
        //TODO   连接BattleSimulator
        //添加一个实际的攻击距离
        //UInt8 distance = GetFactAttackDis();
        
        UInt8 x = bf->GetGroundX();
        UInt8 y = bf->GetGroundY();

        UInt8 landform1 = _mapGround[x+y*_x];
        if( landform1 <= 0 )
            return ;
        UInt8 fightgroud1 = land2FightGround[landform1-1];

        UInt8 bx = bo->GetGroundX();
        UInt8 by = bo->GetGroundY();
        

        UInt8 landform2 = _mapGround[bx+by*_x];
        if( landform2 <= 0 )
            return ;
        UInt8 fightgroud2 = land2FightGround[landform2-1];
        /*
        if( IsInAround(Ascoord(x,y),Ascoord(bx,by)))
        {
            BattleSimulator bsim(bf,bo,distance,);
            bsim.start(); 
            result = bsim.GetWin();
            BattleReport = bsim.getId();
            std::cout << "发生战斗  " << static_cast<UInt32>(bf->GetBattleIndex()) << " VS " << static_cast<UInt32>(bo->GetBattleIndex()) << "  战斗结果: " << static_cast<UInt32>(result) <<" 战报ID:" << BattleReport << std::endl;
            fightTimeCost = bsim.GetTime();
        }
        else
        {
            Ascoord curPos = Ascoord(x,y);
            Ascoord bPos =  Ascoord(bx,by);
            distance = GetDistance(curPos,bPos);
            BattleSimulator bsim(bf,bo,distance+1,fightgroud1,fightgroud2);
            bsim.start(); 
            result = bsim.GetWin();
            BattleReport = bsim.getId();
            std::cout << "发生战斗  " << static_cast<UInt32>(bf->GetBattleIndex()) << " VS " << static_cast<UInt32>(bo->GetBattleIndex()) << "  战斗结果: " << static_cast<UInt32>(result) <<" 战报ID:" << BattleReport << std::endl;
            fightTimeCost = bsim.GetTime();
        }
        */

        Ascoord curPos = Ascoord(x,y);
        Ascoord bPos =  Ascoord(bx,by);
        UInt8 distance = GetDistance(curPos,bPos);
        BattleSimulator bsim(bf,bo,distance+1,fightgroud1,fightgroud2);
        bsim.start(); 
        result = bsim.GetWin();
        BattleReport = bsim.getId();
        std::cout << "发生战斗  " << static_cast<UInt32>(bf->GetBattleIndex()) << " VS " << static_cast<UInt32>(bo->GetBattleIndex()) << "  战斗结果: " << static_cast<UInt32>(result) <<" 战报ID:" << BattleReport << std::endl;
        fightTimeCost = bsim.GetTime();

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

        //设置主将及小兵的血量( 非npc )
        if( fgt->GetOwner() != NULL )
        {
            UInt32 roomId = _id-_mapId;
            Battle::DistributeInfo* info =Battle::battleDistribute.GetDistributeInfo(roomId,_mapId,x,y);
            UInt32 mainFighterHP = info->GetMainFighterHP();
            bf->setHP(mainFighterHP);
            std::vector<UInt32> vecHP = info->GetSoldiersHP();
            UInt8 index = 0;
            for( auto it = vecHP.begin(); it != vecHP.end(); ++it )
            {
                bf->SetSoldierHp(index,(*it));
                ++index;
            }
        }
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

    /*
    BattleFighter* BattleGround::GetMinBattleIndexFighter(std::vector<BattleFighter*> vecFighter)
    {
        UInt8 actId = 0xFF;
        BattleFighter* bf = NULL;
        for(auto it = vecFighter.begin(); it != vecFighter.end(); ++it )
        {
            if((*it) != NULL &&  (*it)->getHP() > 0 && (*it)->GetBattleIndex() < actId )
            {
                bf = (*it);
                actId = (*it)->GetBattleIndex();
            }
        }
        return bf;
    }
    */
    


    void BattleGround::SortByActId()
    {
        for( auto it = camp2fighters.begin(); it != camp2fighters.end(); ++it )
        {
            std::cout<<" 给阵营  "<<static_cast<UInt32>(it->first)<<" 按行动id 进行排序 "<<std::endl;
            std::list<BattleFighter*> listFighter = camp2fighters[it->first];
            LessActId<BattleFighter*> lessActId;
            listFighter.sort(lessActId);
            camp2fighters[it->first] = listFighter;
        }

        for( auto it = camp2fighters.begin(); it != camp2fighters.end(); ++it )
        {
            std::cout<<" 排序完成后  " << static_cast<UInt32>(it->first)<<std::endl;
            for( auto iter = (it->second).begin(); iter != (it->second).end(); ++iter )
            {
               std::cout<<" fighterInfo  " << " x : " <<(*iter)->GetGroundX()<<" y : "<<(*iter)->GetGroundY()<<" actId " <<(*iter)->GetBattleIndex()<<std::endl;
            }
            std::cout<<std::endl;
        }

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
        {
            if( ! _isSetCapture )
            {
                SetCaptureForce();
                _isSetCapture=true;
            }
            return;
        }
        TestCoutBattleS();
        //按照行动力的顺序给相应的阵营中的战将排序
        SortByActId();
        std::map<UInt8,std::list<BattleFighter*>> camp2fighters_copy = camp2fighters;
        
        _pack.init(0x80);
        _pack << static_cast<UInt8>(_mapId);
        MakePreStartInfo();
        UInt16 actCount = 0;
        size_t offset = _pack.size();
        _pack << actCount;

        while(  camp2fighters_copy.size() > 0 )
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
                   camp2fighters_copy.erase(it->first);
                   break;
                }
                else
                {
                    std::list<BattleFighter*> listFighter = camp2fighters_copy[it->first];
                    while( listFighter.front() == NULL || ( listFighter.front() != NULL && listFighter.front()->getHP() <= 0 ))
                    {
                        listFighter.pop_front();
                    }
                    if( listFighter.empty() )
                    {
                        camp2fighters_copy.erase(it->first);
                        break;
                    }
                    BattleFighter* bf =  listFighter.front();
                    if( bf == NULL )
                    {
                        break;
                    }
                    currentBf = bf;
                    Move();
                    listFighter.pop_front();
                    camp2fighters_copy[it->first] = listFighter;
                    ++actCount;
                }
            }
        }

        std::cout<<" ---------------------------------ERROR_------------------------------------------------------------"<<std::endl;
        ++_actId;
        camp2fighters_copy.clear();
        std::cout<<"战术回合"<<static_cast<UInt32>(_actId)<<"用时  "<<static_cast<UInt32>(_oneRoundCostTime)<<" 秒"<<std::endl;
        _pack.data<UInt16>(offset) = actCount;
        UInt32 now = TimeUtil::Now();
        _pack << static_cast<UInt32>(now);
        _pack<<Stream::eos;
        UInt32 battleId = IDGenerator::gBattleOidGenerator0.ID();
        std::cout<<" ****************************************************************该战术战报Id为  "<<static_cast<UInt32>(battleId)<<endl;
        battleReport0.addReport(/*_battleNum*/ battleId,_pack);
        report2IdTable.Insert(_id-_mapId,_mapId,_actId,/*_battleNum*/ battleId,now);
        DB7().PushUpdateData("REPLACE INTO `report2id` value(%u,%u,%u,%u,%u)",_id-_mapId,_mapId,_actId, /*_battleNum*/battleId,now);


        Stream st(0xA6);
        st<< static_cast<UInt8>(1);
        st<< static_cast<UInt8>(_mapId);
        st<< static_cast<UInt32>(battleId/*_battleNum*/);
        st<< Stream::eos;

        for( auto it = setPlayer.begin(); it != setPlayer.end(); ++it )
        {
            (*it)->send(st);
        }

        for( auto it = setPlayer.begin(); it != setPlayer.end(); ++it )
        {
            Stream st(REQ::CLAN_BATTLE_SELFINFO);
            (*it)->GetSelfBattleInfo(st);
            st<<Stream::eos;
            (*it)->send(st);
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
        if( camp2fighters.size() <= 1 )
            return true;
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
            size_t offset = _pack.size();
            UInt8 count = 0;
            _pack<<static_cast<UInt8>(count);
            for(auto iter = (it->second).begin(); iter != (it->second).end(); ++iter)
            {
                if( (*iter) != NULL && (*iter)->getHP() > 0 )
                {
                    ++count;
                    (*iter)->InsertFighterInfo(_pack,1);
                }
            }
            _pack.data<UInt8>(offset) = count;

        }

    }
    

    void BattleGround::start()
    {
        if( map2fighter.size() < 2 )  //只有一个阵营的话  就不开打
            return;
        preStart();
        TestCoutBattleS();
        SortByActId();
        std::map<UInt8,std::list<BattleFighter*>> camp2fighters_copy = camp2fighters;
        _pack.init(0x80);
        _pack << static_cast<UInt8>(_mapId);
        MakePreStartInfo();
        UInt16 actCount = 0;
        size_t offset = _pack.size();
        _pack << actCount;
        do
        {
            while(  camp2fighters_copy.size() > 0 )
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
                        camp2fighters_copy.erase(it->first);
                        break;
                    }
                    else
                    {
                        std::list<BattleFighter*> listFighter = camp2fighters_copy[it->first];
                        while( listFighter.front() != NULL && listFighter.front()->getHP() > 0 )
                        {
                            listFighter.pop_front();
                        }
                        if( listFighter.empty() )
                        {
                            break;
                        }
                        BattleFighter* bf =  listFighter.front();
                        if( bf == NULL )
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
        }while( CheckIsStop() );
        camp2fighters_copy.clear();
        std::cout<<"战术回合"<<static_cast<UInt32>(_actId)<<"用时  "<<static_cast<UInt32>(_oneRoundCostTime)<<" 秒"<<std::endl;
        _pack.data<UInt16>(offset) = actCount;
        UInt32 now = TimeUtil::Now();
        _pack << static_cast<UInt32>(now);
        _pack<<Stream::eos;
        std::cout<<" ****************************************************************该战术战报Id为  "<<static_cast<UInt32>(_battleNum)<<endl;
        battleReport0.addReport(_battleNum,_pack);
    }

    //战将进入战场
    void BattleGround::preStart()  //需要玩家手动操作
    { 
        /*
        if( map2fighter.size() <= 1 )
            return;
        */
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
                GObject::Fighter* fgt = NULL;
                if( owner == NULL )
                {
                    fgt = GObject::globalFighters[fighterId];
                }
                else
                {
                    fgt = owner->findFighter(fighterId);
                }
                if( fgt == NULL )
                    continue;
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
        //Ascoord start = _start;
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

        setPlayer.insert(player);
    }

    void BattleGround::PushNpc(UInt8 forceId,UInt16 fighterId,UInt8 x,UInt8 y)
    {
        map2fighter[forceId].push_back(new FighterInfo(NULL,fighterId,x,y));
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
        std::vector<UInt8> vecCamp = mapInfo->GetActCamp();
        UInt8 forceNum =  vecCamp.size(); //mapInfo->GetCampNum();  //获得阵营的数量
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
        ++_maxID;
        return _maxID;
    }

    void BattleGround::GetAliveForce(std::vector<UInt8> &vecForce)
    {
        if( camp2fighters.empty() )
            return;
        for( auto it = camp2fighters.begin(); it != camp2fighters.end(); ++it )
        {
            bool res = SomeCampIsAllDie(it->first);
            if( !res )
            {
                vecForce.push_back(it->first);
            }
        }

    }

    //设置某一城市的占领方
    UInt8 BattleGround::GetCaptureId()
    {
        std::vector<UInt8> vecAliveForce;
        GetAliveForce(vecAliveForce);
        if( vecAliveForce.empty() )  //都死光了
        {
            return 0;
        }
        else if( vecAliveForce.size() == 1 )
        {
            return vecAliveForce.back();
        }
        else
        {
            return 0;
        }
    }

    void BattleGround::SetCaptureForce()
    {
        if( camp2fighters.empty() )
            return;
        //先获得城市的原来拥有者
        UInt32 roomId = _id-_mapId;
        Battle::RoomAllCityStatus* status = Battle::roomAllCityStatusManager.GetRoomAllCityStatus(roomId);
        if( status == NULL )
            return;
        //UInt8 ownForce = status->GetCityOwnForce(_mapId);
        UInt8 captureForce = GetCaptureId();
        if( captureForce != 0 )
        {
            std::cout<<"现在"<<static_cast<UInt32>(_mapId)<<" 这座城属于势力   " <<static_cast<UInt32>(captureForce)<<endl;
            Battle::roomAllCityStatusManager.SetOwnForce(roomId,_mapId,captureForce);
            //DB7().PushUpdateData("UPDATE  `clan_battle_citystatus`  set ownforce=%u  where roomId = %u and battleId= %u and cityId = %u ", captureForce, roomId,status->GetBattleId(),_mapId);
            DB7().PushUpdateData("REPLACE INTO `clan_battle_citystatus`(`roomId`,`battleId`,`cityId`,`ownforce`)  values(%u, %u, %u, %u)",roomId,status->GetBattleId(),_mapId,captureForce);

        }
    }

    void BattleGround::AutoEnterFighters(UInt8 index, GObject::Player* pl)
    { 
        static UInt8 map2Point[][7][2]= {
            {{1,0},{2,0},{0,1},{1,1},{2,1},{2,0},{2,1}},
            {{3,0},{4,0},{2,1},{3,1},{4,1},{3,2},{4,2}}
        };
        if( index == 0 ) // 1 2 
            return;
        std::map<UInt8, UInt16> _map ;
        if(index - 1)
            _map= pl->GetArenaLayout();
        else
            _map= pl->GetArenaDefendLayout();
        for(auto it = _map.begin(); it != _map.end(); ++it)
        { 
            map2fighter[index].push_back(new FighterInfo(pl,it->second,map2Point[index -1][it->first][0],map2Point[index-1][it->first][1]));
        } 
        setPlayer.insert(pl);
    } 
}
