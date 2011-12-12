#include "Config.h"
#include "URandom.h"
#include "BaseThread.h"

URandom::URandom()
{
	seed();
}

URandom::URandom( UInt32 rseed )
{
	seed(rseed);
}

UInt32 URandom::operator()()
{
	return next();
}

UInt32 URandom::operator()( UInt32 modulo )
{
    if (!modulo)
        return 0;
	return next() % modulo;
}

UInt32 uRand()
{
	return static_cast<BaseThread *>(Thread::current())->uRandom();
}

UInt32 uRand(UInt32 modulo)
{
	return static_cast<BaseThread *>(Thread::current())->uRandom(modulo);
}

extern "C" unsigned int uRandom(unsigned int modulo)
{
	return uRand(modulo);
}
