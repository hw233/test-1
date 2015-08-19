#include "Config.h"
#include "WorldMsgHandler.h"
#include "WorldOuterMsgHandler.h"
#include "WorldInnerMsgHandler.h"
#include "MsgID.h"

MSG_HANDLER_DECLARE(WorldMsgHandler)

//MSG_REG(GameMsgHdr, OnBattleReportReq2);
//MSG_REG_2(GameMsgHdr, REQ::ACTIVE, OnQixiReq);
//MSG_REG_2(GameMsgHdr, 0x150, OnWorldCupResult);
//MSG_REG_2(GameMsgHdr, 0x198, OnPopTimerEvent);
//MSG_REG_2(GameMsgHdr, 0x199, OnPushTimerEvent);  
//MSG_REG_2(GameMsgHdr, 0x19C, OnSearchEvents); 
//MSG_REG_2(GameMsgHdr, REQ::ACTIVE, OnQixiReq);
//
MSG_REG_2(GameMsgHdr,REQ::CLAN_BATTLE_JOIN,OnJoinClanBattle);
MSG_REG_2(GameMsgHdr,REQ::CLAN_BATTLE_INFO,OnClanBattleInfo);
MSG_REG_2(GameMsgHdr,REQ::CLAN_BATTLE_ADDFIGHTER,OnClanBattleAddFighter);
MSG_REG_2(GameMsgHdr,REQ::CLAN_BATTLE_MOVEFIGHTER,OnClanBattleMoveFighter);
MSG_REG_2(GameMsgHdr,REQ::CLAN_BATTLE_CANCELFIGHTER,OnClanBattleDelFighter);
MSG_REG_2(GameMsgHdr,REQ::CLAN_BATTLE_COMMENTS,OnClanBattleComment);
MSG_REG_2(GameMsgHdr,REQ::CLAN_BATTLE_ORDERS,OnClanBattleOrder);
MSG_REG_2(GameMsgHdr,REQ::CLAN_BATTLE_REPORT,OnClanBattleReport);
MSG_REG_2(GameMsgHdr,REQ::CLAN_BATTLE_REPORTList,OnClanBattleReportList);

MSG_HANDLER_END()
