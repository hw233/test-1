
#ifndef LINGBAOLEVEL_H_
#define LINGBAOLEVEL_H_
#include "Common/URandom.h"
namespace GData
{
#define LINBAOTYPE 3
#define LINGBAOMAX 10
    struct lingbaoL
    {
       UInt16 itemNum;  
       UInt32 itemCount;
       float percent;
       UInt32 probability;
       UInt32 honeyFall;
       UInt32 honeyFallP;
       lingbaoL():itemNum(0),itemCount(0),percent(0),probability(0),honeyFall(0),honeyFallP(0) {}
    };
    class lingbaoLevel
    {
        public:
            void SetLingbaoLevelData(UInt8 type,UInt8 level , UInt16 itemNum , UInt32 itemCount ,float percent , UInt32 probability ,UInt32 honeyFall ,UInt32 honeyFallP)
            { 
                if( type >= LINBAOTYPE)
                    return ;
                if( level >= LINGBAOMAX)
                    return ;
                _lbl[type][level].itemNum = itemNum;
                _lbl[type][level].itemCount = itemCount;
                _lbl[type][level].percent = percent;
                _lbl[type][level].probability = probability;
                _lbl[type][level].honeyFall = honeyFall;
                _lbl[type][level].honeyFallP = honeyFallP;
            } 
            lingbaoL getLingbaoLevel(UInt8 type , UInt8 num);
        private:
        lingbaoL _lbl[LINBAOTYPE][LINGBAOMAX];
    };
    extern lingbaoLevel lingbaoLevelData;
}
#endif // LINGBAOLEVEL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

