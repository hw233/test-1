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
	BattleObject(Class c, UInt8 s, UInt8 p): _cls(c), _hp(0), _side(s), _pos(p), _hide(false), _shieldObj(NULL) {}

	inline void setSideAndPos(UInt8 s, UInt8 p) { _side = s; _pos = p; }

	inline void setHP(UInt32 u) {_hp = u;}

	inline Class getClass() {return _cls;}
	inline UInt32 getHP() {return _hp;}

	virtual void makeDamage(UInt32& u)
    {
        if(_hp < u)
            _hp = 0;
        else
            _hp -= u;
    }



	inline bool isChar() { return _cls == Char; }

	inline UInt8 getSide() { return _side; }
	inline UInt8 getPos() { return _pos; }
    inline bool isHide() { return _hide; }
    inline void setHide(bool hide) { _hide = hide; }
    inline void setShieldObj(BattleObject* obj) { if(_shieldObj != obj) _shieldObj = obj; }
    inline BattleObject* getShieldObj() { return _shieldObj; }

protected:
	Class _cls;
	UInt32 _hp;
	UInt8 _side, _pos;
    bool _hide;
    BattleObject* _shieldObj;
};

}

#endif // _BATTLEOBJECT_H_
