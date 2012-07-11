#ifndef _GLOCALOBJECTMANAGER_H_
#define _GLOCALOBJECTMANAGER_H_

#include "GObjectBase.h"
#include "Common/Mutex.h"
#include <list>
#include <vector>

namespace GObject
{

template<typename _V >
class GLocalObjectManagerT
{
public:
	bool add(_V* val)
	{
		UInt32 slot = val->getSlot();
		if(slot != static_cast<UInt32>(-1))
		{
			return false;
		}
		if(_empty.empty())
		{
			_objs.push_back(val);
			val->setSlot(static_cast<UInt32>(_objs.size() - 1));
		}
		else
		{
			size_t id = _empty.front();
			_objs[id] = val;
			val->setSlot(static_cast<UInt32>(id));
			_empty.pop_front();
		}
		return true;
	}
	void remove(_V* val)
	{
		UInt32 slot = val->getSlot();
		if(_objs.size() > slot && _objs[slot] != NULL && _objs[slot] == val)
		{
			_objs[slot]->setSlot(static_cast<UInt32>(-1));
			_objs[slot] = NULL;
			_empty.push_back(slot);
		}
	}
	void remove(UInt32 slot)
	{
	}
	_V * operator[] (const UInt32 slot)
	{
		if(_objs.size() > slot)
		{
			if(_objs[slot]->getSlot() == slot)
				return _objs[slot];
			_objs[slot] = NULL;
		}
		return NULL;
	}

protected:
	std::vector<_V *> _objs;
	std::list<size_t> _empty;
};

}

#endif // _GLOCALOBJECTMANAGER_H_
