#ifndef _GGLOBALOBJECTMANAGER_H_
#define _GGLOBALOBJECTMANAGER_H_

#include "GObjectBase.h"
#include "Common/Compat.h"

#include <set>
#include <unordered_map>
#include "Common/Mutex.h"

namespace GObject
{

template<typename _V>
class Visitor
{
public:
    virtual ~Visitor(){}

    virtual bool operator()(_V* ptr) = 0;
};

template<typename _V, typename _VK >
class GGlobalObjectManagerT
{
public:
    typedef typename std::unordered_map<_VK, _V * >::iterator iterator;
public:
	_V * newObject(const _VK key)
	{
		Mutex::ScopedLock lk(_objMutex);
		_V * v = _V::create(key);
		_objs[v->getId()] = v;
		return v;
	}
	template<typename T1>
	_V * newObject(const _VK key, T1& v1)
	{
		Mutex::ScopedLock lk(_objMutex);
		_V * v = _V::create(key, v1);
		_objs[v->getId()] = v;
		return v;
	}
	template<typename T1, typename T2>
	_V * newObject(const _VK key, T1& v1, T2& v2)
	{
		Mutex::ScopedLock lk(_objMutex);
		_V * v = _V::create(key, v1, v2);
		_objs[v->getId()] = v;
		return v;
	}
	template<typename T1, typename T2, typename T3>
	_V * newObject(const _VK key, T1& v1, T2& v2, T3& v3)
	{
		Mutex::ScopedLock lk(_objMutex);
		_V * v = _V::create(key, v1, v2, v3);
		_objs[v->getId()] = v;
		return v;
	}
	template<typename T1, typename T2, typename T3, typename T4>
	_V * newObject(const _VK key, T1& v1, T2& v2, T3& v3, T4& v4)
	{
		Mutex::ScopedLock lk(_objMutex);
		_V * v = _V::create(key, v1, v2, v3, v4);
		_objs[v->getId()] = v;
		return v;
	}
	template<typename T1, typename T2, typename T3, typename T4, typename T5>
	_V * newObject(const _VK key, T1& v1, T2& v2, T3& v3, T4& v4, T5& v5)
	{
		Mutex::ScopedLock lk(_objMutex);
		_V * v = _V::create(key, v1, v2, v3, v4, v5);
		_objs[v->getId()] = v;
		return v;
	}
	void add(_V* val)
	{
		Mutex::ScopedLock lk(_objMutex);
		_objs[val->getId()] = val;
	}
	void add(const _VK& key, _V* val)
	{
		Mutex::ScopedLock lk(_objMutex);
		_objs[key] = val;
	}
	void remove(const _VK& key)
	{
		Mutex::ScopedLock lk(_objMutex);
		_objs.erase(key);
	}
	_V * operator[] (const _VK& key)
	{
		Mutex::ScopedLock lk(_objMutex);
		typename std::unordered_map<_VK, _V * >::const_iterator it = _objs.find(key);
		if(it == _objs.end())
		{
			return NULL;
		}
		return it->second;
	}
	inline size_t size()
	{
		Mutex::ScopedLock lk(_objMutex);
		return _objs.size();
	}
	template<typename _CBT, typename _PT>
	void enumerate(_CBT cb, _PT param)
	{
		Mutex::ScopedLock lk(_objMutex);
		typename std::unordered_map<_VK, _V * >::iterator it;
		for(it = _objs.begin(); it != _objs.end(); ++ it)
		{
			if(!cb(it->second, param))
				return;
		}
	}

    void reset()
    {
        Mutex::ScopedLock lk(_objMutex);
        typename std::unordered_map<_VK, _V * >::iterator it;
        for(it = _objs.begin(); it != _objs.end(); ++ it)
        {
            delete it->second;
        }
        _objs.clear();
    }

    inline void resetExcept(std::set<UInt64>& eset)
    {
        Mutex::ScopedLock lk(_objMutex);
        typename std::unordered_map<_VK, _V *>::iterator it = _objs.begin();
        while(it != _objs.end())
        {
            if(eset.find(it->first) == eset.end())
            {
                delete it->second;
                _objs.erase(it ++);
            }
            else
                ++ it;
        }
    }

    inline _V* find(UInt64 id, int channelId, int serverId)
    {
        UInt64 pid = (id & 0xFFFF00FFFFFFFFFFull) | (static_cast<UInt64>(channelId) << 40);
        if((pid >> 48) == 0)
            pid |= static_cast<UInt64>(serverId) << 48;
        return operator[](pid);
    }

    void enumerate(Visitor<_V>& visitor)
    {
        Mutex::ScopedLock lk(_objMutex);
        typename std::unordered_map<_VK, _V * >::iterator it;
		for(it = _objs.begin(); it != _objs.end(); ++ it)
		{
			if(!visitor(it->second))
				return;
		}
    }

	inline Mutex& getMutex() { return _objMutex; }
	std::unordered_map<_VK, _V * >& getMap() { return _objs; }

    inline iterator begin() { return _objs.begin(); }
    inline iterator end() { return _objs.end(); }

protected:
	std::unordered_map<_VK, _V * > _objs;
	Mutex _objMutex;
};

template<class T>
class GGlobalObjectManagerIStringT:
	public GGlobalObjectManagerT<T, std::string>
{
public:
	void add(const std::string& key, T * val)
	{
		std::string key_ = key;
		strlwr(&key_[0]);
		GGlobalObjectManagerT<T, std::string>::add(key_, val);
	}
	void remove(const std::string& key)
	{
		std::string key_ = key;
		strlwr(&key_[0]);
		GGlobalObjectManagerT<T, std::string>::remove(key_);
	}
	T * operator[] (const std::string& key)
	{
		std::string key_ = key;
		strlwr(&key_[0]);
		return GGlobalObjectManagerT<T, std::string>::operator[](key_);
	}
};

}

#endif // _GGLOBALOBJECTMANAGER_H_
