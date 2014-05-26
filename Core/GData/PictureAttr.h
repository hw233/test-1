
#ifndef PICTURE_H_
#define PICTURE_H_
#include <map>
#include "Config.h"
#include "GDataDBExecHelper.h"
namespace GData
{
    class PictureAttr
    {
        public:
            struct stPictureAttr
            {
                UInt32 hp; 
                float attack ; 
                float action ;
                stPictureAttr():hp(0),attack(0),action(0){}
                stPictureAttr(UInt32 hp_ , float attack_ , float action_):hp(hp),attack(attack_),action(action_){}
            };
        private:
            std::map<UInt8 , std::vector<UInt8> >  _pictureAttr;  //层数，编号，属性编号
            std::vector<UInt32> _floorCostVec;
            std::vector<stPictureAttr> _vecCubeAttr;   
            std::vector<stPictureAttr> _vecFloorAttr;
        public:
            PictureAttr(){}
            void setPictureAttrTable(UInt32 val ,UInt8 ,UInt8);
            void setVecInfoAttr( UInt32 hp ,float attack , float action ,UInt8 flag = 0)
            {
                if(hp == 0 && attack == 0 && action == 0)
                    return ;
                stPictureAttr st(hp,attack,action);
                if(!flag)
                    _vecCubeAttr.push_back(st); 
                else
                    _vecFloorAttr.push_back(st);
            };
            void setFloorInfo(UInt8 floor , UInt8 index , UInt32 cost ,UInt8 buff)
            {
                if(index == 1)
                    _floorCostVec.push_back(cost);
                _pictureAttr[floor].push_back(buff);
            }
            stPictureAttr*  getPictureAttrTable(UInt8 floor,UInt8 index); 
            stPictureAttr*  getFloorAttrTable(UInt8 floor)
            {
                if(floor >= _vecFloorAttr.size())
                    return NULL;
                return &_vecFloorAttr[floor];
            }; 
            UInt8 getFloorCubeCount(UInt8 floor){ return _pictureAttr[floor].size();}
            UInt32 getFloorCost(UInt8 index);
            void setFloorCost(UInt8 index ,UInt32 value);
    };
    extern PictureAttr pictureAttrData;
}

#endif // PICTURE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

