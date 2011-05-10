#include "Config.h"
#include "Formation.h"

namespace GData
{

Formation::Formation( UInt8 id, std::string n, UInt8 k, UInt16 s, int c, UInt8 * eyes ):
	ObjectBaseT<UInt8>(id, n), _keepNum(k), _skill(s)
{
	setEyes(c, eyes);
}

}
