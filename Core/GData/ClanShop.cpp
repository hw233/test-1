#include"ClanShop.h"

namespace GData
{
    ClanShopInfo clanShopInfo;

    void ClanShopInfo::setClanShopInfo(UInt32 id, ClanShopItems its)
    {
        if( id / 100 >= 40 && id /100 <= MAX_LVL)
        {
            _clanShopInfo[id/1000 - 4].insert(std::make_pair(id , its));
        }
    }
 
    std::map<UInt32, ClanShopInfo::ClanShopItems> ClanShopInfo::getClanShopInfo(UInt8 id)
    {
        return _clanShopInfo[id/10 - 4];
    }

}
