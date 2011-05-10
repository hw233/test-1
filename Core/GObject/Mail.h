#ifndef _MAIL_H_
#define _MAIL_H_

#include "Common/Stream.h"
#include "Common/Mutex.h"

namespace lua_tinker
{
	struct table;
}
namespace Network
{
	class TcpConduit;
}

namespace GObject
{

class Player;

struct Mail
{
	UInt32 id;
	std::string sender;
	UInt32 recvTime;
	UInt8 flag;
	std::string title;
	std::string content;
	UInt32 additional;

	void packetTitle(Stream& st);
};

class MailPackage
{
public:
	enum {Coin = 0x8000, Tael = 0x9000, Coupon = 0xA000, Gold = 0xB000, Achievement = 0xC000};
	struct MailItem
	{
		UInt16 id;
		UInt32 count;
	};
public:
	bool takeIt(Player *);
	UInt16 push(UInt16, UInt16);
	void makeInfo(Stream& st);
private:
	std::map<UInt16, UInt16> _items;
};
struct MailItemsInfo
{
	MailPackage::MailItem *items;
	UInt16 count;
	UInt8 type;
	MailItemsInfo(GObject::MailPackage::MailItem *items_ = NULL, UInt8 type_ = 0, UInt16 count_ = 0):items(items_),count(count_),type(type_){}
};

class MailPackageManager
{
public:
	void push(UInt32, UInt16, UInt32, bool isbind = false);
	void push(UInt32, MailPackage::MailItem *, UInt32, bool isbind = false);
	MailPackage * add(UInt32);
	MailPackage * operator [] (UInt32);
	void remove(UInt32);
private:
	FastMutex _mutex;
	std::map<UInt32, MailPackage> _packages;
};

extern MailPackageManager mailPackageManager;

class MailBox
{
public:
	MailBox(Player * pl): _newMails(0), _owner(pl) { }
	Mail * newMail(Player * sender, UInt8 type, const std::string& title, const std::string& content, UInt32 additional = 0, bool writedb = true, MailItemsInfo * itemsInfo = NULL);
	Mail * newItemMail(UInt8 type, const char * title, const char * content, UInt32 itemId, UInt32 count);
	Mail * newItemPackageMail(const char * title, const char * content, lua_tinker::table table_items);
	Mail * newMail(UInt32 id, const std::string& sender, UInt32 recvTime, UInt8 type, const std::string& title, const std::string& content, UInt32 additional = 0);
	bool delMail(UInt32 id, bool freeAdd);
	void readMail(UInt32 id);
	void clickMail(UInt32 id, UInt8 action);
	void listMail(UInt8, const UInt32 *);
	void listMailID(UInt8 start, UInt8 count);
	void notifyNewMail();
	inline UInt16 getNewMails() { return _newMails; }
private:
	UInt16 countMail() { return _mailBox.size(); }
	Mail * getMail(UInt32 id);
	void updateMail(Mail * mail);
private:
	std::deque<Mail *> _mailBox;
	UInt16 _newMails;
	Player * _owner;
};

}

#endif // _MAIL_H_
