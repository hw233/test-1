#include "Config.h"
#include "Mail.h"
#include "Server/WorldServer.h"
#include "MsgID.h"
#include "Player.h"
#include "Fighter.h"
#include "Trade.h"
#include "Sale.h"
#include "Package.h"
#include "Clan.h"
#include "Server/OidGenerator.h"
#include "Network/TcpConduit.h"
#include "Log/Log.h"
#include "Common/TimeUtil.h"
#include "Common/Itoa.h"
#include "Server/SysMsg.h"
#include "Server/Cfg.h"
#include "Athletics.h"
#include "Script/GameActionLua.h"
#include "Country.h"
#include "Script/lua_tinker.h"
#include <mysql.h>

#ifdef _WIN32
#pragma comment(lib, "libmysql.lib")
#endif

namespace GObject
{

MailPackageManager mailPackageManager;

void Mail::packetTitle( Stream& st )
{
	st << id << sender << recvTime << flag << title;
}

bool MailPackage::takeIt( Player * player )
{
	Package * package = player->GetPackage();
	UInt32 required = 0;
	std::map<UInt16, UInt16>::iterator it;
	for(it = _items.begin(); it != _items.end(); ++ it)
	{
		if(it->first >= 0x8000)
			continue;
		UInt16 isbind = it->second >> 8;
		bool bind = true;
		if(isbind == 0)
			bind = false;
		required += package->GetItemUsedGrids(it->first, it->second & 0x00FF, bind);
	}
	if(required > package->GetRestPackageSize())
		return false;
	for(it = _items.begin(); it != _items.end(); ++ it)
	{
		if(it->first >= 0x8000)
		{
			switch(it->first & 0xF000)
			{
			case Coin:
				player->getCoin(it->second + ((it->first & 0x0FFF) << 16));
				break;
			case Tael:
				player->getTael(it->second + ((it->first & 0x0FFF) << 16));
				break;
			case Coupon:
				player->getCoupon(it->second + ((it->first & 0x0FFF) << 16));
				break;
			case Gold:
				player->getGold(it->second + ((it->first & 0x0FFF) << 16));
				break;
			case Achievement:
				player->getAchievement(it->second + ((it->first & 0x0FFF)<< 16));
				break;
			}
		}
		else
		{
			UInt16 isbind = it->second >> 8;
			bool bind = true;
			if(isbind == 0)
				bind = false;
			package->Add(it->first, it->second & 0x00FF, bind, false, FromMail);
		}
	}
	return true;
}

void MailPackage::makeInfo(Stream& st)
{
	size_t off = st.size();
	st << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt16>(0);
	std::map<UInt16, UInt16>::iterator it;
	UInt16 num = 0;
	for(it = _items.begin(); it != _items.end(); ++ it)
	{
		if(it->first >= 0x8000)
		{
			switch(it->first & 0xF000)
			{
			case Coin:
				st.data<UInt32>(off) = ((it->first & 0x0FFF) << 16) + it->second;
				break;
			case Tael:
				st.data<UInt32>(off + sizeof(UInt32)) = ((it->first & 0x0FFF) << 16) + it->second;
				break;
			case Coupon:
				st.data<UInt32>(off + sizeof(UInt32) * 2) = ((it->first & 0x0FFF) << 16) + it->second;
				break;
			case Gold:
				st.data<UInt32>(off + sizeof(UInt32) * 3) = ((it->first & 0x0FFF) << 16) + it->second;
				break;
			case Achievement:
				break;
			default:
				break;
			}
			continue;
		}
		st << it->first << static_cast<UInt16>(it->second & 0x0FFF);
		num ++;
	}
	st.data<UInt16>(off + sizeof(UInt32) * 4) = num;
}

UInt16 MailPackage::push( UInt16 id, UInt16 cnt )
{
	UInt16& count = _items[id];
	if(id >= 0x8000)
		count = cnt;
	else
		count += cnt;
	return count;
}

void MailPackageManager::push(UInt32 id, UInt16 itemId, UInt32 cnt, bool isbind)
{
	if(cnt == 0)
		return;
	FastMutex::ScopedLock lk(_mutex);
	if(itemId >= 0x8000)
	{
		itemId += (cnt >> 16);
		cnt &= 0xFFFF;
	}
	else if(isbind)
		cnt |= 0x1000;
	UInt16 newCount = _packages[id].push(itemId, cnt);
	DB().PushUpdateData("REPLACE INTO `mail_package`(`id`, `itemId`, `itemCount`) VALUES(%u, %u, %u)", id, itemId, newCount);
}

void MailPackageManager::push(UInt32 id, MailPackage::MailItem * items, UInt32 count, bool isbind)
{
	FastMutex::ScopedLock lk(_mutex);
	MailPackage & package = _packages[id];
	for(UInt32 i = 0; i < count; ++ i)
	{
		UInt16 iid = items[i].id;
		UInt32 cnt = items[i].count;
		if(cnt == 0)
			continue;
		if(iid >= 0x8000)
		{
			iid += (cnt >> 16);
			cnt &= 0xFFFF;
		}
		else if(isbind)
			cnt |= 0x1000;
		UInt16 newCount = package.push(iid, cnt);
		DB().PushUpdateData("REPLACE INTO `mail_package`(`id`, `itemId`, `itemCount`) VALUES(%u, %u, %u)", id, iid, newCount);
	}
}

MailPackage * MailPackageManager::operator[]( UInt32 id )
{
	FastMutex::ScopedLock lk(_mutex);
	std::map<UInt32, MailPackage>::iterator it = _packages.find(id);
	if(it == _packages.end())
		return NULL;
	return &it->second;
}

void MailPackageManager::remove( UInt32 id )
{
	FastMutex::ScopedLock lk(_mutex);
	std::map<UInt32, MailPackage>::iterator it = _packages.find(id);
	if(it == _packages.end())
		return;
	DB().PushUpdateData("DELETE FROM `mail_package` WHERE `id` = %u", id);
	_packages.erase(id);
}

MailPackage * MailPackageManager::add( UInt32 id )
{
	MailPackage& mp = _packages[id];
	return &mp;
}

Mail * MailBox::newMail( Player * sender, UInt8 type, const std::string& title, const std::string& content, UInt32 additional, bool writedb, MailItemsInfo * itemsInfo)
{
	Mutex::ScopedLock lk(_owner->getMutex());

	Mail * mail = newMail(IDGenerator::gMailOidGenerator.ID(), (sender == NULL ? "" : sender->getName()), TimeUtil::Now(), type, title, content, additional);
	if(mail == NULL)
		return NULL;

	if(_owner != NULL)
	{
		Stream st(REP::MAIL_NEW);
		st << _newMails << Stream::eos;
		_owner->send(st);
		SYSMSG_SEND(137, _owner);
		SYSMSG_SEND(1037, _owner);
		if(sender != NULL && sender != _owner)
			SYSMSG_SEND(138, sender);
	}

	if(writedb)
	{
		char title2[256], content2[2048];
		mysql_escape_string(title2, title.c_str(), 256/2-1);
		mysql_escape_string(content2, content.c_str(), 2048/2-1);
		DB().PushUpdateData("INSERT INTO `mail` (`mailId`, `playerId`, `sender`, `recvTime`, `flag`, `title`, `content`, `additionalId`) VALUES (%u, %"I64_FMT"u, '%s', %u, %u, '%s', '%s', %u)", mail->id, ((_owner != NULL) ? _owner->getId() : static_cast<UInt64>(0)), mail->sender.c_str(), mail->recvTime, mail->flag, title2, content2, mail->additional);
	}
	if(itemsInfo != NULL)
	{
		std::string strItems;
		for(UInt32 i = 0; i < itemsInfo->count; i ++)
		{
			strItems += Itoa(itemsInfo->items[i].id);
			strItems += ",";
			strItems += Itoa(itemsInfo->items[i].count);
			strItems += "|";
		}
		DBLOG().PushUpdateData("insert into mailitem_histories(`server_id`, `player_id`, `mail_id`, `mail_type`, `title`, `content_text`, `content_item`, `receive_time`) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '%s', %u)", cfg.serverLogId, _owner->getId(), mail->id, itemsInfo->type, title.c_str(), content.c_str(), strItems.c_str(), mail->recvTime);
	}
	return mail;
}

Mail * MailBox::newMail( UInt32 id, const std::string& sender, UInt32 recvTime, UInt8 type, const std::string& title, const std::string& content, UInt32 additional /*= 0*/ )
{
	Mail * mail = new(std::nothrow) Mail();
	if(mail == NULL)
		return NULL;
	mail->id = id;
	mail->sender = sender;
	mail->recvTime = recvTime;
	mail->flag = type;
	mail->title = title;
	mail->content = content;
	mail->additional = additional;

	if(!(type & 0x80))
		++ _newMails;
	_mailBox.push_front(mail);
	return mail;
}

Mail * MailBox::newItemMail( UInt8 type, const char * title, const char * content, UInt32 itemId, UInt32 count )
{
	MailPackage::MailItem mitem[1] = {{static_cast<UInt16>(itemId), count}};
	MailItemsInfo itemsInfo(mitem, Activity, 1);
	return newMail(NULL, type, title, content, itemId + 0x10000 * count, true, &itemsInfo);
}

Mail * MailBox::newItemPackageMail(const char * title, const char * content, lua_tinker::table table_items)
{
	UInt32 size = table_items.size();
	if(size == 0)
		return NULL;
	MailPackage::MailItem *mitem = new(std::nothrow) MailPackage::MailItem[size/3];
	if(mitem == NULL)
		return NULL;
	UInt32 cnt = 0;
	for(UInt32 j = 0; j < size; )
	{
		mitem[cnt].id = table_items.get<UInt16>(++ j);
		mitem[cnt].count = table_items.get<UInt32>(++ j);
		++ j;
		++ cnt;
	}
	MailItemsInfo itemsInfo(mitem, Activity, cnt);
	Mail *pmail = newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
	if(pmail != NULL)
	{
		UInt16 itemId;
		UInt32 count;
		UInt8 bind;
		for(UInt32 j = 0; j < size; )
		{
			itemId = table_items.get<UInt16>(++ j);
			count = table_items.get<UInt32>(++ j);
			bind = table_items.get<UInt8>(++ j);
			mailPackageManager.push(pmail->id, itemId, count, bind == 1);
		}
	}
	SAFE_DELETE(mitem);
	return pmail;
}


class find_mail_less
{
public:
	bool operator()(const Mail * mail1, const Mail * mail2)
	{
		return mail1->id > mail2->id;
	}

	bool operator()(const Mail * mail, UInt32 id)
	{
		return mail->id > id;
	}

	bool operator()(UInt32 id, const Mail * mail)
	{
		return mail->id < id;
	}

};

bool MailBox::delMail( UInt32 id, bool freeAdd )
{
	Mutex::ScopedLock lk(_owner->getMutex());

	using namespace std::placeholders;

	std::deque<Mail *>::iterator it = std::lower_bound(_mailBox.begin(), _mailBox.end(), id, find_mail_less());
	if(it == _mailBox.end() || (*it)->id != id)
		return false;
	Mail * mail = *it;
	if(freeAdd)
	{
		if(!(mail->flag & 0x80))
		{
			if(_newMails > 0)
				-- _newMails;
		}
		switch(mail->flag & 0x7F)
		{
		case 0x05:
			{
				_owner->GetTrade()->deleteTrade(mail->additional);
			}
			break;
		case 0x06:
			{
				_owner->GetSale()->delSaleMailItems(mail->additional);
			}
			break;
		case 0x21:
			{
				UInt32 count = mail->additional >> 16;
				if(count == 0xFFFE)
					mailPackageManager.remove(id);
			}
			break;
		}
	}

	_mailBox.erase(it);
	DB().PushUpdateData("DELETE FROM `mail` WHERE `mailId` = %u", id);
	DBLOG().PushUpdateData("update `mailitem_histories` set `status`= 2, `delete_time` = %u where server_id = %u and mail_id = %u and `status` = 0", TimeUtil::Now(), cfg.serverLogId, id);


	SAFE_DELETE(mail);
	return true;
}

Mail * MailBox::getMail( UInt32 id )
{
	using namespace std::placeholders;

	std::deque<Mail *>::iterator it = std::lower_bound(_mailBox.begin(), _mailBox.end(), id, find_mail_less());
	if(it == _mailBox.end() || (*it)->id != id)
		return NULL;
	return *it;
}

void MailBox::readMail( UInt32 id )
{
	Mutex::ScopedLock lk(_owner->getMutex());
	Mail * mail = getMail(id);
	if(mail == NULL)
		return;
	if((mail->flag & 0x80) == 0)
	{
		mail->flag |= 0x80;
		if(_newMails > 0)
		{
			-- _newMails;
			// XXX: notifyNewMail();
		}
		DB().PushUpdateData("UPDATE `mail` SET `flag` = %u WHERE `mailId` = %u", mail->flag, id);
	}
	Stream st(REP::MAIL_CONTENTS);
	st << id << mail->content << mail->additional;
	switch(mail->flag & 0x7F)
	{
		case 0x21:
			{
				st << static_cast<UInt8>(1);
				UInt32 count = mail->additional >> 16;
				if(count == 0xFFFF)
				{
					UInt32 pkgId = mail->additional & 0xFFFF;
					if(pkgId > 0xFFF0)
					{
						const UInt32 vipRollTable[10][3] = {{3001, 3002, 0}, {3003, 3004, 3005}, {3006, 3007, 3008},
						{3009, 3010, 0}, {3011, 3012, 0}, {3013, 3014, 0}, {3015, 3016, 0}, {3017, 3018, 0},
						{3019, 3020, 0}, {3021, 3022, 0}};
						const UInt32 vipGoldTable[10] = { 0, 0, 0, 100, 500, 1000, 2000, 3000, 3000, 3000 };
						UInt32 g = vipGoldTable[pkgId - 0xFFF1];
						st << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0) << g;
						size_t off = st.size();
						st << static_cast<UInt16>(0);
						const UInt32 * t = vipRollTable[pkgId - 0xFFF1];
						UInt32 num = 0;
						URandom ur(mail->id);
						for(UInt32 i = 0; i < 3 && t[i] > 0; ++ i)
						{
							const GData::LootItem * li = GData::lootTable[t[i]];
							GData::LootResult r = li->roll(&ur);
							st<< r.id << r.count;
							num ++;
						}
						st.data<UInt16>(off) = num;
					}
					else if(pkgId > 0)
					{
						UInt32 money[4] = {0};//0:coin 1:tael 2:coupon 3:gold
						std::vector<MailPackage::MailItem> mailItems;
						lua_tinker::table items = GameAction()->onGetMailItems(pkgId);
						for(int i = 1; i <= items.size(); )
						{
							MailPackage::MailItem item;
							item.id = items.get<UInt16>(i ++);
							item.count = items.get<UInt32>(i ++);
							if(item.id > 0x8000)
							{
								switch(item.id)
								{
									case MailPackage::Coupon:
										money[2] = item.count;
										break;
									case MailPackage::Gold:
										money[3] = item.count;
										break;
									case MailPackage::Tael:
										money[1] = item.count;
										break;
									case MailPackage::Coin:
										money[0] = item.count;
										break;
									default:
										break;
								}
							}
							else
								mailItems.push_back(item);
						}
						st << money[0] << money[1] << money[2] << money[3] << static_cast<UInt16>(mailItems.size());
						for(UInt32 i = 0; i < mailItems.size(); i ++)
						{
							st<<mailItems[i].id << static_cast<UInt16>(mailItems[i].count);
						}
					}
				}
				else if(count == 0xFFFE)
				{
					MailPackage * pkg = mailPackageManager[id];
					if(pkg == NULL)
						break;
					pkg->makeInfo(st);
				}
				else
				{
					st << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0);
					if(count > 0)
						st << static_cast<UInt16>(1) << static_cast<UInt16>(mail->additional & 0xFFFF) << count;
					else
						st << static_cast<UInt16>(0);
				}
			}
			break;
		case 0x31://athletics award
			{
				st << static_cast<UInt8>(1);
				st << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0);
				size_t off = st.size();
				st << static_cast<UInt16>(0);
				UInt16 EquipId = mail->additional & 0xFFFF;
				UInt8 rank =  static_cast<UInt8>(mail->additional >> 16);
				
				st << EquipId << static_cast<UInt16>(1);
				st << static_cast<UInt16>(8913) << static_cast<UInt16>(1);
				UInt16 num = 2;

				if(rank == 1)
				{
					st << static_cast<UInt16>(8999) << static_cast<UInt16>(3);
					st << static_cast<UInt16>(9000) << static_cast<UInt16>(1);
					num += 2;
				}
				else if(rank == 2)
				{
					st << static_cast<UInt16>(8999) <<static_cast<UInt16>(2);
					num ++;
				}
				else  if(rank == 3)
				{
					st << static_cast<UInt16>(8999) << static_cast<UInt16>(1);
					num ++;
				}
				st.data<UInt16>(off) = num;
			}
			break;
		case 0x05:
			{
				st << static_cast<UInt8>(1);
				st << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0);
				size_t off = st.size();
				st << static_cast<UInt16>(0);	
				UInt16 num = 0;
				_owner->GetTrade()->makeMailInfo(mail->additional, st, num);
				st.data<UInt16>(off) = num;
			}
			break;
		case 0x06://sale
			{
				st << static_cast<UInt8>(1);
				st << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0) << static_cast<UInt32>(0);
				size_t off = st.size();
				st << static_cast<UInt16>(0);
				UInt16 num = 0;
				_owner->GetSale()->makeMailInfo(mail->additional, st, num);
				st.data<UInt16>(off) = num;
			}
			break;
		default://ÓÊ¼þÃ»ÓÐÎïÆ·
			{
				st << static_cast<UInt8>(0);	
			}
			break;
	}
	st << Stream::eos;
	_owner->send(st);
}

void MailBox::clickMail( UInt32 id, UInt8 action )
{
	Mutex::ScopedLock lk(_owner->getMutex());
	using namespace std::placeholders;

	std::deque<Mail *>::iterator it = std::lower_bound(_mailBox.begin(), _mailBox.end(), id, find_mail_less());
	if(it == _mailBox.end() || (*it)->id != id)
		return;
	Mail * mail = *it;

	bool delIt = false;
	switch(mail->flag & 0x7F)
	{
	case 0x21:
		{
			UInt32 count = mail->additional >> 16;
			if(count == 0xFFFF)
			{
				UInt32 pkgId = mail->additional & 0xFFFF;
				if(pkgId > 0xFFF0)
				{
					if(_owner->GetPackage()->GetRestPackageSize() < 3)
					{
						_owner->sendMsgCode(0, 1011);
						return;
					}
					const UInt32 vipRollTable[10][3] = {{3001, 3002, 0}, {3003, 3004, 3005}, {3006, 3007, 3008},
					{3009, 3010, 0}, {3011, 3012, 0}, {3013, 3014, 0}, {3015, 3016, 0}, {3017, 3018, 0},
					{3019, 3020, 0}, {3021, 3022, 0}};
					const UInt32 vipGoldTable[10] = { 0, 0, 0, 100, 500, 1000, 2000, 3000, 6000, 6000 };
					UInt32 g = vipGoldTable[pkgId - 0xFFF1];
					if(g > 0)
						_owner->getGold(g);
					const UInt32 * t = vipRollTable[pkgId - 0xFFF1];
					URandom ur(mail->id);
					for(UInt32 i = 0; i < 3 && t[i] > 0; ++ i)
					{
						const GData::LootItem * li = GData::lootTable[t[i]];
						GData::LootResult r = li->roll(&ur);
						_owner->GetPackage()->Add(r.id, r.count, true, false, FromVipAward);
					}
				}
				else if(pkgId > 0 && !GameAction()->onTakeMailPackage(_owner, pkgId))
				{
					_owner->sendMsgCode(0, 1011);
					return;
				}
			}
			else if(count == 0xFFFE)
			{
				MailPackage * pkg = mailPackageManager[id];
				if(pkg == NULL)
				{
					delIt = true;
					DBLOG().PushUpdateData("update `mailitem_histories` set `status`= 1, `delete_time` = %u where `server_id` = %u and `mail_id` = %u and `status` = 0", TimeUtil::Now(), cfg.serverLogId, mail->id);
					break;
				}
				if(pkg->takeIt(_owner))
				{
					mailPackageManager.remove(id);
				}
				else
				{
					_owner->sendMsgCode(0, 1011);
					return;
				}
			}
			else
			{
				if(count > 0 && _owner->GetPackage()->AddItem(mail->additional & 0xFFFF, count, true, false, FromMail) == NULL)
				{
					_owner->sendMsgCode(0, 1011);
					return;
				}
			}
			DBLOG().PushUpdateData("update `mailitem_histories` set `status`= 1, `delete_time` = %u where server_id = %u and mail_id = %u and `status` = 0", TimeUtil::Now(), cfg.serverLogId, mail->id);
			delIt = true;
		}
		break;
	case 0x31:
		{	
			UInt32 EquipId = mail->additional & 0xFFFF;
			UInt8 rank =  static_cast<UInt8>(mail->additional >> 16);
			if(_owner->GetAthletics()->addAthleticsExtraAward(EquipId, rank))
			{
				delIt = true;
				DBLOG().PushUpdateData("update `mailitem_histories` set `status`= 1, `delete_time` = %u where server_id = %u and mail_id = %u and `status` = 0", TimeUtil::Now(), cfg.serverLogId, mail->id);
			}
		}
		break;

	case 0x13:
		{
			Player * pl = globalNamedPlayers[_owner->fixName(mail->sender)];
			if(pl == NULL)
				return;
			mail->flag = 0x83;
			if(action == 0)
			{
				_owner->addFriend(pl);
				SYSMSG(content, 212);
				mail->content = content;
			}
			else
			{
				SYSMSG(content, 213);
				mail->content = content;
			}
			updateMail(mail);
		}
		break;
	case 0x05:
		{
			if (_owner->GetTrade()->addTradeMailItems(mail->additional))
			{
				delIt = true;
				DBLOG().PushUpdateData("update `mailitem_histories` set `status`= 1, `delete_time` = %u where `server_id` = %u and `mail_id` = %u and `status` = 0", TimeUtil::Now(), cfg.serverLogId, mail->id);
			}
		}
	case 0x06:
		{
			if (_owner->GetSale()->addSaleMailItems(mail->additional))
			{
				delIt = true;
				DBLOG().PushUpdateData("update `mailitem_histories` set `status`= 1, `delete_time` = %u where `server_id` = %u and `mail_id` = %u and `status` = 0", TimeUtil::Now(), cfg.serverLogId, mail->id);
			}
		}
		break;
	case 0x22:
		{
			Player * p = GObject::globalNamedPlayers[_owner->fixName(mail->sender)];
			if (p == NULL)
			{
				delIt = true;
				break;
			}
			struct ClanMailClickInviteReq
			{
				UInt32 id;
				GObject::Player * inviter;
				UInt8 action;
			};
			ClanMailClickInviteReq cmcir = { mail->id, p, action };
			GameMsgHdr hdr(0x173, WORKER_THREAD_WORLD, _owner, sizeof(ClanMailClickInviteReq));
			GLOBAL().PushMsg(hdr, &cmcir);
		}
		break;
	case 0x23:
		{
			Player * p = GObject::globalNamedPlayers[_owner->fixName(mail->sender)];
			if(p == NULL)
			{
				delIt = true;
				break;
			}
			struct ClanMailClickReq
			{
				UInt32 id;
				GObject::Player * applier;
				UInt8 action;
			};
			ClanMailClickReq cmcr = {mail->id, p, action};
			GameMsgHdr hdr(0x174, WORKER_THREAD_WORLD, _owner, sizeof(ClanMailClickReq));
			GLOBAL().PushMsg(hdr, &cmcr);
		}
		break;
	case 0x24:
		{
			struct ClanAllyClickReq
			{
				UInt32 clanId;
				bool   agree;
				Player * receiver;
			};
			Player * launcher = GObject::globalNamedPlayers[_owner->fixName(mail->sender)];
			if (launcher == NULL)
			{
				delIt = true;
				break;
			}
			ClanAllyClickReq cacr = { mail->additional, action == 0 ? true : false, _owner };			
			GameMsgHdr hdr(0x176, WORKER_THREAD_WORLD, launcher, sizeof(ClanAllyClickReq));
			GLOBAL().PushMsg(hdr, &cacr);
			delIt = true;
		}
		break;
	case 0x25:
		{
			//TODO:
		}
		break;
	case 0x26:
		{
			//TODO:
		}
	case 0x27:
		{
			_owner->GetPackage()->AddItem(mail->additional, 1);
			delIt = true;
		}
		break;
	default:
		return;
	}
	if(delIt)
	{
		if(!(mail->flag & 0x80))
		{
			if(_newMails > 0)
			{
				-- _newMails;
                // XXX: 
				//Stream st(REP::MAIL_NEW);
				//st << _newMails << Stream::eos;
				//_owner->send(st);
			}
		}
		Stream st(REP::MAIL_DELETE);
		st << static_cast<UInt8>(1) << mail->id << Stream::eos;
		_owner->send(st);

		_mailBox.erase(it);
		DB().PushUpdateData("DELETE FROM `mail` WHERE `mailId` = %u", id);
		SAFE_DELETE(mail);
	}
}

void MailBox::listMail( UInt8 cnt, const UInt32 * idlist )
{
	if(cnt > 100)
		return;

	Mutex::ScopedLock lk(_owner->getMutex());

	Stream st(REP::MAIL_LIST);
	st << cnt;
	for(UInt8 i = 0; i < cnt; ++ i)
	{
		UInt32 id = idlist[i];
		std::deque<Mail *>::iterator it = std::lower_bound(_mailBox.begin(), _mailBox.end(), id, find_mail_less());
		if(it == _mailBox.end() || (*it)->id != id)
		{
			st << id << "" << static_cast<UInt32>(0) << static_cast<UInt8>(1) << "";
		}
		else
		{
			(*it)->packetTitle(st);
		}
	}
	st << Stream::eos;
	_owner->send(st);
}

void MailBox::listMailID( UInt8 start, UInt8 count )
{
	if(count > 100)
		return;

	Mutex::ScopedLock lk(_owner->getMutex());

	UInt32 start_ = start;
	start_ *= count;
	if(start_ >= _mailBox.size())
	{
		Stream st(REP::MAIL_ID_LIST);
		st << start << static_cast<UInt8>(0) << countMail() << Stream::eos;
		_owner->send(st);
		return;
	}
	Stream st(REP::MAIL_ID_LIST);
	st << start << static_cast<UInt8>(0) << countMail();
	std::deque<Mail *>::iterator it = _mailBox.begin() + start_;
	UInt8 cnt = 0;
	UInt32 curr = 0;
	while(cnt < count && it != _mailBox.end())
	{
		st << (*it)->id;
		if((*it)->recvTime > curr)
			curr = (*it)->recvTime;
		++ cnt;
		++ it;
	}
	st.data<UInt8>(5) = cnt;
	st << Stream::eos;
	_owner->send(st);
}

void MailBox::updateMail( Mail * mail )
{
	Stream st(REP::MAIL_CHANGE);
	st << mail->id << mail->sender << mail->recvTime << mail->flag << mail->title << mail->content << mail->additional << Stream::eos;
	_owner->send(st);
	char title2[128], content2[4096];
	mysql_escape_string(title2, mail->title.c_str(), (128/2)-1);
	mysql_escape_string(content2, mail->content.c_str(), (4096/2)-1);
	DB().PushUpdateData("UPDATE `mail` SET `flag` = %u, `title` = '%s', `content` = '%s', `additionalId` = %u WHERE `mailId` = %u", mail->flag, title2, content2, mail->additional, mail->id);
}

void MailBox::notifyNewMail()
{
	Mutex::ScopedLock lk(_owner->getMutex());

	Stream st(REP::MAIL_NEW);
	st << _newMails << Stream::eos;
	_owner->send(st);
}

}
