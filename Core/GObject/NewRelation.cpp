#include "Config.h"
#include "Player.h"
#include "NewRelation.h"

namespace GObject
{

NewRelation::NewRelation():m_playerId(0), m_mood(0), m_sign("")
{
}

NewRelation::~NewRelation()
{
}

void NewRelation::LoadFromDB(UInt64 playerId, UInt8 mood, std::string& sign)
{
    std::cout << "[test]" << static_cast<UInt32>(mood) << sign << std::endl;
    m_playerId = playerId;
    m_mood = mood;
    m_sign = sign;
}

UInt8 NewRelation::getMood()
{
    return m_mood;
}

void NewRelation::setMood(UInt8 status)
{
    if(status != m_mood)
    {
        m_mood = status;
        DB().PushUpdateData("REPLACE INTO `new_relation` (`playerId`, `mood`, `sign`) VALUES (%"I64_FMT"u, %u, '%s')", m_playerId, m_mood, m_sign.c_str());
    }
}

std::string& NewRelation::getSign()
{
    return m_sign;
}

void NewRelation::setSign(std::string& status)
{
    if(status == m_sign)
        return;

    m_sign = status;
    DB().PushUpdateData("REPLACE INTO `new_relation` (`playerId`, `mood`, `sign`) VALUES (%"I64_FMT"u, %u, '%s')", m_playerId, m_mood, m_sign.c_str());
}

}


