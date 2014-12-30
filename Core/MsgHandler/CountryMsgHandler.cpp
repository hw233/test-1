#include "Config.h"
#include "MsgID.h"
#include "CountryMsgHandler.h"
#include "CountryOuterMsgHandler.h"
#include "CountryInnerMsgHandler.h"

MSG_HANDLER_DECLARE(CountryMsgHandler)

    MSG_REG_2(GameMsgHdr, 0x201, PlayerLogin);

    MSG_REG(GameMsgHdr, OnPlayerInfoReq);
    

MSG_HANDLER_END()
