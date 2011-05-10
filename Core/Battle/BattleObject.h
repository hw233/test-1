#ifndef _BATTLEOBJECT_H_
#define _BATTLEOBJECT_H_

namespace Battle
{

class BattleObject
{
public:
	enum Class
	{
		Char = 0,
		Stone = 253,
		Tree = 254,
		Water = 255
	};
public:
	BattleObject(Class c, UInt8 s, UInt8 p): _cls(c), _hp(0), _side(s), _pos(p) {}

	inline void setSideAndPos(UInt8 s, UInt8 p) { _side = s; _pos = p; }

	inline void setHP(UInt32 u) {_hp = u;}

	inline Class getClass() {return _cls;}
	inline UInt32 getHP() {return _hp;}

	inline void makeDamage(UInt32 u);

	inline bool isChar() { return _cls == Char; }

	inline UInt8 getSide() { return _side; }
	inline UInt8 getPos() { return _pos; }

protected:
	Class _cls;
	UInt32 _hp;
	UInt8 _side, _pos;
};

inline void BattleObject::makeDamage( UInt32 u )
{
	if(_hp < u)
		_hp = 0;
	else
		_hp -= u;
}

}

#endif // _BATTLEOBJECT_H_
