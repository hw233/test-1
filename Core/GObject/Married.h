#ifndef _MARRIED_H_
#define _MARRIED_H_
#include "Config.h"
#include <vector>
#include <map>
#include "Common/Stream.h"
#include "Common/Mutex.h"
#include "GObjectDBExecHelper.h"


namespace GObject
{



    struct CoupleInfo
    {
        std::string petName; 
        UInt8 eLove;
        UInt8 level;//等级
        UInt16 levelExp;//等级进度
        UInt16 friendliness;//亲密度

        CoupleInfo(std::string str1 = "",UInt8 num1 = 0,UInt8 num2 = 0,UInt16 num3 = 0,UInt16 num4 = 0)
        {
            petName = str1;
            eLove = num1;
            level = num2;
            levelExp = num3;
            friendliness = num4;
        }
    };
    
    class Player;
    class Fighter;
    class DBMarriage;
    class DBReplyMarriage;
    class DBMarriedLog;
    
    enum ECoupleAward 
    {
        AWARD_NULL = 0,            //空
        AWARD_WHITEFISH,          //好吃的白鱼
        AWARD_QIXINGFISH,               //七星鱼
        AWARD_JINJINFISH,               //金锦鱼
        AWARD_MEMEORY,               //记忆碎片
        WINNER_COPY,               //副本胜利
        AWARD_QIANFISH,            //千年古迹
        AWARD_WANFISH,             //万年古迹
    };
    
    class MarriedMgr  
    {
       
        public:
            MarriedMgr();
            ~MarriedMgr();

            UInt8 PreCheckingStatus(Player* player);
            void ProcessOnlineAward(Player* player,UInt8 flag /*0-上线 1-下线 */);
            void ReturnFirstStatus(Player* player);//返回家园初始状态
            void GetOnlineAward(Player* player);
            void ReturnCouplePet(Player* player);//返回玩家宠物信息
            void ModifyPetName(Player* player,std::string name);
            void ModifyeLove(Player* player,UInt8 eLove);//更换信物
            UInt8 Fishing(Player* player,UInt8 consumeType/*消费类型*/,UInt8 count/*次数*/);//钓鱼
            UInt8 useMoney(Player* player,UInt8 price_type,UInt32 price_num,UInt8 useType);
            void ChangPetAttr(Player* man_player,Player* woman_player,ECoupleAward eAward);
            void rebuildCouplePet(Player* player);
            void addCouplePetAttr(Player* player,GData::AttrExtra& ae);
            void InsertCoupleInfo(UInt32 time,CoupleInfo*);
            void LoadMarriedCouple(DBMarriedCouple*);
            void AddPetAttr(Player* player,UInt8 type,UInt16 num);
            void AppendPetData(Player* player,Stream& st);
            void eraseCoupleList(Player* player);
           
            //夫妻副本
            void EnterCoupleCopy(Player* player,UInt8 copy_type);
            void OpCoupleCopy(Player* player,UInt8 op_type);
            void EnterBattle(Player* player);
            void ReturnCoupleCopyInfo(Player* player);
            void InvitePlayer(Player* player);
            void sendBattleReport(Player* player,Player* obj_player,GData::NpcGroup* ng, Battle::BattleSimulator& bsim, UInt32& rptid);
            void SetCoupleFix(Player* player,UInt8 flag);

        private:
            std::map<UInt32,CoupleInfo*> m_couple;
            typedef std::map<UInt32,CoupleInfo*> CoupleList;
            Mutex _mutex;
    };
    extern MarriedMgr gMarriedMgr;

}

#endif
