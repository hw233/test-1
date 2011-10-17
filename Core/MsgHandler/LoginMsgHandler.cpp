#include "Config.h"
#include "LoginMsgHandler.h"
#include "LoginOuterMsgHandler.h"
#include "LoginInnerMsgHandler.h"
#include "MsgID.h"

MSG_HANDLER_DECLARE(LoginMsgHandler)
MSG_REG(GameMsgHdr, UserDisconnect);
MSG_REG(LoginMsgHdr, UserLoginReq);
MSG_REG_2(LoginMsgHdr, SPEQ::SOCK_ADDR, SockAddrReq);
MSG_REG(LoginMsgHdr, UserReconnectReq);
MSG_REG(LoginMsgHdr, NewUserReq);
MSG_REG_2(LoginMsgHdr, SPEQ::PLAYERIDAUTH, PlayerIDAuth);
MSG_REG_2(LoginMsgHdr, SPEQ::WORLDANNOUNCE, WorldAnnounce);
MSG_REG_2(LoginMsgHdr, SPEQ::USERRECHARGE, onUserRecharge);
MSG_REG_2(LoginMsgHdr, SPEQ::RERECHARGE, onUserReRecharge);
MSG_REG_2(LoginMsgHdr,SPEQ::KILLUSER,OnKickUser);
MSG_REG_2(LoginMsgHdr,SPEQ::LOCKUSER,LockUser);
MSG_REG_2(LoginMsgHdr,SPEQ::UNLOCKUSER,UnlockUser);
MSG_REG_2(LoginMsgHdr,SPEQ::GMHANDLERFROMBS,GmHandlerFromBs);
MSG_REG_2(LoginMsgHdr,SPEQ::MAILFROMBS,MailFromBs);
MSG_REG_2(LoginMsgHdr,SPEQ::BANCHATFROMBS,BanChatFromBs);
MSG_REG_2(LoginMsgHdr,SPEQ::ADDITEMFROMBS,AddItemFromBs);
MSG_REG_2(LoginMsgHdr,SPEQ::ADDITEMTOALL,AddItemToAllFromBs);
MSG_REG_2(LoginMsgHdr,SPEQ::BATTLEREPORT,BattleReportReq);
MSG_REG_2(LoginMsgHdr,SPEQ::ONLINE,ServerOnlineNum);
MSG_REG_2(LoginMsgHdr,SPEQ::SETLEVEL,SetLevelFromBs);
MSG_REG_2(LoginMsgHdr,SPEQ::SETPROPS,SetPropsFromBs);
MSG_REG_2(LoginMsgHdr,SPEQ::SETMONEY,SetMoneyFromBs);
MSG_REG_2(LoginMsgHdr,SPEQ::LOADLUA,LoadLuaFromBs);
MSG_REG_2(LoginMsgHdr,SPEQ::SETVIPL,SetVIPLFromBs);
MSG_REG_2(LoginMsgHdr,SPEQ::CLSTASK,ClearTaskFromBs);
MSG_REG_2(LoginMsgHdr,SPEQ::SALE_ONOFF,reqSaleOnOffFromBs);

MSG_REG_2(LoginMsgHdr, 0x300, OnCheckPackKey);

MSG_HANDLER_END()
