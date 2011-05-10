#ifndef _EVENTTIMER_INC_
#define _EVENTTIMER_INC_

#include "Common/TimeUtil.h"

class EventTimer
{
public:
	static const UInt32 PermTimer = 0xFFFFFFFF;
	EventTimer() {}
	EventTimer(UInt32 interval, UInt32 count = 1):
	m_Interval(interval), m_Count(count)
	{}
	virtual ~EventTimer() {}

public:
	void Clear()
	{
		m_Interval = 0;
		m_Count = 0;
	}

	inline bool DecCount()
	{
		if(m_Count == 0) return false;
		if(PermTimer == m_Count || (-- m_Count) > 0)
			return true;
		return false;
	}

	inline bool IsActive() const { return m_Count != 0; }
	inline UInt32 GetInterval() const { return m_Interval; }
	inline UInt32 GetLeftTimes() const { return m_Count; }
	inline void SetLeftTimes(UInt32 n) { m_Count = n; }

protected:
	UInt32 m_Interval;
	UInt32 m_Count;
};

#endif
