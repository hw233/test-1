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
        UInt8 sec = tt->tm_sec;

        UInt16 second = 0;
        UInt16 s = 0;
        if( min%TIME_ONCE == 0 && sec == 0 )
        {
            second = 0;
            s = 0 ;
        }
        else
        {
            second = ((min%TIME_ONCE)*60+sec)%TIME_TAB;
            s = TIME_ONCE*60-((min%TIME_ONCE)*60+sec);
        }

        AddTimer(10*60*1000, World_Govern_SendInfo,this,(s+1)*1000);
        AddTimer(15*1000, World_Govern_SendAward, this,(TIME_TAB-second+3)*1000);

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
        /*
        for(UInt8 i = 1; i < 10 ;++i)
        {
            UInt64 pid = (static_cast<UInt64>(1)<<48)|i;
            Player *pl = globalPlayers[pid];
            if(!pl)
                continue;
            Battle::battleManager.EnterBattleGround(BattleId,pl,i);
        }
        */
        UInt8 flag = 1;
        UInt8 i = 1;
        for(auto it = globalPlayerVec.begin(); it != globalPlayerVec.end(); ++it)
        {
            if( i > 11 )
                break;
            Player* pl = *it;
            Battle::battleManager.EnterBattleGround(BattleId,pl,i, flag);
            i++;
            flag = !flag;
        }
        Battle::battleManager.StartGround(BattleId);
    }

    void World::World_Govern_SendInfo(World* world)
    {
        for(auto it = GObject::globalOnlinePlayerSet.begin() ; it != GObject::globalOnlinePlayerSet.end() ; ++it)
        {
            UInt8 type = 0;
            GameMsgHdr hdr(0x156,WORKER_THREAD_COUNTRY_1,(*it),sizeof(type));
            GLOBAL().PushMsg(hdr,&type);
        }
    }

    void World::World_Govern_SendAward(World* world)
    {
        for(auto it = GObject::globalOnlinePlayerSet.begin() ; it != GObject::globalOnlinePlayerSet.end() ; ++it)
        {
            //在线
            time_t now = TimeUtil::Now();
            tm* tt=localtime(&now);
            UInt8 min = tt->tm_min;
            UInt8 sec = tt->tm_sec;
            UInt16 restSec = (min%TIME_ONCE==0 && sec == 0) ? 0: (min%TIME_ONCE*60+sec);
            UInt16 time = 0;
            if( restSec == 0 )
            {
                time = 0;
            }
            else
            {
                time = ( restSec%TIME_TAB == 0 ? restSec/TIME_TAB : restSec/TIME_TAB+1);
            }
            GameMsgHdr hdr(0x155,WORKER_THREAD_COUNTRY_1,(*it),sizeof(time));
            GLOBAL().PushMsg(hdr,&time);
        }
    }

}
