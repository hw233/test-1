#include "Config.h"
#include "MsgID.h"
#include "CountryMsgHandler.h"
#include "CountryOuterMsgHandler.h"
#include "CountryInnerMsgHandler.h"

MSG_HANDLER_DECLARE(CountryMsgHandler)

MSG_REG(GameMsgHdr, OnNullReq);
MSG_REG_2(GameMsgHdr, REQ::ACHIEVEMENT, OnAttainReq);
MSG_REG(GameMsgHdr, OnSelectCountry);
MSG_REG(GameMsgHdr, OnPlayerInfoReq);
MSG_REG(GameMsgHdr, OnNewGuildReq);
MSG_REG_2(GameMsgHdr, REQ::STATE, OnPlayerInfoChangeReq);
MSG_REG_2(GameMsgHdr, REQ::BOOK_LIST, OnBookStoreListReq);
MSG_REG(GameMsgHdr, OnPurchaseBookReq);
MSG_REG(GameMsgHdr, OnStatusChangeReq);
MSG_REG_2(GameMsgHdr, REQ::READ_BATTLE, OnFormationReq);
MSG_REG_2(GameMsgHdr, REQ::BATTLE_CHECK, OnSetFormationReq);
MSG_REG(GameMsgHdr, OnFighterEquipReq);
MSG_REG(GameMsgHdr, OnRecruitFighterReq);
MSG_REG_2(GameMsgHdr, REQ::HERO_INFO, OnFighterInfoReq);
MSG_REG_2(GameMsgHdr, REQ::HOTEL_LIST, OnRecruitListReq);
MSG_REG_2(GameMsgHdr, REQ::TRIPOD_INFO, OnTripodReq);
MSG_REG_2(GameMsgHdr, REQ::TASK_CYC_REFRESH, OnFlushTaskColorReq);
MSG_REG(GameMsgHdr, OnFighterDismissReq);
MSG_REG(GameMsgHdr, OnFighterRegenReq);
MSG_REG(GameMsgHdr, OnFighterTrainReq);
//MSG_REG(GameMsgHdr, OnFighterTrain2Req);
MSG_REG_2(GameMsgHdr, REQ::TRAIN_FIGHTER_OP, OnFighterTrainOpReq);
MSG_REG_2(GameMsgHdr, REQ::HERO_ISLAND, OnHeroIslandReq);
MSG_REG_2(GameMsgHdr, REQ::CLAN_RANKBATTLE_REQINIT, OnClanRankBattleReqInit);
MSG_REG_2(GameMsgHdr, REQ::CLAN_RANKBATTLE_REQ, OnClanRankBattleReq);
MSG_REG_2(GameMsgHdr, REQ::CLAN_RANKBATTLE_SORTLIST, OnClanRankBattleSortList);
MSG_REG_2(GameMsgHdr, REQ::LUCKYDRAW, OnLuckyDrawReq);

MSG_REG(GameMsgHdr, OnPracticeHookAddReq);
MSG_REG(GameMsgHdr, OnExtendPackageReq);
MSG_REG(GameMsgHdr, OnTakeOnlineRewardReq);
MSG_REG(GameMsgHdr, OnEnchantReq);
MSG_REG(GameMsgHdr, OnOpenSocketReq);
//MSG_REG(GameMsgHdr, OnMergeGemReq);
MSG_REG(GameMsgHdr, OnAttachGemReq);
MSG_REG(GameMsgHdr, OnDetachGemReq);
//MSG_REG(GameMsgHdr, OnSplitReq);
//MSG_REG(GameMsgHdr, OnExchangeReq);
MSG_REG(GameMsgHdr, OnForgeReq);
MSG_REG_2(GameMsgHdr, REQ::EQ_DECOMPOSE, OnBatchSplitReq);
//MSG_REG(GameMsgHdr, OnExchangeSetReq);
MSG_REG(GameMsgHdr, OnBatchMergeReq);
//MSG_REG_2(GameMsgHdr, REQ::GEM_UPGRADE, OnBatchMergeReq);
//MSG_REG(GameMsgHdr, OnActivateAttrReq);
//MSG_REG(GameMsgHdr, OutCitySwitchReq);
MSG_REG(GameMsgHdr, OnActivateAttrReq);
MSG_REG(GameMsgHdr, InCitySwitchReq);
MSG_REG(GameMsgHdr, OnTransportReq);
MSG_REG(GameMsgHdr, OnDungeonOpReq);
MSG_REG(GameMsgHdr, OnDungeonInfoReq);
MSG_REG(GameMsgHdr, OnDungeonBattleReq);
MSG_REG(GameMsgHdr, OnDungeonAutoReq);
MSG_REG(GameMsgHdr, OnDungeonCompleteAutoReq);
MSG_REG(GameMsgHdr, OnDailyReq);
MSG_REG(GameMsgHdr, OnNpcTriggerReq);
MSG_REG(GameMsgHdr, OnPlayerGetTaskReq);
MSG_REG(GameMsgHdr, OnCanAcceptTaskReq);
MSG_REG(GameMsgHdr, OnTaskActionReq);
MSG_REG(GameMsgHdr, OnTaskNpcActionReq);
MSG_REG(GameMsgHdr, CountryBattleJoinReq);
//MSG_REG(GameMsgHdr, OnChallengePlayerReq);
MSG_REG(GameMsgHdr, OnLanchChallengeReq);
MSG_REG(GameMsgHdr, OnRequestChallengeReq);
MSG_REG(GameMsgHdr, OnAttackNpcReq);
MSG_REG(GameMsgHdr, OnAutoBattleReq);
MSG_REG(GameMsgHdr, OnCancelAutoBattleReq);
MSG_REG(GameMsgHdr, OnInstantAutoBattleReq);
MSG_REG(GameMsgHdr, OnBattleEndReq);
MSG_REG(GameMsgHdr, OnConveyBattleReq);
MSG_REG(GameMsgHdr, OnCopyReq);
MSG_REG(GameMsgHdr, OnGetHeroMemoAward);
MSG_REG(GameMsgHdr, OnGetShuoShuoAward);
MSG_REG(GameMsgHdr, OnGetCFriendAward);
MSG_REG(GameMsgHdr, OnGetOfflineExp);

MSG_REG_2(GameMsgHdr, REQ::FORMATION_DATA, OnFrontMapReq);
MSG_REG_2(GameMsgHdr, REQ::AUTO_COPY, OnAutoCopy);
MSG_REG_2(GameMsgHdr, REQ::AUTO_FRONTMAP, OnAutoFrontMap);

MSG_REG(GameMsgHdr, OnDayTaskAutoCompletedReq);
MSG_REG(GameMsgHdr, OnQueryAutoCompletedTaskTimeReq);
MSG_REG(GameMsgHdr, OnQueryDayTaskCompletedCountReq);
MSG_REG(GameMsgHdr, OnQueryPackageItemReq);
MSG_REG_2(GameMsgHdr, REQ::PACK_TRIPOD, OnDestroyItemReq);
MSG_REG_2(GameMsgHdr, REQ::PACK_SELL, OnSellItemReq);
MSG_REG(GameMsgHdr, OnUseItemReq);
MSG_REG(GameMsgHdr, OnUseItemOtherReq);

MSG_REG(GameMsgHdr, OnStoreBuyReq);

MSG_REG(GameMsgHdr, OnChatReq);
MSG_REG(GameMsgHdr, OnPrivChatReq);
MSG_REG(GameMsgHdr, OnChatItemReq);

MSG_REG(GameMsgHdr, OnSubmitBugInfo);
MSG_REG(GameMsgHdr, OnYellowDiamondInfo);
MSG_REG(GameMsgHdr, OnYellowDiamondAwardRcv);
MSG_REG(GameMsgHdr, OnYellowDiamondGetPacksRcv);

MSG_REG(GameMsgHdr, OnTradeListReq);
MSG_REG(GameMsgHdr, OnTradeDataReq);
MSG_REG(GameMsgHdr, OnTradeLaunchReq);
MSG_REG(GameMsgHdr, OnTradeReplyReq);
MSG_REG(GameMsgHdr, OnTradeOperate);

MSG_REG(GameMsgHdr, OnSaleSellReq);

MSG_REG(GameMsgHdr, OnRefreshMartialReq);

MSG_REG(GameMsgHdr, OnMailListReq);
MSG_REG(GameMsgHdr, OnMailReq);
MSG_REG(GameMsgHdr, OnMailClickReq);
MSG_REG_2(GameMsgHdr, REQ::MAIL_DELETE, OnMailDelReq);
MSG_REG_2(GameMsgHdr, REQ::MAIL_LIST, OnMailList2Req);
MSG_REG(GameMsgHdr, OnMailSendReq);

MSG_REG(GameMsgHdr, OnFriendListReq);
MSG_REG(GameMsgHdr, OnFriendOpReq);
//MSG_REG(GameMsgHdr, OnFriendActReq);

//MSG_REG(GameMsgHdr, OnAthleticsDataReq);

MSG_REG(GameMsgHdr, OnBlockBossReq);
MSG_REG(GameMsgHdr, OnAttackBlockBossReq);

MSG_REG(GameMsgHdr, OnPwdQuestionReq);
MSG_REG_2(GameMsgHdr, REQ::OP_PWD, OnOpPwdReq);
MSG_REG(GameMsgHdr, OnLockPwdReq);

MSG_REG_2(GameMsgHdr, REQ::EQ_TRUMP_UPGRADE, OnTrumpUpgrade);
MSG_REG(GameMsgHdr, OnTrumpLOrder);

MSG_REG(GameMsgHdr, OnEquipUpgrade);
MSG_REG(GameMsgHdr, OnEquipSpirit);
MSG_REG(GameMsgHdr, OnUseToken);
MSG_REG(GameMsgHdr, OnMDSoul);
MSG_REG(GameMsgHdr, OnSvrSt);
MSG_REG(GameMsgHdr, OnYBBuf);
MSG_REG(GameMsgHdr, OnGuideUdp);
MSG_REG(GameMsgHdr, OnGetAward);
MSG_REG_2(GameMsgHdr, REQ::FOURCOP, OnFourCopReq);
MSG_REG_2(GameMsgHdr, REQ::TEAM_COPY_REQ, OnTeamCopyReq);
MSG_REG_2(GameMsgHdr, REQ::NEWRELATION, OnNewRelationReq);
MSG_REG_2(GameMsgHdr, REQ::SKILLSTRENGTHEN, OnSkillStrengthen);

MSG_REG_2(GameMsgHdr, REQ::ACTIVITY_LIST, OnActivityList);
MSG_REG_2(GameMsgHdr, REQ::ACTIVITY_REWARD,OnActivityReward);

MSG_REG_2(GameMsgHdr, REQ::TOWN_DEAMON, OnTownDeamonReq);

MSG_REG_2(GameMsgHdr, REQ::SECOND_SOUL, OnSecondSoulReq);
MSG_REG_2(GameMsgHdr, REQ::RC7DAY, OnRC7Day);
MSG_REG_2(GameMsgHdr, REQ::RF7DAY, OnRF7Day);

MSG_REG_2(GameMsgHdr, 0x1D0, OnDailyReq2);

MSG_REG_2(GameMsgHdr, 0x1EE, OnReLoadLuaReq);
MSG_REG_2(GameMsgHdr, 0x1ED, OnRunScriptReq);
MSG_REG_2(GameMsgHdr, 0x1EF, OnRunScriptBattleReq);
MSG_REG_2(GameMsgHdr, 0x1F0, PlayerEnter);
MSG_REG_2(GameMsgHdr, 0x1F1, PlayerLeave);
MSG_REG_2(GameMsgHdr, 0x1F2, OnBroadcast);



MSG_REG_2(GameMsgHdr, 0x200, PlayerLogout);
MSG_REG_2(GameMsgHdr, 0x201, PlayerLogin);
MSG_REG_2(GameMsgHdr, 0x202, PlayerReconnect);
MSG_REG_2(GameMsgHdr, 0x203, PlayerExpAdd);

MSG_REG_2(GameMsgHdr, 0x212, OnAthleticsReq);
MSG_REG_2(GameMsgHdr, 0x213, OnAthleticsDeferNotify);
MSG_REG_2(GameMsgHdr, 0x214, OnAthleticsDropNotify);
MSG_REG_2(GameMsgHdr, 0x215, OnAthleticsBeReq);
MSG_REG_2(GameMsgHdr, 0x216, OnAthleticsEnterResp);

MSG_REG_2(GameMsgHdr, 0x217, OnAthleticsAwardReq);
MSG_REG_2(GameMsgHdr, 0x218, OnGetBoxAddSource);
MSG_REG_2(GameMsgHdr, 0x219, OnOnAthleticsEnterNotify);
MSG_REG_2(GameMsgHdr, 0x220, OnAthleticsResNotify);
MSG_REG_2(GameMsgHdr, 0x221, OnAthleticsReportList);
MSG_REG_2(GameMsgHdr, 0x222, OnAthlectisPayPaging);

MSG_REG_2(GameMsgHdr, 0x223, OnNewRelationAttack);
MSG_REG_2(GameMsgHdr, 0x224, OnNewRelationCountryReq);

MSG_REG_2(GameMsgHdr, 0x230, OnArenaBet);
MSG_REG_2(GameMsgHdr, 0x231, OnArenaBetResult);

MSG_REG_2(GameMsgHdr, 0x244, OnDoAttainment);
MSG_REG_2(GameMsgHdr, 0x245, OnDoActivity);
MSG_REG_2(GameMsgHdr, 0x269, OnDailyCheck);

MSG_REG_2(GameMsgHdr, 0x270, OnGetClanBattleRewardReq);
MSG_REG_2(GameMsgHdr, 0x271, OnClanTakeRewardReq);
MSG_REG_2(GameMsgHdr, 0x272, OnClanBattleAttendReq);
MSG_REG_2(GameMsgHdr, 0x274, OnExpGainByInstantCompleteReq);
MSG_REG_2(GameMsgHdr, 0x276, OnAutoCopyAttack);
MSG_REG_2(GameMsgHdr, 0x277, OnPlayerTimeTick);
MSG_REG_2(GameMsgHdr, 0x278, OnAutoFrontMapAttack);

MSG_REG_2(GameMsgHdr, 0x279, OnCancelAutoBattleNotifyReq);

MSG_REG_2(GameMsgHdr, 0x280, OnCancelDungeonAutoNotifyReq);
MSG_REG_2(GameMsgHdr, 0x281, OnCompleteDungeonAutoNotifyReq);
MSG_REG_2(GameMsgHdr, 0x282, OnResetRecharge);
MSG_REG_2(GameMsgHdr, 0x289, OnTimerEventCompletedNotify);
MSG_REG_2(GameMsgHdr, 0x28C, OnSearchAutoBattleResult);
MSG_REG_2(GameMsgHdr, 0x28D, OnSearchAutoDungeonResult);

MSG_REG_2(GameMsgHdr, 0x291, OnLaunchTradeNotify);
MSG_REG_2(GameMsgHdr, 0x292, OnOperateTradeNotify);
MSG_REG_2(GameMsgHdr, 0x293, OnConfirmTradeNotify);
MSG_REG_2(GameMsgHdr, 0x294, OnTradeDataNotify);
MSG_REG_2(GameMsgHdr, 0x295, OnDelTradeNotify);

MSG_REG_2(GameMsgHdr, 0x296, OnAddAwardGold);
MSG_REG_2(GameMsgHdr, 0x297, OnCreateAward);

MSG_REG_2(GameMsgHdr, 0x2F0, OnGoldRecharge);
MSG_REG_2(GameMsgHdr, 0x2F1, OnYDPacks);

MSG_REG_2(GameMsgHdr, 0x300, OnSalePriceCheckNotify);
MSG_REG_2(GameMsgHdr, 0x301, OnSaleAddItemNotify);
MSG_REG_2(GameMsgHdr, 0x302, OnSaleSellReqNotify);
MSG_REG_2(GameMsgHdr, 0x303, OnSaleSellRespNotify);
MSG_REG_2(GameMsgHdr, 0x304, OnSaleItemCancel);	//ȡ?? & ??ʱ
MSG_REG_2(GameMsgHdr, 0x305, OnSaleItemSearchReq);


MSG_REG_2(GameMsgHdr, 0x306, OnGmHandler);

MSG_REG_2(GameMsgHdr, 0x307, OnDelFgtTrainNotify);

MSG_REG_2(GameMsgHdr, 0x308, OnClanSkillDonateCheckReq);
MSG_REG_2(GameMsgHdr, 0x309, OnClanSkillDonateBackReq);
MSG_REG_2(GameMsgHdr, 0x310, OnOperationTaskAction);
MSG_REG_2(GameMsgHdr, 0x311, OnClanOption);
MSG_REG_2(GameMsgHdr, 0x312, OnClanSkillLevel);
MSG_REG_2(GameMsgHdr, 0x313, OnClanSkillList);
//MSG_REG_2(GameMsgHdr, 0x313, OnFriendLevUp);

MSG_REG_2(GameMsgHdr, 0x320, OnPracticeExpAddReq);
MSG_REG_2(GameMsgHdr, 0x321, OnSetPropsReq);
MSG_REG_2(GameMsgHdr, 0x322, OnSetLevelReq);
MSG_REG_2(GameMsgHdr, 0x323, OnSetMoneyReq);
MSG_REG_2(GameMsgHdr, 0x324, OnSetVipLReq);
MSG_REG_2(GameMsgHdr, 0x325, OnClearTaskReq);

MSG_REG_2(GameMsgHdr, 0x330, OnMartialUpdateHdr);
MSG_REG_2(GameMsgHdr, 0x331, OnMartialUpdate);

MSG_REG_2(GameMsgHdr, 0x351, OnMartialUpdate2);

MSG_REG_2(GameMsgHdr, 0x232, OnAthleticsMartialReq);
MSG_REG_2(GameMsgHdr, 0x233, OnAthleticsMartialAttack);
MSG_REG_2(GameMsgHdr, 0x234, OnAthleticsMartialBeAttack);
MSG_REG_2(GameMsgHdr, 0x235, OnAwardAthleticsMartial);

MSG_REG_2(GameMsgHdr, 0x340, OnTownDeamonlBeAttack);
MSG_REG_2(GameMsgHdr, 0x341, OnTownDeamonResNotify);

MSG_REG_2(GameMsgHdr, 0x236, OnAwardHIPrestige);
MSG_REG_2(GameMsgHdr, 0x237, OnSendShusanLoveTitleCard);
MSG_REG_2(GameMsgHdr, 0x238, OnAddPExpBy);
MSG_REG_2(GameMsgHdr, 0x239, OnPracticeAttack);
MSG_REG_2(GameMsgHdr, 0x240, OnHeroMemo);
MSG_REG_2(GameMsgHdr, 0x241, OnAddItemBy);
MSG_REG_2(GameMsgHdr, 0x242, OnCFriendLvlUp);
MSG_REG_2(GameMsgHdr, 0x243, OnShuoShuo);
MSG_REG_2(GameMsgHdr, 0x246, OnSendMayDayTitleCard);
MSG_REG_2(GameMsgHdr, 0x247, OnSendJuneHappyTitleCard);
MSG_REG_2(GameMsgHdr, 0x248, OnAddItemByIDIP);
MSG_REG_2(GameMsgHdr, 0x249, OnSendRNR);


MSG_REG_2(GameMsgHdr, 0x250, OnRemoveClanRank);

MSG_REG_2(GameMsgHdr, 0x251, OnGetArenaMoney);
MSG_REG_2(GameMsgHdr, 0x252, OnArenaEnterCommit);
MSG_REG_2(GameMsgHdr, 0x253, OnSendPExpCard);

MSG_HANDLER_END()
