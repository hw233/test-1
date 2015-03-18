#ifndef _A_STAR_H_
#define _A_STAR_H_

#include <math.h>
#include <list>
#include <vector>
#include <string>
#include "Config.h"

namespace GObject
{
    struct ASCOORD
    {
	    int _x;
	    int _y;

	    ASCOORD()
	    {
	    }
	
	    ASCOORD(int x,int y):
		    _x(x),
		    _y(y)
	    {
	    }

	    inline bool operator == (const ASCOORD& coord)const
	    {
		    return this->_x == coord._x && this->_y == coord._y;
	    }

	    inline bool operator != (const ASCOORD& coord)const
	    {
		    return this->_x != coord._x || this->_y != coord._y;
	    }

	    inline void operator += (const ASCOORD& coord)
	    {
		    this->_x += coord._x ;
		    this->_y += coord._y;
	    }
    };

    struct StepData
    {
	    UInt16 _h;
	    UInt16 _g;

	    ASCOORD _coord;
	    ASCOORD _parentCoord;

	    StepData()
	    {
	    }

	    StepData(const ASCOORD& coord):
		    _coord(coord)
	    {
	    }
    };

    class AStar
    {
    public:
        AStar();

	    bool SetMapSize(UInt8 row,UInt8 col);

	    bool ComputeRoute();

	    bool GetRoute(std::vector<ASCOORD>* list);

	    void SetObstacle(const ASCOORD& coord);

	    void CancleObstacle(const ASCOORD& coord);

	    void ClearObstacles();

	    bool IsObstacle(const ASCOORD& coord);
	
	    void  SetStart(const ASCOORD& coord)  {m_start  = coord;}
	    void  SetTarget(const ASCOORD& coord) {m_target = coord;}
	    const ASCOORD& GetStart()            {return m_start;}
	    const ASCOORD& GetTarget()           {return m_target;}
        void  SetMap(UInt8* map);

    private:
        UInt8*  m_map;

	    ASCOORD m_target;
	    ASCOORD m_start;

        UInt8   m_row;
        UInt8   m_col;

	    UInt8   m_numSurround;
	    UInt8   m_gAdd[6];
	    ASCOORD m_surround[6];  //正六边形
	
	    std::list<StepData> m_openList;
	    std::list<StepData> m_closeList;
	
	    bool popBestStep(StepData* pSD); 
	    bool isInList(std::list<StepData>& list, const ASCOORD& coord);
	    bool judgeSurround(const ASCOORD& coord,const ASCOORD& parentCoord, UInt8 G);

	    StepData* findFromList(std::list<StepData>& list, const ASCOORD& coord);

	    UInt8 computeH(const ASCOORD& coord) {return abs(m_target._x-coord._x)*10 + abs(m_target._y-coord._y)*10;}
        void getAround(ASCOORD& coord);
    };
}
#endif
