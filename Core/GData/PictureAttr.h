
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
                float attack ;    //物理攻击攻击
                float action ;    //策划修改为魔法攻击 
                stPictureAttr():hp(0),attack(0),action(0){}
                stPictureAttr(UInt32 hp_ , float attack_ , float action_):hp(hp_),attack(attack_),action(action_){}
            };
        private:
            std::map<UInt8 , std::vector<UInt8> >  _pictureAttr;  //层数，编号，属性编号
            std::vector<UInt32> _floorCostVec;
            std::vector<stPictureAttr> _vecCubeAttr;   
            std::vector<stPictureAttr> _vecFloorAttr;
            std::map<UInt8 , std::vector<UInt8> >  _cubeCount;  //层数，积木编号，方块数量
        public:
            PictureAttr(){}
            void setPictureAttrTable(UInt32 val ,UInt8 ,UInt8);
            void setVecInfoAttr( UInt32 hp ,float attack , float action ,UInt8 flag = 0)
            {
                stPictureAttr st(hp,attack,action);
                if(!flag)
                    _vecCubeAttr.push_back(st);    //从零开始
                else
                    _vecFloorAttr.push_back(st);   //0保留
            };
            void setFloorInfo(UInt8 floor , UInt8 index , UInt32 cost ,UInt8 buff)
            {
                if(index == 1)
                    _floorCostVec.push_back(cost);
                _pictureAttr[floor].push_back(buff);
            }
            void setCubeCount(UInt8 floor , UInt8 index , UInt8 num)
            {
                if(index != _cubeCount[floor].size())
                    return ;
                _cubeCount[floor].push_back(num);
            }
            stPictureAttr*  getPictureAttrTable(UInt8 floor,UInt8 index); 
            stPictureAttr*  getFloorAttrTable(UInt8 floor)
            {
                if(floor >= _vecFloorAttr.size())
                    return NULL;
                return &_vecFloorAttr[floor];
            }; 
            UInt8 getFloorCubeCount(UInt8 floor){ return _pictureAttr[floor].size();}
            UInt8 getCubeCount(UInt8 floor , UInt8 index)
            {
               if( index >= _cubeCount[floor].size()) 
                   return 0;
               return _cubeCount[floor][index];
            }
            UInt32 getFloorCost(UInt8 index);
    };
    extern PictureAttr pictureAttrData;
}

#endif // PICTURE_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

