#include "Config.h"
#include "Area.h"

namespace GData
{

std::map<UInt32, Area> areaList;

void Area::add( Area::Data& d )
{
	_data.push_back(d);
}

Area::Data& Area::operator[]( int idx )
{
	static Data e = {0, 0, 0, 1.0f};
	if(idx < 0 || idx >= static_cast<int>(_data.size()))
		return e;
	return _data[idx];
}

Area& Area::getDefault()
{
	static Area area;
	static bool inited = false;
	if(!inited)
	{
		inited = true;
		Data d = {0, 0, 0, 1.0f};
		area.add(d);
	}
	return area;
}

}
