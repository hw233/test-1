#include "Config.h"
#include "MsgID.h"
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

    //是否可以达成 在一切非达成状态都可以
    bool AttainMgr::CanAttain(UInt16 attainId)
    {
        attain_c_iterator it = m_AttainList.find(attainId);
        if(it == m_AttainList.end())
            return true;

        if(it->second->status == _ATTAINED_ || it->second->status == _ATTAINED_FINISHED_)
            return false;

        return true;
    }

    //是否可以在某个状态达成  step 向达成方向前进
    void  AttainMgr::CheckAttainWithStatus(UInt16 attainId,  UInt32 tarStatus,  UInt32 step)
    {
        attain_iterator it = m_AttainList.find(attainId);
        if(it != m_AttainList.end())
        {
            //已经达成了
            if(it->second->status >= _ATTAINED_)
                return;
        }

        if(it == m_AttainList.end())
        {
            //直接达成
            if(step >= tarStatus)
            {
                UpdateAttainment( attainId, _ATTAINED_);
            }
            else
                UpdateAttainment( attainId, step);
            //UpdateDB(ad, true);
        }
        else
        {
            if(step >= tarStatus || it->second->status >= (tarStatus - step))
            {
                //达成
                UpdateAttainment(attainId, _ATTAINED_);
            }
            else
            {
                UpdateAttainment(attainId, it->second->status + step);
            }
        }

    }
    void AttainMgr::UpdateDB(const AttainData* ad, bool bInsert)
    {
        if(bInsert)
            DB().PushUpdateData("INSERT INTO `attainment` (`ownerId`, `attainId`, `status`, `updatetime`) VALUES (%"I64_FMT"u, %u, %u, %u)", ad->ownerId, ad->attainId, ad->status, TimeUtil::Now());
        else
            DB().PushUpdateData("UPDATE `attainment` SET `status` = %u , `updatetime` = %u WHERE `ownerId` = %"I64_FMT"u AND `attainId` = %u",ad->status, TimeUtil::Now(),ad->ownerId, ad->attainId);
    }


    void AttainMgr::UpdateAttainment(UInt16 attainId, UInt32 status)
    {
        Stream st(REP::ACHIEVEMENT);
        st << static_cast<UInt16>(1);
        st << attainId;

        attain_iterator it = m_AttainList.find(attainId);
        if(it == m_AttainList.end())
        {
            AttainData* ad = new AttainData;
            ad->ownerId = m_PlayerOwner->getId();
            ad->attainId = attainId;
            ad->status = status;
            ad->updatetime = TimeUtil::Now();
            m_AttainList[attainId] = ad;
            UpdateDB(ad, true );
            if(ad->status == _ATTAINED_)
            {
                st << static_cast<UInt8>(1);
            }
            else if(ad->status == _ATTAINED_FINISHED_)
            {
                st << static_cast<UInt8>(2);
            }
            else
            {
                st << static_cast<UInt8>(0);
            }
        }
        else
        {
            it->second->status = status;
            UpdateDB(it->second, false);
            if(it->second->status == _ATTAINED_)
            {
                st << static_cast<UInt8>(1);
            }
            else if(it->second->status == _ATTAINED_FINISHED_)
            {
                st << static_cast<UInt8>(2);
            }
            else
            {
                st << static_cast<UInt8>(0);
            }
        }

        st << Stream::eos;
        m_PlayerOwner->send(st);
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

    void AttainMgr::sendAttainment()
    {
        Stream st(REP::ACHIEVEMENT);
        UInt16 cnt = m_AttainList.size();
        st << cnt;

        for(attain_c_iterator it = m_AttainList.begin(); it != m_AttainList.end(); ++ it)
        {
            st << it->second->attainId;
            if(it->second->status == _ATTAINED_)
            {
                st << static_cast<UInt8>(1);
            }
            else if(it->second->status == _ATTAINED_FINISHED_)
            {
                st << static_cast<UInt8>(2);
            }
            else
            {
                st << static_cast<UInt8>(0);
            }
        }

        st << Stream::eos;
        m_PlayerOwner->send(st);
    }

}
