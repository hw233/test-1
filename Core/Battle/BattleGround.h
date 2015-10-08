#ifndef BATTLEGROUND_H_
#define BATTLEGROUND_H_
#include "Config.h"
#include <map>
#include <vector>
#include "Common/Stream.h"
#include "Server/OidGenerator.h"
//#include "GObject/AStar.h"
#define GROUND_LENGTH 20
#include "MsgID.h"

namespace GObject
{
    class Fighter;
    class Player;
    class AStar;
}

namespace Battle
{
#define PLAYERMAX 5
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
    typedef std::vector<UInt8> vecInfo;   

    
    struct Ascoord
    {
        UInt8 x;
        UInt8 y;
        Ascoord(const UInt8 vx,const UInt8 vy):x(vx),y(vy) {}
        Ascoord() {}
        inline bool operator == ( const Ascoord p)
        {
            return (this->x == p.x && this->y == p.y );
        }
        inline bool operator != ( const Ascoord p)
        {
            return (this->x != p.x || this->y != p.y );
        }
        inline void operator += ( const Ascoord p)
        {
            this->x += p.x;
            this->y += p.y;
        }
    };

    struct Node
    {
        Ascoord node; 
        Ascoord parent;
        UInt8 g;
        UInt8 h;
        Node(Ascoord p) : node(p) {}
        Node(Ascoord n,Ascoord p,UInt8 lh,UInt8 rh):node(n),parent(p),g(lh),h(rh) {} 
    };

    struct AttackInfo
    {
        Ascoord attack;
        Ascoord target;
        AttackInfo(Ascoord a,Ascoord t) : attack(a) , target(t) {}
    };
    
    template<class T=BattleFighter*>
    struct LessActId
    {
        bool operator() (T bf1,T bf2)
        {
            if( bf1->GetBattleIndex() < bf2->GetBattleIndex() )
            {
                return true;
            }
            return false;
        }
    };

    struct TargetInfo
    {
        BattleFighter * bo;
        Ascoord attack;  //攻击点
        UInt8 cost;    //行动力消耗
        UInt8 pri;     //攻击优先级
        TargetInfo(BattleFighter* b, Ascoord a,UInt8 c,UInt8 p): bo(b),attack(a),cost(c),pri(p) {}
    };

    struct FighterInfo
    {
        FighterInfo(GObject::Player* player,UInt16 id,UInt8 x,UInt8 y) : owner(player),fighterId(id),posx(x),posy(y) {}
        GObject::Player* owner;
        UInt16 fighterId;
        UInt8  posx;
        UInt8  posy;

    };
    

    class BattleGround
    {
        public:
            BattleGround(UInt32 id , UInt8 mapId):_roomId(id),_maxID(0),_battleNum(IDGenerator::gBattleOidGenerator0.ID()),_actId(0),_isFirstRound(true),_oneRoundCostTime(0),_isSetCapture(false)
            {
                //map_player.clear();
                currentBf = NULL;
                _isNpc = false;
                map2fighter.clear();
                _camp2pos.clear();
                camp2fighters.clear();
                InitMapFight(mapId);
                _closeList.clear();
                _openList.clear();
                battleIds.clear();
                _aroundAscoord.clear();
                map2fighter.clear();
            }
            ~BattleGround()
            {
                //delete [] _mapFighters;
                /*
                for(UInt8 j = 0; j < _y ; ++j)
                {
                    for(UInt8 i = 0 ; i < _x ; ++i )
                    {
                        if( _mapFighters[i+j*_x] != NULL )
                        {
                            delete _mapFighters[i+j*_x];
                        }
                    }

                }
                */
                _mapFighters = NULL;

                delete [] _mapGround;
                _mapGround = NULL;

                delete []_mapCamp;
                _mapGround = NULL;

                for( auto it = map2fighter.begin(); it != map2fighter.end(); ++it )
                {
                    for( auto iter = (it->second).begin(); iter != (it->second).end(); ++iter)
                    {
                        delete (*iter);
                    }
                }

                map2fighter.clear();
                _aroundAscoord.clear();
                battleIds.clear();
                num2pos.clear();
                camp2fighters.clear();
                _closeList.clear();
                _openList.clear();
            }

            void InitMapFight(UInt8 mapId);
            void PushBattleInfo(const BattleInfo& bi);
            void PushFighter(GObject::Player*, UInt16 ,UInt8,UInt8);
            void PushNpc(UInt8 forceId,UInt16 fighterId,UInt8 x,UInt8 y);
            void SetCampActId();
            void GetCampAttackOrder(UInt8 campId,UInt8 dir);
            void SearchFromDownToUp(UInt8 campId);
            void SearchFromUpToDown(UInt8 campId);
            void SearchFromRightToLeft(UInt8 campId);
            void SearchFromLeftToRight(UInt8 campId);
            void SetBattleIndex();
            UInt8 GetSpecialDirection();
            void  FightOneRound();
            void  SetEachPosNumber();
            UInt8 GetBattleIndex(UInt8 x,UInt8 y );
            BattleFighter* GetMinBattleIndexFighter(std::vector<BattleFighter*> listFighter);
            bool SomeCampIsAllDie(UInt8 campId);
            bool CheckIsStop();
            void MakePreStartInfo();
            UInt16 GetOneRoundTimeCost() const { return _oneRoundCostTime;}
            void SyncHp(BattleFighter* bft,UInt8 x, UInt8 y);
            void SortByActId();
            //对象移动
            void Move(bool flag = 0 );
            //产生战报信息
            void Fight(BattleFighter *bf , BattleFighter * bo,UInt8& result,UInt32& BattleReport,UInt16& fightTimeCost);
            void FighterMove(BattleFighter *, UInt8 x ,UInt8 y);
            //void GetTargetBo(UInt8 x ,UInt8 y ,UInt8 step = 0);
            UInt8 GetRideSub(const UInt8& posx ,const UInt8& posy);

            BattleFighter * newFighter( UInt8 x , UInt8 y, GObject::Fighter *);
            void setObject(UInt8 x , UInt8 y ,BattleFighter * bf,UInt8 flag = 0);
            void preStart();
            void start();

            void TestCoutBattleS(BattleFighter* bf = NULL);
            void InsertFighterInfo(UInt8 flag = 0);
            void GetAllBattleFighter(std::list<BattleFighter*>& listFighters);
            UInt8 GetFactAttackDis();
            UInt8 GetCampInfo(UInt8 index) { return _mapCamp[index];}

            void GetNearEnemy(UInt8 x,UInt8 y,std::vector<Ascoord>& vecEnemy); 
            void Analyse(std::list<Ascoord>path,Ascoord& target);
            bool IsInAttackZone(Ascoord stand, Ascoord target);
            void GetTarget(UInt8 x,UInt8 y ,UInt8 ride);
            void GetNeighbourEnemy(UInt8 x,UInt8 y,std::vector<Ascoord>& vecAscoord);//相邻的敌人
            void BowAnalyse(std::list<Ascoord> path,Ascoord target);
            void GetBackPosition(std::vector<Ascoord>& vecNearEnemy, std::vector<AttackInfo>& vecFinal);
            void GetRideZone(std::vector<Ascoord>& vecAscoord);
            bool EnemyCanAttack(Ascoord stand, Ascoord target);
            UInt8 GetDirection(UInt8 x,UInt8 y , UInt8 cx, UInt8 cy);
            void MoveAnalyse(std::list<Ascoord> path, Ascoord target, Ascoord& move);
            void GetMoveTargetByDir(UInt8 d, Ascoord& move);
            bool InMyAttackHasEnemy(const Ascoord& p );
            Ascoord ConvertConverstion(const Ascoord& p);
            void GetMovePos(std::list<Ascoord>path, Ascoord& last);
            UInt8 GetShootActionType(UInt8 x ,UInt8 y, std::vector<Ascoord>& vecEnemy);
            void  ShootGetTarget(UInt8 actionType);
            void  ShootFrontAttack();
            void  ShootCurrentPosAttack();
            void  ShootBackAttack();
            void  GiveBattleAward();
            void  MoveGetEnemy(std::vector<Ascoord>& vecAscoord);
            void  GetMovePosition(Ascoord& move);
            UInt8 GetDistance(Ascoord & lp , Ascoord& rp );
            bool  IsNearbyHaveEnemy(Ascoord& p);


            void AttackInStyle(UInt8 x,UInt8 y, Ascoord& target);
            void ShootCurrentAttack(Ascoord& target);
            void ShootFrontAttack(Ascoord& target);
            void GetBackPosition(Ascoord& target,std::vector<AttackInfo>& vecFinal);
            void BowGetTarget(UInt8 x,UInt8 y, std::vector<Ascoord>& vecEnemy);
            void ShootBackAttack(Ascoord& target);

            UInt8 AddPriority(Ascoord& target);
            ////////一下这些家伙全是A*相关东西

            UInt8 GetGValue(const Ascoord& p);
            UInt8 GetHValue(const Ascoord &p);  //获得H值
            void  SetStart(const Ascoord &p);   //设置终止点
            void  SetEnd(const Ascoord &p);  //设置起始点
            
            bool  IsObstract(const Ascoord &p);
            bool  IsInList(std::list<Node>&list, const Ascoord &p );
            bool  JudgeSurround(const Ascoord& cur,const Ascoord& parent, UInt8 g);
            Node* FindFromList(std::list<Node>& list,const Ascoord& p);
            void  CheckUp(std::vector<Ascoord>&vecAScoord);
            bool  ComputeRoute();
            bool  PopBestStep(Node* node);
            bool  GetRoute(std::list<Ascoord>& list);
            void  GetAround(const Ascoord& p,std::vector<Ascoord>&vecAscoord);
            bool  IsInAround(const Ascoord& p , const Ascoord& t);

            //下面这些是跟战役结算相关的
            void  SetCaptureForce();
            UInt8 GetCaptureId();
            void  GetAliveForce(std::vector<UInt8> &vecForce);
            void  SendBattleEachInfo();
            std::vector<UInt8> GetPreStartForces();
            void  SendBattleResultInfo();

            void AutoEnterFighters(UInt8 index, GObject::Player *pl, UInt16 pos = 0);

            void ClearArenaPos();
            
            void SetIsNPC(bool v){ _isNpc = v;}
            bool GetIsNPC(){ return _isNpc;}
            UInt32 GetBattleNUmber(){ return _battleNum;} 

        private:
            UInt32 _roomId;   //房间id  没有的话 就默认就是0
            UInt8 _x;
            UInt8 _y;
            UInt8 _mapId;
            //std::map<UInt8 ,std::list<GObject::Player *> >  map_player;
            
            std::map<UInt8, std::vector<FighterInfo*>> map2fighter;  //战将及所属玩家  对应的阵营
            std::map<UInt8, std::list<Ascoord>> _camp2pos;  //阵营对应的布阵位置  直接按照行动顺序进行布局

            UInt8 * _mapGround;  //地图信息  可以设置战场的环境
            //UInt8 * _mapFlag;    
            UInt8 * _mapCamp;    //地图阵营信息
            BattleObject ** _mapFighters;    //注意和fighters的坐标同步
            std::map<UInt8,Ascoord> num2pos; //地图位置对应编号
            //来一个记录战将分布的结构 满足
            std::map<UInt8,std::list<BattleFighter*>> camp2fighters;
            //std::list<BattleFighter *> fighters[PLAYERMAX];   //阵营中的战将
            std::vector<BattleInfo> battleIds;

            std::vector<TargetInfo> _vecTarget;
            BattleFighter * currentBf;
            Stream _pack;

            UInt16 _maxID;
            UInt32 _battleNum;
            
            UInt16 _actId;
            bool   _isFirstRound;
            UInt16 _oneRoundCostTime; //一回合消耗的时间
            bool   _isSetCapture;
            std::set<GObject::Player*> setPlayer;
            //一下全是跟A*算法  有关的东西
            Ascoord _start;
            Ascoord _end;
            std::list<Node>  _closeList;
            std::list<Node>  _openList;
            std::vector<Ascoord> _aroundAscoord;

            bool _isNpc;

    };
}
#endif // BATTLEGROUND_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

