#ifndef _TITLE_H_
#define _TITLE_H_

namespace GData
{

class TitleList
{
public:
	void set(int idx, const char * t);
	const std::string& operator[](size_t idx);
private:
	std::vector<std::string> _titles;
};

extern TitleList titleList;

}

#endif // _TITLE_H_
