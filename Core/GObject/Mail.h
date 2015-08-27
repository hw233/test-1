
#ifndef MAIL_H_
#define MAIL_H_
#include "GGlobalObjectManager.h"
#include "Config.h"

namespace GObject
{
    class Player;
    class Mail
    {
        public:
            Mail():_id(0),_owner(NULL),_contextId(0),_items(""),_option(0),_overTime(0){}
            Mail(UInt32 id,Player* pl, UInt16 contextId, std::string items, UInt8 option,UInt32 overTime):_id(id),_owner(pl),_contextId(contextId),_items(items),_option(option),_overTime(overTime){}
            void LoadMailInfo(UInt32 id, Player* pl, UInt16 contextId, std::string items, UInt8 option, UInt32 overTime);
            UInt32 GetId(){return _id;}
            Player* GetOwner(){return _owner;}
            UInt16 GetContextId(){return _contextId;}
            std::string GetItems(){return _items;}
            UInt8 GetOption(){return _option;}

            void SetOption(UInt8 option){_option = option;}
            UInt32 GetOverTime(){return _overTime;}

            //void Accept(UInt32 id);
        private:
            UInt32 _id;
            Player* _owner;
            UInt16 _contextId;

            std::string _items;
            UInt8 _option;   //0 未接受  1 已接受       
            UInt32 _overTime;
    };
    typedef GGlobalObjectManagerT<Mail, UInt32> GlobalMails;
    extern GlobalMails globalMails;
}
#endif // MAIL_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

