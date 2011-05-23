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
		UInt8 link;
		UInt8 link_ratio;
		UInt8 rescue;
		UInt8 rescue_ratio;
		const AttrExtra * attrExtra;
		GridEffect(): pos(0), link(0), link_ratio(0), rescue(0), rescue_ratio(0),
			attrExtra(NULL) { }
	};
public:
	Formation(UInt8, std::string, UInt8, UInt16, int, UInt8 *);
	inline void clearGrids() { _grids.clear(); }
	inline void addGrid(GridEffect& ge) { _grids.push_back(ge); }
	void addGrid(int c, GridEffect* ge);
	inline void setEyes(int c, UInt8 * eyes) { _formationEye.clear(); for(int i = 0; i < c; ++ i) _formationEye.push_back(eyes[i]); }
	inline int getGridCount() const { return static_cast<int>(_grids.size()); }
	inline const GridEffect& operator[](int i) const { return _grids[i]; }
	inline UInt8 getKeepNum() const { return _keepNum; }
	inline void setKeepNum(UInt8 n) { _keepNum = n; }
	inline bool isEye(int) const;

private:
	UInt8 _keepNum;
	UInt16 _skill;
	std::vector<UInt8> _formationEye;
	std::vector<GridEffect> _grids;
};

inline void Formation::addGrid(int c, GridEffect* ge)
{
	for(int i = 0; i < c; ++ i)
	{
		_grids.push_back(ge[i]);
	}
}

inline bool Formation::isEye( int pos ) const
{
	size_t c = _formationEye.size();
	for(size_t i = 0; i < c; ++ i)
	{
		if(pos == _formationEye[i])
			return true;
	}
	return false;
}

typedef ObjectListT<Formation> FormationManager;
#define formationManager FormationManager::Instance()

}

#endif // _FORMATION_H_
