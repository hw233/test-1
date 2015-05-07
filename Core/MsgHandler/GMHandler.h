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

public:
    GObject::Player* _printMsgPlayer;

private:

	void OnAddExp(GObject::Player *, std::vector<std::string>&);
	void OnAddVar(GObject::Player *, std::vector<std::string>&);
	void OnSetGVar(GObject::Player *, std::vector<std::string>&);
	void OnSendMail(GObject::Player *, std::vector<std::string>&);
    void OnAddFighter( GObject::Player * player, std::vector<std::string>& args );
    void OnAddMoney(GObject::Player *player,std::vector<std::string>&);

};

extern GMHandler gmHandler;

#endif // _GMHANDLER_H_
