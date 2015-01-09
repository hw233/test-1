#include "BattleGround.h"

#include "BattleFighter.h" 
#include "GObject/Player.h"
#include "Battle/BattleSimulator.h"
#include "Battle/BattleReport.h"
#define MAX(x,y) x>y?x:y
#define ABS(x,y) x>y?x-y:y-x
namespace Battle
{
    //UInt8 scopeForOne[12] = {-2,0, -1,2, 1,-2, 2,0, 1,-2, -1,-2};
    static UInt8 getDistanceForTwo(UInt8 x ,UInt8 y, UInt8 x1 , UInt8 y1)
    {
        return MAX((ABS(x,x1)),(ABS(y,y1)));
    }
    void BattleGround::InitMapFight(UInt8 mapId)   
    { 
        static UInt8 point[][2] = {
            {3, 1}, 
            {4, 1}, 
            {5, 1}, 
            {2, 2}, 
            {4, 2}, 
            {6, 2}, 
            {8, 2}, 
            {3, 3}, 
            {5, 3}, 
            {6, 3}, 
            {2, 4}, 
            {4, 4}, 
            {6, 4}, 
            {8, 4}, 
        };
        _mapGround = new UInt8[_x*_y];
        memset(_mapGround , 0 ,_x*_y*sizeof(UInt8));
        for(UInt8 i = 0; i < 15 ; ++i)
        {
           _mapGround[point[i][0] + point[i][1]*_x] = 1;
        }
        _mapFighters = new BattleObject*[_x*_y];
        _mapFlag = new UInt8[_x*_y];
        memset(_mapFighters,0,sizeof(BattleObject*)*_x*_y);
        memset(_mapFlag,0,sizeof(UInt8)*_x*_y);
        _pack.init(0x80);
        return ;

    } 
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
        currentBf->InsertFighterInfo(_pack);  //Stream

        GetNearPos(currentBf->GetRide(),currentBf->GetGroundX(),currentBf->GetGroundY());
        if(targetVec.size() == 0)
        {
            //TODO  按照原定计划行动
            UInt8 resx = 0;
            UInt8 resy = 0;
            UInt8 nowx = currentBf->GetGroundX();
            UInt8 nowy = currentBf->GetGroundY();
            UInt8 ride  = currentBf->GetRide() ;
            UInt8 destx = _x - currentBf->GetEnterPosX();;
            UInt8 desty = _y - currentBf->GetEnterPosY();
            UInt8 minDistance = -1 ;

            for(UInt8 j = (nowy > ride)?(nowy - ride):0; j < _y && j < (nowy + ride) ; ++j)
            {
                for(UInt8 i = (nowx > 2*ride)?(nowx - 2*ride):0; i < _x && j < (nowx + 2*ride) ; ++i)
                {
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

            currentBf->SetGroundX(resx);
            currentBf->SetGroundY(resy);

            std::cout <<"无方案" << ":移动到 "<<static_cast<UInt32>(resx)<< "," << static_cast<UInt32>(resy) <<std::endl;
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
            GetTargetBo(x ,y,flag);

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
    UInt8 BattleGround::GetRideSub(const UInt8& posx ,const UInt8& posy)
    { 

        //TODO  返回消耗 (考虑周围敌军情况)
        return 1;
    } 

    void BattleGround::GetTargetBo(UInt8 x,UInt8 y , UInt8 ride )
    { 
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
                if(_mapFighters[i + j*_x] == currentBf)
                    continue;
                if(_mapFighters[i + j*_x]->getClass() > 100)
                    continue;
                if(!_mapFighters[i + j*_x]->getHP())
                    return ;
                if( step != 2)
                    step = 2;
                targetVec.push_back(TargetInfo(static_cast<BattleFighter *>(_mapFighters[i + j*_x]),x,y,MAX((ABS(x,i)),(ABS(j,y))) )) ;
            }
        }
    } 

    void BattleGround::Fight(BattleFighter *bf , BattleFighter * bo, UInt8& result, UInt32& BattleReport)
    { 
        //TODO   连接BattleSimulator
        BattleSimulator bsim(bf,bo);
        //bsim.start(); 
        result = bsim.GetWin();
        BattleReport = bsim.getId();
        std::cout << "发生战斗" << static_cast<UInt32>(bf->GetBattleIndex()) << " VS " << static_cast<UInt32>(bo->GetBattleIndex()) << "  战斗结果:" << static_cast<UInt32>(result) <<" 战报ID:" << BattleReport << std::endl;
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
        do
        {
            for(UInt8 i = 0; i < PLAYERMAX; ++i)  //PLAYERMAX 表示军团数量
            { 
                bool flag = false;
                if(fighters[i].size() == 0)
                    continue;
                for(UInt8 j = 0; j < fighters[i].size();++j)
                { 
                    if(fighters[i][j] && fighters[i][j]->getHP())
                    {
                        flag = true ;
                        currentBf = fighters[i][j];
                        Move();
                    }
                } 
                if(!flag)
                { 
                    alive |= (1 <<i);
                    ++count;
                } 
            } 
            ++Round;
        }while(count != (PLAYERMAX - 1) && Round < 200);
        _pack<<Stream::eos;
        battleReport.addReport(_battleNum,_pack);
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
    void BattleGround::preStart()
    { 
        static UInt16 point[][2] = {
            {3, 1}, 
            {4, 1}, 
            {5, 1}, 
            {2, 2}, 
            {4, 2}, 
            {6, 2}, 
            {8, 2}, 
            {3, 3}, 
            {5, 3}, 
            {6, 3}, 
            {2, 4}, 
            {4, 4}, 
            {6, 4}, 
            {8, 4}, 
        };
        std::map<UInt8 ,std::vector<GObject::Player *> >::iterator it = map_player.begin();
        _pack << map_player.size() ;
        for(;it != map_player.end(); ++it)
        {
            if(it->first >= PLAYERMAX)
                break;
            size_t offset = _pack.size();
            UInt8 count = 0;
            _pack << count ;
            std::vector<GObject::Player *> vec = it->second;
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
            _pack.data<UInt16>(offset) = count;
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
                    std::cout << std::endl;
                }
            } 
        } 
    } 

}
