#ifndef _CHATITEM_H_
#define _CHATITEM_H_

#include "Common/Stream.h"
#include "Common/Mutex.h"

namespace GObject
{

class ItemEquip;
class Player;
class Fighter;

class ChatItem
{
public:
	ChatItem() {}
	void post(UInt8, UInt64, UInt32, Player *);
	void purge(UInt32);

private:
	void addItem(Player *, UInt32);
    void addPetItem(Player *, UInt32, UInt32);
	UInt32 addFighter(Player *, UInt32);
    UInt32 addFairyPet(Player *, UInt32);
    void addCouplePet(Player *);
    void addZhenyuan(Player *, UInt32);
    void addLingshi(Player *, UInt32, UInt32);

	struct ChatItemData
	{
		Stream st;
		UInt32 lastAccess;
	};

	struct FIndex
	{
		UInt64 pid;
		UInt32 fid;
		bool operator <(const FIndex& other) const
		{
			if(pid == other.pid)
				return fid < other.fid;
			return pid < other.pid;
		}
	};
	FastMutex _itemMutex, _fighterMutex, _petItemMutex, _petMutex, _coupleMutex, _zhenyuanMutex, _lingshiMutex;
	std::map<UInt32, ChatItemData> _itemData;
	std::map<FIndex, ChatItemData> _fighterData;
	std::map<UInt32, ChatItemData> _petItemData;
	std::map<FIndex, ChatItemData> _fairyPetData;
	std::map<UInt64, ChatItemData> _couplePetData;
	std::map<UInt32, ChatItemData> _zhenyuanData;
	std::map<UInt32, ChatItemData> _lingshiData;
};

extern ChatItem chatItem;

}

#endif // _CHATITEM_H_
