#include "Config.h"
#include "WorldMsgHandler.h"
#include "WorldOuterMsgHandler.h"
#include "WorldInnerMsgHandler.h"
#include "MsgID.h"

MSG_HANDLER_DECLARE(WorldMsgHandler)

MSG_REG(GameMsgHdr, OnClanListReq);
MSG_REG(GameMsgHdr, OnClanInfoReq);
MSG_REG(GameMsgHdr, OnClanCreateReq);
MSG_REG(GameMsgHdr, OnClanApplyReq);
MSG_REG_2(GameMsgHdr, REQ::CLAN_OPERATE, OnClanOpReq);
MSG_REG(GameMsgHdr, OnClanModifyReq);
MSG_REG(GameMsgHdr, OnClanQueryReq);
MSG_REG(GameMsgHdr, OnClanQuery2Req);
MSG_REG(GameMsgHdr, OnClanBattleReportReq);
//MSG_REG(GameMsgHdr, OnClanPersonalRewardReq);
MSG_REG_2(GameMsgHdr, REQ::CLAN_BUILD, OnClanPracticePlaceOpReq);
MSG_REG(GameMsgHdr, OnClanRewardReq);
MSG_REG(GameMsgHdr, OnClanPersonalRewardOpReq);
MSG_REG_2(GameMsgHdr, REQ::ALLOCATECLANREWARD, OnAllocateClanBattleRewardReq);
MSG_REG(GameMsgHdr, OnClanAllocRecordReq);
MSG_REG(GameMsgHdr, OnClanGetDynamicMsgReq);
MSG_REG(GameMsgHdr, OnPlayerIdReq);
MSG_REG(GameMsgHdr, OnStoreListReq);
MSG_REG(GameMsgHdr, OnLeaderboardReq);
MSG_REG(GameMsgHdr, OnOwnLeaderboardReq);
MSG_REG(GameMsgHdr, OnBattleReportReq);
MSG_REG(GameMsgHdr, OnSaleListReq);
MSG_REG(GameMsgHdr, OnSaleBuyAndCancelReq);
MSG_REG(GameMsgHdr, OnPracticePlaceReq);
MSG_REG(GameMsgHdr, OnPracticeListReq);
MSG_REG(GameMsgHdr, OnPracticeChallengeReq);
MSG_REG(GameMsgHdr, OnPracticePayReq);
MSG_REG(GameMsgHdr, OnPracticeSitDownReq);
MSG_REG(GameMsgHdr, OnPracticeStopReq);
MSG_REG(GameMsgHdr, OnAthleticsListReq);
MSG_REG_2(GameMsgHdr, REQ::ATHLETICS_CHALLENGE, OnAthleticsChallengeReq);
MSG_REG(GameMsgHdr, OnGetOwnBoxReq);

MSG_REG_2(GameMsgHdr, REQ::CLAN_SKILL, OnClanTechOpReq);
MSG_REG_2(GameMsgHdr, REQ::CLAN_BATTLE, OnClanCityBattleReq);
MSG_REG_2(GameMsgHdr, REQ::CLANCITYBATTLELIST, OnClanCityBattleListReq);
MSG_REG(GameMsgHdr, OnArenaInfoReq);
MSG_REG(GameMsgHdr, OnArenaEliminationReq);
MSG_REG_2(GameMsgHdr, REQ::SERVER_ARENA_OP, OnArenaOpReq);

MSG_REG_2(ArenaMsgHdr, 0x101, OnArenaConnected);
MSG_REG_2(ArenaMsgHdr, 0x102, OnPlayerEntered);
MSG_REG_2(ArenaMsgHdr, 0x103, OnLineupCommited);
MSG_REG_2(ArenaMsgHdr, 0x104, OnArenaPriliminary);
MSG_REG_2(ArenaMsgHdr, 0x105, OnPriliminaryInfo);

MSG_REG_2(GameMsgHdr, 0x160, OnClanChatReq);

MSG_REG_2(GameMsgHdr, 0x171, OnClanTakeRewardResultReq);
MSG_REG_2(GameMsgHdr, 0x173, OnClanMailInviteClick);
MSG_REG_2(GameMsgHdr, 0x174, OnClanMailClick);
MSG_REG_2(GameMsgHdr, 0x175, OnClanTechDonateCheckResp);
MSG_REG_2(GameMsgHdr, 0x176, OnClanAllyMailResp);
MSG_REG_2(GameMsgHdr, 0x177, OnClanBattlerOffTimeCheck);
MSG_REG_2(GameMsgHdr, 0x178, OnDoInstantAutoBattleReq);
MSG_REG_2(GameMsgHdr, 0x179, OnDoCancelAutoBattleReq);
MSG_REG_2(GameMsgHdr, 0x17A, OnDonateClanbyGM);
MSG_REG_2(GameMsgHdr, 0x17B, OnAddClanBoxGM);

MSG_REG_2(GameMsgHdr, 0x17C, OnTrainAccelerateReq);
MSG_REG_2(GameMsgHdr, 0x17D, OnDoInstantPracticeAccReq);

MSG_REG_2(GameMsgHdr, 0x180, OnCancelDungeonAutoReq);
MSG_REG_2(GameMsgHdr, 0x181, OnCompleteDungeonAutoReq);

MSG_REG_2(GameMsgHdr, 0x198, OnPopTimerEvent);
MSG_REG_2(GameMsgHdr, 0x199, OnPushTimerEvent);
MSG_REG_2(GameMsgHdr, 0x19C, OnSearchEvents);

MSG_REG_2(GameMsgHdr, 0x19D, OnAthleticsOver);
MSG_REG_2(GameMsgHdr, 0x19E, OnAthleticsEnter);
MSG_REG_2(GameMsgHdr, 0x19F, OnAthleticsAndClanNotify);


MSG_REG_2(GameMsgHdr, 0x1A1, OnAddTradeCheckNotify);
MSG_REG_2(GameMsgHdr, 0x1A2, OnDelTradeCheckNotify);

MSG_REG_2(GameMsgHdr, 0x1A3, OnSpecialAward);

MSG_REG_2(GameMsgHdr, 0x1A4, OnUpdateBlockBossRank);
MSG_REG_2(GameMsgHdr, 0x1A5, OnBloackBossDataReq);

MSG_REG_2(GameMsgHdr, 0x1B1, OnSearchSaleNotify);
MSG_REG_2(GameMsgHdr, 0x1B2, OnPutSaleNotify);
MSG_REG_2(GameMsgHdr, 0x1B3, OnBuySaleCheckOKNotify);
MSG_REG_2(GameMsgHdr, 0x1EE, OnReloadLuaReq);
MSG_REG_2(GameMsgHdr, 0x1ED, OnRunscriptReq);
MSG_REG_2(GameMsgHdr, 0x1EF, OnRunscriptBattleReq);


MSG_HANDLER_END()
