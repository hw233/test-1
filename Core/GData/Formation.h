#ifndef _FORMATION_H_
#define _FORMATION_H_

#include "ObjectManager.h"
#include "AttrExtra.h"

namespace GData
{

class Formation:
	public ObjectBaseT<UInt8>
{
public:
	struct GridEffect
	{
		UInt8 pos;
		const AttrExtra* attrExtra;
		GridEffect(): pos(0), attrExtra(NULL) { }
	};
public:
	Formation(UInt8, std::string&);
	inline void clearGrids() { _grids.clear(); }
	inline void addGrid(GridEffect& ge) { _grids.push_back(ge); }
	void addGrid(int c, GridEffect* ge);
	inline int getGridCount() const { return static_cast<int>(_grids.size()); }
	inline const GridEffect& operator[](int i) const { return _grids[i]; }

private:
	std::vector<GridEffect> _grids;
};

inline void Formation::addGrid(int c, GridEffect* ge)
{
	for(int i = 0; i < c; ++ i)
	{
		_grids.push_back(ge[i]);
	}
}

typedef ObjectListT<Formation> FormationManager;
#define formationManager FormationManager::Instance()

}

#endif // _FORMATION_H_
