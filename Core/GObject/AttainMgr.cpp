#include "Config.h"
#include "Server/WorldServer.h"
#include "Player.h"
#include "AttainMgr.h"
#include "EventBase.h"
#include "DB/DBExecutor.h"
#include "DB/DBConnectionMgr.h"
#include "GObjectDBExecHelper.h"
#include "Server/OidGenerator.h"
#include "Server/Cfg.h"
#include "GData/GDataManager.h"
#include "Server/OidGenerator.h"
#include "DB/DBWorker.h"
#include "Log/Log.h"
#include "Script/GameActionLua.h"
#include "Mail.h"
#include "Country.h"
#include "Fighter.h"
#include "Common/StringTokenizer.h"
#include "MsgHandler/CountryMsgStruct.h"
#include "Common/TimeUtil.h"
#include "Server/SysMsg.h"
#include "SpecialAward.h"

namespace GObject
{
    AttainMgr::AttainMgr(Player* pl) : m_PlayerOwner(pl)
    {
    }

    AttainMgr::~AttainMgr()
    {
        for(attain_iterator it = m_AttainList.begin(); it != m_AttainList.end(); ++ it)
        {
            SAFE_DELETE(it->second);
        }
    }

    void AttainMgr::LoadAttain(AttainData* attainData)
    {
        m_AttainList[attainData->attainId] = attainData;
    }

    bool AttainMgr::HasFinishAttainment(UInt16 attainId)
    {
        attain_c_iterator it = m_AttainList.find(attainId);
        if(it == m_AttainList.end())
            return false;

        if(it->second->status != _ATTAINED_FINISHED_)
            return false;

        return true;
    }

    bool AttainMgr::HasAttained(UInt16 attainId)
    {
        attain_c_iterator it = m_AttainList.find(attainId);
        if(it == m_AttainList.end())
            return false;

        if(it->second->status != _ATTAINED_)
            return false;

        return true;
    }

    bool AttainMgr::CanAttain(UInt16 attainId)
    {
        attain_c_iterator it = m_AttainList.find(attainId);
        if(it == m_AttainList.end())
            return true;

        if(it->second->status == _ATTAINED_ || it->second->status == _ATTAINED_FINISHED_)
            return false;

        return true;
    }

    void AttainMgr::UpdateAttainment(UInt16 attainId, UInt32 status)
    {
        attain_iterator it = m_AttainList.find(attainId);
        if(it == m_AttainList.end())
        {
            AttainData* ad = new AttainData;
            ad->ownerId = m_PlayerOwner->getId();
            ad->attainId = attainId;
            ad->status = status;
            ad->updatetime = TimeUtil::Now();
            m_AttainList[attainId] = ad;
            DB().PushUpdateData("INSERT INTO `attainment` (`ownerId`, `attainId`, `status`, `updatetime`) VALUES (%"I64_FMT"u, %u, %u, %u)", ad->ownerId, ad->attainId, ad->status, ad->updatetime);
        }
        else
        {
            it->second->status = status;
            DB().PushUpdateData("UPDATE `attainment` SET `status` = %u WHERE `ownerId` = %u AND `attainId` = %u", it->second->status, it->second->ownerId, it->second->attainId);
        }
    }

    UInt32 AttainMgr::getAttainStatus(UInt16 attainId)
    {
        attain_c_iterator it = m_AttainList.find(attainId);
        if(it == m_AttainList.end())
            return 0;

        return it->second->status;
    }

    void AttainMgr::MidnightCheckAttain()
    {
        for(attain_iterator it = m_AttainList.begin(); it != m_AttainList.end(); ++ it)
        {
            if(it->second->status == _ATTAINED_FINISHED_)
                continue;

            if(it->second->status == _ATTAINED_)
                continue;

            if(it->second->updatetime + 24*3600 < TimeUtil::Now())
                it->second->status = 0;
        }
    }

}
