#include "Config.h"
#include "GMHandler.h"
#include "MsgID.h"
#include "Server/WorldServer.h"
#include "Server/OidGenerator.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "GObject/Player.h"
#include "Script/GameActionLua.h"
#include "Script/ConfigScript.h"
#include "Battle/BattleSimulator.h"
#include "Common/StringTokenizer.h"
#include "CountryMsgStruct.h"
#include "MsgHandler/JsonParser.h"
#include "Common/Itoa.h"
#include "Script/BattleFormula.h"

#include "Memcached.h"
#include "Version.h"
#include "GObject/GVar.h"

GMHandler gmHandler;

GMHandler::GMHandler()
{

    Reg(3, "addvar", &GMHandler::OnAddVar);
    _printMsgPlayer = NULL;
}

void GMHandler::OnAddVar( GObject::Player * player, std::vector<std::string>& args )
{
    if(args.empty())
        return;
    if(args.size() == 2)
    {
        UInt32 var = atoi(args[0].c_str());
        UInt32 value = atoi(args[1].c_str());
        UInt32 num = player->GetVar(var);
        player->SetVar(var,num+value);
    }
}

void GMHandler::OnSetGVar(GObject::Player *, std::vector<std::string>& args)
{ 
    if(args.empty())
        return;
    if(args.size() == 2)
    {
        UInt32 var = atoi(args[0].c_str());
        UInt32 value = atoi(args[1].c_str());
        GObject::GVAR.SetVar(var,value);
    }
} 
void GMHandler::Reg( int gmlevel, const std::string& code, GMHandler::GMHPROC proc ) 
{
    std::string code_ = code;
    std::string lwr_ = strlwr(&code_[0]);
    for(int i = gmlevel; i < 4; ++ i)
    {
        GMHP& gmhp = _procMap[i][lwr_];
        gmhp.np = false;
        gmhp.proc = proc;
        gmhp.procnp = NULL;
    }
}

void GMHandler::Reg( int gmlevel, const std::string& code, GMHandler::GMHPROCNP proc )
{
    std::string code_ = code;
    std::string lwr_ = strlwr(&code_[0]);
    for(int i = gmlevel; i < 4; ++ i)
    {
        GMHP& gmhp = _procMap[i][lwr_];
        gmhp.np = true;
        gmhp.proc = NULL;
        gmhp.procnp = proc;
    }
}
