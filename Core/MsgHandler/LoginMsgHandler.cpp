#include "Config.h"
#include "LoginMsgHandler.h"
#include "LoginOuterMsgHandler.h"
#include "LoginInnerMsgHandler.h"
#include "MsgID.h"

MSG_HANDLER_DECLARE(LoginMsgHandler)
MSG_REG(LoginMsgHdr, UserLoginReq);
//MSG_REG_2(LoginMsgHdr, SPEQ::USERRECHARGE, onUserRecharge);
MSG_REG(LoginMsgHdr, NewUserReq);
MSG_REG(LoginMsgHdr, CreateAccount);
MSG_REG(LoginMsgHdr, IdentifyAccount);
//MSG_REG_2(LoginMsgHdr, 0x330, OnForbidSale);
//MSG_REG(GameMsgHdr, UserDisconnect);
MSG_REG_2(LoginMsgHdr, 0x123, OnRecharge);

MSG_HANDLER_END()
