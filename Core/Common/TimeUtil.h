#ifndef _TIMEUTIL_H_
#define _TIMEUTIL_H_

#ifndef _WIN32
#include <sys/time.h>
#endif

class TimeUtil
{
public:
	static inline void Init() { tzset(); }
	static inline UInt32 Now() { return static_cast<UInt32>(time(NULL)); }
	static inline UInt8 Day(UInt32 now = Now())
	{
		time_t now2 = static_cast<time_t>(now);
		struct tm * local = localtime(&now2);
		return static_cast<UInt8>(local->tm_wday);
	}
	static inline UInt32 SharpDayT(int c = 0, UInt32 cur = Now())
    {
        time_t t = cur;
        struct tm* t_tm = localtime(&t);
        t_tm->tm_hour = 0;
        t_tm->tm_min = 0;
        t_tm->tm_sec = 0;
        time_t t2 = mktime(t_tm);
		return t2 + c * 24 * 60 * 60;
    }
	static inline UInt32 SharpDay(int c = 0, UInt32 cur = Now())
	{
		UInt32 tmptm = (cur + timezone) / 86400 * 86400 + timezone;
		if(tmptm > cur)
			tmptm -= 86400;
		else if(tmptm + 86400 <= cur)
			tmptm += 86400;
		return tmptm + c * 86400;
	}
	static inline UInt32 SharpHour(int c, UInt32 tm = Now())
	{
		return (tm + c * 3600) / 3600 * 3600;
	}
	static inline UInt32 SharpFourtyMin(int c, UInt32 tm = Now())
	{
		return (tm + c * 2400) / 2400 * 2400;
	}
	static inline UInt32 SharpMinute(int c, UInt32 tm = Now())
	{
		return (tm + c * 60) / 60 * 60;
	}
	static inline UInt64 GetTick()
	{
#ifdef _WIN32
		FILETIME ft;
		GetSystemTimeAsFileTime(&ft);
		ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
		epoch.LowPart  = 0xD53E8000;
		epoch.HighPart = 0x019DB1DE;

		ULARGE_INTEGER ts;
		ts.LowPart  = ft.dwLowDateTime;
		ts.HighPart = ft.dwHighDateTime;
		ts.QuadPart -= epoch.QuadPart;
		return ts.QuadPart / 10;
#else
		struct timeval tv;
		if(gettimeofday(&tv, NULL))
			return 0;
		return tv.tv_sec * 1000000 + tv.tv_usec;
#endif
	}
};

#endif // _TIMEUTIL_H_
