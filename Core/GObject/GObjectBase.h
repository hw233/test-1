#ifndef _GOBJECTBASE_H_
#define _GOBJECTBASE_H_

namespace GObject
{

template<typename T, typename K = UInt32>
class GObjectBaseT
{
public:
	static T * create(const K key) {return new(std::nothrow) T(key);}
public:
	GObjectBaseT(const K key): _id(key), _slot(static_cast<UInt32>(-1)) { }
	inline const K getId() const {return _id;}
	inline void setSlot(UInt32 slot) {_slot = slot;}
	inline const UInt32 getSlot() const {return _slot;}
protected:
	K _id;
	UInt32 _slot;
	bool _trashed;
};

}

#endif // _GOBJECTBASE_H_
