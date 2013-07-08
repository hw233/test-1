#include "Config.h"
#include "LoadWorker.h"
#include "Mail.h"
#include "Player.h"

#include "Server/Cfg.h"
#include "Common/SHA1Engine.h"
#include "Common/LoadingCounter.h"
#include "GObjectDBExecHelper.h" 
#include <sstream>

namespace GObject
{
    LoadWorker::LoadWorker(UInt8 type, UInt8 worker) :
        WorkerRunner<>(25), m_Type(type), m_Worker(worker)
    {

    }

    LoadWorker::~LoadWorker()
    {

    }

    bool LoadWorker::Init()
    {
        return true;
    }

    std::string LoadWorker::GetLogName()
    {
        return "log/LoadWorker/";
    }

    void LoadWorker::UnInit()
    {
        OnTimer();
    }

    void LoadWorker::OnTimer()
    {
        std::vector<UrlParam> list;
        {
            FastMutex::ScopedLock lk(m_Mutex);
            list.swap(m_QueryList);
        }

        for (std::vector<UrlParam>::iterator it = list.begin(); it != list.end(); ++ it)
        {
            GObject::Player * pl = GObject::globalPlayers[it->playerId];
            if(!pl)
            {
                continue;
            }

            if(0 == it->type)
            {
                GetMailInfo(it->playerId);
            }
        }
    }

    void LoadWorker::OnPause()
    {

    }

    void LoadWorker::Push(UInt64 playerId, UInt16 type)
    {
        UrlParam urlParam;
        urlParam.playerId = playerId;
        urlParam.type = type;

        FastMutex::ScopedLock lk(m_Mutex);
        m_QueryList.push_back(urlParam);
    }

    bool LoadWorker::GetMailInfo(UInt64 playerId)
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
        if (execu.get() == NULL || !execu->isConnected()) return false;

        LoadingCounter lc("Loading mail package:");
        DBMailPackageData mpdata;
        char buf[1024] = {0};
        snprintf(buf, sizeof(buf), "SELECT `id`, `itemId`, `itemCount` FROM `mail_package` m_p, `mail` m  WHERE m_p.id = m.mailId AND m.playerId = %"I64_FMT"u  ORDER BY `id`", playerId);
        if(execu->Prepare(buf, mpdata) != DB::DB_OK) return false;
        lc.reset(50);
        UInt32 last_pid = 0xFFFFFFFF;
        MailPackage * mp = NULL;

        while(execu->Next() == DB::DB_OK)
        {
            lc.advance();
            if(mpdata.id != last_pid)
            {
                last_pid = mpdata.id;
                bool needjump = false;
                mp = mailPackageManager.add(last_pid, &needjump);
                if(needjump)
                {
                    continue;
                }
            }
            mp->push(mpdata.itemId, mpdata.itemCount);
        }
        lc.finalize();

        lc.prepare("Loading mails:");
        DBMailData mdata;
        char buf1[1024] = {0};
        snprintf(buf1, sizeof(buf1), "SELECT `mailId`, `playerId`, `sender`, `recvTime`, `flag`, `title`, `content`, `additionalId` FROM `mail` WHERE `playerId` = %"I64_FMT"u ORDER BY `playerId`, `mailId`", playerId);
        if(execu->Prepare(buf1, mdata) != DB::DB_OK) return false;
        lc.reset(500);
        GObject::Player *pl = GObject::globalPlayers[playerId];
        if(pl == NULL) return false;

        while(execu->Next() == DB::DB_OK)
        {
            lc.advance();
            pl->GetMailBox()->newMail(mdata.id, mdata.sender, mdata.recvTime, mdata.flag, mdata.title, mdata.content, mdata.additionalId);
        }
        lc.finalize();

        return true;
    }
}
