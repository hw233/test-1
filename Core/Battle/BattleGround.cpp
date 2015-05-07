#include "BattleGround.h"
#include "BattleFighter.h" 
#include "GObject/Player.h"
#include "Battle/BattleSimulator.h"
#include "Battle/BattleReport.h"
#include "Common/StringTokenizer.h"
#include "GData/Map.h"
#include "GObject/Country.h"
#include "Script/GameActionLua.h"

#define MAX(x,y) x>y?x:y
#define ABS(x,y) x>y?x-y:y-x

namespace Battle
{
    void BattleGround::InitMapFight(UInt8 mapId)   
    { 

       //设置地图信息
       vecInfo MapInfo = GData::map.GetMap(mapId);
       if( MapInfo.empty() || MapInfo.size() <= 2 )
           return;
        _y = MapInfo[0];  // 3
        _x = MapInfo[1];  // 5
        _mapGround = new UInt8[_x*_y];
        memset(_mapGround , 0 ,_x*_y*sizeof(UInt8));
        for( UInt8 j = 2 ; j < MapInfo.size() ; ++j)
        {
            _mapGround[j-2] = MapInfo[j];
        }

        //设置地图上的阵营信息(跟上面的地图是对应的)
        vecInfo CampInfo = GData::map.GetCamp(mapId);
        _mapCamp = new UInt8[_x*_y];
        memset(_mapCamp , 0 ,_x*_y*sizeof(UInt8));
        
        for( UInt8 j = 2 ; j < CampInfo.size() ; ++j )
        {
            _mapCamp[j-2] = CampInfo[j];
        }
        _mapFighters = new BattleObject* [_x*_y];
        _mapFlag = new UInt8[_x*_y];
        memset(_mapFighters,0,sizeof(BattleObject*)*_x*_y);
        memset(_mapFlag,0,sizeof(UInt8)*_x*_y);
        _pack.init(0x80);
        return ;
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

    UInt8 BattleGround::GetDirection(UInt8 x, UInt8 y, UInt8 cx, UInt8 cy)
    {
        //
        //  1         2
        //
        //  
        //  3         4
        UInt8 direction = 0;
        if( x > cx && y > cy )          //中心点的右下方
        {
            direction = 4;
        }
        else if ( x > cx && y < cy )    //中心点的右上方
        {
            direction = 2;
        }
        else if ( x < cx && y < cy )     //中心点的左上方
        {
            direction = 1;
        }
        else if ( x < cx && y > cy )     //中心点的左下方
        {
            direction = 3;
        }
        else
        {
            direction = 0;               //本身就是中心点
        }
        return direction;
    }

    //根据自身方向  确定要移动至哪一个目标点
    void BattleGround::GetMoveTargetByDir(UInt8 d, Ascoord& move)
    {
       if( d < 0 || d > 4 )
           return;
       Ascoord target;
       UInt8 x = currentBf->GetGroundX();
       UInt8 y = currentBf->GetGroundY();
       switch(d)
       {
           case 0:
               //中心点
               move = Ascoord(x,y);
               return;
           case 1:    //向4方向移动
               target = Ascoord(_x-1,_y-1);
               break;
           case 2:    //向3方向移动
               target = Ascoord(0,_y-1);
               break;
           case 3:    //向2方向移动
               target = Ascoord(_x-1,0);
               break;
           case 4:    //向1方向移动
               target = Ascoord(0,0);
               break;
           default:
               break;
       }
       SetStart(Ascoord(x,y));
       SetEnd(target);
       ComputeRoute();
       std::list<Ascoord>path;
       GetRoute(path);
       MoveAnalyse(path,target,move);
    }

    void BattleGround::MoveAnalyse(std::list<Ascoord> path, Ascoord& target, Ascoord& last)
    {
        //只走一个行动力
        UInt8 ride = currentBf->GetMaxRide();
        UInt8 cost = 0;
        if( path.size() < 2 )
        {
            return;
        }
        else if ( path.size() == 2 )
        {
            if( _mapGround[target.x+target.y+_x] == 0 )
            {
                last = _start;
            }
            else
            {
                last = target;
            }
        }
        else
        {
            auto begin = path.begin();
            begin++;
            auto end = path.end();
            if( _mapGround[target.x+target.y+_x] == 0 )
            {
                --end;
            }
            for( auto it = begin; it != end ;++it )
            {
                if( cost > ride )
                    break;
                last = *it;
                cost+=GetRideSub(last.x,last.y);
            }

        }

    }

    void BattleGround::Move()
    {
        //选择对象
        if(!currentBf)
            return ;
        //写入当前战将信息
        GetTarget(currentBf->GetGroundX(),currentBf->GetGroundY(),currentBf->GetMaxRide());
        if( _vecTarget.empty() )
        {

            UInt8 x = currentBf->GetGroundX();
            UInt8 y = currentBf->GetGroundY();

            UInt8 cx = _x/2;
            UInt8 cy = _y/2;
           

            //如果周围没有敌人要沿对角线前进至敌方阵营
            UInt8 dir = GetDirection(x,y,cx,cy);
            Ascoord move;
            GetMoveTargetByDir(dir,move);

            currentBf->SetGroundX(move.x);
            currentBf->SetGroundY(move.y);

            std::cout << "战将编号:      "  << static_cast<UInt32>(currentBf->GetBattleIndex());
            std::cout << "战将编号:      "  << static_cast<UInt32>(currentBf->GetBattleIndex());
            std::cout <<" 无方案" << ":移动到 "<<static_cast<UInt32>(move.x)<< "," << static_cast<UInt32>(move.y) <<std::endl;
            currentBf->InsertFighterInfo(_pack);  //Stream

            _pack << static_cast<UInt8>(move.x) << static_cast<UInt8>(move.y) << static_cast<UInt8>(0); //无战斗发生
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
            currentBf->SetGroundX(target.attack.x);
            currentBf->SetGroundY(target.attack.y);

            currentBf->InsertFighterInfo(_pack);  //Stream
            _pack << static_cast<UInt8>(target.goal.x) << static_cast<UInt8>(target.goal.y);
            _pack << static_cast<UInt8>(1);

            //currentBf->InsertFighterInfo(_pack);
            target.bo->InsertFighterInfo(_pack);

            UInt8 win = 0;
            UInt32 reportId = 0;
            Fight(currentBf, target.bo, win, reportId);
            _pack << win << reportId;

            //cout
            TestCoutBattleS(target.bo);
            std::cout << std::endl;
        }
        memset(_mapFlag,0,_x*_y*sizeof(UInt8));
        currentBf = NULL;
    }

    void BattleGround::GetNearEnemy(UInt8 x,UInt8 y,std::vector<Ascoord>& vecEnemy)
    {
        UInt8 attackDis = currentBf->GetDistance();   //获得攻击距离
        UInt8 ride = currentBf->GetMaxRide();
        UInt8 range = attackDis+ride;
        UInt8 side  = currentBf->GetSide();

        for(UInt8 j = y > range ? y - range : 0 ; y < ( y + range  > _y ? _y : y + range ) ; ++j)
        {
            if( j > _y )
                break;
            for( UInt8 i = x > range ? x - range : 0 ; i < ( x + range > _x ? _x : x +range) ; ++i)
            {
                if( i > _x )
                    break;
                if( _mapGround[i+j*_x] == 0 )      //0代表不可放置的位置
                    continue;
                if( _mapFighters[i+j*_x] == NULL )
                    continue;
                if( _mapFighters[i+j*_x]->getClass() > 100)
                    continue;
                if( _mapFighters[i+j*_x]->GetSide() == side )  //属于同一个阵营  
                    continue;
                if( _mapFighters[i+j*_x]->getHP() < 0 )    //已经死了
                    continue;
                if( _mapFighters[i+j*_x] == currentBf )     //不是自己
                    continue;
                vecEnemy.push_back(Ascoord(i,j));
            }
        }
    }

    bool BattleGround::IsInAttackZone(Ascoord stand, Ascoord target)   //站在某一点是不是可以攻击到目标呢
    {
        UInt8 attackDis = currentBf->GetDistance();
        UInt8 dis = (stand.x-target.x)*(stand.x-target.x) + (stand.y-target.y)*(stand.y-target.y);
        if( dis < attackDis*attackDis )
        {
            return true;
        }
        return false;
    }

    bool BattleGround::EnemyCanAttack(Ascoord stand, Ascoord target)
    {
        UInt8 x = stand.x;
        UInt8 y = stand.y;
        if( _mapFighters[x+y*_x] == NULL)
            return false;
        UInt8 attackDis = static_cast<BattleFighter*>(_mapFighters[x+y*_x])->GetDistance();
        UInt8 dis = (stand.x-target.x)*(stand.x-target.x) + (stand.y-target.y)*(stand.y-target.y);
        if( dis < attackDis*attackDis )
        {
            return true;
        }
        return false;

    }

    //获得与弓箭兵相邻的敌人数组
    void BattleGround::GetNearbyEnemy(UInt8 x,UInt8 y,std::vector<Ascoord>& vecAscoord)
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
        UInt8 ride = currentBf->GetMaxRide();
        UInt8 x = currentBf->GetGroundX();
        UInt8 y = currentBf->GetGroundY();
        for(UInt8 j = ( y > ride ? y -ride : 0 ) ; j < ( y + ride > _y ? _y : y + ride); ++j)
        {
            if( j > _y )
               break;
            for(UInt8 i = (i > ride ? i -ride : 0 ) ; i < ( i +ride > _x ? _x : x+ride); ++i )
            {
                if( i > _x )
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
            if( currentBf->GetClass() == 3 )
            {
                //Shoot Man back     //弓箭兵后退
                std::vector<Ascoord> vecNearEnemy;
                GetNearbyEnemy(x,y,vecNearEnemy);
                if( vecNearEnemy.empty() || vecNearEnemy.size() == 6 )
                {
                    //不需要后退  因为周边没有敌人  或者被围攻
                }
                else
                {
                    //确定后退到哪一个点
                    std::vector<AttackInfo> vecFinal;   //最终确立的那个要后退的一系列点
                    GetBackPosition(vecNearEnemy,vecFinal);
                    if( vecFinal.empty())
                    {
                        //找了半天  敌人还是可以打到 
                    }
                    else
                    {
                        //然后找出来  
                        SetStart(Ascoord(x,y));

                        for( auto it = vecFinal.begin() ; it != vecFinal.begin(); ++it)
                        {
                            SetEnd((*it).attack);
                            ComputeRoute();
                            std::list<Ascoord>path;
                            GetRoute(path);
                            std::reverse(path.begin(),path.end());
                            BowAnalyse(path,(*it).target);
                        }
                    }
                }
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
        static UInt8 priority [4][4] = {
                          {1,2,3,4},
                          {1,2,3,4},
                          {1,2,3,4},
                          {1,2,3,4}
                      };
        //按照这个路径走一下  获得总的一个行动力消耗 走到可攻击点就行了
        UInt8 cost = 0;
        Ascoord attack;
        UInt8 x = currentBf->GetGroundX();
        UInt8 y = currentBf->GetGroundY();
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
        if( cost > currentBf->GetMaxRide())
        {
            return;
        }
        else
        {
            UInt8 pri = priority[currentBf->getClass()-1][_mapFighters[target.x+target.y*_x]->getClass()-1];
            _vecTarget.push_back(TargetInfo(static_cast<BattleFighter*>(_mapFighters[x+y*_x]),attack,target,cost,pri));
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
        UInt8 x = currentBf->GetGroundX();
        UInt8 y = currentBf->GetGroundY();
        std::reverse(path.begin(),path.end());
        if( path.size() < 2 )
        {
            return ;
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
        if( cost > currentBf->GetMaxRide())
        {
            return;
        }
        else
        {
            UInt8 pri = priority[currentBf->getClass()-1][_mapFighters[target.x+target.y*_x]->getClass()-1];
            _vecTarget.push_back(TargetInfo(static_cast<BattleFighter *>(_mapFighters[x+y*_x]),attack,target,cost,pri));
        }

    }

    UInt8 BattleGround::GetRideSub(const UInt8& posx ,const UInt8& posy)
    { 
         //TODO  返回消耗 (考虑周围敌军情况)
         UInt8 form = _mapGround[posx+posy*_x];
         UInt8 rideSub = GameAction()->GetRide(form); 
         return rideSub;
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


    BattleFighter* BattleGround::newFighter(UInt8 x,UInt8 y ,GObject::Fighter * fgt)
    { 
        BattleFighter * bf = BattleSimulator::CreateFighter(fgt->GetTypeId(),NULL,fgt, x, y);
        setObject(x, y, bf ,1);
        bf->SetEnterPos(x,y);
        bf->SetBattleIndex(++_maxID);
        bf->InsertFighterInfo(_pack,1);
        //std::cout << "入场战将编号 : " << static_cast<UInt32>(bf->GetBattleIndex()) << std::endl;
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

    void BattleGround::start()
    { 
        preStart();  //
        TestCoutBattleS();
        UInt32 alive = 0;
        UInt8 count = 0;
        UInt32 Round = 0;
        UInt16 actCount = 0;
        size_t offset = _pack.size();
        _pack << actCount ;
        do
        {
            for(UInt8 i = 0; i < PLAYERMAX; ++i)  //PLAYERMAX 表示军团数量
            { 
                if(fighters[i].size() == 0)
                    continue;

                bool flag = false;
                for(UInt8 j = 0; j < fighters[i].size();++j)
                { 
                    if(fighters[i][j] && fighters[i][j]->getHP())
                    {
                        flag = true ;
                        currentBf = fighters[i][j];
                        Move();
                        ++actCount;
                    }
                } 
                if(!flag && !(alive &(1 <<i)) )
                { 
                    alive |= (1 <<i);
                    ++count;
                } 
            } 
            ++Round;
            std::cout << "回合：" << static_cast<UInt32>(Round) << std::endl;
        }while(count < (map_player.size() - 1) && Round < 200);

        _pack.data<UInt16>(offset) = actCount;
        _pack<<Stream::eos;
        battleReport0.addReport(_battleNum,_pack);
    }


    void BattleGround::PushPlayer(GObject::Player * pl,UInt8 index, UInt8 flag)
    { 
        if(map_player.size() >= PLAYERMAX)
            return ;
        UInt8 side = 2 - flag ;
        map_player[side].push_back( pl);
        pl->SetBattleId(_id);
        pl->SetBattleSide(side);
    } 


    std::vector<UInt8> BattleGround::GetSameCamp(UInt8 side)
    {
        std::vector<UInt8> vecCamp;
        for( UInt8 i = 0 ; i < _x*_y ; ++i)
        {
            if( _mapCamp[i] == side )
            {
                vecCamp.push_back(i);
            }
        }
        return vecCamp;
    }

    //战将进入战场
    void BattleGround::preStart()  //需要玩家手动操作
    { 
        std::map<UInt8 ,std::vector<GObject::Player *> >::iterator it = map_player.begin();
        _pack << static_cast<UInt8>(1);
        _pack << static_cast<UInt8>(map_player.size());
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

    UInt8 BattleGround::GetFactAttackDis()
    {
        return 1;
    }

    bool BattleGround::IsObstract(const Ascoord &p)
    {
        /*
        UInt8 x = p.x;
        UInt8 y = p.y;
        bool ret = false;

        if( _mapGround[x+y*_x] == 0 )
        {
            ret = true;
        }
        if( _mapFighters[x+y*_x] != NULL && _mapFighters[x+y*_x]->GetSide() != currentBf->GetSide() && _mapFighters[x+y*_x]->getHP()> 0 )
        {
            ret = true;
        }
        */
        return false;
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
        return abs(p.x-_end.x)+abs(p.y-_end.y);
    }

    UInt8 BattleGround::GetGValue()
    {
        return 1;
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
                if( JudgeSurround((*it),p,node.g+1))
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


}
