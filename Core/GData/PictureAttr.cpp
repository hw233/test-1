
#include "PictureAttr.h"
namespace GData
{
    PictureAttr pictureAttrData;
    void PictureAttr::setPictureAttrTable(UInt32 floor , UInt8 index ,UInt8 da)
    {
        _pictureAttr[floor].push_back(da); 
    }

    PictureAttr::stPictureAttr*  PictureAttr::getPictureAttrTable(UInt8 floor , UInt8 index)
    {
        if( /*(_pictureAttr[floor].size() )&&*/ index < _pictureAttr[floor].size() )
            return &_vecCubeAttr[_pictureAttr[floor][index]];
        return NULL;
    }
    UInt32 PictureAttr::getFloorCost(UInt8 index)
    {
        if(index == 0 || index > _floorCostVec.size())
            return -1;
        return _floorCostVec[index - 1];
    }
    void PictureAttr::setFloorCost(UInt8 index ,UInt32 value)
    {
        if( index ==0 || (index-1) != _floorCostVec.size()) 
            return ;
        _floorCostVec.push_back(value);
    }
}
