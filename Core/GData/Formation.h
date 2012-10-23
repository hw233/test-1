#ifndef _FORMATION_H_
#define _FORMATION_H_

#include "ObjectManager.h"
#include "AttrExtra.h"

#define FORMATION_1  1201
#define FORMATION_2  901

#define FORMATION_LEVEL_MAX 100
#define FORMATION_ID(x)  (((UInt16)(x))/FORMATION_LEVEL_MAX)
#define FORMATION_LEVEL(x) (((UInt16)(x))%FORMATION_LEVEL_MAX)

namespace GData
{

class Formation:
	public ObjectBaseT<UInt16>
{
public:
	struct GridEffect
	{
		UInt8 pos;
		const AttrExtra* attrExtra;
		GridEffect(): pos(0), attrExtra(NULL) { }
	};
public:
	Formation(UInt16, std::string&);
	inline void clearGrids() { _grids.clear(); }
	inline void addGrid(GridEffect& ge) { _grids.push_back(ge); std::sort(_grids.begin(), _grids.end(), Testless); }
	inline void addLevUpItem(UInt32 itemId) { _levUpItem.push_back(itemId); }
	void addGrid(int c, GridEffect* ge);
	inline int getGridCount() const { return static_cast<int>(_grids.size()); }
	inline const GridEffect& operator[](int i) const { return _grids[i]; }
	inline int getLevUpItemCount() const { return static_cast<int>(_levUpItem.size()); }
	inline const UInt32 LevUpItem(int i) const { return _levUpItem[i]; }
    inline void setBattlePoint(UInt16 v) { _battlePoint = v; }
    inline UInt16 getBattlePoint() const { return _battlePoint; }

    static bool Testless(const GridEffect& a, const GridEffect& b)
    {
        return a.pos < b.pos;
    }
    inline UInt8 getPos(int gridIdx) const {return _grids[gridIdx].pos;}
private:
	std::vector<GridEffect> _grids;
    std::vector<UInt32> _levUpItem;
    UInt16 _battlePoint;
};

inline void Formation::addGrid(int c, GridEffect* ge)
{
	for(int i = 0; i < c; ++ i)
	{
		_grids.push_back(ge[i]);
        std::sort(_grids.begin(), _grids.end(), Testless);
	}
}

typedef ObjectMapT<Formation> FormationManager;
#define formationManager FormationManager::Instance()

}

#endif // _FORMATION_H_
