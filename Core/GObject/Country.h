
#ifndef COUNTRY_H_
#define COUNTRY_H_
#include "Server/WorkerRunner.h"
#include "MsgHandler/CountryMsgHandler.h"
#include "Script/GameActionLua.h"
namespace Script
{ 
    class GameActionLua;
    class BattleFormula;
} 
namespace GObject
{
    class Country
        :public WorkerRunner<CountryMsgHandler>
    {
        public:
            Country(UInt8 id);
            inline UInt8 TID() const { return m_ThreadID; }
            bool Init();
            void UnInit();
            std::string GetLogName();

            inline Script::GameActionLua* GetGameActionLua()
            {
                return m_GameActionLua;
            }

            inline Script::BattleFormula* GetBattleFormula()
            { 
                return m_BattleFormula;
            } 
        
        private:
            UInt8       m_ThreadID;

            Script::GameActionLua*	m_GameActionLua;
            Script::BattleFormula* m_BattleFormula;
    };
}
#define CURRENT_COUNTRY() WorkerThread<GObject::Country>::LocalWorker()
#define GameAction() CURRENT_COUNTRY().GetGameActionLua()

#endif // COUNTRY_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

