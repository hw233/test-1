
#ifndef HORCRUXHOLDATTR_H_
#define HORCRUXHOLDATTR_H_
#include <map>
#include "Config.h"
namespace GData
{
class HorcruxHoldAttr
{
public:
      HorcruxHoldAttr(){} 
      void setHorcruxHoldAttr1(UInt32 ,UInt32);
      void setHorcruxHoldAttr2(UInt32 ,float);
      UInt32 getHorcruxHoldAttr1(UInt32);
      float getHorcruxHoldAttr2(UInt32);
private:
    std::map<UInt32 , UInt32>  _horcruxHoldAttr1;
    std::map<UInt32 , float>  _horcruxHoldAttr2;
};
extern HorcruxHoldAttr horcruxHoldAttr ;
}
#endif // HORCRUXHOLDATTR_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

