#ifndef _OBJECTBASE_H_
#define _OBJECTBASE_H_

namespace GData
{

template<typename _OKT = UInt32>
class ObjectBaseT
{
public:
	ObjectBaseT(_OKT id, const std::string& n = ""): _id(id), _name(n) {}
	inline const _OKT getId() const {return _id;}
	inline const std::string& getName() const {return _name;}
	inline void setName(const std::string& n) { _name = n; }
protected:
	_OKT _id;
	std::string _name;
};

template<typename _OKT = UInt32>
class ObjectBaseNT
{
public:
	ObjectBaseNT(_OKT id): _id(id) {}
	inline const _OKT getId() const {return _id;}
protected:
	_OKT _id;
};

}

#endif // _OBJECTBASE_H_
