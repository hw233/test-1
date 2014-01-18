#include "Marry.h"
#include "Mail.h"
#include "GObject/Player.h"
#include "Common/TimeUtil.h"
#include "Server/SysMsg.h"
#include "Log/Log.h"
#include <algorithm>
#include "Common/Stream.h"
#include "MsgID.h"
#include "GObjectDBExecHelper.h"
#include "GData/AttrExtra.h"
#include "GObject/World.h"
#include "MarryBoard.h"
#include "Married.h"

namespace GObject
{
    MarryMgr gMarryMgr;
    MarryMgr::MarryMgr()
    {
    }

    MarryMgr::~MarryMgr()
    {
    }

    void MarriageInfo::addInfo(MarriageInfo* sMarriageInfo)
    {
        if(sMarriageInfo->marriageTime != 0)
        { 
            marriageTime = sMarriageInfo->marriageTime;
        }
        if(sMarriageInfo->pronouncement != "")
        {  
            pronouncement = sMarriageInfo->pronouncement;
        }
        if(sMarriageInfo->eLove != LOVE_NULL)
        {
            eLove = sMarriageInfo->eLove;
        }
        if(sMarriageInfo->jieyuanTime != 0)
        {
            jieyuanTime = sMarriageInfo->jieyuanTime;
        }
        if(sMarriageInfo->yuyueTime != 0)
        {
            yuyueTime = sMarriageInfo->yuyueTime;
        }
        if(sMarriageInfo->eWedding != 0)
        {
            eWedding = sMarriageInfo->eWedding;
        }
        if(sMarriageInfo->divorceTime != 0)
        {
            divorceTime = sMarriageInfo->divorceTime;
        }
        if(sMarriageInfo->lovers != 0)
        {
            lovers = sMarriageInfo->lovers;
        }
    }

    void MarriageInfo::eraseInfo(MarriageParm eParm)
    {
        if(eParm == PARM_marriageTime || eParm == PARM_ALL)
            marriageTime = 0;
        if(eParm == PARM_pronouncement || eParm == PARM_ALL)
            pronouncement = "";            
        if(eParm == PARM_eLove || eParm == PARM_ALL)
            eLove = LOVE_NULL;
        if(eParm == PARM_jieyuanTime || eParm == PARM_ALL)
            jieyuanTime = 0;
        if(eParm == PARM_yuyueTime || eParm == PARM_ALL)
            yuyueTime = 0;
        if(eParm == PARM_eWedding || eParm == PARM_ALL)
            eWedding = WEDDING_NULL;
        if(eParm == PARM_divorceTime || eParm == PARM_ALL)
            divorceTime = 0;
        if(eParm == PARM_lovers || eParm == PARM_ALL)
            lovers = 0;
    }

    std::map<UInt64,UInt32>& MarryMgr::GetOppsiteSexList(Player* player)//获得异性征婚薄
    {
        if(!player->getMainFighter()->getSex())//男的
            return m_femaleList;
        else
            return m_maleList;
    }
    
    std::map<UInt64,UInt32>& MarryMgr::GetSameSexList(Player* player)//获得同性征婚薄
    {
        if(!player->getMainFighter()->getSex())//男的
            return m_maleList;
        else
            return m_femaleList;
    }

    void MarryMgr::erase_marryList(Player* player)//删除结婚名单的记录
    {
        if(!player->getMainFighter()->getSex())//男的
        {
            if(m_marryList.find(player->getId()) != m_marryList.end())
                m_marryList.erase(m_marryList.find(player->getId())); 
        }
        else
        {
            if(m_marryList.find(player->GetMarriageInfo()->lovers) != m_marryList.end())
                m_marryList.erase(m_marryList.find(player->GetMarriageInfo()->lovers));
        }
    }

    UInt8 MarryMgr::getMoney(Player* player,ELoveToken e_parm, bool isUseMoney )
    {
        sMoney.clear();
        switch(e_parm) 
        {
            case LOVE_TONGXINJIE:
                sMoney.price_num = TONGXINJIE;
                sMoney.price_type = MailPackage::Gold;  
                sMoney.useType= 1;
                sMoney.eParm= 1;
                break;
            case LOVE_YUPEI:
                sMoney.price_num = YUPEI;
                sMoney.price_type = MailPackage::Gold;  
                sMoney.useType= 1;
                sMoney.eParm= 2;
                break;
            case LOVE_JINZAN:
                sMoney.price_num = JINZAN;
                sMoney.price_type = MailPackage::Gold;  
                sMoney.useType= 1;
                sMoney.eParm= 3;
                break;
            default:
                player->sendMsgCode(0, 6005);
                return 3;
        }
        if(isUseMoney)
        {
            if(player->getGold() < sMoney.price_num)
            {
                player->sendMsgCode(0, 1101);
                return 2;
            }
        }
        return 0;
    }

    UInt8 MarryMgr::getMoney(Player* player,EWedding e_parm,bool isUseMoney )
    {
        sMoney.clear();
        switch(e_parm) 
        {
            case WEDDING_BIYIQIFEI :
                sMoney.price_num = BIYIQIFEI;
                sMoney.price_type = MailPackage::Gold;  
                sMoney.useType= 2;
                sMoney.eParm= 1;
                break;
            case WEDDING_ZHULIANBIHE :
                sMoney.price_num = ZHULIANBIHE;
                sMoney.price_type = MailPackage::Gold;  
                sMoney.useType= 2;
                sMoney.eParm= 2;
                break;
            case WEDDING_LONGFENGCHENGXIANG:
                sMoney.price_num= LONGFENGCHENGXIANG;
                sMoney.price_type = MailPackage::Gold;  
                sMoney.useType= 2;
                sMoney.eParm= 3;
                break;
            default:
                player->sendMsgCode(0, 6006);
                return 3;
        }
        if(isUseMoney)
        { 
            if(player->getGold() < sMoney.price_num)
            {
                player->sendMsgCode(0, 1101);
                return 2;
            }
        }
        return 0;
    }


    void MarryMgr::GoBackPlayerStatus(Player* player, UInt8 status)
    {
        if(status == 0)
            player->GetMarriageInfo()->eraseInfo();
       
        player->SetVar(VAR_MARRY_STATUS,status);

    }

    UInt8 MarryMgr::sendMoneyMail(Player* player,UInt16 price_type,UInt32 price_num,UInt8 useType,UInt8 eParm)
    {
         
        MailPackage::MailItem * mitem = new MailPackage::MailItem;
        mitem->id = price_type;
        mitem->count = price_num;
        
        std::string str_type; 
        std::string str_Parm;
        UInt16 content_flag = 0;
        switch (useType)
        {
            case 1:
                //str_type = "征婚信物";
                str_type = "征婚退还仙石";
                switch (eParm)
                {
                    case 1:
                        str_Parm = "同心结";
                        break;
                    case 2:
                        str_Parm = "玉佩";
                        break;
                    case 3:
                        str_Parm = "金簪";
                        break;
                    default:
                        return 1;
                }
                content_flag = 910;
                break;
            case 2:
                //str_type = "预约婚礼";
                str_type = "婚礼退还仙石";
                switch (eParm)
                {
                    case 1:
                        str_Parm = "比翼齐飞";
                        break;
                    case 2:
                        str_Parm = "珠联璧合";
                        break;
                    case 3:
                        str_Parm = "龙凤呈祥";
                        break;
                    default:
                        return 1;
                }
                content_flag = 911;
                break;
            case 3:
                //str_type = "离婚申请";
                str_type = "离婚退还仙石";
                switch (eParm)
                {
                    case 1:
                        str_Parm = "共同离婚";
                        break;
                    default:
                        return 1;
                }
                content_flag = 913;
                break;
            case 4:
                str_type = "征婚费用";
                content_flag = 914;
                break;
            case 5:
                str_type = "成功离婚";
                content_flag = 915;
                break;
            case 6:
                str_type = "婚礼退还仙石";
                content_flag = 912;
                break;
            case 7:
                str_type = "婚礼超时处理";
                content_flag = 917;
                break;
            case 8:
                str_type = "婚礼超时处理";
                content_flag = 918;
                break;
            default:
                return 1;
        }
        SYSMSGV(title, 916,str_type.c_str());
        SYSMSGV(content, content_flag,price_num);
        
        MailItemsInfo itemsInfo(mitem, BuChangMarry, 1);
        Mail * pmail; 
        if(price_num == 0)    
            pmail = player->GetMailBox()->newMail(NULL, 0x01, title, content, 0xFFFE0000, true, &itemsInfo);
        else
            pmail = player->GetMailBox()->newMail(NULL, 0x21, title, content, 0xFFFE0000, true, &itemsInfo);
        if(pmail != NULL)
            mailPackageManager.push(pmail->id, mitem, 1, true);

        return 0;
    }

    UInt8 MarryMgr::useMoney(Player* player,UInt8 price_type,UInt32 price_num,UInt8 useType)
    {
        ConsumeInfo ci; 
        switch (useType)
        {
            case 1:
                ci.purchaseType = DINGQINGXINWU; 
                break;
            case 2:
                ci.purchaseType = JieHun; 
                break;
            case 3:
                ci.purchaseType = LiHun;
                break;
            case 4:
                ci.purchaseType = ZhengHun; 
                break;
            default:
                return 1;
        }
        if(price_type == 0)
            player->useGold(price_num,&ci);
        else
            player->useTael(price_num,&ci);
        return 0;
    }

    void MarryMgr::CheckingListTimeOut(std::map<UInt64,UInt32>& m_MarriageList)
    {
        Mutex::ScopedLock lk(_mutex);
        for(MarriageList::iterator it = m_MarriageList.begin();it != m_MarriageList.end();)
        {
            if(it->second > TimeUtil::Now() - MARRY_TIME_OUT)
            { 
                it++;
                break;
            }
            else
            {
                GObject::Player * player = GObject::globalPlayers[it->first];
                it++;
                CancelMarriage(player,1); 
            }
        }
        return;    
    }

    bool MarryMgr::CheckingTimeOut(UInt32 marriageTime)
    {
        if(marriageTime <= TimeUtil::Now() - MARRY_TIME_OUT)
            return false; 

        return true;
    }


    UInt8 MarryMgr::StartMarriage(Player* player,MarriageInfo* sMarriage)
    {
        Mutex::ScopedLock lk(_mutex); 

        //CheckingTimeOut(player,m_maleList);    
        if(player->GetVar(VAR_MARRY_STATUS) == 1 && player->GetMarriageInfo()->pronouncement == "")//回复结缘的状态
        {
            return 2;
        }

        if(player->GetVar(VAR_MARRY_STATUS) != 0 )
        {
            player->sendMsgCode(0, 6002);
            return 1;
        }
        UInt8 ret;
        if(!player->getMainFighter()->getSex())//男的
        {
            ret = getMoney(player,sMarriage->eLove,true);
            if(ret != 0)
                return ret;
            if(player->getGold() < ZHENGHUN + sMoney.price_num)
            {
                player->sendMsgCode(0, 1101);
                return 1;
            }
        }
        else
        {
            if(player->getGold() < ZHENGHUN)
            {
                player->sendMsgCode(0, 1101);
                return 1;
            }
        }
       
//        CheckingTimeOut(m_maleList);
  //      CheckingTimeOut(m_femaleList);

        MarriageList::iterator it = GetSameSexList(player).find(player->getId()); 
        if(it != GetSameSexList(player).end())
        {
            player->sendMsgCode(0, 6007);
            return 1;
        }

        ReplyMarriageList::iterator it1 = m_replyList.find(player->getId());
        if(it1 != m_replyList.end())
        {
            player->sendMsgCode(0, 6007);
            return 1;
        }

        UInt32 now_time = TimeUtil::Now();
        sMarriage->marriageTime = now_time;
        player->GetMarriageInfo()->addInfo(sMarriage);
        
        GetSameSexList(player).insert(std::make_pair(player->getId(),now_time));

        player->SetVar(VAR_MARRY_STATUS,1);

        std::vector<UInt64> vec;    
        m_replyList.insert(std::make_pair(player->getId(),vec)) ;
       
        DB7().PushUpdateData("REPLACE INTO `marriage` VALUES(%" I64_FMT "u, %u, '%s',%u,%u)", player->getId(), now_time, sMarriage->pronouncement.c_str(),static_cast<UInt8>(sMarriage->eLove),0);
        //扣钱
        if(!player->getMainFighter()->getSex())//男的
        {
            useMoney(player,sMoney.price_type,sMoney.price_num,sMoney.useType);
            useMoney(player,0,ZHENGHUN,4);
            player->udpLog("jiehunqianzhi", "F_140102_1", "", "", "", "", "act");
            switch(sMarriage->eLove)
            {
                case LOVE_TONGXINJIE:
                    player->udpLog("jiehunqianzhi", "F_140102_5", "", "", "", "", "act");
                    break;
                case LOVE_YUPEI:
                    player->udpLog("jiehunqianzhi", "F_140102_6", "", "", "", "", "act");
                    break;
                case LOVE_JINZAN:
                    player->udpLog("jiehunqianzhi", "F_140102_7", "", "", "", "", "act");
                    break;
                default:
                    break;
            }
        }
        else
        {
            useMoney(player,0,ZHENGHUN,4);
            player->udpLog("jiehunqianzhi", "F_140102_2", "", "", "", "", "act");
        }
       
        Stream st2(REP::MARRYMGR);
        st2 << static_cast<UInt8>(3) << static_cast<UInt8>(0) << static_cast<UInt8>(sMarriage->eLove) << sMarriage->pronouncement << Stream::eos;
        player->send(st2);

        GObject::Player * obj_player = 0; 
        sendMarriageTimeOut(player,obj_player);
        return 0;
    }

    UInt8 MarryMgr::doCancelMarriage(Player* player,UInt8 flag)
    {
        Mutex::ScopedLock lk(_mutex); 
        return CancelMarriage(player,flag);
    }

    UInt8 MarryMgr::CancelMarriage(Player* player, UInt8 flag)
    {
        if(player->GetVar(VAR_MARRY_STATUS) != 1 )
        {
            player->sendMsgCode(0, 6002);
            return 1;
        }
        
        if(flag == 0)
        {
            if(player->GetVar(VAR_CANCEL_MARRIAGE) < TimeUtil::Now())
                player->SetVar(VAR_CANCEL_MARRIAGE,TimeUtil::Now() + 86400);
            else
            {
                player->sendMsgCode(0, 6025);
                return 1;
            }
        }

        MarriageList::iterator it = GetSameSexList(player).find(player->getId()); 
        if(it != GetSameSexList(player).end())
            GetSameSexList(player).erase(it);         
        else
        {
            player->sendMsgCode(0, 6009);
            return 1;
        }        
        if(!player->getMainFighter()->getSex())
        { 
            UInt8 ret = getMoney(player,player->GetMarriageInfo()->eLove);
            if(ret != 0)
                return 1;
            //还信物钱
            sendMoneyMail(player,sMoney.price_type,sMoney.price_num,sMoney.useType,sMoney.eParm); 
        }
        sendMoneyMail(player,MailPackage::Gold,ZHENGHUN,4,1); 

        Stream st2(REP::MARRYMGR);
        st2 << static_cast<UInt8>(9) << static_cast<UInt8>(10) << player->getId() << player->getMainFighter()->getName() << player->getMainFighter()->getColor() << Stream::eos;
        
        std::vector<UInt64>& vec = m_replyList.find(player->getId())->second;
        while(!vec.empty())            
        {
            Player * pl = globalPlayers[vec[0]];
            if(!pl)
            {
                vec.erase(vec.begin());
                continue;
            }
             
            if(flag == 1)
                pl->send(st2);

            if(pl->GetVar(VAR_MARRY_STATUS) == 1)
            { 
                if(!pl->getMainFighter()->getSex())
                { 
                    if(!getMoney(pl,pl->GetMarriageInfo()->eLove))
                        sendMoneyMail(pl,sMoney.price_type,sMoney.price_num,sMoney.useType,sMoney.eParm); //还信物钱
                }
            }
            pl->GetMarriageInfo()->eraseInfo();
            pl->SetVar(VAR_MARRY_STATUS,0);
           
            Stream st3(REP::MARRYMGR);
            st3 << static_cast<UInt8>(7) << static_cast<UInt8>(pl->GetVar(VAR_MARRY_STATUS)) << static_cast<UInt8>(0) << static_cast<UInt8>(0) << Stream::eos ;
            pl->send(st3);

            vec.erase(vec.begin());
    	    DB7().PushUpdateData("DELETE FROM `marriage` WHERE `status` = %" I64_FMT "u", player->getId());

        }
        
        ReplyMarriageList::iterator i = m_replyList.find(player->getId());
        if(i != m_replyList.end())
            m_replyList.erase(m_replyList.find(player->getId()));            

	    DB7().PushUpdateData("DELETE FROM `marriage` WHERE `playerid` = %" I64_FMT "u", player->getId());
        GoBackPlayerStatus(player,0);

        if(flag == 1)
            player->send(st2);
        return 0; 

    }

    UInt8 MarryMgr::ReplyMarriage(Player* player,MarriageInfo* sMarriage, UInt64 obj_playerid)
    {
        Mutex::ScopedLock lk(_mutex); 
        GObject::Player * obj_player = GObject::globalPlayers[obj_playerid];//获得结缘玩家对象
        if(!obj_player)
        {
            player->sendMsgCode(0, 6012);
            return 1;
        }
        if(player->GetVar(VAR_MARRY_STATUS) != 0)
        {
            player->sendMsgCode(0, 6002);
            return 1;
        }
        if(!CheckingTimeOut(obj_player->GetMarriageInfo()->marriageTime))
        {
            Stream st4(REP::MARRYMGR);
            st4 << static_cast<UInt8>(9) << static_cast<UInt8>(10) << obj_player->getId() << obj_player->getMainFighter()->getName() << obj_player->getMainFighter()->getColor() << Stream::eos;
            player->send(st4); 
            return 2;
        }
        if(!player->getMainFighter()->getSex())//男的
        {
            UInt8 ret = getMoney(player,sMarriage->eLove,true);
            if(ret != 0)
                return 1;
        }
        if(m_replyList.find(obj_playerid) != m_replyList.end())
        {
            std::vector<UInt64>& vec = m_replyList.find(obj_playerid)->second;
            if(std::find(vec.begin(),vec.end(),static_cast<UInt64>(player->getId())) != vec.end())
                return 1;
            else
            {
                vec.insert(vec.begin(),static_cast<UInt64>(player->getId()));
                UInt32 now_time = TimeUtil::Now();
                sMarriage->marriageTime = now_time;
                DB7().PushUpdateData("REPLACE INTO `marriage` VALUES(%" I64_FMT "u, %u, '%s',%u,%" I64_FMT "u)", player->getId(), now_time, sMarriage->pronouncement.c_str(),static_cast<UInt8>(sMarriage->eLove),obj_playerid);
                player->SetVar(VAR_MARRY_STATUS,1);
                if(!player->getMainFighter()->getSex())//男的
                { 
                    player->GetMarriageInfo()->addInfo(sMarriage);
                    //扣钱
                    useMoney(player,sMoney.price_type,sMoney.price_num,sMoney.useType);
                    player->udpLog("jiehunqianzhi", "F_140102_3", "", "", "", "", "act");
                    switch(sMarriage->eLove)
                    {
                        case LOVE_TONGXINJIE:
                            player->udpLog("jiehunqianzhi", "F_140102_5", "", "", "", "", "act");
                            break;
                        case LOVE_YUPEI:
                            player->udpLog("jiehunqianzhi", "F_140102_6", "", "", "", "", "act");
                            break;
                        case LOVE_JINZAN:
                            player->udpLog("jiehunqianzhi", "F_140102_7", "", "", "", "", "act");
                            break;
                        default:
                            break;
                    }
                }
                else
                    player->udpLog("jiehunqianzhi", "F_140102_4", "", "", "", "", "act");

            }
        }
        else
            return 1;

        player->sendMsgCode(0, 6021);

        Stream st(REP::MARRYMGR);
        st << static_cast<UInt8>(3) << static_cast<UInt8>(6) << static_cast<UInt8>(sMarriage->eLove) << sMarriage->pronouncement << Stream::eos;
        player->send(st);
        
        Stream st2(REP::MARRYMGR);
        st2 << static_cast<UInt8>(9) << static_cast<UInt8>(1) << player->getId() << player->getName() << player->getMainFighter()->getColor() << Stream::eos;

        player->GetMarriageInfo()->lovers = obj_playerid;
        obj_player->send(st2);
       
        Stream st3(REP::MARRYMGR);
        st3 << static_cast<UInt8>(3) << static_cast<UInt8>(2) << static_cast<UInt8>(player->GetMarriageInfo()->eLove) << "" << Stream::eos;
        player->send(st3);

        sendMarriageTimeOut(player,obj_player);
        GetList(player,0,1);

        return 0;
    }

    UInt8 MarryMgr::CancelReplayMarriage(Player* player, UInt64 obj_playerid)
    {
        Mutex::ScopedLock lk(_mutex); 
        if(player->GetVar(VAR_CANCEL_REPLYMARRIAGE) < TimeUtil::Now())
            player->SetVar(VAR_CANCEL_REPLYMARRIAGE,TimeUtil::Now() + 86400);
        else
        {
            player->sendMsgCode(0, 6024);
            return 1;
        }
        
        if(player->GetVar(VAR_MARRY_STATUS) != 1)
        {
            player->sendMsgCode(0, 6002);
            return 1;
        }
        GObject::Player * obj_player = GObject::globalPlayers[obj_playerid];//获得结缘玩家对象
        if(!obj_player)
        {
            player->sendMsgCode(0, 6012);
            return 1;
        }
        if(!CheckingTimeOut(obj_player->GetMarriageInfo()->marriageTime))
        {
            Stream st4(REP::MARRYMGR);
            st4 << static_cast<UInt8>(9) << static_cast<UInt8>(10) << obj_player->getId() << obj_player->getMainFighter()->getName() << obj_player->getMainFighter()->getColor() << Stream::eos;
            player->send(st4); 
            return 2;
        }
        if(!player->getMainFighter()->getSex())//男的
        {
            UInt8 ret = getMoney(player,player->GetMarriageInfo()->eLove);
            if(ret != 0)
                return 1;
        }
        if(m_replyList.find(obj_playerid) != m_replyList.end())
        {
            std::vector<UInt64>& vec = m_replyList.find(obj_playerid)->second;
            std::vector<UInt64>::iterator it = std::find(vec.begin(),vec.end(),player->getId());
            if(it != vec.end())
            {
                vec.erase(it);
                if(!player->getMainFighter()->getSex())
                { 
                    //还信物钱
                    sendMoneyMail(player,sMoney.price_type,sMoney.price_num,sMoney.useType,sMoney.eParm); 
                }
                player->GetMarriageInfo()->eraseInfo();
                DB7().PushUpdateData("DELETE FROM `marriage` WHERE `playerid` = %" I64_FMT "u", player->getId());
                player->SetVar(VAR_MARRY_STATUS,0);
                player->sendMsgCode(0, 6022);

            }
            else
            {
                player->sendMsgCode(0, 6010);
                return 1;
            }
        }
        else
            return 1;

        return 0;
    }

    UInt8 MarryMgr::JieYuan(Player* player, UInt64 obj_playerid)
    {
        Mutex::ScopedLock lk(_mutex); 
        GObject::Player * obj_player = GObject::globalPlayers[obj_playerid];//获得结缘玩家对象
        if(!obj_player)
        {
            player->sendMsgCode(0, 6012);
            return 1;
        }
        if(!CheckingTimeOut(player->GetMarriageInfo()->marriageTime))
        {
            Stream st4(REP::MARRYMGR);
            st4 << static_cast<UInt8>(9) << static_cast<UInt8>(10) << player->getId() << player->getMainFighter()->getName() << player->getMainFighter()->getColor() << Stream::eos;
            player->send(st4); 
            return 2;
        }
        if(player->GetVar(VAR_MARRY_STATUS) != 1 || obj_player->GetVar(VAR_MARRY_STATUS) != 1 || obj_player->GetMarriageInfo()->pronouncement != "")
        {
            player->sendMsgCode(0, 6002);
            return 1;
        }
        
        if(!player->getMainFighter()->getSex())//男的
        {
            UInt8 ret = getMoney(player,player->GetMarriageInfo()->eLove);
            if(ret != 0)
                return 1;
        }
        
        ReplyMarriageList::iterator i = m_replyList.find(player->getId());
        if(i == m_replyList.end())
        {
            player->sendMsgCode(0, 6010);
            return 1;
        }
                    
        MarriageList::iterator it;
        PairList::iterator it1; 
        if(!player->getMainFighter()->getSex())//男的
        {
            it1 = m_jieyuanList.find(player->getId());
            if(it1 != m_jieyuanList.end())
            {
                player->sendMsgCode(0, 6016);
                return 1;
            }
            
            it = m_maleList.find(player->getId()); 
            if(it != m_maleList.end())
                m_maleList.erase(it);
            else
            {
                player->sendMsgCode(0, 6009);
                return 1;
            }
        }
        else
        {
            it1 = m_jieyuanList.find(obj_player->getId());
            if(it1 != m_jieyuanList.end())
            {
                player->sendMsgCode(0, 6016);
                return 1;
            }
            
            it = m_femaleList.find(player->getId()); 
            if(it != m_femaleList.end())
                m_femaleList.erase(it);
            else
            {
                player->sendMsgCode(0, 6009);
                return 1;
            }
        }
                
        std::vector<UInt64>& vec = m_replyList.find(player->getId())->second;
        while(!vec.empty())            
        {
            if(vec[0] == obj_playerid)
            {
               vec.erase(vec.begin());
                continue;
            }
            Player * pl = globalPlayers[vec[0]];
            if(pl->GetVar(VAR_MARRY_STATUS) != 1)
            {
               vec.erase(vec.begin());
                continue;
            }
            
            if(!pl->getMainFighter()->getSex())
            { 
                UInt8 ret1 = getMoney(pl,pl->GetMarriageInfo()->eLove);
                if(ret1 == 0)
                {
                    //还信物钱
                    sendMoneyMail(pl,sMoney.price_type,sMoney.price_num,sMoney.useType,sMoney.eParm); 
                }
            }
            pl->GetMarriageInfo()->eraseInfo();
            vec.erase(vec.begin());
    	    DB7().PushUpdateData("DELETE FROM `marriage` WHERE `status` = %" I64_FMT "u", pl->getId());
            pl->SetVar(VAR_MARRY_STATUS,0);

        }
        
        UInt64 man_playerid,woman_playerid;
        UInt8 marriage_buyer = 0;
        UInt8 lover_item = 0;
        if(!player->getMainFighter()->getSex())//男的
        {
            man_playerid = player->getId();
            woman_playerid = obj_player->getId();
            marriage_buyer = 0; 
            lover_item = static_cast<UInt8>(player->GetMarriageInfo()->eLove);
        }
        else
        {
            woman_playerid = player->getId();
            man_playerid = obj_player->getId();
            marriage_buyer = 1;
            lover_item = static_cast<UInt8>(obj_player->GetMarriageInfo()->eLove);
        }
        m_jieyuanList.insert(std::make_pair(man_playerid,woman_playerid));
        m_replyList.erase(m_replyList.find(player->getId()));            
        
        UInt32 now_time = TimeUtil::Now();
        player->GetMarriageInfo()->jieyuanTime = now_time;
        player->GetMarriageInfo()->lovers = obj_playerid;

        obj_player->GetMarriageInfo()->jieyuanTime = now_time;
        obj_player->GetMarriageInfo()->lovers = player->getId();

        if(player->GetMarriageInfo()->pronouncement == "")
            sendMoneyMail(obj_player,MailPackage::Gold,ZHENGHUN,4,1); 
        else
            sendMoneyMail(player,MailPackage::Gold,ZHENGHUN,4,1); 


    	DB7().PushUpdateData("DELETE FROM `marriage` WHERE `playerid` = %" I64_FMT "u AND `status` = %" I64_FMT "u", player->getId(),0);
    	DB7().PushUpdateData("DELETE FROM `marriage` WHERE `playerid` = %" I64_FMT "u AND `status` = %" I64_FMT "u", obj_player->getId(),player->getId());
        DB7().PushUpdateData("REPLACE INTO `reply_marriage` VALUES(%" I64_FMT "u, %" I64_FMT "u, %u,%u,%u,%u)",man_playerid,woman_playerid,time,0,0,0);
        DB7().PushUpdateData("REPLACE INTO `marry_log` VALUES(%" I64_FMT "u, %" I64_FMT "u, '%s',%u,%u,%u,%u)", man_playerid, woman_playerid, player->GetMarriageInfo()->pronouncement.c_str(),lover_item,player->GetMarriageInfo()->marriageTime,obj_player->GetMarriageInfo()->marriageTime,marriage_buyer);

        player->SetVar(VAR_MARRY_STATUS,2);
        obj_player->SetVar(VAR_MARRY_STATUS,2);
        
        Stream st(REP::MARRYMGR);
        st << static_cast<UInt8>(3) << static_cast<UInt8>(8) << obj_playerid << obj_player->getName() << obj_player->getMainFighter()->getClass() << obj_player->getMainFighter()->getSex() << obj_player->getMainFighter()->getColor() << Stream::eos;
        player->send(st);
        
        Stream st2(REP::MARRYMGR);
        st2 << static_cast<UInt8>(9) << static_cast<UInt8>(3) << player->getId() << player->getName() << player->getMainFighter()->getColor() << Stream::eos;

        obj_player->send(st2);
        
        return 0;
    }

    UInt8 MarryMgr::ReqWeddingAppointMent(Player* player,MarriageInfo* sMarry)
    {
        Mutex::ScopedLock lk(_mutex); 
        if(!player->GetMarriageInfo()->lovers)
        {
            player->sendMsgCode(0, 6011);
            return 1;
        }
        Player * obj_player = globalPlayers[player->GetMarriageInfo()->lovers];
        if(player->GetVar(VAR_MARRY_STATUS) != 2)
        {
            player->sendMsgCode(0, 6002);
            return 1;
        }
        if(!obj_player)
        {
            player->sendMsgCode(0, 6012);
            return 1;
        }
        if(obj_player->GetVar(VAR_MARRY_STATUS) != 2)
        {
            player->sendMsgCode(0, 6013);
            return 1;
        }
        if(getMoney(player,sMarry->eWedding,true) != 0)
            return 1;
        
        if(TimeUtil::GetYYMMDD(sMarry->yuyueTime) == TimeUtil::GetYYMMDD())
        {
            player->sendMsgCode(0, 6023);
            return 1;
        }
        
        if(sMarry->yuyueTime < TimeUtil::Now())
        {
            player->sendMsgCode(0, 6023);
            return 1;
        }

        useMoney(player,sMoney.price_type,sMoney.price_num,sMoney.useType);

        player->GetMarriageInfo()->addInfo(sMarry);
		
        if(!player->getMainFighter()->getSex())//男的
            DB7().PushUpdateData("UPDATE `reply_marriage` SET `jh_time` = %u,`wedding_type` = %u,`wedding_buyer` = %u  WHERE `man_playerid` = %" I64_FMT "u", sMarry->yuyueTime,static_cast<UInt8>(sMarry->eWedding),0,player->getId());
        else    
            DB7().PushUpdateData("UPDATE `reply_marriage` SET `jh_time` = %u,`wedding_type` = %u,`wedding_buyer` = %u  WHERE `woman_playerid` = %" I64_FMT "u", sMarry->yuyueTime,static_cast<UInt8>(sMarry->eWedding),1,player->getId());
        player->SetVar(VAR_MARRY_STATUS,3);

        ELoveToken eLove = LOVE_NULL;
        std::string str_pronouncement = "";
        if(player->GetMarriageInfo()->eLove != LOVE_NULL)
            eLove = player->GetMarriageInfo()->eLove;
        else
            eLove = obj_player->GetMarriageInfo()->eLove;
        if(player->GetMarriageInfo()->pronouncement != "")
            str_pronouncement = player->GetMarriageInfo()->pronouncement;
        else
            str_pronouncement = obj_player->GetMarriageInfo()->pronouncement;

        Stream st(REP::MARRYMGR);
        st << static_cast<UInt8>(6) << static_cast<UInt8>(1) << static_cast<UInt32>(sMarry->yuyueTime) << static_cast<UInt8>(sMarry->eWedding) << static_cast<UInt8>(eLove) << str_pronouncement<< Stream::eos;
         
        player->send(st);
        obj_player->send(st);
        
        Stream st2(REP::MARRYMGR);
        st2 << static_cast<UInt8>(9) << static_cast<UInt8>(4) << player->getId() << player->getName() << player->getMainFighter()->getColor() << Stream::eos;

        obj_player->send(st2);

        switch(sMarry->eWedding)
        {
            case WEDDING_BIYIQIFEI:
                player->udpLog("jiehunqianzhi", "F_140102_8", "", "", "", "", "act");
                break;
            case WEDDING_ZHULIANBIHE:
                player->udpLog("jiehunqianzhi", "F_140102_9", "", "", "", "", "act");
                break;
            case WEDDING_LONGFENGCHENGXIANG:
                player->udpLog("jiehunqianzhi", "F_140102_10", "", "", "", "", "act");
                break;
            default:
                break;
        }
        
        return 0;
    }

    UInt8 MarryMgr::ConfirmReqWeddingAppointMent(Player* player)
    {
        Mutex::ScopedLock lk(_mutex); 
        if(player->GetVar(VAR_MARRY_STATUS) != 2)
        {
            player->sendMsgCode(0, 6002);
            return 1;
        }
        Player * obj_player = globalPlayers[player->GetMarriageInfo()->lovers];
        if(!obj_player)
        {
            player->sendMsgCode(0, 6012);
            return 1;
        }
        if(obj_player->GetVar(VAR_MARRY_STATUS) != 3)
        {
            player->sendMsgCode(0, 6013);
            return 2;
        }
        PairList::iterator it1 ;

        std::pair<ReserveList,bool> ret;
        ReserveList::iterator it; 

        for(it = m_yuyueList.begin();it != m_yuyueList.end(); ++it)
        {
            if(TimeUtil::GetYYMMDD(it->first) == TimeUtil::GetYYMMDD(obj_player->GetMarriageInfo()->yuyueTime))
            {
                player->sendMsgCode(0, 6013);//预约列表已被占用
                sendyuyueList(player,obj_player); 
                return 2;
            }
        }
        UInt32 time = obj_player->GetMarriageInfo()->yuyueTime;
        std::pair<UInt32,std::pair<UInt64,UInt64>> pair1;
        std::pair<UInt64,UInt64> pair2;

        if(!player->getMainFighter()->getSex())//男的
        {
            it1 = m_jieyuanList.find(player->getId());
            if(it1 == m_jieyuanList.end())
            {
                player->sendMsgCode(0, 6015);
                return 1;
            }
   
            pair2 = std::make_pair(static_cast<UInt64>(player->getId()),static_cast<UInt64>(obj_player->getId()));
        }
        else
        {
            it1 = m_jieyuanList.find(obj_player->getId());
            if(it1 == m_jieyuanList.end())
            {
                player->sendMsgCode(0, 6015);
                return 1;
            }

            pair2 = std::make_pair(static_cast<UInt64>(obj_player->getId()),static_cast<UInt64>(player->getId()));
        }
        pair1 = std::make_pair(time,pair2);
        m_yuyueList.insert(pair1);
        
        if(!player->getMainFighter()->getSex())//男的
            m_jieyuanList.erase(m_jieyuanList.find(player->getId())); 
        else
            m_jieyuanList.erase(m_jieyuanList.find(obj_player->getId())); 

        player->SetVar(VAR_MARRY_STATUS,4);
        obj_player->SetVar(VAR_MARRY_STATUS,4);

        player->udpLog("jiehunqianzhi", "F_140102_11", "", "", "", "", "act");
        sendWhoisMarrybuyer(player,obj_player);//告知客户端谁是婚礼购买者

        Process();
        return 0;
    }
    
    UInt8 MarryMgr::CancelReqWeddingAppointMent(Player* player)
    {
        Mutex::ScopedLock lk(_mutex); 
        
        Player * obj_player = globalPlayers[player->GetMarriageInfo()->lovers];
        if(!obj_player)
        {
            player->sendMsgCode(0, 6012);
            return 1;
        }
        if(player->GetVar(VAR_MARRY_STATUS) != 2 || obj_player->GetVar(VAR_MARRY_STATUS) != 3)
        {
            player->sendMsgCode(0, 6002);
            return 1;
        }

        if(getMoney(obj_player,obj_player->GetMarriageInfo()->eWedding) != 0)
        {
            player->sendMsgCode(0, 6019);
            return 1;
        }        
        obj_player->GetMarriageInfo()->eraseInfo(PARM_yuyueTime);  
        obj_player->GetMarriageInfo()->eraseInfo(PARM_eWedding);  
        
        sendMoneyMail(obj_player,sMoney.price_type,sMoney.price_num,sMoney.useType,sMoney.eParm); 
        
        if(!player->getMainFighter()->getSex())//男的
            DB7().PushUpdateData("UPDATE `reply_marriage` SET `jh_time` = %u,`wedding_type` = %u,`wedding_buyer` = %u  WHERE `man_playerid` = %u", 0,0,0,player->getId());
        else
            DB7().PushUpdateData("UPDATE `reply_marriage` SET `jh_time` = %u,`wedding_type` = %u,`wedding_buyer` = %u  WHERE `woman_playerid` = %u", 0,0,0,player->getId());

        obj_player->SetVar(VAR_MARRY_STATUS,2);

        Stream st2(REP::MARRYMGR);
        st2 << static_cast<UInt8>(9) << static_cast<UInt8>(5) << player->getId() << player->getName() << player->getMainFighter()->getColor() << Stream::eos;
        obj_player->send(st2);
       
        return 0;
    }

    UInt8 MarryMgr::doCancelAppointMent(Player* player)
    {
        Mutex::ScopedLock lk(_mutex); 
        UInt8 ret = CancelAppointMent(player);
        return ret;
    }

    UInt8 MarryMgr::CancelAppointMent(Player* player)
    {
        
        Player * obj_player = globalPlayers[player->GetMarriageInfo()->lovers];
        if(!obj_player)//爱人不存在
        {
            player->sendMsgCode(0, 6012);
            return 1;
        }
        UInt32 time1 = 0; 
        if(player->GetMarriageInfo()->yuyueTime != 0)
            time1 = player->GetMarriageInfo()->yuyueTime;
        if(obj_player->GetMarriageInfo()->yuyueTime != 0)
            time1 = obj_player->GetMarriageInfo()->yuyueTime;
        if(time1 == 0)
        {
            player->sendMsgCode(0, 6018);
            return 1;
        }
        if(time1 - TimeUtil::Now() < 86400 * 2)//判断前两天不能取消确定的婚约
        {
            player->sendMsgCode(0, 6017);
            return 1;
        }

        if(player->GetVar(VAR_MARRY_STATUS) == 3)
        {
            if(obj_player->GetMarriageInfo()->yuyueTime == 0 )  
            {
                if(getMoney(player,player->GetMarriageInfo()->eWedding) != 0)
                    return 1; 
                 
                player->GetMarriageInfo()->eraseInfo(PARM_eWedding);//移除婚礼规格
                player->GetMarriageInfo()->eraseInfo(PARM_yuyueTime);//移除预约时间
                sendMoneyMail(player,sMoney.price_type,sMoney.price_num,sMoney.useType,sMoney.eParm); 
                if(!player->getMainFighter()->getSex())//男的
                    DB7().PushUpdateData("UPDATE `reply_marriage` SET `jh_time` = %u,`wedding_type` = %u,`wedding_buyer` = %u  WHERE `man_playerid` = %" I64_FMT "u", 0,0,0,player->getId());
                else
                    DB7().PushUpdateData("UPDATE `reply_marriage` SET `jh_time` = %u,`wedding_type` = %u,`wedding_buyer` = %u  WHERE `woman_playerid` = %" I64_FMT "u", 0,0,0,player->getId());
                
                player->SetVar(VAR_MARRY_STATUS,2);        
                return 0;
            }
            else
            {
                player->sendMsgCode(0, 6002);
                return 1;
            }
        }
        
        if(player->GetVar(VAR_MARRY_STATUS) == 4 && obj_player->GetVar(VAR_MARRY_STATUS) == 4)
        {
                            
            if(player->GetMarriageInfo()->yuyueTime != 0 && player->GetMarriageInfo()->eWedding != WEDDING_NULL )  
            {
                if(getMoney(player,player->GetMarriageInfo()->eWedding) != 0)
                    return 1; 
                sMoney.price_num = sMoney.price_num * 0.8;
                
                ReserveList::iterator it = m_yuyueList.find(player->GetMarriageInfo()->yuyueTime);
                if(it != m_yuyueList.end())
                {
                    m_yuyueList.erase(it);
                    player->GetMarriageInfo()->eraseInfo(PARM_yuyueTime);
                    player->GetMarriageInfo()->eraseInfo(PARM_eWedding);
                }
                else
                {
                    player->sendMsgCode(0, 6014);
                    return 1;
                }
                sendMoneyMail(player,sMoney.price_type,sMoney.price_num,6,sMoney.eParm); 
            }
            else
            {
                if(getMoney(obj_player,obj_player->GetMarriageInfo()->eWedding) != 0)
                    return 1; 

                ReserveList::iterator it = m_yuyueList.find(obj_player->GetMarriageInfo()->yuyueTime);
                if(it != m_yuyueList.end())
                {
                    m_yuyueList.erase(it);
                    obj_player->GetMarriageInfo()->eraseInfo(PARM_yuyueTime);
                    obj_player->GetMarriageInfo()->eraseInfo(PARM_eWedding);
                }
                else
                {
                    player->sendMsgCode(0, 6014);
                    return 1;
                }
                sendMoneyMail(obj_player,sMoney.price_type,sMoney.price_num,sMoney.useType,sMoney.eParm); 
            }
            
            if(!player->getMainFighter()->getSex())//男的
                m_jieyuanList.insert(std::make_pair(static_cast<UInt64>(player->getId()),obj_player->getId()));
            else
                m_jieyuanList.insert(std::make_pair(static_cast<UInt64>(obj_player->getId()),player->getId()));
            
            if(!player->getMainFighter()->getSex())//男的
                DB7().PushUpdateData("UPDATE `reply_marriage` SET `jh_time` = %u,`wedding_type` = %u,`wedding_buyer` = %u  WHERE `man_playerid` = %" I64_FMT "u", 0,0,0,player->getId());
            else
                DB7().PushUpdateData("UPDATE `reply_marriage` SET `jh_time` = %u,`wedding_type` = %u,`wedding_buyer` = %u  WHERE `woman_playerid` = %" I64_FMT "u", 0,0,0,player->getId());
            obj_player->SetVar(VAR_MARRY_STATUS,2);
            player->SetVar(VAR_MARRY_STATUS,2); 
            
            Stream st(REP::MARRYMGR);
            st << static_cast<UInt8>(9) << static_cast<UInt8>(5) << player->getId() << player->getName() << player->getMainFighter()->getColor() << Stream::eos;
            obj_player->send(st);
            
            return 0;
        }
        else
        {
            player->sendMsgCode(0, 6002);
            return 1;
        }
        
        return 1;
    }

    UInt8 MarryMgr::FinishMarry(UInt64 playerid,UInt64 obj_playerid)
    {
        Mutex::ScopedLock lk(_mutex); 
		Player * player = globalPlayers[playerid];
		Player * obj_player = globalPlayers[obj_playerid];
        
        if(player->GetVar(VAR_MARRY_STATUS) != 4 || obj_player->GetVar(VAR_MARRY_STATUS) != 4 )
        {
            player->sendMsgCode(0, 6002);
            return 1;
        }

        ReserveList::iterator it = m_yuyueList.find(player->GetMarriageInfo()->yuyueTime);
        ReserveList::iterator it1 = m_yuyueList.find(obj_player->GetMarriageInfo()->yuyueTime);
        PairList::iterator it2 = m_marryList.find(playerid); 
        if(it == m_yuyueList.end() && it1 == m_yuyueList.end() && it2 == m_marryList.end())
        {
            player->sendMsgCode(0, 6014);
            return 1;
        }
        if(it == m_yuyueList.end())
            m_yuyueList.erase(it1);
        else
            m_yuyueList.erase(it);
        
        m_marryList.insert(std::make_pair(playerid,obj_playerid));
        obj_player->GetMarriageInfo()->eLove = player->GetMarriageInfo()->eLove;//双方都加上信物buffer

        DB7().PushUpdateData("DELETE FROM `reply_marriage` WHERE `man_playerid` = %" I64_FMT "u", playerid);
        DB7().PushUpdateData("DELETE FROM `marry_log` WHERE `man_playerid` = %" I64_FMT "u", playerid);

        std::string str_pronouncement;
        UInt32 jh_time;
        UInt32 marriage_time;
        UInt32 replymarriage_time;
        UInt8 wedding_type;
        if(player->GetMarriageInfo()->pronouncement != "")
        {
            str_pronouncement = player->GetMarriageInfo()->pronouncement;
            marriage_time = player->GetMarriageInfo()->marriageTime;
            replymarriage_time = obj_player->GetMarriageInfo()->marriageTime;
        }
        else
        {
            str_pronouncement = obj_player->GetMarriageInfo()->pronouncement;
            marriage_time = obj_player->GetMarriageInfo()->marriageTime;
            replymarriage_time = player->GetMarriageInfo()->marriageTime;
        }
        
        if(player->GetMarriageInfo()->yuyueTime != 0 && player->GetMarriageInfo()->eWedding != WEDDING_NULL)
        {
            jh_time = player->GetMarriageInfo()->yuyueTime;
            obj_player->GetMarriageInfo()->yuyueTime = player->GetMarriageInfo()->yuyueTime;
            wedding_type = static_cast<UInt8>(player->GetMarriageInfo()->eWedding);
        }
        else
        {
            jh_time = obj_player->GetMarriageInfo()->yuyueTime;
            player->GetMarriageInfo()->yuyueTime = obj_player->GetMarriageInfo()->yuyueTime;
            wedding_type = static_cast<UInt8>(obj_player->GetMarriageInfo()->eWedding);
        }

        DB7().PushUpdateData("REPLACE INTO `married_log` VALUES(%u,%" I64_FMT "u,%" I64_FMT "u, '%s',%u,%u,%u,%u,%u )", jh_time,playerid,obj_playerid, str_pronouncement.c_str(),static_cast<UInt8>(player->GetMarriageInfo()->eLove),marriage_time,replymarriage_time,player->GetMarriageInfo()->jieyuanTime,wedding_type);
        
        DB4().PushUpdateData("REPLACE INTO `married_couple` VALUES(%u,%" I64_FMT "u,%" I64_FMT "u,%u,%u,'%s',%u,%u,%u )", jh_time,playerid,obj_playerid, static_cast<UInt8>(player->GetMarriageInfo()->eLove),0,"",0,0,0);

        std::string str_tmp = "";
        CoupleInfo* ci = new CoupleInfo();
        ci->eLove = player->GetMarriageInfo()->eLove;
        gMarriedMgr.InsertCoupleInfo(jh_time,ci);

        obj_player->SetVar(VAR_MARRY_STATUS,5);
        player->SetVar(VAR_MARRY_STATUS,5); 
   
        SetDirty(player,obj_player); 
        
        //通知结婚养成
        Stream st1(REP::MARRIEDMGR);
        st1 << static_cast<UInt8>(1) << static_cast<UInt8>(1) << Stream::eos;
        player->send(st1);
        obj_player->send(st1);
        return 0;
    }

    UInt8 MarryMgr::DivorceMarry(Player* player,UInt8 status)
    {
        Mutex::ScopedLock lk(_mutex); 
        UInt32 now = TimeUtil::Now();
		Player * obj_player = globalPlayers[player->GetMarriageInfo()->lovers];
        if(!obj_player) 
        {
            player->sendMsgCode(0, 6012);
            return 1;
        }
        PairList::iterator it;
        Stream st1(REP::MARRIEDMGR);
        Stream st2(REP::MARRYMGR);
        switch(status)
        {
            case 0:
                if(player->GetVar(VAR_CANCEL_GIVEUP_JIEYUAN) < now)
                {
                    player->SetVar(VAR_CANCEL_GIVEUP_JIEYUAN,now + 86400);
                }
                else
                {
                    player->sendMsgCode(0, 6026);
                    return 1;
                }

                if(player->GetVar(VAR_MARRY_STATUS) != 2)
                {
                    player->sendMsgCode(0, 6002);
                    return 1;
                }
                if(!player->getMainFighter()->getSex())//男的
                {
                    if(getMoney(player,player->GetMarriageInfo()->eLove) != 0)
                    {
                        player->sendMsgCode(0, 6003);
                        return 1;
                    }
                }
                else
                {
                    if(getMoney(obj_player,obj_player->GetMarriageInfo()->eLove) != 0)
                    {
                        player->sendMsgCode(0, 6003);
                        return 1;
                    }
                }

                if(!player->getMainFighter()->getSex())//男的
                {
                    it = m_jieyuanList.find(player->getId());
                    if(it == m_jieyuanList.end())
                    {
                        player->sendMsgCode(0, 6015);
                        return 1;
                    }   
                
                    if(player->GetMarriageInfo()->pronouncement == "" && obj_player->GetMarriageInfo()->pronouncement == "")
                    {
                        player->sendMsgCode(0, 6015);
                        return 1;
                    }
                    
                    //换信物钱                   
                    sendMoneyMail(player,sMoney.price_type,sMoney.price_num,sMoney.useType,sMoney.eParm); 
                    m_jieyuanList.erase(it); 
                    DB7().PushUpdateData("DELETE FROM `reply_marriage` WHERE `man_playerid` = %" I64_FMT "u", player->getId());
                    DB7().PushUpdateData("DELETE FROM `marry_log` WHERE `man_playerid` = %" I64_FMT "u", player->getId());
                }
                else
                {
                    it = m_jieyuanList.find(player->GetMarriageInfo()->lovers);
                    if(it == m_jieyuanList.end())
                    {
                        player->sendMsgCode(0, 6015);
                        return 1;
                    }   
                    
                    if(player->GetMarriageInfo()->pronouncement == "" && obj_player->GetMarriageInfo()->pronouncement == "")
                    {
                        player->sendMsgCode(0, 6015);
                        return 1;
                    }
                    
                    m_jieyuanList.erase(it);
                    //还信物钱
                    sendMoneyMail(obj_player,sMoney.price_type,sMoney.price_num,sMoney.useType,sMoney.eParm); 
                    DB7().PushUpdateData("DELETE FROM `reply_marriage` WHERE `woman_playerid` = %" I64_FMT "u", player->getId());
                    DB7().PushUpdateData("DELETE FROM `married_log` WHERE `woman_playerid` = %" I64_FMT "u", player->getId());
                }

                if(obj_player->GetMarriageInfo()->eWedding != WEDDING_NULL)
                {
                    getMoney(obj_player,obj_player->GetMarriageInfo()->eWedding);
                    sendMoneyMail(obj_player,sMoney.price_type,sMoney.price_num,sMoney.useType,sMoney.eParm); 
                }

                player->GetMarriageInfo()->eraseInfo();
                obj_player->GetMarriageInfo()->eraseInfo();
                
                player->SetVar(VAR_MARRY_STATUS,0);
                obj_player->SetVar(VAR_MARRY_STATUS,0);
                
                st2 << static_cast<UInt8>(9) << static_cast<UInt8>(2) << player->getId() << player->getName() << player->getMainFighter()->getColor() << Stream::eos;

                obj_player->send(st2);
    
                break;
            case 1:
                if(player->GetVar(VAR_MARRY_STATUS) != 5 && player->GetVar(VAR_MARRY_STATUS) != 6)
                {
                    player->sendMsgCode(0, 6002);
                    return 1;
                }
                if(player->getGold() < ALONE_LIHUN)
                {
                    player->sendMsgCode(0, 1101);
                    return 1;
                }
                //付钱离婚 500
                useMoney(player,0,ALONE_LIHUN,3);
               
                if(player->GetVar(VAR_MARRY_STATUS) == 6)
                    sendMoneyMail(player,MailPackage::Gold,TWO_LIHUN,3,1); 

                if(obj_player)
                {
                    if(obj_player->GetVar(VAR_MARRY_STATUS) == 6)
                    {
                        //还共同离婚钱 250
                        sendMoneyMail(obj_player,MailPackage::Gold,TWO_LIHUN,3,1); 
                    }
                    obj_player->GetMarriageInfo()->eraseInfo();
                }
                player->GetMarriageInfo()->eraseInfo();
                erase_marryList(player);
                SetDirty(player,obj_player); 
               
                if(!player->getMainFighter()->getSex())//男的
                    DB7().PushUpdateData("DELETE FROM `married_log` WHERE `man_playerid` = %" I64_FMT "u", player->getId());
                else
                    DB7().PushUpdateData("DELETE FROM `married_log` WHERE `woman_playerid` = %" I64_FMT "u", player->getId());

                sendMoneyMail(player,MailPackage::Gold,0,5,1); 
                sendMoneyMail(obj_player,MailPackage::Gold,0,5,1); 
                
                player->SetVar(VAR_MARRY_STATUS,0);
                obj_player->SetVar(VAR_MARRY_STATUS,0);
                
                st2 << static_cast<UInt8>(9) << static_cast<UInt8>(6) << player->getId() << player->getName() << player->getMainFighter()->getColor() << Stream::eos;

                //通知结婚养成
                obj_player->send(st2);
                st1 << static_cast<UInt8>(1) << static_cast<UInt8>(0) << Stream::eos;
                player->send(st1);
                obj_player->send(st1);
                
                break;
            case 2:
                if(player->GetVar(VAR_MARRY_STATUS) != 5)
                {
                    player->sendMsgCode(0, 6002);
                    return 1;
                }
                if(obj_player)
                {
                    if(player->getGold() < TWO_LIHUN)
                    {
                        player->sendMsgCode(0, 1101);
                        return 1;
                    }

                    if(obj_player->GetVar(VAR_MARRY_STATUS) == 6)
                    {
                        obj_player->GetMarriageInfo()->eraseInfo();
                        player->GetMarriageInfo()->eraseInfo();
                        SetDirty(player,obj_player); 

                        if(!player->getMainFighter()->getSex())//男的
                            DB7().PushUpdateData("DELETE FROM `married_log` WHERE `man_playerid` = %" I64_FMT "u", player->getId());
                        else
                            DB7().PushUpdateData("DELETE FROM `married_log` WHERE `woman_playerid` = %" I64_FMT "u", player->getId());

                        erase_marryList(player);        
                        sendMoneyMail(player,MailPackage::Gold,0,5,1); 
                        sendMoneyMail(obj_player,MailPackage::Gold,0,5,1); 
                        //通知结婚养成
                        st1 << static_cast<UInt8>(1) << static_cast<UInt8>(0) << Stream::eos;
                        player->send(st1);
                        obj_player->send(st1);
                        
                        player->SetVar(VAR_MARRY_STATUS,0);
                        obj_player->SetVar(VAR_MARRY_STATUS,0);
                    }
                    else
                    {
                        //提醒共同离婚
                        player->SetVar(VAR_MARRY_STATUS,6);
                        st2 << static_cast<UInt8>(9) << static_cast<UInt8>(7) << player->getId() << player->getName() << player->getMainFighter()->getColor() << Stream::eos;
                        obj_player->send(st2);
                    }
                    //付钱共同离婚 250
                    useMoney(player,0,TWO_LIHUN,3);
                    
                }
                else
                    return 0xFE;                
                break;
            
            case 3:
                {
                    st2 << static_cast<UInt8>(9) << static_cast<UInt8>(8) << player->getId() << player->getName() << player->getMainFighter()->getColor() << Stream::eos;

                    obj_player->send(st2);
                    
                    return 0;
                }
                break;

            case 4:
                {
                    if(player->GetVar(VAR_MARRY_STATUS) != 5 || obj_player->GetVar(VAR_MARRY_STATUS) != 6)
                    {
                        player->sendMsgCode(0, 6002);
                        return 1;
                    }
                    sendMoneyMail(obj_player,MailPackage::Gold,TWO_LIHUN,3,1); 
                    obj_player->SetVar(VAR_MARRY_STATUS,5);
                }
                break;

        }
        return 0;
    }
    
    UInt8 MarryMgr::SetMarryStatus(Player* player)
    {
        Mutex::ScopedLock lk(_mutex); 

        if(!player->GetMarriageInfo()->lovers)
            return 1;
        Player * obj_player = globalPlayers[player->GetMarriageInfo()->lovers];
        if(!player->GetMarriageInfo()->lovers)
            return 1;
        
        if(player->GetVar(VAR_MARRY_STATUS) != 4 && player->GetVar(VAR_MARRY_STATUS) != 4)
        {
            player->sendMsgCode(0, 6002);
            return 1;
        }
/*        if(!player->getMainFighter()->getSex())//男的
            FinishMarry(player->getId(),obj_player->getId());
        else
            FinishMarry(obj_player->getId(),player->getId());*/

        return 0;
    }

    void MarryMgr::cleanMemmory()
    {
        m_maleList.clear();
        m_femaleList.clear();
        m_replyList.clear();
        m_jieyuanList.clear();
        m_yuyueList.clear();
        m_marryList.clear();
        m_divorceList.clear();
        return;
    }

    void MarryMgr::cleanPlayerData(Player* player)
    {
        player->GetMarriageInfo()->eraseInfo();
        return;
    }

    UInt8 MarryMgr::ModifyMarriageInfo(Player* player,MarriageInfo* sMarry,UInt8 flag)
    {
        Mutex::ScopedLock lk(_mutex); 

        if(player->GetVar(VAR_MARRY_STATUS) != 1 && player->GetMarriageInfo()->pronouncement == "")
        {
            player->sendMsgCode(0, 6002);
            return 1;
        }
        
        if(flag == 0 || flag == 2)
        { 
            UInt8 ret1 = getMoney(player,sMarry->eLove,true);
            if(ret1 != 0)
                return ret1;
        }
        
        if(flag != 0 && flag != 1 && flag != 2)
            return 0xFE;
        if(flag == 1 || flag == 2)
        {
            if(sMarry->pronouncement != "")
            {
                player->GetMarriageInfo()->eraseInfo(PARM_pronouncement);
                player->GetMarriageInfo()->pronouncement = sMarry->pronouncement;
                DB7().PushUpdateData("UPDATE `marriage` SET `prouncement` = '%s' WHERE `playerid` = %" I64_FMT "u", sMarry->pronouncement.c_str(),player->getId());
            }
            else
            {
                player->sendMsgCode(0, 6002);
                return 1;
            }
        }
        if(flag == 0 || flag == 2)
        {
            if(sMarry->eLove != LOVE_NULL)
            {
                sendMoneyMail(player,sMoney.price_type,sMoney.price_num,sMoney.useType,sMoney.eParm); 
                player->GetMarriageInfo()->eLove = sMarry->eLove;
                player->GetMarriageInfo()->pronouncement = sMarry->eLove;
                DB7().PushUpdateData("UPDATE `marriage` SET `lover_item` = %u WHERE `playerid` = %" I64_FMT "u", static_cast<UInt8>(sMarry->eLove),player->getId());
            }
            else
            {
                player->sendMsgCode(0, 6002);
                return 1;
            }
            
            useMoney(player,sMoney.price_type,sMoney.price_num,sMoney.useType);
        }

        return 0;
    }
  
    UInt8 MarryMgr::DoGetList(Player* player,UInt8 flag,UInt16 idx)
    {
        Mutex::ScopedLock lk(_mutex); 
        return GetList(player,flag,idx); 
    }

    UInt8 MarryMgr::GetList(Player* player,UInt8 flag,UInt16 idx)
    {
        UInt8 len = 3;
        UInt16 count;//总数
        UInt16 num;//页数
        UInt8 xnum = 0;
        switch(flag)
        {
            case 0:
                {
                    MarriageList::iterator it = GetOppsiteSexList(player).begin();
                    Stream st(REP::MARRYMGR);
                    count = GetOppsiteSexList(player).size();
                    if(count == 0)
                    {
                        num = 0;
                        idx = 0;
                    }
                    else
                    {
                        num = (count/len);
                        if(count%len)
                            ++ num;

                        if(idx * len > count && num < idx)
                            idx = 1;
                            
                        if(idx > 0)
                            std::advance(it, (idx - 1)*len);
                    }
                    
                    if(idx < num)
                        xnum = len;
                    else if(idx == 1)
                            xnum = count; 
                         else
                            xnum = count - (num - (idx - 1)) * len;

                    st << static_cast<UInt8>(5) << static_cast<UInt8>(0) << num << idx << xnum; 
                    for(UInt8 cnt = 0;cnt < xnum;cnt++)
                    {
                        if(it == GetOppsiteSexList(player).end())
                            break;
                        UInt64 tmpid = it->first;
                        GObject::Player * tmp_player = GObject::globalPlayers[tmpid];
                        st << tmp_player->getName() << it->first << tmp_player->getMainFighter()->getColor() ;
                        if(player->GetMarriageInfo()->lovers && player->GetVar(VAR_MARRY_STATUS) == 1)
                        {
                            if(player->GetMarriageInfo()->lovers == tmpid)
                                st<< static_cast<UInt8>(1);
                            else
                                st<< static_cast<UInt8>(2);
                        } 
                        else
                            st<< static_cast<UInt8>(0);
        
                        ++it;
                    }
                    st << Stream::eos;
                    player->send(st);
                    
                break;
                }
            case 1:
                {
                    Stream st(REP::MARRYMGR);
                    ReplyMarriageList::iterator i = m_replyList.find(player->getId());
                    if(i != m_replyList.end())
                    {
                        count = (i->second).size();
                        std::vector<UInt64>::iterator it = (i->second).begin();
                        std::vector<UInt64>::iterator it_end = (i->second).end();
                        if(count == 0)
                        {
                            num = 0;
                            idx = 0;
                        }
                        else
                        {
                            num = (count/len);
                            if(count%len)
                                ++ num;

                            if(idx * len > count && num < idx)
                                idx = 1;

                            if(idx > 0)
                                std::advance(it, (idx - 1)*len);
                        }

                        if(idx < num)
                            xnum = len;
                        else if(idx == 1)
                                xnum = count; 
                             else
                                xnum = count - (num - (idx - 1)) * len;
                        
                        st << static_cast<UInt8>(5) << static_cast<UInt8>(1) << num << idx << xnum; 
                        for(UInt8 cnt = 0;cnt < xnum;cnt++)
                        {
                            if(it == it_end)
                                break;
                            GObject::Player * tmp_player = GObject::globalPlayers[*it];
                            if(player)
                            {
                                st << tmp_player->getName() << *it << tmp_player->getMainFighter()->getColor() << static_cast<UInt8>(0);

                            }
            
                            ++it;
                        }
                    }
                    else
                        st << static_cast<UInt8>(5) << static_cast<UInt8>(1) << 0 << 0 << 0; 
                    st << Stream::eos;
                    player->send(st);
                    
                    break;
                }

            case 2:
                {
                    len = 4;
                    Stream st(REP::MARRYMGR);
                    ReserveList::iterator it =  m_yuyueList.begin();
                    if(it != m_yuyueList.end())
                    {
                        count = m_yuyueList.size();
                        if(count == 0)
                        {
                            num = 0;
                            idx = 0;
                        }
                        else
                        {
                            num = (count/len);
                            if(count%len)
                                ++ num;

                            if(idx * len > count && num < idx)
                                idx = 1;
                            
                            if(num < idx)
                                idx = 1;

                            if(idx > 0)
                                std::advance(it, (idx - 1)*len);
                        }

                        if(idx < num)
                            xnum = len;
                        else if(idx == 1)
                                xnum = count; 
                             else
                                xnum = count - (num - (idx - 1)) * len;
                        
                        st << static_cast<UInt8>(5) << static_cast<UInt8>(2) << num << idx << xnum; 
                        for(UInt8 cnt = 0;cnt < xnum;cnt++)
                        {
                            if(it == m_yuyueList.end())
                                break;
                            GObject::Player * man_player = GObject::globalPlayers[(it->second).first];
                            GObject::Player * woman_player = GObject::globalPlayers[(it->second).second];
                            if(man_player&&woman_player)
                            {
                                st << it->first << man_player->getName() << man_player->getMainFighter()->getColor() << woman_player->getName() << woman_player->getMainFighter()->getColor();
                            }
            
                            ++it;
                        }
                    }
                    else
                        st << static_cast<UInt8>(5) << static_cast<UInt8>(2) << 0 << 0 << 0; 

                    st << Stream::eos;
                    player->send(st);
                    break;
                }

        }
        
        return 0;
    }

    UInt8 MarryMgr::LoadMarriage(Player* player,DBMarriage* dbpn)
    {
        MarriageInfo* sMarriageInfo =  new MarriageInfo(dbpn->marriage_time,dbpn->pronouncement,static_cast<ELoveToken>(dbpn->lover_item)); 
        if(dbpn->status == 0)
        {
            GetSameSexList(player).insert(std::make_pair(player->getId(),dbpn->marriage_time));     
            std::vector<UInt64> vec;    
            m_replyList.insert(std::make_pair(player->getId(),vec));
        }
        else
        {
            ReplyMarriageList::iterator it = m_replyList.find(dbpn->status);
            if(it == m_replyList.end())
                return 1;
            std::vector<UInt64>& vec = it->second;
            vec.insert(vec.begin(),static_cast<UInt64>(dbpn->playerid));
            sMarriageInfo->lovers = static_cast<UInt64>(dbpn->status);
        }
        player->GetMarriageInfo()->addInfo(sMarriageInfo);
        return 0;
    } 

    UInt8 MarryMgr::LoadReplyMarriage(Player* player,Player* obj_player,DBReplyMarriage* dbpn)
    {
        if(player->GetVar(VAR_MARRY_STATUS) == 2 || obj_player->GetVar(VAR_MARRY_STATUS) == 2)//预约婚礼成功 
            m_jieyuanList.insert(std::make_pair(dbpn->man_playerid,dbpn->woman_playerid));
        
        player->GetMarriageInfo()->lovers = obj_player->getId();
        obj_player->GetMarriageInfo()->lovers = player->getId();

        player->GetMarriageInfo()->eLove = static_cast<ELoveToken>(dbpn->lover_item); 
        if(!dbpn->marriage_buyer)
            player->GetMarriageInfo()->pronouncement = dbpn->pronouncement; 
        else
            obj_player->GetMarriageInfo()->pronouncement = dbpn->pronouncement; 

        if(dbpn->jh_time && dbpn->wedding_type )
        {
            
            if(player->GetVar(VAR_MARRY_STATUS) == 4 && obj_player->GetVar(VAR_MARRY_STATUS) == 4)//预约婚礼成功 
            {
                std::pair<UInt32,std::pair<UInt64,UInt64>> pair1;
                std::pair<UInt64,UInt64> pair2;
                pair2 = std::make_pair(static_cast<UInt64>(player->getId()),static_cast<UInt64>(obj_player->getId()));
                pair1 = std::make_pair(dbpn->jh_time,pair2);
                m_yuyueList.insert(pair1);  
            }
            
            MarriageInfo* sMarriageInfo1 = new MarriageInfo(0,"",LOVE_NULL,dbpn->jy_time,dbpn->jh_time,static_cast<EWedding>(dbpn->wedding_type)); 
            MarriageInfo* sMarriageInfo2 = new MarriageInfo(0,"",LOVE_NULL,dbpn->jy_time); 
            
            if(!dbpn->wedding_buyer)//婚礼预约花钱的人 0 - 男 1 - 女
            {
                player->GetMarriageInfo()->addInfo(sMarriageInfo1);
                obj_player->GetMarriageInfo()->addInfo(sMarriageInfo2);                 
            }
            else
            {
                player->GetMarriageInfo()->addInfo(sMarriageInfo2);
                obj_player->GetMarriageInfo()->addInfo(sMarriageInfo1);            
            }
            
            if(!dbpn->marriage_buyer)
                player->GetMarriageInfo()->pronouncement = dbpn->pronouncement ; 
            else
                obj_player->GetMarriageInfo()->pronouncement = dbpn->pronouncement ;
            
        }

        
        
        return 0;
    } 

    UInt8 MarryMgr::LoadMarriedLog(Player* player,Player* obj_player,DBMarriedLog* dbpn)
    {
        MarriageInfo* sMarriageInfo =  new MarriageInfo(dbpn->marriage_time,dbpn->pronouncement,static_cast<ELoveToken>(dbpn->lover_item),dbpn->jy_time,dbpn->jh_time,static_cast<EWedding>(dbpn->wedding_type));
        player->GetMarriageInfo()->addInfo(sMarriageInfo);
        obj_player->GetMarriageInfo()->addInfo(sMarriageInfo);
        player->GetMarriageInfo()->lovers = obj_player->getId();
        obj_player->GetMarriageInfo()->lovers = player->getId();

        return 0;
    } 

    void MarryMgr::RepairBug()
    {
        if(GVAR.GetVar(GVAR_REPAIRMARRYBUG) == 1)
            return;
        ReserveList::iterator it; 
        for(it = m_yuyueList.begin();it != m_yuyueList.end(); ++it)
        {
            GObject::Player * man_player = GObject::globalPlayers[(it->second).first];
            GObject::Player * woman_player = GObject::globalPlayers[(it->second).second];
            if(man_player->GetVar(VAR_MARRY_STATUS) == 4)
            {
                if(it->first == 1) 
                {
                    if(man_player->GetMarriageInfo()->eWedding == WEDDING_NULL)    
                        doCancelAppointMent(man_player);
                    else
                        doCancelAppointMent(woman_player);
                }
            }
            
            if(woman_player->GetVar(VAR_MARRY_STATUS) == 4)
            {
                if(it->first == 1) 
                {
                    if(woman_player->GetMarriageInfo()->eWedding == WEDDING_NULL)    
                        doCancelAppointMent(woman_player);
                    else
                        doCancelAppointMent(man_player);
                }
            }
        }
        GVAR.SetVar(GVAR_REPAIRMARRYBUG,1);
        return;
    }



    void MarryMgr::addMarriedAttr(Player* player,GData::AttrExtra& ae)
    {
        switch(player->GetMarriageInfo()->eLove)
        {
            case LOVE_TONGXINJIE:
                ae.hp += 30; 
                break;
            case LOVE_YUPEI:
                ae.hp += 180; 
                break;
            case LOVE_JINZAN:
                ae.hp += 400; 
                break;
            default:
                break;
        }
    } 

    void MarryMgr::sendyuyueList(Player* player,Player* obj_player)
    {
        Stream st(REP::MARRYMGR);
        ReserveList::iterator it2 = m_yuyueList.begin();
        UInt32 dailyList = 0;
        for(UInt8 idx = 0; idx < 21; ++idx)
        {
            if(it2 == m_yuyueList.end())
                break;
            if(TimeUtil::GetYYMMDD(it2->first) < TimeUtil::GetYYMMDD())
                it2++;
            if(TimeUtil::GetYYMMDD(it2->first) == TimeUtil::GetYYMMDD(TimeUtil::Now() + idx * 86400))
            {
                dailyList = SET_BIT(dailyList,idx);
                it2 ++;
            }
        }
        st << static_cast<UInt8>(0x10) << dailyList << Stream::eos;
        if(player)
            player->send(st);
        if(obj_player)
            obj_player->send(st);
    }

    void MarryMgr::sendMarriageTimeOut(Player* player,Player* obj_player)
    {
        if(player)
        {
            if(player->GetMarriageInfo()->marriageTime != 0)
            {
                Stream st(REP::MARRYMGR);
                st << static_cast<UInt8>(0x11) << player->GetMarriageInfo()->marriageTime + 86400 * 2 - TimeUtil::Now() << Stream::eos;
                if(player)
                    player->send(st);
                if(obj_player)
                    obj_player->send(st);
            }
        }
        if(obj_player)
        {
            if(obj_player->GetMarriageInfo()->marriageTime != 0)
            {
                Stream st(REP::MARRYMGR);
                st << static_cast<UInt8>(0x11) << player->GetMarriageInfo()->marriageTime + 86400 * 2 - TimeUtil::Now() << Stream::eos;
                if(player)
                    player->send(st);
                if(obj_player)
                    obj_player->send(st);
            }
        }
    }

    void MarryMgr::sendWhoisMarrybuyer(Player* player, Player* obj_player)
    {
        Stream st(REP::MARRYMGR);
        if(player->GetMarriageInfo()->yuyueTime == 0)
            st << static_cast<UInt8>(0x12) << static_cast<UInt8>(0) << Stream::eos;
        else
            st << static_cast<UInt8>(0x12) << static_cast<UInt8>(1) << Stream::eos;
        player->send(st);

        Stream st1(REP::MARRYMGR);
        if(obj_player->GetMarriageInfo()->yuyueTime == 0)
            st1 << static_cast<UInt8>(0x12) << static_cast<UInt8>(0) << Stream::eos;
        else
            st1 << static_cast<UInt8>(0x12) << static_cast<UInt8>(1) << Stream::eos;
        obj_player->send(st1);

        return;
    }
    

    void MarryMgr::DoProcess()
    {
        CheckingListTimeOut(m_maleList);
        CheckingListTimeOut(m_femaleList);
        { // 限制lock的生命周期
            Mutex::ScopedLock lk(_mutex);
            Process();
        }
    }

    void MarryMgr::Process()
    {
        UInt32 now = TimeUtil::Now();
        if(GVAR.GetVar(GVAR_MARRY_TIME1) < now)
            GVAR.SetVar(GVAR_MARRY_TIME1,0);
        if(GVAR.GetVar(GVAR_MARRY_TIME2) < now)
            GVAR.SetVar(GVAR_MARRY_TIME2,0);

        if(GVAR.GetVar(GVAR_MARRY_TIME1) >= now + 86400*2)
            GVAR.SetVar(GVAR_MARRY_TIME1,0);
        if(GVAR.GetVar(GVAR_MARRY_TIME2) >= now + 86400*2)
            GVAR.SetVar(GVAR_MARRY_TIME2,0);
        //if(GVAR.GetVar(GVAR_MARRY_TIME3) >= now + 86400*3)
          //  GVAR.SetVar(GVAR_MARRY_TIME3,0);

        GObject::Player * player;
        GObject::Player * obj_player;
        UInt8 eWedding = 0;
        ReserveList::iterator it = this->m_yuyueList.begin();
        std::pair<UInt64,UInt64> it1; 
        
        if(!GVAR.GetVar(GVAR_MARRY_TIME1) || !GVAR.GetVar(GVAR_MARRY_TIME2) || !GVAR.GetVar(GVAR_MARRY_TIME3))//发放烟花
        {
            for(int i = 1; it != this->m_yuyueList.end(); it++)
            {
                if(i > 2)
                    break;
                if(it->first < now)
                    continue;
                if(it->first == GVAR.GetVar(GVAR_MARRY_TIME1) || it->first == GVAR.GetVar(GVAR_MARRY_TIME2) ) //|| it->first == GVAR.GetVar(GVAR_MARRY_TIME3))
                    continue;
                if(it->first >= (now + 86400*2))
                    break;
                if(GVAR.GetVar(GVAR_MARRY_TIME1) == 0)
                    GVAR.SetVar(GVAR_MARRY_TIME1,it->first);
                else if(GVAR.GetVar(GVAR_MARRY_TIME2) == 0)
                    GVAR.SetVar(GVAR_MARRY_TIME2,it->first);
                    else if(GVAR.GetVar(GVAR_MARRY_TIME3) == 0)
                        GVAR.SetVar(GVAR_MARRY_TIME3,it->first);
                it1 = it->second;
                player = GObject::globalPlayers[it1.first];
                obj_player = GObject::globalPlayers[it1.second];
                if(player->GetMarriageInfo()->eWedding == WEDDING_NULL)
                    eWedding = static_cast<UInt8>(obj_player->GetMarriageInfo()->eWedding);
                else
                    eWedding = static_cast<UInt8>(player->GetMarriageInfo()->eWedding);
                //MarryBoard::instance().SendPreMarryPresent(player,obj_player,eWedding);
                i++;
            }
        }
        
        if(!GVAR.GetVar(GVAR_CREATMARRY_TIMES))//是否创建婚礼
        {
            if(m_yuyueList.begin() == m_yuyueList.end())
                return;
            it = m_yuyueList.begin();
            
            while(it != m_yuyueList.end())
            {
                if(it->first < now)
                {
                    if(TimeUtil::GetYYMMDD(it->first) == TimeUtil::GetYYMMDD())
                        break;  
                    it1 = it->second;
                    player = GObject::globalPlayers[it1.first];
                    obj_player = GObject::globalPlayers[it1.second];
                    if(!player || !obj_player)
                        continue;
                    UInt8 ret; 
                    if(player->GetMarriageInfo()->eWedding == WEDDING_NULL)
                    {
                        ret = CancelAppointMent(player);
                        sendMoneyMail(obj_player,MailPackage::Gold,0,7,1); 
                    }
                    else
                    {
                        ret = CancelAppointMent(obj_player);
                        sendMoneyMail(player,MailPackage::Gold,0,7,1); 
                    }
                }
                
                if(TimeUtil::GetYYMMDD(it->first) == TimeUtil::GetYYMMDD())
                {
                    it1 = it->second;
                    player = GObject::globalPlayers[it1.first];
                    obj_player = GObject::globalPlayers[it1.second];
                    if(player->GetMarriageInfo()->eWedding == WEDDING_NULL)
                        eWedding = static_cast<UInt8>(obj_player->GetMarriageInfo()->eWedding);
                    else
                        eWedding = static_cast<UInt8>(player->GetMarriageInfo()->eWedding);
                    WORLD().CreateMarryBoard(it1.first,it1.second,eWedding,it->first + 60*30); 
                }
                if(it != m_yuyueList.end())
                    it++;
            }
         }

        return;
    }
    
    void MarryMgr::SetDirty(Player* player,Player* obj_player)
    {

        std::map<UInt32, Fighter *>& fighters = player->getFighterMap();
        std::map<UInt32, Fighter *>& fighters2 = obj_player->getFighterMap();
        for(std::map<UInt32, Fighter *>::iterator it = fighters.begin(); it != fighters.end(); ++it)
        {
            it->second->setDirty();
        }
        for(std::map<UInt32, Fighter *>::iterator it = fighters2.begin(); it != fighters2.end(); ++it)
        {
            it->second->setDirty();
        }

    }
    
    void MarryMgr::MarryingCrush()
    {
        Mutex::ScopedLock lk(_mutex);
        if(m_yuyueList.begin() == m_yuyueList.end())
            return;
        ReserveList::iterator it = this->m_yuyueList.begin();
        
        if(it->first < TimeUtil::Now())
        {
            std::pair<UInt64,UInt64> it1 = it->second;
            GObject::Player * player = GObject::globalPlayers[it1.first];
            GObject::Player * obj_player = GObject::globalPlayers[it1.second];
            UInt8 ret; 
            if(player->GetMarriageInfo()->eWedding == WEDDING_NULL)
                ret = CancelAppointMent(player);
            else
                ret = CancelAppointMent(obj_player);
            if(ret == 0)
                sendMoneyMail(player,MailPackage::Gold,0,8,1); 
        }

        return;
    }

}
