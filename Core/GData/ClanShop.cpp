#include"ClanShop.h"

namespace GData
{
    ClanShopInfo clanShopInfo;

    void ClanShopInfo::setClanShopInfo(UInt32 id, ClanShopItems its)
    {
        _clanShopInfo.insert(std::make_pair(id , its));
    }
 
    std::map<UInt32, ClanShopInfo::ClanShopItems> ClanShopInfo::getClanShopInfo()
    {
        return _clanShopInfo;
    }

}
