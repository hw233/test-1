#ifndef _A_STAR_H_
#define _A_STAR_H_

#include <math.h>
#include <list>
#include <vector>
#include <string>
#include "Config.h"
#include "../Battle/BattleObject.h"

namespace GObject
{
    struct Ascoord
    {
	    int _x;
	    int _y;

	    Ascoord()
	    {
	    }
	
	    Ascoord(int x,int y):
		    _x(x),
		    _y(y)
	    {
	    }

	    inline bool operator == (const Ascoord& coord)const
	    {
		    return this->_x == coord._x && this->_y == coord._y;
	    }

	    inline bool operator != (const Ascoord& coord)const
	    {
		    return this->_x != coord._x || this->_y != coord._y;
	    }

	    inline void operator += (const Ascoord& coord)
	    {
		    this->_x += coord._x ;
		    this->_y += coord._y;
	    }
    };

    struct StepData
    {
	    UInt16 _h;
	    UInt16 _g;

	    Ascoord _coord;
	    Ascoord _parentCoord;

	    StepData()
	    {
	    }

	    StepData(const Ascoord& coord):
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

	    bool GetRoute(std::vector<Ascoord>* list);

	    void SetObstacle(const Ascoord& coord);

	    void CancleObstacle(const Ascoord& coord);

	    void ClearObstacles();

	    bool IsObstacle(const Ascoord& coord);
	
	    void  SetStart(const Ascoord& coord)  {  m_start  = coord;  }
	    void  SetTarget(const Ascoord& coord) {  m_target = coord;  }
	    const Ascoord& GetStart()            {   return m_start;  }
	    const Ascoord& GetTarget()           {   return m_target; }
        void  SetMap(UInt8* map);
        void  SetBattleInfo(UInt8* map);
        void  SetSide(UInt8 side) { m_side = side; }
    private:
        UInt8   m_side;   //属于哪一种势力
        UInt8*  m_map;
        UInt8*  m_battleInfo;   //战场分布情况

	    Ascoord m_target;
	    Ascoord m_start;

        UInt8   m_row;
        UInt8   m_col;

	    UInt8   m_numSurround;
	    UInt8   m_gAdd[6];
	    Ascoord m_surround[6];  //正六边形
	
	    std::list<StepData> m_openList;
	    std::list<StepData> m_closeList;
	
	    bool popBestStep(StepData* pSD); 
	    bool isInList(std::list<StepData>& list, const Ascoord& coord);
	    bool judgeSurround(const Ascoord& coord,const Ascoord& parentCoord, UInt8 G);

	    StepData* findFromList(std::list<StepData>& list, const Ascoord& coord);

	    UInt8 computeH(const Ascoord& coord) {return abs(m_target._x-coord._x)*10 + abs(m_target._y-coord._y)*10;}
        void getAround(Ascoord& coord);
    };
}
#endif
