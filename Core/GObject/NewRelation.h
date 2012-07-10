#ifndef _NEW_RELATION_H_
#define _NEW_RELATION_H_

namespace GObject
{

class NewRelation
{
public:
    NewRelation();
    ~NewRelation();
    void LoadFromDB(UInt64 playerId, UInt8 mood, std::string& sign);
    UInt8 getMood();
    void setMood(UInt8 status);
    std::string& getSign();
    void setSign(std::string& status);
private:
    UInt64 m_playerId;
    UInt8 m_mood;
    std::string m_sign;
};

}

#endif

