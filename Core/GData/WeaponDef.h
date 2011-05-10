#ifndef _WEAPONDEF_H_
#define _WEAPONDEF_H_

#include "ObjectManager.h"
#include "Area.h"

namespace GData
{

class WeaponDef:
	public ObjectBaseT<UInt8>
{
public:
	WeaponDef(UInt8 id, std::string n, UInt8 cr, bool cc, Area& area): ObjectBaseT<UInt8>(id, n), _clsReq(cr), _canCounter(cc), _area(area) {}
	inline Area& getArea() const { return _area; }
	inline bool canCounter() const { return _canCounter; }
	inline bool isClassWeapon(UInt8 cls) const {return (_clsReq & (1 << ((cls - 1) / 2))) > 0;}
private:
	UInt8 _clsReq;
	bool _canCounter;
	Area& _area;
};

typedef ObjectMapT<WeaponDef, UInt8> WeaponDefManager;
#define weaponDefManager WeaponDefManager::Instance()

}

#endif // _WEAPONDEF_H_
