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
	void OnLuckyDraw(GObject::Player *, std::vector<std::string>&);
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
	void OnSetBosslevel(GObject::Player * player, std::vector<std::string>& args);
	void OnClanBox(GObject::Player * player, std::vector<std::string>&);
	void OnsetWeekDay(std::vector<std::string>&);
	void OnForge(GObject::Player *, std::vector<std::string>&);
	void OnGem(GObject::Player *, std::vector<std::string>&);
	void OnEnchant(GObject::Player *, std::vector<std::string>&);
	void OnResetIc(GObject::Player *, std::vector<std::string>&);
	void OnAutoCB(GObject::Player *, std::vector<std::string>&);
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
	void OnOcupyPlace(GObject::Player *, std::vector<std::string>&);
	void OnEnterCopy(GObject::Player *, std::vector<std::string>&);
    void OnWorldAnnounce(GObject::Player *, std::vector<std::string>&);
    void OnGmCheck(GObject::Player *player, std::vector<std::string>&);
    void OnMoney2All(GObject::Player *player, std::vector<std::string>&);
    void OnKick(GObject::Player *player, std::vector<std::string>&);
    void OnCount(GObject::Player *player, std::vector<std::string>&);
    void OnThreadId(GObject::Player *player, std::vector<std::string>&);
};

extern GMHandler gmHandler;

#endif // _GMHANDLER_H_
