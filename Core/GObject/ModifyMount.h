#ifndef _MODIFYMOUNT_H_
#define _MODIFYMOUNT_H_

#include "GData/SkillTable.h"
#include "Common/Stream.h"
#include "GData/AttrExtra.h"
#include "Item.h"

namespace GObject
{

#define MOUNTCHIP_MAX 8
class Player;

class ModifyMount
{
public:
	ModifyMount(UInt8 id, Player * owner): _id(id), _owner(owner), _curFloor(0), _curFloor1(0), _failTimes(0)
    { memset(_chips, 0, sizeof(_chips)); }

	~ModifyMount() {}

    inline UInt8 getId() { return _id; }
    inline void setCurfoor(UInt8 f) { _curFloor = f; }
    inline void setCurfoor1(UInt8 f) { _curFloor1 = f; }
    inline void setFailtimes(UInt16 t) { _failTimes = t; }
    inline bool isFullFloor() { return _curFloor >= MOUNTCHIP_MAX; }
    void updateToDB();
    bool hasChip(UInt32 itemId);
    bool hasFullChips();
    bool addChip(UInt32 itemId);
    void setChipFromDB(UInt8, UInt32);
    void appendMountInfo(Stream& st);
    void addAttrExtra(GData::AttrExtra&);
    void cangjianya(UInt8, bool);
    void sendMountInfo();
private:
    UInt8 _id;
    Player * _owner;
    UInt8 _curFloor;    //记录的当前层数(0-8)
    UInt8 _curFloor1;   //记录当前层数满了之后继续唤剑的层数(0-8)
    UInt16 _failTimes;
    UInt32 _chips[MOUNTCHIP_MAX];
};

}

#endif // _MODIFYMOUNT_H_
