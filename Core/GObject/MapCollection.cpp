#include "Config.h"
#include "MapCollection.h"

#include "Server/ServerTypes.h"

namespace GObject
{

MapCollection mapCollection;

void MapCollection::setCountry( UInt8 id, UInt8 country )
{
	if(id >= _mapToCountry.size())
		_mapToCountry.resize(id + 1);
	_mapToCountry[id] = country;
}

UInt8 MapCollection::getCountry( UInt8 id )
{
	if(id >= _mapToCountry.size())
		return 255;
	return _mapToCountry[id];
}

UInt8 MapCollection::getCountryFromSpot( UInt16 spot )
{
	return getCountry(spot >> 8);
}

}
