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
MSG_REG(GameMsgHdr, OnBattleReportReq2);
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
MSG_REG_2(GameMsgHdr, REQ::ATHLETICS_PAGING,    OnAthleticsPaging);
MSG_REG_2(GameMsgHdr, REQ::ATHLETICS_KILLCD,    OnAthleticsKillCD);
MSG_REG(GameMsgHdr, OnGetOwnBoxReq);
MSG_REG(GameMsgHdr, OnAthleticsGetAwardReq);
MSG_REG(GameMsgHdr, OnLuckDrawRankListReq);

MSG_REG(GameMsgHdr, OnItemHistoryReq);
MSG_REG_2(GameMsgHdr, REQ::CLAN_PACKAGE, OnClanPackageReq);

MSG_REG_2(GameMsgHdr, REQ::CLAN_SKILL, OnClanTechOpReq);
MSG_REG_2(GameMsgHdr, REQ::CLAN_BATTLE, OnClanCityBattleReq);
MSG_REG_2(GameMsgHdr, REQ::CLANCITYBATTLELIST, OnClanCityBattleListReq);
MSG_REG(GameMsgHdr, OnArenaInfoReq);
MSG_REG(GameMsgHdr, OnArenaLeaderBoardReq);
MSG_REG_2(GameMsgHdr, REQ::SERVER_ARENA_EXTRA_ACT, OnArenaExtraActReq);
MSG_REG_2(GameMsgHdr, REQ::SERVER_ARENA_OP, OnArenaOpReq);
MSG_REG_2(GameMsgHdr, REQ::SERVERWAR_ARENA_OP, OnArenaWarOpReq);

MSG_REG_2(GameMsgHdr, REQ::ACTIVE, OnQixiReq);
MSG_REG_2(GameMsgHdr, REQ::MARRYBOARD, OnMarryBard);

MSG_REG_2(GameMsgHdr, REQ::TOWN_DEAMON, OnTownDeamonReq);

MSG_REG_2(GameMsgHdr, REQ::SINGLE_HERO, OnSingleHeroReq);
MSG_REG_2(GameMsgHdr, REQ::CLAN_QQ, OnClanQQ);

MSG_REG_2(ArenaMsgHdr, 0x101, OnArenaConnected);
MSG_REG_2(ArenaMsgHdr, 0x102, OnPlayerEntered);
MSG_REG_2(ArenaMsgHdr, 0x103, OnLineupCommited);
MSG_REG_2(ArenaMsgHdr, 0x104, OnArenaPreliminary);
MSG_REG_2(ArenaMsgHdr, 0x105, OnPreliminaryInfo);
MSG_REG_2(ArenaMsgHdr, 0x106, OnArenaBattleReport);
MSG_REG_2(ArenaMsgHdr, 0x107, OnArenaSupport);
MSG_REG_2(ArenaMsgHdr, 0x108, OnArenaBattlePoint);
MSG_REG_2(ArenaMsgHdr, 0x109, OnArenaLeaderBoard);

MSG_REG_2(ArenaMsgHdr, 0x110, OnTeamArenaEntered);
MSG_REG_2(ArenaMsgHdr, 0x111, OnTeamArenaConnected);
MSG_REG_2(ArenaMsgHdr, 0x112, OnTeamArenaPreliminary);
MSG_REG_2(ArenaMsgHdr, 0x113, OnTeamArenaBattlePoint);
MSG_REG_2(ArenaMsgHdr, 0x114, OnTeamArenaLeaderBoard);
MSG_REG_2(ArenaMsgHdr, 0x115, OnTeamArenaInspireLevel);
MSG_REG_2(ArenaMsgHdr, 0x116, OnTeamArenaLastRank);
MSG_REG_2(ArenaMsgHdr, 0x117, OnUpdateArenaSession);

MSG_REG_2(ServerWarMsgHdr, 0x121, OnServerWarConnected);
MSG_REG_2(ServerWarMsgHdr, 0x122, OnServerWarPlayerEntered);
MSG_REG_2(ServerWarMsgHdr, 0x123, OnServerWarLineupCommited);
MSG_REG_2(ServerWarMsgHdr, 0x124, OnServerWarPreliminary);
//MSG_REG_2(ServerWarMsgHdr, 0x125, OnServerWarPreliminaryInfo);
MSG_REG_2(ServerWarMsgHdr, 0x126, OnServerWarBattleReport);
MSG_REG_2(ServerWarMsgHdr, 0x127, OnServerWarSupport);
MSG_REG_2(ServerWarMsgHdr, 0x128, OnServerWarBattlePoint);
MSG_REG_2(ServerWarMsgHdr, 0x129, OnServerWarLeaderBoard);
MSG_REG_2(ServerWarMsgHdr, 0x12A, OnServerRechargeRank);

MSG_REG_2(ServerLeftMsgHdr, 0x131, OnServerLeftConnected);  //连接
MSG_REG_2(ServerLeftMsgHdr, 0x132, OnServerLeftPlayerEntered); //组队攻击
MSG_REG_2(ServerLeftMsgHdr, 0x133, OnServerLeftLineupCommited);//阵形确认
MSG_REG_2(ServerLeftMsgHdr, 0x134, OnServerLeftGetSpirit);  //帮派获得元气
MSG_REG_2(ServerLeftMsgHdr, 0x135, OnServerLeftGetAward);  //帮派获得仙界赐福
MSG_REG_2(ServerLeftMsgHdr, 0x136, OnServerLeftBattleReport); //接受战报
MSG_REG_2(ServerLeftMsgHdr, 0x137, OnServerLeftRevInfo);   //接受遗迹信息
MSG_REG_2(ServerLeftMsgHdr, 0x138, OnServerLeftErrInfo);   //接受错误信息

MSG_REG_2(GameMsgHdr, 0x160, OnClanChatReq);
MSG_REG_2(GameMsgHdr, 0x165, OnSpreadModifyVar);

MSG_REG_2(GameMsgHdr, 0x171, OnClanTakeRewardResultReq);
MSG_REG_2(GameMsgHdr, 0x172, OnTeamArenaAddMember);
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

MSG_REG_2(GameMsgHdr, 0x17E, OnCancelJobHunterAutoReq);

MSG_REG_2(GameMsgHdr, 0x180, OnCancelDungeonAutoReq);
MSG_REG_2(GameMsgHdr, 0x181, OnCompleteDungeonAutoReq);

MSG_REG_2(GameMsgHdr, 0x198, OnPopTimerEvent);
MSG_REG_2(GameMsgHdr, 0x199, OnPushTimerEvent);

MSG_REG_2(GameMsgHdr, 0x19A, OnAthleticsPayRet);

MSG_REG_2(GameMsgHdr, 0x19C, OnSearchEvents);

MSG_REG_2(GameMsgHdr, 0x19D, OnAthleticsOver);
MSG_REG_2(GameMsgHdr, 0x19E, OnAthleticsEnter);
MSG_REG_2(GameMsgHdr, 0x19F, OnAthleticsAndClanNotify);

MSG_REG_2(GameMsgHdr, 0x1A1, OnAddTradeCheckNotify);
MSG_REG_2(GameMsgHdr, 0x1A2, OnDelTradeCheckNotify);

MSG_REG_2(GameMsgHdr, 0x1A3, OnSpecialAward);

MSG_REG_2(GameMsgHdr, 0x1A4, OnUpdateBlockBossRank);
MSG_REG_2(GameMsgHdr, 0x1A5, OnBloackBossDataReq);
MSG_REG_2(GameMsgHdr, 0x1A6, OnFighterStandup);
MSG_REG_2(GameMsgHdr, 0x1A7, OnArenaAddLevelMan);

MSG_REG_2(GameMsgHdr, 0x1A8, OnSHFighterCloneRes);
MSG_REG_2(GameMsgHdr, 0x1A9, OnSHReset);
MSG_REG_2(GameMsgHdr, 0x1AA, OnSHEnter);
MSG_REG_2(GameMsgHdr, 0x1AB, OnEnterArena);
MSG_REG_2(GameMsgHdr, 0x1AC, OnSHStageOnOff);
MSG_REG_2(GameMsgHdr, 0x1AD, OnCreateTeamArena);
MSG_REG_2(GameMsgHdr, 0x1AE, OnAddeamArenaScore);

MSG_REG_2(GameMsgHdr, 0x1B1, OnSearchSaleNotify);
MSG_REG_2(GameMsgHdr, 0x1B2, OnPutSaleNotify);
MSG_REG_2(GameMsgHdr, 0x1B3, OnBuySaleCheckOKNotify);
MSG_REG_2(GameMsgHdr, 0x1B4, OnSaleItemCancle);

MSG_REG_2(GameMsgHdr, 0x1B5, OnAddQiShiBanCount);

MSG_REG_2(GameMsgHdr, 0x1C0, OnLuckyDraw);
MSG_REG_2(GameMsgHdr, 0x1C1, OnRechargeRank);
MSG_REG_2(GameMsgHdr, 0x1C2, OnConsumeRank);
MSG_REG_2(GameMsgHdr, 0x1C3, OnSendRechargeRank);
MSG_REG_2(GameMsgHdr, 0x1C4, OnSendConsumeRank);

MSG_REG_2(GameMsgHdr, 0x1C5, OnGetQgameGiftAward);

MSG_REG_2(GameMsgHdr, 0x1C6, OnSendPopularityRank);
MSG_REG_2(GameMsgHdr, 0x1C7, OnPopularityRank);
MSG_REG_2(GameMsgHdr, 0x1C8, OnLuckyBagRank);
MSG_REG_2(GameMsgHdr, 0x1C9, OnSendLuckyBagRank);
MSG_REG_2(GameMsgHdr, 0x1CC, OnClearLuckyBagRank);

MSG_REG_2(GameMsgHdr, 0x1CA, OnRechargeRP7Rank);
MSG_REG_2(GameMsgHdr, 0x1CB, SendRechargeRP7Rank);
MSG_REG_2(GameMsgHdr, 0x1CD, On11PlayerGradeRank);
MSG_REG_2(GameMsgHdr, 0x1CE, On11ClanGradeRank);
MSG_REG_2(GameMsgHdr, 0x1CF, On11CountryGradeRank);
MSG_REG_2(GameMsgHdr, 0x1D0, OnSendClanMemberList);
MSG_REG_2(GameMsgHdr, 0x1D1, OnSendClanMemberGrade);
MSG_REG_2(GameMsgHdr, 0x1D2, OnSend11PlayerGradeRank);
MSG_REG_2(GameMsgHdr, 0x1D3, OnSend11ClanGradeRank);
MSG_REG_2(GameMsgHdr, 0x1D4, Update11ClanRank);
MSG_REG_2(GameMsgHdr, 0x1D7, SetQiShiBanRank);
MSG_REG_2(GameMsgHdr, 0x1D8, SendQiShiBanRank);
MSG_REG_2(GameMsgHdr, 0x1D5, OnGuangGunRank);
MSG_REG_2(GameMsgHdr, 0x1D6, OnSendGuangGunRank);
MSG_REG_2(GameMsgHdr, 0x1D9, OnSendOffQQClan);
MSG_REG_2(GameMsgHdr, 0x1DA, OnSendHappyFireRank);
MSG_REG_2(GameMsgHdr, 0x1DB, OnHappyFireRank);
MSG_REG_2(GameMsgHdr, 0x1DC, SendQSBState);
MSG_REG_2(GameMsgHdr, 0x1DD, OnSetCFriendInvited);
MSG_REG_2(GameMsgHdr, 0x1DE, OnDelCFriendInvited);
MSG_REG_2(GameMsgHdr, 0x1DF, OnSendCFriendInvited);

//FB占位(0x1E0~0x1E9),简体千万别使用
//0x1E0
//0x1E1
//0x1E2
//0x1E3
//0x1E4
//0x1E5
//0x1E6
//0x1E7
//0x1E8
//0x1E9

MSG_REG_2(GameMsgHdr, 0x1EA, OnServerWarResNotify);
MSG_REG_2(GameMsgHdr, 0x1EB, OnServerWarBossTimer);

MSG_REG_2(GameMsgHdr, 0x1EC, OnDaysRankMsg);
MSG_REG_2(GameMsgHdr, 0x1EE, OnReloadLuaReq);
MSG_REG_2(GameMsgHdr, 0x1ED, OnRunscriptReq);
MSG_REG_2(GameMsgHdr, 0x1EF, OnRunscriptBattleReq);

MSG_REG_2(GameMsgHdr, 0x1F0, OnLevelChange);
//MSG_REG_2(GameMsgHdr, 0x1F1, OnAthleticsMartialFlush);
MSG_REG_2(GameMsgHdr, 0x1F2, OnAthMartialOver);

MSG_REG_2(GameMsgHdr, 0x1F3, OnReleaseClanSkillLevelOp);

MSG_REG_2(GameMsgHdr, 0x1F4, OnMoveAllToMax);
MSG_REG_2(GameMsgHdr, 0x1F5, OnReplaceOwner);

MSG_REG_2(GameMsgHdr, 0x1F6, OnPracticeAddExp);
MSG_REG_2(GameMsgHdr, 0x1F7, OnPracticeStop);

//MSG_REG_2(GameMsgHdr, 0x1F8, OnAthleticsFlush);

MSG_REG_2(GameMsgHdr, 0x1F9, OnRoamResult);

MSG_REG_2(GameMsgHdr, 0x1FA, OnReCalcWeekDayAddTimer);
MSG_REG_2(GameMsgHdr, 0x1FB, OnReCalcWeekDayRemoveTimer);

MSG_REG_2(GameMsgHdr, 0x1FC, OnTownDeamonResNotify);
MSG_REG_2(GameMsgHdr, 0x1FD, OnTownDeamonAttackNpcNotify);

MSG_REG_2(GameMsgHdr, 0x1FE, OnKillMonsterRoamResult);
MSG_REG_2(GameMsgHdr, 0x1FF, OnKillMonsterReqInfo);

#ifdef _ARENA_SERVER
MSG_REG_2(LoginMsgHdr, 0x01, onServerReg);
MSG_REG_2(GameMsgHdr, 0x02, onPlayerEnter);
MSG_REG_2(GameMsgHdr, 0x03, onLineupCommit);
MSG_REG_2(GameMsgHdr, 0x06, onBetInfo);
MSG_REG_2(GameMsgHdr, 0x07, onSingleBet);
MSG_REG_2(LoginMsgHdr, 0x84, GmHandlerFromBs);
MSG_REG_2(LoginMsgHdr, 0x90, BattleReportReq);
MSG_REG_2(GameMsgHdr, 0xFF, onDisconnected);
#endif

MSG_HANDLER_END()
