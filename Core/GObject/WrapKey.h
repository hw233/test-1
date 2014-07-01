#ifndef WRAPKEY_H_
#define WRAPKEY_H_
#include "Common/URandom.h"
class WrapKey
{
public:
    WrapKey(){}
    virtual UInt32 wrapTheKey() = 0;
    virtual bool checkTheKey(UInt32 num) = 0;
    virtual UInt32 unWrapTheKey(UInt32 num) = 0;
    virtual void InitTheKey(UInt32 begin = 0, UInt32 range = 0) = 0;
    virtual UInt32 getTheKey() = 0;
};

class OrdinarireWrapKey : public WrapKey 
{
public:
    OrdinarireWrapKey():WrapKey(),_key(1024){}
    UInt32 wrapTheKey(){  return ((_key+23)*6/3 - 7); }
    UInt32 getTheKey(){return _key;}
    UInt32 unWrapTheKey(UInt32 num) { return ((num-22)/4 + 77);}
    bool checkTheKey(UInt32 num ){ return _key == (unWrapTheKey(num));}
    void InitTheKey(UInt32 begin ,UInt32 range){ _key = uRand(range) + begin ;}
private:
    UInt32 _key;
};

#endif // WRAPKEY_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

