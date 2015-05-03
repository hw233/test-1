#ifndef BATTLEGROUND_H_
#define BATTLEGROUND_H_
#include "Config.h"
#include <map>
#include <vector>
#include "Common/Stream.h"
#include "Server/OidGenerator.h"
#include "GObject/AStar.h"

#define GROUND_LENGTH 20

namespace GObject
{
    class Fighter;
    class Player;
    class AStar;
}

namespace Battle
{
#define PLAYERMAX 10
    struct BattleInfo
    {
        UInt16 _round;
        UInt8 _x;  //战斗地点  //XXX
        UInt8 _y;  
        UInt8 _attackSide;  //进攻方
        UInt16 _attackFighter;  //进攻散仙
        UInt8 _defendSide;   //防守方
        UInt16 _defendFighter; //防守散仙
        UInt64 _brtId;   // 战报ID
        UInt8 _res;  //战斗结果 返回胜利方的side 0表示胜负未分
        BattleInfo(UInt16 round ,UInt8 x,UInt8 y, UInt8 attackSide ,UInt16 attackFighter ,UInt8 defendSide ,UInt16 defendFighter,UInt64 brtId, UInt8 res):_round(round),_x(x),_y(y),_attackSide(attackSide),_attackFighter(attackFighter),_defendSide(defendSide),_defendFighter(defendFighter),_brtId(brtId),_res(res){}
        BattleInfo(){}
    };

    class BattleObject;
    class BattleFighter;
    typedef std::vector< std::vector<UInt8> > vecInfo;    
    /*
    struct TargetInfo
    {
        BattleFighter * bo;
        UInt8 x ;
        UInt8 y ;
        UInt8 step;
        TargetInfo():bo(NULL),x(0),y(0),step(0){}
        TargetInfo(BattleFighter * bf,UInt8 x1, UInt8 y1, UInt8 step1):bo(bf),x(x1),y(y1),step(step1){}
        void clear(){bo = NULL ; x =0; y =0;step=0;}
    };
    */

    struct TargetInfo
    {
        BattleFighter * bo;
        GObject::Ascoord stand;
        GObject::Ascoord goal;
        UInt8 size;   //从起点到攻击点的路径长度
        UInt8 pri;   //攻击优先级
        TargetInfo(BattleFighter * bf,GObject::Ascoord st,GObject::Ascoord g,UInt8 s,UInt8 p):bo(bf),stand(st),goal(g),size(s),pri(p) {}
    };


    class BattleGround
    {
        public:
            BattleGround(UInt32 id , UInt8 mapId):_id(id),_maxID(0),_battleNum(IDGenerator::gBattleOidGenerator0.ID())
            {
                map_player.clear();
                InitMapFight(mapId);
            }
            BattleGround():_astar(GObject::AStar::AStar()){}
            ~BattleGround()
            {
                delete [] _mapGround;
                delete [] _mapFighters;
                //delete [] _mapFlag;
            }

            void InitMapFight(UInt8 mapId);
            UInt8 PushPlayer(GObject::Player *,UInt8 );
            void PushBattleInfo(const BattleInfo& bi);

            //对象移动
            void Move();
            //产生战报信息
            void Fight(BattleFighter *bf , BattleFighter * bo,UInt8& result,UInt32& BattleReport);
            void FighterMove(BattleFighter *, UInt8 x ,UInt8 y);
            //void GetNearPos(UInt8,const UInt8& ,const UInt8&,UInt8 flag = 0 );
            void GetTarget(UInt8,const UInt8& ,const UInt8&,UInt8 flag = 0 );
            void GetTargetBo(UInt8 x ,UInt8 y ,UInt8 step = 0);
            UInt8 GetRideSub(const UInt8& posx ,const UInt8& posy);

            BattleFighter * newFighter( UInt8 x , UInt8 y, GObject::Fighter *);
            void setObject(UInt8 x , UInt8 y ,BattleFighter * bf,UInt8 flag = 0);

            void preStart();
            void start();

            void TestCoutBattleS(BattleFighter* bf = NULL);
            void InsertFighterInfo(UInt8 flag = 0);

            void GetEnemys(UInt8 x , UInt8 y,std::vector<GObject::Ascoord>& vecEnemy);  //获得敌人的坐标的
            UInt8 GetFactAttackDis();
            //TargetInfo makeTarget(GObject::Ascoord traget,UInt8 ride);
            void BowGetTarget(UInt8 direction, UInt8 destx , UInt8 desty);
            void SetBattleInfo();
            bool CanAttack(GObject::Ascoord stand, GObject::Ascoord target);
            void ParsePath(std::vector<GObject::Ascoord> &path);
            void MoveToCenter(UInt8 direction);
            void GetBestTarget(UInt8 x, UInt8 y );
        private:
            UInt32 _id;
            UInt8 _x;
            UInt8 _y;
            std::map<UInt8 ,std::vector<GObject::Player *> >  map_player;

            UInt8 * _mapGround;  //地图信息  可以设置战场的环境
            //UInt8 * _mapFlag;    
            UInt8 * _mapCamp;    //地图阵营信息
            BattleObject ** _mapFighters;    //注意和fighters的坐标同步

            //来一个记录战将分布的结构 满足
            std::vector<BattleFighter *> fighters[PLAYERMAX];   //阵营中的战将
            std::vector<BattleInfo> battleIds;

            //std::vector<TargetInfo>  _vecTarget;
            std::vector<TargetInfo> _vecTarget;
            BattleFighter * currentBf;
            //static UInt8 scopeForOne[12];
            Stream _pack;

            UInt16 _maxID;
            UInt32 _battleNum;
            GObject::AStar  _astar;   //A* 算法
    };
}
#endif // BATTLEGROUND_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

