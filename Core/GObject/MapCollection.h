#ifndef _MAPCOLLECTION_H_
#define _MAPCOLLECTION_H_

namespace GObject
{

class MapCollection
{
public:
	void setCountry(UInt8 id, UInt8 country);
	UInt8 getCountry(UInt8 id);
	UInt8 getCountryFromSpot(UInt16 spot);
private:
	std::vector<UInt8> _mapToCountry;
};

extern MapCollection mapCollection;

}

#endif // _MAPCOLLECTION_H_
