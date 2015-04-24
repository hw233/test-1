#include "AStar.h"
namespace GObject
{
    AStar::AStar():
	    m_numSurround(6),
        m_mapFighters(NULL),
        m_currentBf(NULL)
    {

	    for(UInt8 i = 0 ; i<6 ; ++i)
        {
		    m_gAdd[i] = 10;
            m_surround[i] = Ascoord(0,0);
        }
	    ClearObstacles();
    }

    void AStar::getAround(Ascoord& coord)
    {
        if(coord._y%2 == 0 )
        {
            m_surround[0] = Ascoord(coord._x-1,coord._y);
            m_surround[1] = Ascoord(coord._x+1,coord._y);
            m_surround[2] = Ascoord(coord._x,coord._y+1);
            m_surround[3] = Ascoord(coord._x+1,coord._y+1);
            m_surround[4] = Ascoord(coord._x,coord._y-1);
            m_surround[5] = Ascoord(coord._x-1,coord._y-1);
        }
        else
        {
            m_surround[0] = Ascoord(coord._x-1,coord._y);
            m_surround[1] = Ascoord(coord._x+1,coord._y);
            m_surround[2] = Ascoord(coord._x-1,coord._y+1);
            m_surround[3] = Ascoord(coord._x,coord._y+1);
            m_surround[4] = Ascoord(coord._x-1,coord._y-1);
            m_surround[5] = Ascoord(coord._x,coord._y-1);
        }
    }

    void AStar::ClearObstacles()
    {
        m_map = NULL;
	    m_closeList.clear();
	    m_openList.clear();
    }

    bool AStar::ComputeRoute()
    {
	    m_openList.clear();
	    m_closeList.clear();

	    StepData sd(m_start);         
	    sd._g = 0;
	    sd._h = computeH(m_start);   //计算H值 即当前点到目标点的消耗

	    m_openList.push_back(sd);

	    while (!m_openList.empty())
	    {
		    popBestStep(&sd);
		    m_closeList.push_back(sd);

		    Ascoord coord;
		
		    for(UInt8 i = 0 ; i<m_numSurround ; ++i)
		    {
			    coord  = sd._coord;
                getAround(sd._coord);
			    coord = m_surround[i];  //6方向获取周围的点
			
			    if(judgeSurround(coord,sd._coord,sd._g+m_gAdd[i]))
				    return true;
		    }
	    }
	    return false;
    }
    

    //G值  即起点到当前节点的消耗
    bool AStar::judgeSurround(const Ascoord& coord,const Ascoord& parentCoord,UInt8 G)
    {
	    if(coord._x>=0 && coord._x< m_col && coord._y>=0 && coord._y< m_row && !isInList(m_closeList,coord) /*&& !IsObstacle(coord)*/)
	    {
		    StepData* pSD = findFromList(m_openList,coord); //判断将要加入的节点是否已在open表中
		    if (pSD && pSD->_g > G)   //如果在 判断G值  比较新G值的大小  如果比原来的要小的话  更新路径
		    {
			    pSD->_g = G;
			    pSD->_parentCoord = parentCoord;
		    }
		    else if(!pSD)   //不存在的话  加入open表中
		    {
			    StepData newSD(coord);
			    newSD._g = G;
			    newSD._h = computeH(coord);
			    newSD._parentCoord = parentCoord;

			    m_openList.push_back(newSD);

			    if(newSD._coord == m_target)
				    return true;
		    }
	    }

	    return false;
    }
    

    //比较F值  即G+H  将最小的节点从open表中删除   
    bool AStar::popBestStep(StepData* pSD)
    {
	    std::list<StepData>::iterator       i   = m_openList.begin();
	    std::list<StepData>::const_iterator end = m_openList.end();

	    if(i!=end)
	    {
		    pSD->_g       = i->_g;
		    pSD->_h       = i->_h;
		    pSD->_coord   = i->_coord;
		    pSD->_parentCoord = i->_parentCoord;

		    i++;

		    while(i!=end)
		    {
			    if(pSD->_g+pSD->_h > i->_g+i->_h)
			    {
				    pSD->_g       = i->_g;
				    pSD->_h       = i->_h;
				    pSD->_coord   = i->_coord;
				    pSD->_parentCoord = i->_parentCoord;
			    }

			    i++;
		    }

		    i = m_openList.begin();
		    while(i->_coord != pSD->_coord)
		    {
			    i++;
		    }

		    m_openList.erase(i);
		    return true;
	    }
	    return false;
    }
   
    //判断是否在列表中
    bool AStar::isInList(std::list<StepData>& list,const Ascoord& coord)
    {
	    std::list<StepData>::iterator       i   = list.begin();
	    std::list<StepData>::const_iterator end = list.end();

	    while(i!=end)
	    {
		    if(coord == i->_coord)
			    return true;

		    i++;
	    }
	    return false;
    }


    //从列表中找到某一点
    StepData* AStar::findFromList(std::list<StepData>& list, const Ascoord& coord)
    {
	    std::list<StepData>::iterator       i   = list.begin();
	    std::list<StepData>::const_iterator end = list.end();

	    while(i!=end)
	    {
	    	if(coord == i->_coord)
		    	return &(*i);

		    i++;
	    }

	    return 0;
    }
    
    //找到这条路径
    bool AStar::GetRoute(std::vector<Ascoord> *list)
    {
	    list->clear();

	    StepData* sd = findFromList(m_openList,m_target); //从终点开始
	    //UInt8 size = m_openList.size();
	    if(sd!=NULL)
	    {
		    list->push_back(sd->_coord);

		    sd = findFromList(m_closeList,sd->_parentCoord);

		    while(sd->_coord != m_start)
		    {
			    list->push_back(sd->_coord);
			    sd = findFromList(m_closeList,sd->_parentCoord);
		    }

		    list->push_back(sd->_coord);
		    return true;
	    }

	    return false;
    }
    
    void AStar::SetMap(UInt8* map)
    {
        if(m_map)
        {
            delete m_map;
        }
        m_map = map;

    }

    bool AStar::SetMapSize(UInt8 row, UInt8 col)
    {
	    if(row<= 0 || col<=0)
		    return false;

	    m_row = row;
	    m_col = col;

	    return true;
    }

    bool AStar::IsObstacle(const Ascoord& coord)
    {
        /*
        bool ret = false;
        if( m_mapFighters[coord._x+coord._y*m_row] != NULL && currentBf->GetSide() != m_mapFighters[coord._x+coord._y*m_row].GetSide())
        {
            ret = true;
        }

        if ( m_map[coord._x + coord._y * m_row ] == 0 )
        {
            ret = true;
        }
        */
        return true;
    }
}
