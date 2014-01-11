#ifndef _MARRY_H_
#define _MARRY_H_
#include "Config.h"
#include <vector>
#include <map>
#include "Common/Stream.h"
#include "Common/Mutex.h"

namespace GData
{
    class AttrExtra;
}

namespace GObject
{
    /*
     征婚Var--VAR_MARRY_STATUS:
        0:等待征婚
        1:等待回应
        2:等待预约
        3:等待确定
        4:等待婚礼
        5:已经结婚
        6:等待离婚
     */

    /*
    取消婚礼预约Var--VAR_CANCEL_APPOINTMENT:
    0:默认
    *:取消婚约到期时间
    */

    enum ELoveToken
    {
        LOVE_NULL = 0,            //空
        LOVE_TONGXINJIE,          //同心结
        LOVE_YUPEI,               //玉佩
        LOVE_JINZAN               //金簪
    };

    enum EWedding
    {
        WEDDING_NULL = 0,            //空
        WEDDING_BIYIQIFEI,           //比翼齐飞
        WEDDING_ZHULIANBIHE,         //珠联璧合 
        WEDDING_LONGFENGCHENGXIANG   //龙凤呈祥
    };
   
    enum MarriageParm
    {
        PARM_ALL = 0,
        PARM_marriageTime,
        PARM_pronouncement,
        PARM_eLove,
        PARM_jieyuanTime,
        PARM_yuyueTime,
        PARM_eWedding,
        PARM_divorceTime,
        PARM_lovers
    };

#define TONGXINJIE 100
#define YUPEI 500 
#define JINZAN 1000
#define BIYIQIFEI 520
#define ZHULIANBIHE 3344
#define LONGFENGCHENGXIANG 9999
#define ALONE_LIHUN 500
#define TWO_LIHUN 250
#define ZHENGHUN 50 


    struct MarriageInfo          //征婚信息
    { 
        UInt32 marriageTime;//征婚时间or回复征婚时间
        std::string pronouncement;     //宣言
        ELoveToken eLove;         //定情信物 
        UInt32  jieyuanTime;//结缘时间
        UInt32  yuyueTime;//婚礼预约时间or结婚时间
        EWedding eWedding;//婚礼规格
        UInt32  divorceTime;//离婚时间
        UInt64  lovers;//爱人(结缘对象or结婚对象or征婚回复对象)
        
        MarriageInfo(UInt32 time = 0,std::string str = "", ELoveToken eToken = LOVE_NULL, UInt32 time1 = 0,UInt32 time2 = 0,EWedding eType = WEDDING_NULL , UInt32 time3 = 0,UInt64 playerid = 0)
        {
            pronouncement = str; 
            eLove = eToken;
            marriageTime = time; 
            jieyuanTime = time1;
            yuyueTime = time2;
            eWedding = eType;
            divorceTime = time3;
            lovers = playerid;

        } 
        void addInfo(MarriageInfo* sMarriageInfo);
        void eraseInfo(MarriageParm eParm = PARM_ALL);
    };

    struct MoneyStruct
    {
        UInt32 price_num;
        UInt16 price_type;
        UInt8 useType;
        UInt8 eParm;
        
        MoneyStruct():price_num(0),price_type(0),useType(0),eParm(0){}
        
        void clear()
        {
            price_type = 0;
            price_num = 0;
            useType = 0;
            eParm = 0;
        }
    };

    class Player;
    class Fighter;
    class DBMarriage;
    class DBReplyMarriage;
    class DBMarriedLog;

    class MarryMgr
    {
        static const UInt32 MARRY_TIME_OUT = 2 * 24 * 60 * 60;//征婚2天超时期限

        public:
            MarryMgr();
            ~MarryMgr();
            bool CheckingTimeOut(UInt32 );//检测征婚薄超时
            
            void CheckingListTimeOut(std::map<UInt64,UInt32>&);//检测征婚薄超时
       
            UInt8 StartMarriage(Player* ,MarriageInfo*);//发起征婚
    
            UInt8 doCancelMarriage(Player* player,UInt8 flag = 0);
            
            UInt8 CancelMarriage(Player*,UInt8 flag = 0);//取消征婚
            
            UInt8 ReplyMarriage(Player* ,MarriageInfo* ,UInt64);//征婚回复
            
            UInt8 CancelReplayMarriage(Player* ,UInt64);//取消征婚回复(重新结缘)
            
            UInt8 JieYuan(Player* ,UInt64);//结缘                
            
            UInt8 doCancelAppointMent(Player*);//取消婚礼预约 加锁 发通知 
            
            UInt8 CancelAppointMent(Player*);//取消婚礼预约 0--取消预约申请 1-- 取消确定的预约
            
            UInt8 DivorceMarry(Player* ,UInt8);//离婚 0--放弃结缘 1-- 单独离婚 2--共同离婚
    
            UInt8 FinishMarry(UInt64,UInt64);

            std::map<UInt64,UInt32>& GetOppsiteSexList(Player* player);//获得异性征婚薄
    
            std::map<UInt64,UInt32>& GetSameSexList(Player* player);//获得异性征婚薄
            
            void erase_marryList(Player* player);//删除结婚名单的记录
            
            UInt8 getMoney(Player* player,ELoveToken e_parm, bool isUseMoney = false);

            UInt8 getMoney(Player* player,EWedding e_parm,bool isUseMoney = false);

            void GoBackPlayerStatus(Player* player, UInt8 status);
    
            UInt8 sendMoneyMail(Player* player,UInt16 price_type,UInt32 price_num,UInt8 useType,UInt8 eParm);

            UInt8 useMoney(Player* player,UInt8 price_type,UInt32 price_num,UInt8 useType);

            UInt8 ReqWeddingAppointMent(Player* player,MarriageInfo* sMarry);

            UInt8 ConfirmReqWeddingAppointMent(Player* player);

            UInt8 CancelReqWeddingAppointMent(Player* player);

            UInt8 ModifyMarriageInfo(Player* player,MarriageInfo* sMarry,UInt8 flag);

            UInt8 GetList(Player* player,UInt8 flag,UInt16 idx = 1);
            
            UInt8 LoadMarriage(Player* player, DBMarriage*);
            
            UInt8 LoadReplyMarriage(Player* player,Player* obj_player,DBReplyMarriage*);
            
            UInt8 LoadMarriedLog(Player* player,Player* obj_player,DBMarriedLog*);

            void cleanMemmory();

            UInt8 SetMarryStatus(Player*);

            void cleanPlayerData(Player* player);

            void addMarriedAttr(Player* player,GData::AttrExtra& ae);
    
            void sendyuyueList(Player* player,Player* obj_player);
    
            void sendMarriageTimeOut(Player* player,Player* obj_player);
            
            void sendWhoisMarrybuyer(Player* player,Player* obj_player);

            void DoProcess();
            
            void Process();
            
            void SetDirty(Player*,Player*);
            
            void MarryingCrush();
            
            void RepairBug();
        private:
            Mutex _mutex;
            MoneyStruct sMoney; 
            typedef std::map<UInt64,UInt32> MarriageList;
            typedef std::map<UInt64,std::vector<UInt64>> ReplyMarriageList;
            typedef std::map<UInt64,UInt64> PairList;
            typedef std::map<UInt32,std::pair<UInt64,UInt64>> ReserveList;
            MarriageList m_maleList;//男性的征婚薄  <时间，玩家id>
            MarriageList m_femaleList;//女性的征婚薄  
            ReplyMarriageList m_replyList;//征婚应征名单 
            PairList m_jieyuanList;//结缘名单
            ReserveList m_yuyueList;//婚礼预约名单
            PairList m_marryList;//结婚名单
            PairList m_divorceList;//离婚名单
    };


    extern MarryMgr gMarryMgr; 

}
#endif
