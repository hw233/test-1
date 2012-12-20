#include "Config.h"
#include "RechargeTmpl.h"
#include "MsgID.h"
#include "Package.h"
#include "DB/DBExecutor.h"
#include "DB/DBConnectionMgr.h"
#include "GData/GDataDBExecHelper.h"

#ifndef _WIN32
#include <sys/ioctl.h> // for ioctl
#include <net/if.h> // for struct ifreq, IF_NAMESIZE
#endif

namespace GObject
{
    RechargeTmpl::RechargeTmpl() : _filename("scripts/RechargeTmpl.lua")
    {
        m_isCleared = false;
    }
    void RechargeTmpl::Init()
    {
    }

    void RechargeTmpl::load(const char * scriptPath)
    {
        Script::RechargeTmplScript script(this);
        if(scriptPath != NULL)
        {
            script.runScript(scriptPath);
            return;
        }
        script.doFile(_filename.c_str());

        loadPlayersItem();
    }
    void RechargeTmpl::loadPlayersItem()
    {
        std::unique_ptr<DB::DBExecutor> execu(DB::gObjectDBConnectionMgr->GetExecutor());
	    if (execu.get() == NULL || !execu->isConnected()) return ;

	    GData::DBRechargeItem dbexp;
        if(execu->Prepare("SELECT `player_id`,`item_id`,`num` FROM `recharge_item`", dbexp) != DB::DB_OK)
		    return;

	    while(execu->Next() == DB::DB_OK)
        {
            std::map<UInt64, std::map<UInt32, int> >::iterator it = _playerItems.find(dbexp.player_id);
            if (it == _playerItems.end())
            {
                std::map<UInt32, int> tmp;
                tmp.insert(make_pair(dbexp.item_id, dbexp.num));
                _playerItems.insert(make_pair(dbexp.player_id, tmp));
            }
            else
            {
                (it->second)[dbexp.item_id] = dbexp.num;
            }
        }
    }
    void RechargeTmpl::clear()
    {
        if (m_isCleared)
            return;
        DB1().PushUpdateData("delete from recharge_item");
        _playerItems.clear();
        _items.clear();
        m_isCleared = true;
    }
    void RechargeTmpl::addItem(UInt32 id, UInt32 score, int num)
    {
        RechargeItem item(id, score, num);
        _items.insert(item);
    }
    void RechargeTmpl::addScore(Player* pl, UInt32 oldRecharge, UInt32 newRecharge)
    {
        static const UInt32 s_scores[][2] = {
            {10,100},
            {99,500},
            {399,260},
            {999,500},
            {1999,1000},
            {3599,1100},
            {5999,1300},
            {8999,1750},
            {13999,2400},
            {19999,2500},
            {28999,3000},
            {39999,4000},
            {55999,5000}
        };
        UInt8 count = sizeof(s_scores)/sizeof(s_scores[0][0])/2;
        UInt32 s = 0;
        for (int i = 0; i < count; ++i)
        {
            if (newRecharge < s_scores[i][0])
                break;
            if (oldRecharge < s_scores[i][0] && newRecharge >= s_scores[i][0])
                s += s_scores[i][1];
        }
        pl->AddVar(VAR_RECHARGE_SCORE, s);
    }
    void RechargeTmpl::addScore(Player* pl, UInt32 s)
    {
        pl->AddVar(VAR_RECHARGE_SCORE, s);
    }
    void RechargeTmpl::sendStreamInfo(Player* pl)
    {
        Stream st(REP::ACTIVITY_REWARD);
        st << static_cast<UInt8>(10);//列表
        UInt32 score = pl->GetVar(VAR_RECHARGE_SCORE);
        UInt8 count = _items.size();
        st << score << count;
        std::set<RechargeItem>::iterator it;
        for (it = _items.begin(); it != _items.end(); ++it)
        {
            UInt32 itemId = it->id;
            UInt32 itemScore = it->score;
            int    num =  it->num;
            if (num != -1)
                num = findNum(pl, itemId, num);
            st << itemId << itemScore << num;
        }
        st << Stream::eos;
        pl->send(st);
    }
    void RechargeTmpl::sendScoreInfo(Player* pl)
    {
        Stream st(REP::ACTIVITY_REWARD);
        st << static_cast<UInt8>(12);
        st << pl->GetVar(VAR_RECHARGE_TOTAL) << pl->GetVar(VAR_RECHARGE_SCORE) << Stream::eos;
        pl->send(st);
    }
    UInt8 RechargeTmpl::getItem(Player* pl, UInt32 itemId, int& n)
    {
        if(pl->GetPackage()->GetRestPackageSize() < 1)
            return 1;
        
        const RechargeItem* pItem = findItem(itemId);
        if (NULL == pItem)
            return 2;
        UInt32 s = pl->GetVar(VAR_RECHARGE_SCORE);
        if (s < pItem->score)
            return 3;

        n = pItem->num;
        //不是无限量的
        if (pItem->num != -1)
        {
            std::map<UInt64, std::map<UInt32, int> >::iterator it = _playerItems.find(pl->getId());
            if (it == _playerItems.end())
            {
                std::map<UInt32, int> tmp;
                tmp.insert(make_pair(itemId, pItem->num));
                _playerItems.insert(make_pair(pl->getId(), tmp));
                it = _playerItems.find(pl->getId());
            }
            std::map<UInt32, int>::iterator it1 = it->second.find(itemId);
            //没有领过该物品
            if (it1 == it->second.end())
            {
                (it->second)[itemId] = --n;
            }
            else if (it1->second > 0)
            {
                it1->second -= 1;
                n = it1->second;
            }
            else
            {
                return 4;
            }
            //数量减1
            DB1().PushUpdateData("REPLACE INTO `recharge_item` values(%"I64_FMT"u,%d,%d)", pl->getId(), itemId, n);
        }
        //additem
        pl->GetPackage()->Add(itemId, 1, true); 
        s -= pItem->score;
        pl->SetVar(VAR_RECHARGE_SCORE, s);

        char str[32] = {0};
        sprintf(str, "F_10000_1201_%d", itemId);
        pl->udpLog("huodong", str, "", "", "", "", "act");
        return 0;
    }
    int RechargeTmpl::findNum(Player* pl, UInt32 itemId, int maxNum)
    {
        std::map<UInt64, std::map<UInt32, int> >::iterator it = _playerItems.find(pl->getId());
        if (it != _playerItems.end())
        {
            std::map<UInt32, int>::iterator it1 = it->second.find(itemId);
            if (it1 != it->second.end())
            {
                return it1->second;
            }
        }
        return maxNum;
    }
    const RechargeItem* RechargeTmpl::findItem(UInt32 id)
    {
        RechargeItem t(id, 0, 0);
        std::set<RechargeItem>::iterator it = _items.find(t);
        if (it != _items.end())
            return &(*it);
        return NULL;
    }

}
namespace Script
{
    RechargeTmplScript::RechargeTmplScript( GObject::RechargeTmpl * RechargeTmpl )
    {
        class_add<GObject::RechargeTmpl>("RechargeTmpl");
        class_def<GObject::RechargeTmpl>("addItem", &GObject::RechargeTmpl::addItem);
        set("RechargeTmpl", RechargeTmpl);
    }
}

