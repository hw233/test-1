#ifndef _NEW_RELATION_H_
#define _NEW_RELATION_H_

#include "Player.h"
namespace GObject
{
struct Lineup;
class NewRelation
{
public:
    NewRelation(Player* player);
    ~NewRelation();
    void LoadFromDB(UInt8 mood, std::string& sign);
    UInt8 getMood();
    void setMood(UInt8 status);
    std::string& getSign();
    void setSign(std::string& status);
    void beAttack(Player * atker, UInt16 formation, UInt16 portrait, Lineup * lineup, Player *defer);
    void attack(Player *atker, Player *defer);
    void countrySend(Player* responder, UInt8 type, std::string& name);
    void challengeRequest(Player *atker, std::string& name);
    void challengeRespond(Player *responder, std::string& name, UInt8 accept);
private:
    Player* _owner;
    UInt8 m_mood;
    std::string m_sign;
};

}

#endif

