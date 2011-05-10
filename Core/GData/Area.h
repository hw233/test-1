#ifndef _AREA_H_
#define _AREA_H_

namespace GData
{

class Area
{
public:
	struct Data
	{
		Int8 x, y;
		Int8 type;
		float factor;
	};
public:
	Area(UInt8 s = 1): _side(s) {}
	inline int getCount() { return _data.size(); }
	void add(Data&);
	Data& operator[](int);
	inline void setSide(UInt8 s) { _side = s; }
	inline UInt8 getSide() { return _side; }
	static Area& getDefault();
private:
	std::vector<Data> _data;
	UInt8 _side;
};

extern std::map<UInt32, Area> areaList;

}

#endif // _AREA_H_
