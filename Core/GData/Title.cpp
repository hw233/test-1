#include "Config.h"
#include "Title.h"

namespace GData
{

TitleList titleList;

void TitleList::set( int idx, const char * t )
{
	if(idx >= static_cast<int>(_titles.size()))
		_titles.resize(idx + 1);
	_titles[idx] = t;
}

const std::string& TitleList::operator[]( size_t idx )
{
	static std::string empty;
	if(idx >= _titles.size())
		return empty;
	return _titles[idx];
}

}
