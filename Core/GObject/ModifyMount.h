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
	ModifyMount(UInt8 id, Player * owner): _id(id), _owner(owner)
    { memset(_chips, 0, sizeof(_chips)); }
	~ModifyMount() {}

    inline UInt8 getId() { return _id; }
    void updateToDB();
    bool hasChip(UInt32 itemId);
    bool hasFullChips();
    bool addChip(UInt32 itemId);
    void setChipFromDB(UInt8, UInt32);
    void appendMountInfo(Stream& st);
    void addAttrExtra(GData::AttrExtra&);
private:
    UInt8 _id;
    Player * _owner;
    UInt32 _chips[MOUNTCHIP_MAX];
};

}

#endif // _MODIFYMOUNT_H_
