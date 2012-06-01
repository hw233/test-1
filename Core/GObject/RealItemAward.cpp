#include "Config.h"
#include "RealItemAward.h"
#include "Log/Log.h"
#include "Server/WorldServer.h"
#include "Server/SysMsg.h"
#include "Player.h"

namespace GObject
{

RealItemAwardMgr realItemAwardMgr;

void RealItemAwardMgr::load(UInt32 id, UInt32 cd, std::string& card_no, std::string& card_psw)
{
    RealItemAward& award = m_awards[id];
    award.cd = cd;
    award.card_no = card_no;
    award.card_psw = card_psw;
    return;
}

bool RealItemAwardMgr::hasAward(UInt32 id)
{
    FastMutex::ScopedLock lk(m_mutex);

    std::map<UInt32, RealItemAward>::iterator it = m_awards.find(id);
    if(it == m_awards.end())
        return false;

    RealItemAward& award = it->second;
    -- award.cd;
    DB1().PushUpdateData("UPDATE `real_item_award` SET `cd` = %u WHERE `id` = %u", award.cd, id);
    if(award.cd == 0)
        return true;

    return false;
}

void RealItemAwardMgr::getAward(Player* pl, UInt32 id)
{
    FastMutex::ScopedLock lk(m_mutex);
    std::map<UInt32, RealItemAward>::iterator it = m_awards.find(id);
    if(it == m_awards.end())
        return;

    RealItemAward& award = it->second;

    if(id == 2 || id == 1)
    {
        UInt32 num = 0;
        if(id == 1)
            num = 60;
        else
            num = 100;
        SYSMSG(title, 2366);
        SYSMSGV(content, 2367, num, award.card_no.c_str(), award.card_psw.c_str());
        Mail * mail = pl->GetMailBox()->newMail(NULL, 0x03, title, content);

        DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '', %u)", cfg.serverLogId, pl->getId(), mail->id, RealItemAwardActive, title, content, mail->recvTime);

        m_awards.erase(it);
        DB1().PushUpdateData("DELETE FROM `real_item_award` WHERE `id` = %u", id);
    }

    return;
}

}

