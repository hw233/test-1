#ifndef _GMHANDLER_H_
#define _GMHANDLER_H_

namespace GObject
{
	class Player;
}

class GMHandler
{
public:
	GMHandler();
	bool Handle(const std::string&, GObject::Player *,bool isFromBackstage=false);

private:
	typedef void (GMHandler::*GMHPROC)(GObject::Player *, std::vector<std::string>&);
	typedef void (GMHandler::*GMHPROCNP)(std::vector<std::string>&);
	struct GMHP {
		bool np;
		GMHPROC proc;
		GMHPROCNP procnp;
	};
	void Reg(int, const std::string&, GMHPROC);
	void Reg(int, const std::string&, GMHPROCNP);
	std::map<std::string, GMHP> _procMap[4];

private:
	void OnClanDonate(GObject::Player *, std::vector<std::string>&);
	void OnSetVip(GObject::Player *, std::vector<std::string>&);
	void OnBanChat(std::vector<std::string>&);
	void OnAddExp(GObject::Player *, std::vector<std::string>&);
	void OnAddItem(GObject::Player *, std::vector<std::string>&);
	void OnAddItemB(GObject::Player *, std::vector<std::string>&);
	void OnAddItemSet(GObject::Player *, std::vector<std::string>&);
	void OnEquipAll(GObject::Player *, std::vector<std::string>&);
	void OnAddMoney(GObject::Player *, std::vector<std::string>&);
	void OnTopup(GObject::Player *, std::vector<std::string>&);
	void OnKillMoney(GObject::Player *, std::vector<std::string>&);
	void OnHoly(GObject::Player *, std::vector<std::string>&);
	void OnAddFighter(GObject::Player *, std::vector<std::string>&);
	void OnSetPotential(GObject::Player *, std::vector<std::string>&);
	void OnSetFriendliness(GObject::Player *, std::vector<std::string>&);
	void OnSetSpeed(GObject::Player *, std::vector<std::string>&);
	void OnRename(GObject::Player *, std::vector<std::string>&);
	void OnPlayerInfo(GObject::Player *, std::vector<std::string>&);
	void OnCharInfo(GObject::Player *, std::vector<std::string>&);
	void OnMove(GObject::Player *, std::vector<std::string>&);
	void OnRegen(GObject::Player *, std::vector<std::string>&);
	void OnSetGM(std::vector<std::string>&);
	void OnTestLua(GObject::Player *, std::vector<std::string>&);
	void OnReLoadLua(std::vector<std::string>&);
	void OnSetDL(GObject::Player *, std::vector<std::string>&);
	void OnPlayerWallow(GObject::Player * player, std::vector<std::string>&);
	void OnWallow(std::vector<std::string>&);
	void OnLimitLuckyDraw(std::vector<std::string>&);
	void OnRunScript(std::vector<std::string>&);
	void OnSuper(GObject::Player *, std::vector<std::string>&);
	void OnReconn(std::vector<std::string>&);
	void OnSpawn(std::vector<std::string>&);
	void OnUnspawn(std::vector<std::string>&);
	void OnSetTitle(std::vector<std::string>&);
	void OnChallenge(GObject::Player *, std::vector<std::string>&);
	void OnUpSkill(GObject::Player *, std::vector<std::string>&);
	void OnOffSkill(GObject::Player *, std::vector<std::string>&);
	void OnLearnCitta(GObject::Player *, std::vector<std::string>&);
	void OnUpCitta(GObject::Player *, std::vector<std::string>&);
	void OnOffCitta(GObject::Player *, std::vector<std::string>&);
    void OnUpPasSkill(GObject::Player *, std::vector<std::string>&);
    void OnOffPasSkill(GObject::Player *, std::vector<std::string>&);
    void OnUpPeerless(GObject::Player *, std::vector<std::string>&);
    void OnOffPeerless(GObject::Player *, std::vector<std::string>&);
    void OnSetLevel(GObject::Player *, std::vector<std::string>&);
	void OnAttack(GObject::Player *, std::vector<std::string>&);
	void OnClanBox(GObject::Player * player, std::vector<std::string>&);
	void OnsetWeekDay(std::vector<std::string>&);
	void OnForge(GObject::Player *, std::vector<std::string>&);
	void OnGem(GObject::Player *, std::vector<std::string>&);
	void OnEnchant(GObject::Player *, std::vector<std::string>&);
	void OnResetIc(GObject::Player *, std::vector<std::string>&);
	void OnAutoCB(GObject::Player *, std::vector<std::string>&);
	void OnAutoFB(GObject::Player *, std::vector<std::string>&);
	void OnEnterArena(GObject::Player *, std::vector<std::string>&);
	void OnNextArena(GObject::Player *, std::vector<std::string>&);
	void OnEnterClan(GObject::Player *, std::vector<std::string>&);
    void OnPay4Pra(GObject::Player * player, std::vector<std::string>&);
    void OnSitPra(GObject::Player * player, std::vector<std::string>&);
    void OnFlushTask(GObject::Player * player, std::vector<std::string>&);
    void OnSetCountry(GObject::Player * player, std::vector<std::string>&);
    void OnAddPExp(GObject::Player * player, std::vector<std::string>&);
    void OnSetAcu(GObject::Player * player, std::vector<std::string>&);
    void OnUseItem(GObject::Player * player, std::vector<std::string>&);
    void OnUseItemOther(GObject::Player * player, std::vector<std::string>&);
	void OnOcupyPlace(GObject::Player *, std::vector<std::string>&);
	void OnEnterCopy(GObject::Player *, std::vector<std::string>&);
    void OnWorldAnnounce(GObject::Player *, std::vector<std::string>&);
    void OnGmCheck(GObject::Player *player, std::vector<std::string>&);
    void OnMoney2All(GObject::Player *player, std::vector<std::string>&);
    void OnKick(GObject::Player *player, std::vector<std::string>&);
    void OnCount(GObject::Player *player, std::vector<std::string>&);
    void OnThreadId(GObject::Player *player, std::vector<std::string>&);
    void OnAutoCopy(GObject::Player *player, std::vector<std::string>&);
    void OnLock(GObject::Player *player, std::vector<std::string>&);
    void OnUnLock(GObject::Player *player, std::vector<std::string>&);
    void OnBigLock(GObject::Player *player, std::vector<std::string>&);
    void OnBigUnLock(GObject::Player *player, std::vector<std::string>&);
    void OnForbidSale(GObject::Player *player, std::vector<std::string>&);
    void OnUnForbidSale(GObject::Player *player, std::vector<std::string>&);
    void OnSetLoginLimit(GObject::Player *player, std::vector<std::string>&);
    void OnSysUpdate(GObject::Player *player, std::vector<std::string>&);

    void OnSetBosslevel(GObject::Player *player, std::vector<std::string>&);
    void OnQQVipKey(GObject::Player *player, std::vector<std::string>&);
    void OnClanBuild(GObject::Player *player, std::vector<std::string>&);
    void OnClanItem(GObject::Player *player, std::vector<std::string>&);
    void OnClanBattleScore(GObject::Player *player, std::vector<std::string>&);
    void OnInfoHI(GObject::Player *player, std::vector<std::string>&);
    void OnEnterHI(GObject::Player *player, std::vector<std::string>&);
    void OnLeaveHI(GObject::Player *player, std::vector<std::string>&);
    void OnAttackHI(GObject::Player *player, std::vector<std::string>&);
    void OnMoveHI(GObject::Player *player, std::vector<std::string>&);
    void OnGetIDHI(GObject::Player *player, std::vector<std::string>&);
    void OnTaskHI(GObject::Player *player, std::vector<std::string>&);
    void OnCiTaskHI(GObject::Player *player, std::vector<std::string>&);
    void OnRestartHI(GObject::Player *player, std::vector<std::string>&);
    void OnAwardHI(GObject::Player *player, std::vector<std::string>&);
    void OnUseSkillHI(GObject::Player *player, std::vector<std::string>&);
    void OnAppearBoss(GObject::Player *player, std::vector<std::string>&);
    void OnResetTeamCopyPlayer(GObject::Player* player, std::vector<std::string>&);
    void OnMoneyIn(GObject::Player* player, std::vector<std::string>&);
    void OnNewYear(GObject::Player* player, std::vector<std::string>&);
    void OnFgtForge(GObject::Player* player, std::vector<std::string>&);
    void OnGetHeroMemoAward(GObject::Player* player, std::vector<std::string>&);
    void OnSysDailog(GObject::Player* player, std::vector<std::string>&);
    void OnRegenAll(GObject::Player* player, std::vector<std::string>& args);
    void OnSetBossHp(GObject::Player* player, std::vector<std::string>& args);
    void OnTime(GObject::Player* player, std::vector<std::string>& args);
    void OnToken(GObject::Player* player, std::vector<std::string>& args);
    void OnRecharge(GObject::Player* player, std::vector<std::string>& args);
    void OnBossHP(GObject::Player* player, std::vector<std::string>& args);
    void OnJson(GObject::Player* player, std::vector<std::string>& args);
    void OnRC7Awd(GObject::Player* player, std::vector<std::string>& args);
    void OnRC7TurnOn(GObject::Player* player, std::vector<std::string>& args);
    void OnAddVarS(GObject::Player* player, std::vector<std::string>& args);
    void OnLuckyDraw(GObject::Player* player, std::vector<std::string>& args);

    void OnHandleSignIn(GObject::Player* player, std::vector<std::string>& args);
    void OnNewRelation(GObject::Player* player, std::vector<std::string>& args);
    void OnSSOpen(GObject::Player* player, std::vector<std::string>& args);
    void OnSSUp(GObject::Player* player, std::vector<std::string>& args);
    void OnSSErase(GObject::Player* player, std::vector<std::string>& args);
    void OnSoSoGet(GObject::Player* player, std::vector<std::string>& args);

    void OnTj1(GObject::Player* player, std::vector<std::string>& args);
    void OnTj2(GObject::Player* player, std::vector<std::string>& args);
    void OnTj3(GObject::Player* player, std::vector<std::string>& args);
    void OnTj4(GObject::Player* player, std::vector<std::string>& args);
    void OnOpenTj(GObject::Player* player, std::vector<std::string>& args);
    void OnAddIdip(GObject::Player* player, std::vector<std::string>& args);
    void OnClearTask(GObject::Player* player, std::vector<std::string>& args);
    void OnClearCFT(GObject::Player* player, std::vector<std::string>& args);

    void OnStatueExp(GObject::Player* player, std::vector<std::string>& args);
    void OnSetClanCopyLevel(GObject::Player* player, std::vector<std::string>& args);
    void OnSetClanCopyTime(GObject::Player* player, std::vector<std::string>& arge);
    void OnEnterSingleHeroStage(GObject::Player* player, std::vector<std::string>& arge);
    void OnSingleHeroStageNextTime(GObject::Player* player, std::vector<std::string>& arge);
    void OnSingleHeroStageReset(GObject::Player* player, std::vector<std::string>& arge);
    void OnSingleHeroStageGoTo(GObject::Player* player, std::vector<std::string>& arge);
    void OnNewPlayerAuto(GObject::Player* player, std::vector<std::string>& arge);
    void OnNewPlayerAutoSuper(GObject::Player* player, std::vector<std::string>& arge);
    void OnShowBattlePoint(GObject::Player* player, std::vector<std::string>& arge);
    void OnIdipBuy(GObject::Player* player, std::vector<std::string>& arge);
    void OnStrengthen(GObject::Player *player, std::vector<std::string>& args);
    void OnNewCountryBattle(GObject::Player *player, std::vector<std::string>& args);
    void OnCFriend(GObject::Player *player, std::vector<std::string>& args);
    void OnShuoShuo(GObject::Player *player, std::vector<std::string>& args);
    void OnSaveGoldAct(GObject::Player *player, std::vector<std::string>& args);
    void OnLingbao(GObject::Player * player, std::vector<std::string>& args);
    void OnLingbaoSkill(GObject::Player * player, std::vector<std::string>& args);
    void OnLingbaos(GObject::Player * player, std::vector<std::string>& args);
    void testLingbao(GObject::Player * player, std::vector<std::string>& args);

    void OnDreamerTimeSet(GObject::Player *player, std::vector<std::string>& args);
    void OnDreamerKeySet(GObject::Player *player, std::vector<std::string>& args);
    void OnDreamerEyeSet(GObject::Player *player, std::vector<std::string>& args);
    void OnSomeAct(GObject::Player *player, std::vector<std::string>& args);
    void OnDragonKingAct(GObject::Player *player, std::vector<std::string>& args);
    void OnFairyPetGM(GObject::Player *player, std::vector<std::string>& args);
    void OnSurnameleg(GObject::Player *player, std::vector<std::string>& args);
    void OnFoolsDayGM(GObject::Player *player, std::vector<std::string>& args);
};

extern GMHandler gmHandler;

#endif // _GMHANDLER_H_
