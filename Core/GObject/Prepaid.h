#ifndef _PREPAID_H_
#define _PREPAID_H_

namespace GObject
{

class Prepaid
{
public:
	void push(UInt64, UInt32, bool = true);
	UInt32 pop(UInt64);
private:
	std::map<UInt64, UInt32> _paidSet;
};

extern Prepaid prepaid;

}

#endif // _PREPAID_H_
