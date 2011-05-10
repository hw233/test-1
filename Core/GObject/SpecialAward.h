#ifndef _SPECIALAWARD_H_
#define _SPECIALAWARD_H_

namespace GObject
{

class Player;
class SpecialAward
{
public:
	SpecialAward();
	~SpecialAward();

public:
	void AddAwardListFromDB(UInt8 type, Player *player);
	void AddSpecialAwardList(UInt8 type, Player *player);
public:
	UInt8 getAwardSize(UInt8 type);
	void AthleticsAward();
public:
	void luckydrawAward(Player *);

public:
	void newServerActivity(Player *pl, UInt8 challengeNum);

private:
	std::vector<Player *> m_SpecialAward[4];

};


extern SpecialAward gSpecialAward;

}

#endif // _SPECIALAWARD_H_
