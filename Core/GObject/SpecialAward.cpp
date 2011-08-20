#include "Config.h"
#include "Player.h"
#include "SpecialAward.h"
#include "Server/WorldServer.h"
#include "Server/SysMsg.h"
#include "DB/DBExecutor.h"
#include "DB/DBConnectionMgr.h"
#include "AthleticsRank.h"
#include "Common/TimeUtil.h"
#include "Mail.h"

namespace GObject{

static const UInt16 CunBattleAward[] = {500, 300, 100};
static const UInt16 EnchantEquipAward[] = {300, 200, 100};
static const UInt16 ServerAthleticsAward[] = {100, 200, 500, 800, 1200, 2000, 3000};
static const UInt16 ServerLuckyDrawAward[] = {200, 300, 500};

SpecialAward gSpecialAward;

SpecialAward::SpecialAward()
{
}
SpecialAward::~SpecialAward()
{

}

void SpecialAward::AddAwardListFromDB(UInt8 type, Player *player)
{
	UInt8 size = m_SpecialAward[type].size();
	static UInt8 AwardMaxSize[] = {3, 3, 7, 3};
	if(size >= AwardMaxSize[type])
		return;
	m_SpecialAward[type].push_back(player);
}

UInt8 SpecialAward::getAwardSize(UInt8 type)
{
	return m_SpecialAward[type].size();
}

void SpecialAward::AthleticsAward()
{
	if(!World::_actAvailable1)
		return;
	UInt8 size = m_SpecialAward[2].size();
	if(size >= 7)
		return;
	std::list<AthleticsRankData *> *athletics = gAthleticsRank.getAthleticsRank();
	if(athletics[1].size() == 0 && athletics[0].size() == 0)
		return;
	std::list<AthleticsRankData *>::iterator it = athletics[0].begin();
	if(athletics[1].size() != 0)
		it = athletics[1].begin();
	Player *player = (*it)->ranker;
	if(player == NULL)
		return;
	//SYSMSG(title, 360);
	//SYSMSGV(content, 361, ServerAthleticsAward[size]);
	Mail *pmail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
	mailPackageManager.push(pmail->id, MailPackage::Gold, ServerAthleticsAward[size]);
	//SYSMSG_BROADCASTV(362, player->getCountry(), player->getName().c_str(), ServerAthleticsAward[size]);
	m_SpecialAward[2].push_back(player);
	DB().PushUpdateData("INSERT INTO `SpecialAward` VALUES(%u, %u, %"I64_FMT"u, %u)", size, 2, player->getId(), TimeUtil::Now());

}
void SpecialAward::AddSpecialAwardList(UInt8 type, Player *player)
{
	UInt8 size = m_SpecialAward[type].size();
	static UInt8 AwardMaxSize[] = {3, 3};
	if(size >= AwardMaxSize[type])
		return;
	std::vector<Player *>::iterator it = m_SpecialAward[type].begin();
	for( ;it != m_SpecialAward[type].end(); it ++)
	{
		if((*it) == player)
			return;
	}

	UInt16 Award = 0;		
	m_SpecialAward[type].push_back(player);
	
	switch(type)
	{
		case 0:
			Award = CunBattleAward[size];
			//SYSMSG_BROADCASTV(336, player->getCountry(), player->getName().c_str(), Award);
			break;
		case 1:
			Award = EnchantEquipAward[size];
			//SYSMSG_BROADCASTV(337, player->getCountry(), player->getName().c_str(), size + 1);
			break;
	}

	UInt32 now = TimeUtil::Now();
	DB().PushUpdateData("INSERT INTO `SpecialAward` VALUES(%u, %u, %"I64_FMT"u, %u)", size, type, player->getId(), now);

	GameMsgHdr hdr(0x296, player->getThreadId(), player, sizeof(Award));
	GLOBAL().PushMsg(hdr, &Award);
	//发消息到玩家线程 加礼券。
}

void SpecialAward::luckydrawAward(Player * player)
{	
	if(!World::_actAvailable1)
		return;
	UInt8 size = m_SpecialAward[3].size();
	if(size >= 3)
		return;
	if(player == NULL)
		return;
	m_SpecialAward[3].push_back(player);
	//SYSMSG(title, 363);
	//SYSMSGV(content, 364, ServerLuckyDrawAward[size]);
	Mail *pmail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000);
	mailPackageManager.push(pmail->id, MailPackage::Coupon, ServerLuckyDrawAward[size]);
	DB().PushUpdateData("INSERT INTO `SpecialAward` VALUES(%u, %u, %"I64_FMT"u, %u)", size, 3, player->getId(), TimeUtil::Now());
}

void SpecialAward::newServerActivity(Player *pl, UInt8 challengeNum)
{
	UInt32 thisDay = TimeUtil::SharpDay();
	if(challengeNum == 10)
	{
		if(World::_isNewServer && thisDay - TimeUtil::SharpDay(0, PLAYER_DATA(pl, created)) < 8 * 86400)
		{
			SYSMSG(title, 540);
			SYSMSGV(content, 541);
			MailPackage::MailItem mitem[1] = {{9000, 1}};
			MailItemsInfo itemsInfo(mitem, Activity, 1);
			pl->GetMailBox()->newMail(NULL, 0x21, title, content, (1 << 16) | 9000, true, &itemsInfo);
		}
	}
}


}
