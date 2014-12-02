#ifndef _BATTLEFIELD_H_
#define _BATTLEFIELD_H_

#include "BattleFighter.h"
//#include "GData/Formation.h"

/* Battle Field grid indexes:
defender:	9 8 7 6 5
			4 3 2 1 0

attacker:	0 1 2 3 4
			5 6 7 8 9
*/

#define FIELD_WIDTH 20
#define FIELD_HIGH  9
#define STEP 50
namespace GObject
{
	class Player;
}

namespace Battle
{

const int MAX_REIATSU = 100;

enum fieldType
{
    filed_none = 0,
    filed_flat = 1,
    filed_glass = 2,
    filed_desert = 3,
    filed_water = 4,
};

class BattleObject;
class BattleField
{

public:
	BattleField();
	virtual ~BattleField();
	void clear();
	void reset();
	void setObject(UInt8, UInt8, BattleObject *, UInt8 = 0);
	void setObjectXY(UInt8, UInt8, UInt8, BattleObject *, bool = false, UInt8 = 0, UInt8 = 0);

	BattleObject * operator()(UInt8, UInt8);
	BattleObject * getObjectXY(UInt8, UInt8, UInt8);

    UInt8 getDistance(UInt8, UInt8, UInt8, UInt8);
    int getSpecificTarget(UInt8 side, bool(*f)(BattleObject* bo));
    bool getSpecificTargets(UInt8 side, UInt8 pos, UInt8 val, std::vector<UInt8>& poslist, bool(*f)(BattleObject* bo, UInt8 targetPos, UInt8 maxLength));
    int getPossibleTarget(UInt8, UInt8); // return -1 for no found target, overload in Simulator

	UInt32 getAliveCount(UInt8);
	UInt32 getObjHp(UInt8 x, UInt8 y);

	inline BattleObject * getObject(UInt8 side, UInt8 idx)
    {
        if(side < 0 || side > 1 || idx < 0 || idx > 24)
            return NULL;
        return _objs[side][idx];
    }
    inline void deleteObject(UInt8 side, UInt8 idx)
    {
        if(side < 0 || side > 1 || idx < 0 || idx > 24)
            return;
        if(_objs[side][idx])
        {
            //delete _objs[side][idx];
            _objs[side][idx] = NULL;
        }
    }

    BattleObject * GetTarget(UInt8 side , UInt8 posX , UInt8 posY);

protected:
	bool anyObjectInRow(UInt8, UInt8);
	void updateStats(UInt8);
	void updateStats(UInt8, UInt8);

protected:
	BattleObject * _objs[FIELD_HIGH][FIELD_WIDTH];     //战场成员
    fieldType _field[FIELD_HIGH][FIELD_WIDTH];    //场地信息
};

}

#endif // _BATTLEFIELD_H_
