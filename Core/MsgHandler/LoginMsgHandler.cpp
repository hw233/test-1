#include "Config.h"
#include "LoginMsgHandler.h"
#include "LoginOuterMsgHandler.h"
#include "LoginInnerMsgHandler.h"

MSG_HANDLER_DECLARE(LoginMsgHandler)
MSG_REG(GameMsgHdr, UserDisconnect);
MSG_REG(LoginMsgHdr, UserLoginReq);
MSG_REG_2(LoginMsgHdr, 0x02, SockAddrReq);
MSG_REG(LoginMsgHdr, UserReconnectReq);
MSG_REG(LoginMsgHdr, NewUserReq);
MSG_REG_2(LoginMsgHdr, 0xFD, PlayerIDAuth);
MSG_REG_2(LoginMsgHdr, 0xFE, WorldAnnounce);
MSG_REG_2(LoginMsgHdr, 0x100, onUserRecharge);
MSG_REG_2(LoginMsgHdr,0x101,OnKickUser);
MSG_REG_2(LoginMsgHdr,0x102,LockUser);
MSG_REG_2(LoginMsgHdr,0x103,UnlockUser);
MSG_REG_2(LoginMsgHdr,0x104,GmHandlerFromBs);
MSG_REG_2(LoginMsgHdr,0x105,MailFromBs);
MSG_REG_2(LoginMsgHdr,0x106,BanChatFromBs);
MSG_REG_2(LoginMsgHdr,0x107,AddItemFromBs);
MSG_REG_2(LoginMsgHdr,0x110,BattleReportReq);
MSG_HANDLER_END()
