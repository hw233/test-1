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

    MSG_REG_2(GameMsgHdr, REQ::CHAT, OnChat);

    MSG_REG_2(GameMsgHdr, REQ::MAIL, OnMail);
    MSG_REG_2(GameMsgHdr, REQ::MAIL_GET, OnMailGet);
    MSG_REG_2(GameMsgHdr, REQ::MAIL_DELETE, OnMailDelete);
    MSG_REG_2(GameMsgHdr, REQ::MAIL_GET_ALL, OnMailGetAll);
    MSG_REG_2(GameMsgHdr, REQ::MAIL_DELETE_ALL, OnMailDeleteAll);

MSG_HANDLER_END()
