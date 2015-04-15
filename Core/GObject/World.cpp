#include "Config.h"
#include "World.h"
#include "Common/TimeUtil.h"
#include "Script/WorldScript.h"
#include "Script/BattleFormula.h"
#include "GObject/GVar.h"
#include "Battle/BattleManager.h"
#include "Player.h"
#include "ChatHold.h"
#include <time.h>

#define W_CHAT_MAX 20

namespace GObject
{
    
    UInt8 time = 0;
    bool World::Init()
    {
        //chatHold = new ChatHold(W_CHAT_MAX);

        std::string path = cfg.scriptPath + "World/main.lua";
        TimeUtil::TimeAdd(GVAR.GetVar(GVAR_TIMEADD));
        {
            _worldScript = new Script::WorldScript(path.c_str()); 
        }

        path = cfg.scriptPath + "formula/main.lua";
        _battleFormula = new Script::BattleFormula(path.c_str());

        UInt32 now = TimeUtil::Now(), sday = TimeUtil::SharpDay(1) - 10; 

        if(sday < now) sday += 86400;
        AddTimer(86400 * 1000, World_Midnight_Check, this, (sday - now) * 1000);

        AddTimer(86400 * 1000, World_Test, this, 10* 1000);
        
        
        time_t n = now;
        tm* tt=localtime(&n);
        UInt8 min = tt->tm_min;

        AddTimer(60*60*1000, World_Govern_SendInfo,this, (60-min)*60*1000);

        UInt8 sec = tt->tm_sec;
        AddTimer(2*60*1000, World_Govern_SendAward, this, ((TIME_TAB-min%TIME_TAB)*60+sec)* 1000);

        return true; 
    }
    void World::UnInit() 
    {

    }
    std::string World::GetLogName()
    {
        return "log/World/"; 
    }

    void World::World_Midnight_Check( World * world )
    {
            UInt32 curtime = TimeUtil::Now();

            //记录此刻活动状态
            world->_worldScript->onActivityCheck(curtime+300);  //延迟300秒计算是否活动时间已过
            
    }
    void World::World_Test( World * world )
    { 
        struct NewUserStruct  
        {
            UInt64 phoneId;
            UInt16 len1;
            char accounts[10];
            //std::string accounts;
            UInt16 len2;
            char password[10];
            //std::string password;
            UInt16 len3;
            char _name[10];
            //std::string _name;
            UInt8 _class;
            //UInt8 _rp; //1:回流免费用户 2:回流vip1-vip4用户 3:回流vip5+用户
            std::string _para;
        };
        NewUserStruct ns ;
        ns.len1 = 10;
        ns.len2 = 10;
        ns.len3 = 10;
        ns.phoneId = 1;
        ns.accounts = {'l','b'};
        ns.password = {'1'};
        ns._name = {'Q'};
        ns._class = 5;
        ns._para = "192.168.88.250";
        LoginMsgHdr hdr1(0xE1, WORKER_THREAD_LOGIN, 8500, 1212121 , sizeof(ns)); 
        GLOBAL().PushMsg(hdr1, &ns);
        UInt32 BattleId = Battle::battleManager.CreateBattleGround();
        for(UInt8 i = 1; i < 10 ;++i)
        {
            UInt64 pid = (static_cast<UInt64>(1)<<48)|i;
            Player *pl = globalPlayers[pid];
            if(!pl)
                continue;
            Battle::battleManager.EnterBattleGround(BattleId,pl,i);
        }
        Battle::battleManager.StartGround(BattleId);
    }

    void World::World_Govern_SendInfo(World* world)
    {
        for(auto it = GObject::globalOnlinePlayerSet.begin() ; it != GObject::globalOnlinePlayerSet.end() ; ++it)
        {
            (*it)->GetGovernManager()->SendGovernResult(0);
        }
    }

    void World::World_Govern_SendAward(World* world)
    {
        for(auto it = GObject::globalPlayerVec.begin() ; it != GObject::globalPlayerVec.end() ; ++it)
        {
            if((*it)->isOnline())
            {
                time_t now = TimeUtil::Now();
                tm* tt=localtime(&now);
                UInt8 min = tt->tm_min;
                UInt8 time = (( min%TIME_TAB == 0 )? (min/TIME_TAB):(min/TIME_TAB+1));
                (*it)->GetGovernManager()->SendOnlineGovernAward(time);
            }
            else
            {
                (*it)->GetGovernManager()->SendOfflineGovernAward();

            }
        }
    }

}
