
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
        if( /*(_pictureAttr[floor].size() )&&*/index > 0 && static_cast<UInt32>(index  - 1)< _pictureAttr[floor].size() )
            return &_vecCubeAttr[_pictureAttr[floor][index - 1]];
        return NULL;
    }
    UInt32 PictureAttr::getFloorCost(UInt8 index)
    {
        if(index == 0 || index > _floorCostVec.size())
            return -1;
        return _floorCostVec[index - 1];
    }
}
