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
    //UInt8 scopeForOne[12] = {-2,0, -1,2, 1,-2, 2,0, 1,-2, -1,-2};
    //static UInt8 getDistanceForTwo(UInt8 x ,UInt8 y, UInt8 x1 , UInt8 y1)
    //{
    //    return MAX((ABS(x,x1)),(ABS(y,y1)));
    //}
     
    /*
    void BattleGround::InitMapFight(UInt8 mapId)   
    { 
        static UInt8 point[][2] = {
            {3, 1}, 
            {5, 1}, 
            {7, 1}, 
            {2, 2}, 
            {4, 2}, 
            {6, 2}, 
            {8, 2}, 
            {3, 3}, 
            {5, 3}, 
            {7, 3}, 
            {2, 4}, 
            {4, 4}, 
            {6, 4}, 
            {8, 4}, 
        };


        std::vector<std::string > vec = GData::map.GetMapInfo(mapId);
        UInt8 flag = 0;
        _y = vec.size();
        if(_y)
        { 
            StringTokenizer st(vec[0],",");
            _x = st.count()*2 + 1;
            _y = 10;
            _x = 20;
        } 
        _astar.SetMapSize(_x,_y);

        _mapGround = new UInt8[_x*_y];
        memset(_mapGround , 0 ,_x*_y*sizeof(UInt8));
        
        for(UInt8 i = 0; i < vec.size(); ++i)
        { 
            flag = !flag;
            StringTokenizer st(vec[i],",");
            for(UInt8 j = 0; j < st.count(); ++j)
            { 
                UInt8 value = static_cast<UInt8>(::atoi(st[j].c_str()));
                _mapGround[j*2+flag + i*_x] = value;
                std::cout << "坐标：" << static_cast<UInt32>(j*2+flag)<<" , " << static_cast<UInt32>(i) << "地形" << static_cast<UInt32>(value)<<std::endl;
            } 
        } 

        for(UInt8 i = 0; i < 15 ; ++i)
        {
            if(point[i][0] + point[i][1]*_x >= _x*_y)
                continue;
           _mapGround[point[i][0] + point[i][1]*_x] = 1;
        }
        _mapFighters = new BattleObject* [_x*_y];
        _mapFlag = new UInt8[_x*_y];
        memset(_mapFighters,0,sizeof(BattleObject*)*_x*_y);
        memset(_mapFlag,0,sizeof(UInt8)*_x*_y);
        _pack.init(0x81);
        _astar.SetMap(_mapGround);
        _astar.SetMapSize(_x,_y);
        return ;

    }
    */


    void BattleGround::InitMapFight(UInt8 mapId)   
    { 

       //设置地图信息
        GData::map.loadMapInfo(mapId);
        vecInfo MapInfo = GData::map.GetMapInfo();
        _y = MapInfo.size();
        if( _y <= 0 )
            return;
        _x = MapInfo[0].size();
        _mapGround = new UInt8[_x*_y];
        memset(_mapGround , 0 ,_x*_y*sizeof(UInt8));

        for(UInt8 i = 0 ; i < _x  ; ++i )
        {
            for( UInt8 j =0 ; j < _y ; ++j)
            {
               _mapGround[i+j*_x] = MapInfo[i][j];
               std::cout<<"坐标:"<< "x :" << i << "y :"<<  j << "地形" << MapInfo[i][j];

            }
            std::cout<<std::endl;
        }
        _astar.SetMapSize(_y,_x);
        _astar.SetMap(_mapGround);

        //设置地图上的阵营信息(跟上面的地图是对应的)
        GData::map.loadCampInfo(mapId);
        vecInfo CampInfo = GData::map.GetCampInfo();
        _mapCamp = new UInt8[_x*_y];
        memset(_mapCamp , 0 ,_x*_y*sizeof(UInt8));

        for( UInt8 i = 0 ; i < _x ; ++i )
        {
            for( UInt8 j = 0 ; j < _y ; ++j )
            {

                _mapCamp[i+j*_x] = CampInfo[i][j];
                std::cout<<"坐标:" << "x : " << i << "y :" << j << "属于势力" << CampInfo[i][j];
            }
            std::cout<<std::endl;
        }

        _mapFighters = new BattleObject* [_x*_y];
        //_mapFlag = new UInt8[_x*_y];
        memset(_mapFighters,0,sizeof(BattleObject*)*_x*_y);
        //memset(_mapFlag,0,sizeof(UInt8)*_x*_y);
        _pack.init(0x80);
        return ;
    }



    /*
    void BattleGround::Move()
    { 
        //选择对象
        static UInt8 priority [4][4] = {
            {1,2,3,4},
            {1,2,3,4},
            {1,2,3,4},
            {1,2,3,4}
        };
        if(!currentBf)
            return ;
        //写入当前战将信息

        GetNearPos(currentBf->GetRide(),currentBf->GetGroundX(),currentBf->GetGroundY());
        if(targetVec.size() == 0)
        {
            //TODO  按照原定计划行动
            UInt8 resx = 0;
            UInt8 resy = 0;
            UInt8 nowx = currentBf->GetGroundX();
            UInt8 nowy = currentBf->GetGroundY();
            UInt8 ride  = currentBf->GetRide() ;
            UInt8 destx = _x/2;// - currentBf->GetEnterPosX();;
            UInt8 desty = _y/2;// - currentBf->GetEnterPosY();
            UInt8 minDistance = -1 ;

            for(UInt8 j = (nowy > ride)?(nowy - ride):0; j < _y && j < (nowy + ride) ; ++j)
            {
                for(UInt8 i = (nowx > 2*ride)?(nowx - 2*ride):0; i < _x && j < (nowx + 2*ride) ; ++i)
                {
                    if(!_mapGround[i+j*_x])
                        continue ;
                    if(!_mapFlag[i+j*_x])
                        continue ;
                    UInt8 distancePre = getDistanceForTwo(i,j,destx,desty);
                    if(distancePre < minDistance)
                    {
                        resx = i;
                        resy = j;
                        minDistance = distancePre;
                    }
                }
            }
            if(currentBf->GetGroundX() == resx && currentBf->GetGroundY() == resy)
            {
                std::cout << "战将编号"  << static_cast<UInt32>(currentBf->GetBattleIndex()) << " 失去目标" << static_cast<UInt32>(resx) << " , " << static_cast<UInt32>(resy) << std::endl;
                return ;
            }

            currentBf->SetGroundX(resx);
            currentBf->SetGroundY(resy);

            std::cout << "战将编号:"  << static_cast<UInt32>(currentBf->GetBattleIndex());
            std::cout <<" 无方案" << ":移动到 "<<static_cast<UInt32>(resx)<< "," << static_cast<UInt32>(resy) <<std::endl;
            currentBf->InsertFighterInfo(_pack);  //Stream
            _pack << static_cast<UInt8>(resx) << static_cast<UInt8>(resy) << static_cast<UInt8>(0); //无战斗发生
        }
        else
        {
            UInt8 res = 0;
            UInt8 max = 0;
            UInt8 maxD = 0;
            std::cout << std::endl;
            TestCoutBattleS(currentBf);
            std::cout << "可有选择:" << std::endl;
            for(UInt8 i = 0 ; i < targetVec.size() ; ++i)
            {
                UInt8 cur = priority[currentBf->getClass()-1][targetVec[i].bo->getClass()-1] ;
                //克制关系
                std::cout <<"方案:"<< static_cast<UInt32>(i) << "  地点 :"  << static_cast<UInt32>(targetVec[i].x) <<"  "<< static_cast<UInt32>(targetVec[i].y) <<" 距离:"<< static_cast<UInt32>(targetVec[i].step) << " 权值："<< static_cast<UInt32>(cur) <<std::endl;

                if(cur >= max)
                {
                    //TODO 根据距离判断选择哪一个位置
                    if(cur == max && targetVec[i].step < maxD)
                        continue ;
                    {
                        res = i;
                        max = cur;
                        maxD = targetVec[i].step;
                        std::cout << "maxDistance：" << static_cast<UInt32>(maxD) << std::endl;
                    }
                }
            }
            //战斗
            currentBf->SetGroundX(targetVec[res].x);
            currentBf->SetGroundY(targetVec[res].y);

            std::cout <<"选择方案"<< static_cast<UInt32>(res) << ":移动到"<<static_cast<UInt32>(targetVec[res].x)<< "," << static_cast<UInt32>(targetVec[res].y) <<std::endl;
            std::cout << "攻击" <<std::endl;


            //Stream
            currentBf->InsertFighterInfo(_pack);  //Stream
            _pack << static_cast<UInt8>(targetVec[res].x) << static_cast<UInt8>(targetVec[res].y);
            _pack << static_cast<UInt8>(1);

            //currentBf->InsertFighterInfo(_pack);
            targetVec[res].bo->InsertFighterInfo(_pack);

            UInt8 win = 0;
            UInt32 reportId = 0;
            Fight(currentBf, targetVec[res].bo, win, reportId);
            _pack << win << reportId;

            //cout
            TestCoutBattleS(targetVec[res].bo);
            std::cout << std::endl;
        }
        memset(_mapFlag,0,_x*_y*sizeof(UInt8));
        targetVec.clear();
        currentBf = NULL;
    } 
    */


    bool MoreFit(TargetInfo info1, TargetInfo info2)
    {
        if( info1.pri > info2.pri )
            return true;
        else if( info1.size < info2.size)
            return true;
        else
            return false;

    }


    void BattleGround::MoveToCenter(UInt8 direction)
    {
        if( direction <= 0 || direction >= 5 )
        {
            return ;
        }

        UInt8 x = currentBf->GetGroundX();
        UInt8 y = currentBf->GetGroundY();
        //       1                2
        //
        //
        //               x
        //
        //       3                4
        //
        UInt8 ride = currentBf->GetMaxRide();
        switch(direction)
        {
            case 1:         //向4这个方向运行
                for(UInt8 i = x ; i < (x + ride > _x  ? _x : x + ride);++i)
                {
                    for(UInt8 j = y ; j < ( y + ride > _y ? _y : y + ride); ++j)
                    {
                        if( ride <= 0 )
                        {
                            break;
                        }
                        if( _mapGround[i+j*_x] == 0 )
                            continue;
                        if( _mapFighters[i+j*_x] != NULL && _mapFighters[i+j*_x]->getHP() < 0 )  //如果是自己人可以穿越
                            continue;
                    }
                }
                break;
            case 2:         //向3这个方向运行
                break;
            case 3:         //向2这个方向运行
                break;
            case 4:         //向1这个方向运行
                break;
        }

    }


    void BattleGround::Move()
    {

        //选择对象
        if(!currentBf)
            return ;
        //写入当前战将信息
        //GetTarget(currentBf->GetMaxRide(),currentBf->GetGroundX(),currentBf->GetGroundY());
        GetBestTarget(currentBf->GetGroundX(),currentBf->GetGroundY());
        if( _vecTarget.empty() )
        {

            UInt8 x = currentBf->GetGroundX();
            UInt8 y = currentBf->GetGroundY();

            UInt8 centerx = _x/2;
            UInt8 centery = _y/2;
           
            UInt8 ride = currentBf->GetMaxRide();
            UInt8 direction = 0;
            //
            //
            //     1                 2
            //             x
            //     3                 4
            //
            //
            if( x > centerx && y > centery )
            {
                direction = 4;
            }
            else if( x > centerx && y < centery )
            {
                direction = 2;
            }
            else if( x < centerx && y < centery )
            {
                direction = 1;
            }
            else if( x < centerx && y < centery )
            {
                direction = 3;
            }
            else
            {
                direction = 5;
            }
            //往中心点移动
            MoveToCenter(direction);

            for(UInt8 i = ( x > ride ? (x -ride) : 0 ) ; i < ( x + ride > _x ? _x : x + ride ) ; ++i )
            {
                for(UInt8 j = ( y > ride ? y - ride : 0 ) ; j < ( y + ride > _y ? _y : y + ride ) ; ++j)
                {
                    if( !_mapGround[i+j*_x] )
                        continue;
                }
            }
            //if( currentBf->GetGroundX() == resx && currentBf->GetGroundY() == resy )
            //{
              //  std::cout << "战将编号"  << static_cast<UInt32>(currentBf->GetBattleIndex()) << " 失去目标" << static_cast<UInt32>(resx) << " , " << static_cast<UInt32>(resy) << std::endl;
                //return ;
            //}

            //currentBf->SetGroundX(resx);
            //currentBf->SetGroundY(resy);

            //std::cout << "战将编号:      "  << static_cast<UInt32>(currentBf->GetBattleIndex());
            //std::cout << "战将编号:      "  << static_cast<UInt32>(currentBf->GetBattleIndex());
            //std::cout <<" 无方案" << ":移动到 "<<static_cast<UInt32>(resx)<< "," << static_cast<UInt32>(resy) <<std::endl;
            //currentBf->InsertFighterInfo(_pack);  //Stream

            //_pack << static_cast<UInt8>(resx) << static_cast<UInt8>(resy) << static_cast<UInt8>(0); //无战斗发生
        }
        else
        {
            std::sort(_vecTarget.begin(),_vecTarget.end(),MoreFit);
            TargetInfo target = _vecTarget.front();
            std::cout << std::endl;
            TestCoutBattleS(currentBf);
            //战斗
            std::cout <<"从     "<<static_cast<UInt32>(currentBf->GetGroundX())<<" , "<< static_cast<UInt32>(currentBf->GetGroundY())<<std::endl;
            std::cout <<"移动到  "<<static_cast<UInt32>(target.stand._x)<< "," << static_cast<UInt32>(target.stand._y) <<std::endl;
            std::cout << "攻击目标  " <<static_cast<UInt32>(target.goal._x)<<","<<static_cast<UInt32>(target.goal._y)<<std::endl;

            //Stream
            currentBf->SetGroundX(target.stand._x);
            currentBf->SetGroundY(target.stand._y);

            currentBf->InsertFighterInfo(_pack);  //Stream
            _pack << static_cast<UInt8>(target.goal._x) << static_cast<UInt8>(target.goal._y);
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
        //memset(_mapFlag,0,_x*_y*sizeof(UInt8));
        currentBf = NULL;
    }




    void BattleGround::GetEnemys(UInt8 x,UInt8 y,std::vector<GObject::Ascoord>& vecEnemy) //这个是初步的敌人  后面还要进行筛查
    {
        UInt8 attackDis = currentBf->GetDistance();   //获得攻击距离
        UInt8 ride = currentBf->GetMaxRide();
        UInt8 range = attackDis+ride;
        UInt8 campType = _mapCamp[x+y*_x];
        for(UInt8 i = x > range ? x-range : 0 ; i < ( x + range  > _x ? _x : x+range ) ; ++i)
        {
            if( i > _x )
                break;
            for( UInt8 j = y > range ? y - range : 0 ; j < ( y + range > _y ? _y : y +range) ; ++j)
            {
                if( j > _y )
                    break;
                if( _mapGround[i+j*_x] == 0 )  //0代表不可放置的位置
                    continue;
                if( _mapFighters[i+j*_x] == NULL )
                    continue;
                if( _mapFighters[i+j*_x]->getClass() > 100)
                    continue;
                if( _mapCamp[i+j*_x] == campType ) //属于同一个阵营  
                    continue;
                if( _mapFighters[i+j*_x]->getHP() < 0 ) //已经死了
                    continue;
                if( _mapFighters[i+j*_x] == currentBf )  //不是自己
                    continue;
                vecEnemy.push_back(GObject::Ascoord(i,j));
            }
        }
    }



    void BattleGround::SetBattleInfo()  //战场的排布情况
    {
        UInt8* battleInfo = new UInt8[_x*_y];
        battleInfo = _mapCamp;
        //0 代表不可放置战将的障碍   1 代表势力     2   代表势力2  
        for(UInt8 i = 0 ; i < _x; ++i)   //如果哪个位置的战将死了  将这个位置的设为3  没有战将的也设置为3 可以自由移动的
        {
            for( UInt8 j = 0 ; j < _y ; ++j)
            {
                if( battleInfo[i+j*_x] != 0 && _mapFighters[i+j*_x] == NULL )   //没放战将
                {
                    battleInfo[i+j*_x] = 3;
                }
                if( _mapFighters[i+j*_x] != NULL && _mapFighters[i+j*_x]->getHP() < 0 )  //死了  
                {
                    battleInfo[i+j*_x] = 3;
                }
                if( _mapFighters[i+j*_x] != NULL && _mapFighters[i+j*_x]->getHP() > 0 )
                {
                    battleInfo[i+j*_x] = _mapFighters[i+j*_x]->GetSide();
                }
            }
        }
        _astar.SetBattleInfo(battleInfo);
        _astar.SetSide(currentBf->GetSide());

    }


    void BattleGround::BowGetTarget(UInt8 direction, UInt8 destx , UInt8 desty)
    {
        /*
        static UInt8 priority [4][4] = {
            {1,2,3,4},
            {1,2,3,4},
            {1,2,3,4},
            {1,2,3,4}
        };
        */
        if(!currentBf)
        if( direction <= 0 || direction >= 4 )
        {
            return;
        }
        UInt8 ride = currentBf->GetMaxRide();
        UInt8 x = currentBf->GetGroundX();
        UInt8 y = currentBf->GetGroundY();

        UInt8 standx = 0;
        UInt8 standy = 0;

        if( x ==  _x )
        {
            standx = _x;
        }
        if( y == _y )
        {
            standy = _y;
        }

        bool can = true;
        UInt8 size = 0xFF;
        GObject::Ascoord stand = GObject::Ascoord(-1,-1);

        switch(direction)
        {
            case 1:
                for(UInt8 i = x ; i > 0  ; --i)
                {
                    for(UInt8 j = y ; j > 0 ; --j )
                    {
                        if( (_mapGround[i+j*_x] != 0 && _mapFighters[i+j*_x] == NULL) || ( _mapFighters[i+j*_x] != NULL && _mapFighters[i+j*_x]->getHP() <= 0 )) 
                        {
                            if( ride < 0 &&  !can  )
                                break;
                            can = CanAttack(GObject::Ascoord(i,j),GObject::Ascoord(destx,desty));
                            UInt8 landform = _mapGround[ i + j*_x] ;
                            UInt8 rideSub = GameAction()->GetRide(landform);
                            ride = ride - rideSub;
                            stand = GObject::Ascoord(i,j);
                            ++size;
                        }
                    }
                }
                break;
            case 2:
                for( UInt8 i = x ; i < ( x+ride > _x ? x+ride : _x) ; ++i)
                {
                    for( UInt8 j = y; j > ( y -ride < 0 ? 0 : y - ride ) ; --j )
                    {
                        if( (_mapGround[i+j*_x] != 0 && _mapFighters[i+j*_x] == NULL) || ( _mapFighters[i+j*_x] != NULL && _mapFighters[i+j*_x]->getHP() <= 0 )) 
                        {
                            if( ride < 0 && !can )
                                break;
                            can = CanAttack(GObject::Ascoord(i,j),GObject::Ascoord(destx,desty));
                            UInt8 landform = _mapGround[i + j*_x] ;
                            UInt8 rideSub = GameAction()->GetRide(landform);
                            ride = ride - rideSub;
                            stand = GObject::Ascoord(i,j);
                            ++size;
                        }
                    }
                }
                break;
            case 3:
                for( UInt8 i = x ; i > ( x - ride < 0  ? 0 : x - ride );  --i)
                {
                    for( UInt8 j = y ; j < ( y + ride > _y ? _y : y + ride ) ; ++j)
                    {
                        if( (_mapGround[i+j*_x] != 0 && _mapFighters[i+j*_x] == NULL) || ( _mapFighters[i+j*_x] != NULL && _mapFighters[i+j*_x]->getHP() <= 0 )) 
                        {
                            if( ride < 0 && !can )
                                break;
                            can = CanAttack(GObject::Ascoord(i,j),GObject::Ascoord(destx,desty));
                            UInt8 landform = _mapGround[i + j*_x] ;
                            UInt8 rideSub = GameAction()->GetRide(landform);
                            ride = ride - rideSub;
                            stand = GObject::Ascoord(i,j);
                            ++size;
                        }
                    }
                }
                break;
            case 4:
                for( UInt8 i = x ; i < ( x + ride > _x  ? _x : x + ride ) ; ++ i)
                {
                    for( UInt8 j = y; j < ( y + ride > _y ? _y : y + ride ) ; ++ j )
                    {
                        if( (_mapGround[i+j*_x] != 0 && _mapFighters[i+j*_x] == NULL) || ( _mapFighters[i+j*_x] != NULL && _mapFighters[i+j*_x]->getHP() <= 0 )) 
                        {
                            if( ride < 0 && !can )
                                break;
                            can = CanAttack(GObject::Ascoord(i,j),GObject::Ascoord(destx,desty));
                            UInt8 landform = _mapGround[i + j*_x] ;
                            UInt8 rideSub = GameAction()->GetRide(landform);
                            ride = ride - rideSub;
                            stand = GObject::Ascoord(i,j);
                            ++size;
                        }
                    }
                }
                break;
            default:
                break;
        }
        //UInt8 pri = priority[currentBf()->GetClass()-1][_mapFighters[destx+desty*_x]->getClass()-1];
        if( stand._x == -1 && stand._y == -1 )
        {
            // _vecTarget.push_back(TargetInfo(stand,GObject::Ascoord(destx,desty),size,pri));
        }
    }


    void BattleGround::GetBestTarget(UInt8 x, UInt8 y )
    {
        SetBattleInfo();
        std::vector<GObject::Ascoord> vecEnemy;
        GetEnemys(x,y,vecEnemy);
        if( vecEnemy.empty() )
        {
            //在自己所能打到的最大范围内度没有敌人哦  只能向中心点移动 
        }
        else
        {
            if( currentBf->GetClass() == 3 )
            {
                //弓箭兵的行为比较特殊  涉及到后退  单独处理
                for(auto it = vecEnemy.begin(); it != vecEnemy.end(); ++it)
                {
                    UInt8 x = currentBf->GetGroundX();
                    UInt8 y = currentBf->GetGroundY();

                    //得到中心点
                    UInt8 centerx = _x/2;
                    UInt8 centery = _y/2;


                    UInt8 destx = (*it)._x;
                    UInt8 desty = (*it)._y;

                    //趋利避害 后退一下  再攻击
                    //
                    //    1                  2
                    //
                    //              x                 --->目标点
                    //    3                  4 
                    //
                    UInt8 direction = 0;
                    if( x > destx && y > desty )
                    {
                        direction = 4 ;
                    }
                    else if( x > destx && y < desty )
                    {
                        direction = 2;
                    }
                    else if( x < destx && y < desty )
                    {
                        direction = 1;
                    }
                    else if( x < destx && y > desty )
                    {
                        direction = 3;
                    }
                    else
                    {
                        direction = 5;
                    }

                    BowGetTarget( direction, destx , desty );
                }

            }
            else
            {
                _astar.SetStart(GObject::Ascoord(x,y));
                for( auto it = vecEnemy.begin(); it != vecEnemy.end() ; ++it)
                {
                    std::vector<GObject::Ascoord> path;
                    _astar.SetTarget((*it));
                    _astar.ComputeRoute();
                    _astar.GetRoute(&path);
                    ParsePath(path);
                }
            }
        }

    }

    bool BattleGround::CanAttack(GObject::Ascoord stand, GObject::Ascoord target)   //站在某一点是不是可以攻击到目标呢
    {
        UInt8 attackDis = currentBf->GetDistance();
        UInt8 dis = (stand._x-target._x)*(stand._x-target._x) + (stand._y-target._y)*(stand._y-target._y);
        if( dis < attackDis*attackDis )
        {
            return true;
        }
        return false;
    }

    void BattleGround::ParsePath(std::vector<GObject::Ascoord> &path)
    {
        //分析这个路径 提取有用的信息
        //按照这个路径 行走一下
        static UInt8 priority [4][4] = {
            {1,2,3,4},
            {1,2,3,4},
            {1,2,3,4},
            {1,2,3,4}
        };

        UInt8 ride = currentBf->GetMaxRide();
        if( path.size() < 2 )
        {
            return;
        }
        else
        {
            auto begin = path.begin();  //起点
            auto end   = path.end();
            UInt8 size = 0xFF;
            while( begin != end )
            {
                if( CanAttack(*begin,*end))   //
                    break;
                if( ride <= 0 )
                    break;
                GObject::Ascoord p = (*begin);   //可以走到的位置
                //先获得地形 
                UInt8 landform = _mapGround[p._x + p._y*_x] ;
                UInt8 rideSub = GameAction()->GetRide(landform);
                ride = ride - rideSub;
                ++begin;
                ++size;
            }
            BattleFighter* ft = static_cast<BattleFighter*>(_mapFighters[(*end)._x + (*end)._y*_x]);
            UInt8 pri = priority[currentBf->GetClass()-1][ft->GetClass()-1]; 
            _vecTarget.push_back(TargetInfo(ft,*begin,*end,size,pri));
        }
    }
    
    /*
    void BattleGround::GetTarget(UInt8 ride, const UInt8& x,const UInt8& y,UInt8 flag)
    {
        _astar.SetFighters(_mapFighters);
        _astar.SetCurrentBf(currentBf);
        static UInt8 priority [4][4] = {
            {1,2,3,4},
            {1,2,3,4},
            {1,2,3,4},
            {1,2,3,4}
        };
        //在行动力力加攻击范围所达的距离内寻找要攻击的目标坐标  放入数组中
        std::vector<GObject::Ascoord> vecScoord;
        UInt8 i=0;
        UInt8 j=0;
        UInt8 distance = currentBf->GetDistance();
        if( distance > 5 )
            return;
		UInt8 scale = ride + distance ;
        for(j = y > scale ? (y-scale):0 ; j < y+scale ; ++j)
        {
            if( j >= _y )
                break;
            UInt8 step = 1;
            for(i = x > scale ? (x-scale):0 ; i< x+scale ; i=i+step)     
            {
               if( i >=  _x )
                   continue;
               if( _mapFighters[ i+j*_x] != NULL && _mapFighters[ i+j*_x]->GetSide() == currentBf->GetSide() )
                   continue;
               if( (j == (y -scale) || j == (y+distance)) && ((i == (x-scale) || i == (x+scale) )  ))
                   continue;
               if(_mapFighters[ i+j*_x] == NULL )
                   continue;
               if(_mapFighters[ i+j*_x] == currentBf )
                   continue;
               if( _mapFighters[i+j*_x]->getHP() <= 0 )
                   continue;
               if(_mapFighters[i+j*_x]->getClass() > 100 )
                   continue;
               if( step != 2 )
                   step = 2;
               vecScoord.push_back(GObject::Ascoord(i,j));
            }
        }
        //然后对每一个攻击目标用A*算法求最优的路径
		_astar.SetStart(GObject::Ascoord(x,y));
        std::vector<attackInfo> vecInfo;
        for( auto it =vecScoord.begin();it != vecScoord.end() ; ++it)
        {
            GObject::Ascoord target = (*it);
            _astar.SetTarget(target);
            std::vector<GObject::Ascoord> path;
            _astar.ComputeRoute();
            _astar.GetRoute(&path);
            struct attackInfo info;
            info.size = path.size();
            info.pri  = priority[currentBf->getClass()-1][_mapFighters[target._x+target._y*_x]->getClass()-1];
            info.target = target;
            vecInfo.push_back(info);
        }
        if( !vecInfo.empty())
        {
            std::sort(vecInfo.begin(),vecInfo.end(),MoreFit);
            GObject::Ascoord target = vecInfo.front().target;
            _target = makeTarget(target,ride);
        }
        else
        {
            _target = TargetInfo();  //默认的  全是零
        }
    }

    TargetInfo BattleGround::makeTarget(GObject::Ascoord target,UInt8 ride)
    {
        GObject::Ascoord start = _astar.GetStart();
        _astar.SetTarget(target);
        std::vector<GObject::Ascoord> path;
        _astar.ComputeRoute();
        _astar.GetRoute(&path);
        std::reverse(path.begin(),path.end());  //
        UInt8 range = currentBf->GetDistance() + ride; 
        BattleFighter* ft = static_cast<BattleFighter*>(_mapFighters[target._x + target._y*_x]);
        if( path.size()  == 2 )
        {
           //站在现有位置就可以攻击到目标
           return TargetInfo(ft,start._x,start._y,target._x,target._y);
        }
        else if( static_cast<UInt8>(path.size()) > range+1 )  //攻击路径比较长  
        {
            GObject::Ascoord attack = path[ride];
            return TargetInfo(ft,attack._x,attack._y,target._x,target._y);
        }
        else
        {
            GObject::Ascoord attack;
            //工兵比较特殊  为了保护自己  工兵在条件满足时  可以向后移动  再攻击
            if( currentBf->GetClass() == 3 )
            {
                //TODO
                UInt8 distance = currentBf->GetDistance();
                //以目标点为中心 
                UInt8 cx = currentBf->GetGroundX();
                UInt8 cy = currentBf->GetGroundY();
                UInt8 x = target._x;
                UInt8 y = target._y;
                UInt8 i = 0;
                UInt8 j = 0;

                UInt8 lx = 0;
                UInt8 ly = 0;
                if( cx > x && cy > y )  //目标点在自己的左上角
                {
                    lx = x+distance > _x ? _x : x+distance; 
                    ly = y+distance > _y ? _y : y+distance;

                    for( j = ly; j > y ; --j)
                    {
                        for( i = lx ; i > x; --i)
                        {
                            if(_mapFighters[i+j*_x] == NULL )
                            {
                                attack._x = i;
                                attack._y = j;
                                break;
                            }
                        }

                    }
                }

                else if( cx > x && cy < y ) //左下角
                {
                    lx = x+distance > _x ? _x : x+distance;
                    ly = y-distance < 0 ? 0 : y-distance;
                    for( j = ly; j < y ; ++j)
                    {
                        for( i = lx ; i > x ; --i )
                        {
                            if(_mapFighters[i+j*_x] == NULL )
                            {
                                attack._x = i;
                                attack._y = j;
                                break;
                            }
                        }
                    }
                }

                else if( cx < x && cy > y ) //右下角
                {
                    lx = x - distance < 0 ? 0 : x-distance;
                    ly = y+distance > _y ? _y : y+distance;
                    for( j = y ; j > y ; --j)
                    {
                        for( i = lx ; i < x ; ++i)
                        {
                            if(_mapFighters[i+j*_x] == NULL )
                            {
                                attack._x = i;
                                attack._y = j;
                                break;
                            }
                        }
                    }
                }

                else if( cx < x && cy < y )  // 右下角
                {
                    lx = x - distance < 0 ? 0: x-distance;
                    ly = y - distance < 0 ? 0: x-distance;
                    for( j = y ; j < y ; ++j)
                    {
                        for( i = lx ; i < x ; ++i)
                        {
                            if(_mapFighters[i+j*_x] == NULL )
                            {
                                attack._x = i;
                                attack._y = j;
                                break;
                            }
                        }
                    }
                }
                else
                {
                   return TargetInfo();
                }
                
            }
            else
            {
                attack = path[path.size()-currentBf->GetDistance()-1];
            }
            return TargetInfo(ft,attack._x,attack._y,target._x,target._y);
        }
    }
    */
    /* 
    void BattleGround::GetNearPos(UInt8 ride ,const UInt8& x ,const UInt8& y , UInt8 flag)
    { 
        UInt8 scopeForOne[12] = {254,0, 255,1, 1,255, 2,0, 1,255, 255,255};

        if(!_mapGround[x + y*_x])
            return ;

        if(_mapFlag[x+y*_x])
            return ;
        if(ride == 0 || ride > 10)
            return ;

        if(x >= _x || x >= _y)
            return ;
        if(!_mapFighters[x+y*_x] || _mapFighters[x+y*_x] == currentBf )// || _mapFighters[x+y*_x]->GetSide() == currentBf->GetSide()*/
      //     GetTargetBo(x ,y,flag);
      /*
        for(UInt8 i = 0; i < 6 ; ++i) 
        {
            UInt8 posx = x + scopeForOne[2*i];
            UInt8 posy = y + scopeForOne[2*i + 1];

            if(_mapFlag[posx + posy * _x])
                continue;

            UInt8 rideSub = GetRideSub(posx , posy);
            if(rideSub < 1)
                continue;
            if(ride <= rideSub )
                continue;

            _mapFlag[posx + posy * _x] = 1;
            GetNearPos(ride - rideSub , posx,posy , flag + rideSub);
        }
    } 
    */
    UInt8 BattleGround::GetRideSub(const UInt8& posx ,const UInt8& posy)
    { 

        //TODO  返回消耗 (考虑周围敌军情况)
        return 1;
    }

    /*
    void BattleGround::GetTargetBo(UInt8 x,UInt8 y , UInt8 ride )
    {
        if(!_mapGround[x+y*_x])
            return ;
       UInt8 distance = currentBf->GetDistance()*2 ;
        if(distance > 5)
            return ;

        for(UInt8 j = y > distance?(y-distance):0 ; j < y + distance ; j += 1 )
        {
            if( j >= _y)
                break;
            UInt8 step = 1;
            for(UInt8 i = x > distance?(x-distance):0 ; i < x + distance ; i += step ) 
            {
                if(i >= _x)
                    continue;
                if((j == (y - distance) || j == (y + distance)) && (i == (x - distance) || i == (x + distance)))
                    continue;
                if(!_mapFighters[i + j*_x])
                    continue;
                if( _mapFlag[i+j*_x] )
                    continue;
                if(_mapFighters[i + j*_x] == currentBf)
                    continue;
                if(_mapFighters[i + j*_x]->getClass() > 100)
                    continue;
                if(!_mapFighters[i + j*_x]->getHP())
                    return ;
                if( step != 2)
                    step = 2;
                targetVec.push_back(TargetInfo(static_cast<BattleFighter *>(_mapFighters[i + j*_x]),x,y,MAX((ABS(x,i)),(ABS(j,y))) )) ;
                _mapFlag[i+j*_x] = 1;  //已经搜寻过
            }
        }
    } 
   */

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

        std::cout<< "A  "   <<  static_cast<UInt8>(bf->GetGroundX())  <<" , "<<   static_cast<UInt8>(bf->GetGroundY()) << " --------------->     " << static_cast<UInt8>(bo->GetGroundX())<<"  ,  "<<static_cast<UInt8>(bf->GetGroundY())<<std::endl;

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


    UInt8 BattleGround::PushPlayer(GObject::Player * pl,UInt8 index)
    { 
        if(map_player.size() >= PLAYERMAX)
            return 0;
        if(map_player.find(index) != map_player.end())
            return 0;
        map_player[index].push_back( pl);
        pl->SetBattleId(_id);
        pl->SetBattleSide(index);
        return 1;
    } 

    //战将进入战场
    void BattleGround::preStart()  //需要玩家手动操作
    { 
        static UInt8 point[][2] = {
            {1, 0}, 
            {3, 0}, 
            //{4, 0}, 
            //{0, 1}, 
            //{1, 1}, 
            //{3, 1}, 
            //{4, 1}, 
            //{1, 2}, 
            //{2, 2}, 
            //{4, 2}, 
        };
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
            for(UInt8 i = 0; i < vec.size(); ++i)
            {
                //for(UInt8 j = 0; j < 6 ; ++j)
                {
                    //军团成员放入战将
                    GObject::Fighter * fgt = it->second.at(0)->getMainFighter();
                    if(!fgt)
                        continue;
                    fighters[it->first].push_back(newFighter(point[it->first][0],point[it->first][1],fgt));
                    ++count;
                }
            }
            _pack.data<UInt8>(offset) = count;
            std::cout << "  人数"<< static_cast<UInt32>(count);
        }
    } 

    void BattleGround::TestCoutBattleS(BattleFighter * bf)
    { 
        for(UInt8 i = 0 ; i < _x; ++i)
        { 
            for(UInt8 j = 0; j < _y; ++j)
            { 
                if(_mapFighters[i+j*_x] &&( bf == NULL || _mapFighters[i+j*_x] == bf))
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
        /*
        if( _target.bo == NULL )
            return 1;
        //TODO  如果是弓兵 判断攻击点和目标点连线上是否有障碍物
        UInt8 dis = currentBf->GetDistance();
        UInt8 ax  = _target.ax;
        UInt8 ay  = _target.ay;
        UInt8 gx  = _target.gx;
        UInt8 gy  = _target.gy;
        UInt8 factDis = sqrt((ax-gx)*(ax-gx)+(ay-gy)*(ay-gy));
        if(dis >= factDis)
        {
            return factDis;
        }
        else
        {
            return dis;
        }
        */
        return 1;
    }

}
