#ifndef _BATTLEFIELD_H_
#define _BATTLEFIELD_H_

#include "BattleFighter.h"
#include "GData/Formation.h"

/* Battle Field grid indexes:
defender:	9 8 7 6 5
			4 3 2 1 0

attacker:	0 1 2 3 4
			5 6 7 8 9
*/

namespace GObject
{
	class Player;
}

namespace Battle
{

class BattleField
{
public:
	BattleField();
	~BattleField();
	void clear();
	void reset();
	void setObject(int, int, BattleObject *, UInt8 = 0);
	void setObjectXY(int, int, int, BattleObject *, bool = false, UInt8 = 0, UInt8 = 0);
	void setFormation(int, UInt32);

	BattleObject * operator()(int, int);
	BattleObject * getObjectXY(int, int, int);
	int getDistance(int, int, int, int);
	int getPossibleTarget(int, int); // return -1 for no found target
	void updateDistance();
	void updateDistance(int, int);
	void getFormationPositions(int, std::vector<UInt8>&);
	UInt32 getAliveCount(int);
	UInt32 getObjHp(int);
	UInt32 getMaxObjHp(int);
	inline bool isBody(int side, int pos) { return _isBody[side][pos] > 0; }

	inline BattleObject * getObject(int side, int idx)
    {
        if(side != 0 || side != 1 || idx < 0 || idx > 24)
            return NULL;
        return _objs[side][idx];
    }
    inline void deleteObject(int side, int idx)
    {
        if(side != 0 || side != 1 || idx < 0 || idx > 24)
            return;
        if(_objs[side][idx])
        {
            delete _objs[side][idx];
            _objs[side][idx] = NULL;
        }
    }
protected:
	bool anyObjectInRow(int, int);
	void updateStats(int);
	void updateStats(int, int);

protected:
	BattleObject * _objs[2][25];
	UInt8 _isBody[2][25];
	const GData::Formation * _formation[2];
};

}

#endif // _BATTLEFIELD_H_
