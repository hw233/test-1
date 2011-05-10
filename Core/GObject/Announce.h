#ifndef _ANNOUNCE_H_
#define _ANNOUNCE_H_

namespace GObject
{

struct AnnItem
{
	std::string text;
	std::string link;
	UInt32 interval;
	UInt8 type;
	UInt32 timeEnd;
};

class Announce
{
public:
	Announce();
	void reload();
	void process(UInt32);
private:
	std::multimap<UInt32, AnnItem> _items;
};

extern Announce announce;

}

#endif // _ANNOUNCE_H_
