#include "Country.h"
#include "Common/Itoa.h"
#include "Server/Cfg.h"
#include "Script/BattleFormula.h"
namespace GObject
{
    Country::Country(UInt8 id):m_ThreadID(id),m_BattleFormula(NULL)
    { 

    } 
    bool Country::Init()
    {
        std::string path = cfg.scriptPath + "main.lua";
        path = cfg.scriptPath + "formula/main.lua";
        m_BattleFormula = new Script::BattleFormula(path.c_str()) ;
        return true; 
    }
    void Country::UnInit() 
    {

    }
    std::string Country::GetLogName()       
    {
        return "log/Country" + Itoa(TID()) + "/";
    }
}
