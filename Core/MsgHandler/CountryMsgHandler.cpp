#include "Config.h"
#include "MsgID.h"
#include "CountryMsgHandler.h"
#include "CountryOuterMsgHandler.h"
#include "CountryInnerMsgHandler.h"

MSG_HANDLER_DECLARE(CountryMsgHandler)

    MSG_REG_2(GameMsgHdr, 0x201, PlayerLogin);

    MSG_REG(GameMsgHdr, OnPlayerInfoReq);
    
    MSG_REG_2(GameMsgHdr, REQ::ENCHART, OnEnchantReq);

    MSG_REG_2(GameMsgHdr, REQ::PACKAGE_INFO, OnPackageInfo);

    MSG_REG(GameMsgHdr, OnBattleReportReq);

    MSG_REG(GameMsgHdr, OnFriendListReq);

    MSG_REG(GameMsgHdr, OnFriendFindReq);

    MSG_REG(GameMsgHdr, OnFriendApplyReq);

    MSG_REG(GameMsgHdr, OnFriendAddReq);

    MSG_REG(GameMsgHdr, OnFriendDeleteReq);

    MSG_REG(GameMsgHdr, OnFriendRecommandReq);

MSG_HANDLER_END()
