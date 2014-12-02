#include "Country.h"
#include "Common/Itoa.h"
namespace GObject
{
    Country::Country(UInt8 id):m_ThreadID(id)
    { 

    } 
    bool Country::Init()
    {
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
