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

    MSG_REG_2(GameMsgHdr, REQ::FIND_FIGHTER, OnFindFighter);
    MSG_REG_2(GameMsgHdr, REQ::FIND_INFO, OnFindInfo);

    MSG_REG_2(GameMsgHdr, REQ::UP_FIGHTER, OnFindUp);

    MSG_REG(GameMsgHdr, OnBattleReportReq0);
    MSG_REG(GameMsgHdr, OnBattleReportReq1);

    //MSG_REG_2(GameMsgHdr,REQ::FRIEND_LIST, OnFriendListReq);

    MSG_REG(GameMsgHdr, OnFriendFindReq);

    MSG_REG(GameMsgHdr, OnFriendApplyReq);

    MSG_REG(GameMsgHdr, OnFriendAddReq);

    MSG_REG(GameMsgHdr, OnFriendDeleteReq);

    MSG_REG(GameMsgHdr, OnFriendRecommandReq);

    //MSG_REG(GameMsgHdr, OnFriendBaseInfoReq);

    MSG_REG_2(GameMsgHdr, REQ::CHAT, OnChat);

    MSG_REG_2(GameMsgHdr, REQ::MAIL, OnMail);

    MSG_REG_2(GameMsgHdr, REQ::MAIL_GET, OnMailGet);

    MSG_REG_2(GameMsgHdr, REQ::MAIL_DELETE, OnMailDelete);

    MSG_REG_2(GameMsgHdr, REQ::MAIL_GET_ALL, OnMailGetAll);

    MSG_REG_2(GameMsgHdr, REQ::MAIL_DELETE_ALL, OnMailDeleteAll);

    MSG_REG(GameMsgHdr, OnReplaceFighter);

    //MSG_REG_2(GameMsgHdr, REQ::GOVERN_GAIN, OnGovernGain);

    MSG_REG(GameMsgHdr, OnGovernSpeedUp);

    MSG_REG(GameMsgHdr, OnGovernInfo);
    MSG_REG_2(GameMsgHdr, REQ::CLAN_CREATE, OnClanCreate);
    //MSG_REG_2(GameMsgHdr, REQ::CLAN_FLASH, OnClanFlash);
    MSG_REG_2(GameMsgHdr, REQ::CLAN_OPTION, OnClanOption);

    //MSG_REG_2(GameMsgHdr, REQ::CLAN_APPLY, OnClanApply);

    MSG_REG_2(GameMsgHdr, REQ::GMHAND, OnGMHandler);

    MSG_REG_2(GameMsgHdr,0x155,OnGiveOnlineAward);

    MSG_REG_2(GameMsgHdr,0x156,OnSendGovernResult);

    //推图战役奖励
    MSG_REG_2(GameMsgHdr,REQ::BATTLE_AWARD,OnFarWardAward);

MSG_HANDLER_END()
