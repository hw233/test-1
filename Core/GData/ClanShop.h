#ifndef CLANSHOP_H
#define CLANSHOP_H
#include<map>
#include"Config.h"
#include"GDataDBExecHelper.h"

namespace GData
{
#define MAX_LVL 130
    class ClanShopInfo
    {
        public:
                struct ClanShopItems
                {
                    UInt32 id;
                    UInt32 itemid;
                    std::string name;
                    UInt32 price;
                    UInt32 prob;
                    UInt8 color;
                    UInt8 lvl;
                    ClanShopItems(UInt32 arg1=0, UInt32 arg2=0,std::string arg3="", UInt32 arg4=0 , UInt32 arg5=0, UInt8 arg6=0, UInt8 arg7=0)
                    {
                        id = arg1;
                        itemid = arg2;
                        name = arg3;
                        price = arg4;
                        prob = arg5;
                        color = arg6;
                        lvl = arg7;
                    }
                };

        private:
                std::map<UInt32, ClanShopItems> _clanShopInfo[(MAX_LVL - 30)/10];

        public:
                ClanShopInfo() {}
                void setClanShopInfo(UInt32 id, ClanShopItems its);
                std::map<UInt32, ClanShopItems> getClanShopInfo(UInt8 id);
    };
    extern ClanShopInfo clanShopInfo;
}

#endif //CLANSHOP_H
