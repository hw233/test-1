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
        UInt32 onlineTime;
        std::string petName; 
        UInt8 eLove;
        UInt8 level;//等级
        UInt16 levelExp;//等级进度
        UInt8 status;//状态(钓鱼次数)
        UInt16 friendliness;//亲密度

        CoupleInfo(UInt32 num1 = 0,std::string str1 = "",UInt8 num2 = 0,UInt16 num3 = 0,UInt8 num4 = 0,UInt16 num5 = 0)
        {
            onlineTime = num1;
            petName = str1;
            eLove = num2;
            levelExp = num3;
            status = num4;
            friendliness = num5;
        }
    };

    class Player;
    class Fighter;
    class DBMarriage;
    class DBReplyMarriage;
    class DBMarriedLog;
    
    
    
    class MarriedMgr  
    {
       
        public:
            MarriedMgr();
            ~MarriedMgr();

            UInt8 PreCheckingStatus(Player* player);
            void ProcessOnlineAward(Player* player,UInt8 flag /*0-上线 1-下线 */);
            void ReturnFirstStatus(Player* player);//返回家园初始状态
            void GetOnlineAward(Player* player);
            void ModifyPetName(Player* player,std::string name);
            void ModifyeLove(Player* player);//更换信物
            void Fishing(Player* player,UInt8 consumeType/*消费类型*/,UInt8 count/*次数*/);//钓鱼
            void InsertCoupleInfo(UInt32 time,CoupleInfo*);
            void LoadMarriedCouple(DBMarriedCouple*);

        private:
            std::map<UInt32,CoupleInfo*> m_couple;
            typedef std::map<UInt32,CoupleInfo*> CoupleList;
            Mutex _mutex;
    };
    extern MarriedMgr gMarriedMgr;

}

#endif
