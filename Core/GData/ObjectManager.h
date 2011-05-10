#ifndef _OBJECTMANAGER_H_
#define _OBJECTMANAGER_H_

#include "Common/Singleton.h"
#include "ObjectBase.h"
#include "Common/Mutex.h"

namespace GData
{

template<typename _OIT = ObjectBaseT<>, typename _OKT = UInt32 >
class ObjectMapT:
	public Singleton<ObjectMapT<_OIT, _OKT> >
{
typedef
	std::map<_OKT, _OIT *> ObjMap;
	typedef typename ObjMap::const_iterator const_obj_iterator;
public:
	void add(_OIT * item)
	{
		_objs[item->getId()] = item;
	}
	void add(const _OKT& key, _OIT * elem)
	{
		_objs[key] = elem;
	}
	const _OIT* operator[](_OKT key)
	{
		const_obj_iterator it = _objs.find(key);
		if(it == _objs.end())
			return NULL;
		return it->second;
	}
	typedef bool (*EnumCB)(const _OKT&, const _OIT *, void *);
	void enumerate(EnumCB cb, void * param)
	{
		typename ObjMap::const_iterator it;
		for(it = _objs.begin(); it != _objs.end(); ++ it)
		{
			if(!cb(it->first, it->second, param))
				break;
		}
	}
protected:
	ObjMap _objs;
};

template<typename _OIT = ObjectBaseT<> >
class ObjectListT:
	public Singleton<ObjectListT<_OIT> >
{
	typedef std::deque<_OIT *> ObjList;
	typedef typename ObjList::const_iterator const_obj_iterator;
public:
	void add(_OIT * item)
	{
		if(_objs.size() <= item->getId())
			_objs.resize((item->getId() + 0x40) & ~0x3F);
		_objs[item->getId()] = item;
	}
	const _OIT* operator[](UInt32 key)
	{
		if(key >= _objs.size())
			return NULL;
		return _objs[key];
	}
	typedef bool (*EnumCB)(const _OIT *, void *);
	void enumerate(EnumCB cb, void * param)
	{
		typename ObjList::const_iterator it;
		for(it = _objs.begin(); it != _objs.end(); ++ it)
		{
			if(*it == NULL)
				continue;
			if(!cb(*it, param))
				break;
		}
	}
protected:
	ObjList _objs;
};

}

#endif // _OBJECTMANAGER_H_
