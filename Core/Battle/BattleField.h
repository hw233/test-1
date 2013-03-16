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

const int MAX_REIATSU = 100;

enum ReiatsuType
{
    e_reiatsu_normal_attack = 1,
    e_reiatsu_skill_attack = 2,
    e_reiatsu_peerless = 3,
    e_reiatsu_round = 4,
};

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
    void setPetObject(int, BattleObject *, UInt8 = 0);

    bool addReiatsu(int, int);
    int getReiatsu(int side);
    inline int getToggleReiatsu(int side) 
    {
        if (side < 0 || side >= 2)
            return 0;
        return _toggleReiatsu[side];
    }

	BattleObject * operator()(int, int);
	BattleObject * getObjectXY(int, int, int);
	int getDistance(int, int, int, int);
    int getSpecificTarget(int side, bool(*f)(BattleObject* bo));
	int getPossibleTarget(int, int); // return -1 for no found target, overload in Simulator
	void updateDistance();
	void updateDistance(int, int);
	void getFormationPositions(int, std::vector<UInt8>&);
	UInt32 getAliveCount(int);
	UInt32 getObjHp(int);
	UInt32 getMaxObjHp(int);
	inline bool isBody(int side, int pos) { return _isBody[side][pos] > 0; }

	inline BattleObject * getObject(int side, int idx)
    {
        if(side < 0 || side > 1 || idx < 0 || idx > 24)
            return NULL;
        return _objs[side][idx];
    }
    inline void deleteObject(int side, int idx)
    {
        if(side < 0 || side > 1 || idx < 0 || idx > 24)
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
    BattleObject * _backupObjs[2];
	UInt8 _isBody[2][25];
    UInt8 _reiatsu[2];          // 战场中双方的灵压(这单词是网上搜的，就是灵压的意思)
    UInt8 _toggleReiatsu[2];    // 后备军团上场所需的灵压（仙宠上场）
    UInt8 _backupTargetPos[2];  // 备胎上场时的位置
	const GData::Formation * _formation[2];
};

}

#endif // _BATTLEFIELD_H_
