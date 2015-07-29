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
#include "GObject/Mail.h"
#include "GObject/Fighter.h"
#include "Server/OidGenerator.h"
#include "Battle/BattleSimulator.h"
#include "Battle/BattleFighter.h"

GMHandler gmHandler;

GMHandler::GMHandler()
{

    Reg(3, "addvar", &GMHandler::OnAddVar);
    Reg(3, "mail", &GMHandler::OnSendMail);
    Reg(3, "addfighter", &GMHandler::OnAddFighter);
    Reg(3, "addmoney",&GMHandler::OnAddMoney);
    Reg(3, "battle",&GMHandler::OnBattle);
    Reg(3, "setgvar",&GMHandler::OnSetGVar);
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
bool GMHandler::Handle( const std::string& txt, GObject::Player * player, bool isFromBackstage)
{
	UInt8 gml = 3;
	//if(cfg.GMCheck && !isFromBackstage)
	//{
	//	if(player == NULL)
	//		return false;
	//	gml = player->getGMLevel();
	//	if(gml > 3)
	//		return false;
	//}
	//if(txt.empty())
	//	return false;
	//std::string ptxt = txt;
	//if(ptxt[0] == '#')
	//{
	//	if(ptxt.length() < 9)
	//		return false;
	//	ptxt.erase(ptxt.begin(), ptxt.begin() + 9);
	//}
	//if(ptxt[0] != '/')
	//	return false;
	//ptxt.erase(ptxt.begin());
	StringTokenizer tk(txt, " ", StringTokenizer::TOK_IGNORE_EMPTY || StringTokenizer::TOK_TRIM);
	if(tk.count() == 0)
		return true;
	std::string cmd = tk[0];
	strlwr(&cmd[0]);
	std::map<std::string, GMHP>::iterator it = _procMap[gml].find(cmd);
	if(it == _procMap[gml].end())
		return true;
	std::vector<std::string> sl(tk.begin() + 1, tk.end());
	if(it->second.np)
	{
		GMHPROCNP proc = it->second.procnp;
		(this->*proc)(sl);
	}
	else
	{
		if(player == NULL)
			return false;
		GMHPROC proc = it->second.proc;
		(this->*proc)(player, sl);
	}
	return true;
}
void GMHandler::OnSendMail( GObject::Player * player, std::vector<std::string>& args )
{ 
    GObject::Mail* mail = new GObject::Mail(IDGenerator::gMailOidGenerator.ID(),player,1,"20001,20",0,static_cast<UInt32>(-1));
    if(mail)
    { 
        globalMails.add(mail->GetId(), mail);
        player->AddMail(mail->GetId());
    }
} 

void GMHandler::OnAddFighter( GObject::Player * player, std::vector<std::string>& args )
{ 
    if(args.size() != 2)
        return ;
    UInt16 id = atoi(args[1].c_str());
    player->addFighter(id);
}



void GMHandler::OnAddMoney( GObject::Player * player, std::vector<std::string>& args)
{
    if( args.size() != 2 )
        return;
    UInt8 id = atoi(args[0].c_str());
    if( id <= 0 || id > 3)
       return;
    UInt32 num = atoi(args[1].c_str());
    player->AddMoney(id , num);
}

void GMHandler::OnBattle( GObject::Player * player, std::vector<std::string>& args)
{
    if( args.size() != 2 )
        return;
    UInt16 id1 = atoi(args[0].c_str());
    GObject::Fighter* fgt1 = GObject::globalFighters[id1];
    if(!fgt1)
        return ;
    UInt16 id2 = atoi(args[1].c_str());
    GObject::Fighter* fgt2 = GObject::globalFighters[id2];
    if(!fgt2)
        return ;
    Battle::BattleFighter * bf1 = Battle::BattleSimulator::CreateFighter(fgt1->GetClass(),NULL,fgt1, 0, 0);
    Battle::BattleFighter * bf2 = Battle::BattleSimulator::CreateFighter(fgt2->GetClass(),NULL,fgt2, 0, 0);
    
    Battle::BattleSimulator bsim(bf1,bf2,1);
    bsim.start();
    UInt32 BattleReport = bsim.getId();
    cout << "GM命令 战斗ID " << BattleReport << std::endl;
}

void GMHandler::Battle(UInt16 id1, UInt16 id2)
{ 

    GObject::Fighter* fgt1 = GObject::globalFighters[id1];
    if(!fgt1)
        return ;
    GObject::Fighter* fgt2 = GObject::globalFighters[id2];
    if(!fgt2)
        return ;
    Battle::BattleFighter * bf1 = Battle::BattleSimulator::CreateFighter(fgt1->GetClass(),NULL,fgt1, 0, 0);
    Battle::BattleFighter * bf2 = Battle::BattleSimulator::CreateFighter(fgt2->GetClass(),NULL,fgt2, 0, 0);
    
    Battle::BattleSimulator bsim(bf1,bf2,1);
    bsim.start();
    UInt32 BattleReport = bsim.getId();
    cout << "GM命令 战斗ID " << BattleReport << std::endl;
}

