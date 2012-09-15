#include "Config.h"
#include "RealItemAward.h"
#include "Log/Log.h"
#include "Server/WorldServer.h"
#include "Server/SysMsg.h"
#include "Player.h"
#include "Common/Stream.h"

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
    UInt32 num = 0;
    if(id == 2 || id == 1)
    {
        if(id == 1)
            num = 60;
        else
            num = 100;
        SYSMSG(title, 2366);
        SYSMSGV(content, 2367, num, award.card_no.c_str(), award.card_psw.c_str());
        Mail * mail = pl->GetMailBox()->newMail(NULL, 0x01, title, content);

        DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '', %u)", cfg.serverLogId, pl->getId(), mail->id, RealItemAwardActive, title, content, mail->recvTime);
    }
    if(id >=3 && id <= 32)
    {
        num = 10;
        SYSMSG(title, 2368);
        SYSMSGV(content, 2369, num, award.card_no.c_str(), award.card_psw.c_str());
        Mail * mail = pl->GetMailBox()->newMail(NULL, 0x01, title, content);

        DBLOG1().PushUpdateData("insert into mailitem_histories(server_id, player_id, mail_id, mail_type, title, content_text, content_item, receive_time) values(%u, %"I64_FMT"u, %u, %u, '%s', '%s', '', %u)", cfg.serverLogId, pl->getId(), mail->id, RealItemAwardActive, title, content, mail->recvTime);
    }

    m_awards.erase(it);
    DB1().PushUpdateData("DELETE FROM `real_item_award` WHERE `id` = %u", id);
    return;
}

void RealItemAwardMgr::getInfo(Stream& st)
{
    FastMutex::ScopedLock lk(m_mutex);
    UInt8 cnt = 0;
    st << cnt;
    size_t offset = st.size();
    for(std::map<UInt32, RealItemAward>::iterator it = m_awards.begin(); it != m_awards.end(); ++ it)
    {
        RealItemAward& award = it->second;
        st << static_cast<UInt8>(it->first);
        st << award.cd << award.card_no << award.card_psw;
        ++ cnt;
    }
    st.data<UInt8>(offset) = cnt;
}

bool RealItemAwardMgr::addAward(UInt32 id, UInt32 cd, std::string& card_no, std::string& card_psw)
{
    FastMutex::ScopedLock lk(m_mutex);

    std::map<UInt32, RealItemAward>::iterator it = m_awards.find(id);
    if(it != m_awards.end())
        return false;

    RealItemAward& award = m_awards[id];
    award.cd = cd;
    award.card_no = card_no;
    award.card_psw = card_psw;

    DB1().PushUpdateData("INSERT INTO `real_item_award`(`id`, `cd`, `card_no`, `card_psw`) VALUES(%u, %u, '%s', '%s')", id, award.cd, award.card_no.c_str(), award.card_psw.c_str());

    return true;
}

bool RealItemAwardMgr::delAward(UInt32 id)
{
    FastMutex::ScopedLock lk(m_mutex);

    std::map<UInt32, RealItemAward>::iterator it = m_awards.find(id);
    if(it == m_awards.end())
        return false;

    m_awards.erase(it);
    return true;
}

}

