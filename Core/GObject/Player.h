
#ifndef PLAYER_H_
#define PLAYER_H_
#include "GObjectManager.h"
#include "Common/Stream.h"
#include "Common/TimeUtil.h"
#include "Common/MCached.h"
#include "Var.h"
#include "GVar.h" 
#include "Script/lua_tinker.h"
#include "kingnet_analyzer.h"
#include "Friend.h"
#include "Package.h"
#include "Server/WorldServer.h"
//#include "Player.h"
#include "Battle/BattleSimulator.h"

//VAR
//Package
//Friend
//SelfInfo
//FriendManager
//Session
//thread
//Online
//DCLoger
//serverNo
//log 
//Fighter
//BattleSimulator
//Function

namespace Battle
{
    class BattleSimulator;
}

namespace GObject
{
    struct lineup
    {
        lineup(): fid(0), pos(0) {} //, fighter(NULL) {}
        inline bool available() { return fid != 0; } 
        void updateId();
        UInt32 fid; 
        UInt8 pos; 
        //Fighter * fighter;
    };

    struct PlayerData
    {
        std::string name;
        UInt16 location;
        UInt32 packSize;  //背包空间
        lineup lineip[5];
    };
    class FriendManager ;
    class Fighter;
    class Player //:
        //public GObjectBaseT<Player,std::string>
    {
        public:
            Player();
            ~Player();
            //VAR
            VarSystem * getVarSystem();
            UInt32 GetVar(UInt32 id);
            Int32 GetVarS(Int32 id);
            void LoadVar(UInt32 id, UInt32 val, UInt32 overTime);
            void SetVar(UInt32 id, UInt32 val);
            void DelVar(UInt32 id);
            void AddVar(UInt32 id, UInt32 val);
            void AddVarS(UInt32 id, Int32 val);
            UInt32 GetVarNow(UInt32 id, UInt32 now);
            void SetVarNow(UInt32 id,  UInt32 val, UInt32 now);
            void AddVarNow(UInt32 id , UInt32 val, UInt32 now);
            void SetVarOffset(UInt32 offset);

            //Package
            Package * GetPackage() { return m_Package; } 

            //Friend
            bool _hasFriend( UInt8 type, Player * pl ) const;
            Player * _findFriend( UInt8 type, std::string& name );
            inline bool _hasFriend(Player *pl) const { return _hasFriend(0, pl); }

            //SelfInfo
            void GetSelfInfoStream(Stream &st);

            //FriendManager
            FriendManager * GetFriendManager(){ return m_friendMgr;}

            //Session
            inline void SetSessionID(int session) { _session = session; }
            inline int GetSessionID() const { return _session; }
            void send( const void * buf, int size);
            inline void send(Stream& st)
            {send(&st[0], st.size());}

            //thread
            UInt8 getThreadId(){ return 1;}

            template <typename MsgType>
                inline void send(MsgType& msg)
                { if(_isOnline) NETWORK()->SendMsgToClient(_session, msg);  }

            //Online
            inline bool isOnline() const { return _isOnline; }  
            inline void setOnline(bool o) { _isOnline = o; }


            //DCLoger
            UInt32 getClientAddress();
            inline const char* getDomain() const { return m_domain; } 
            inline const char* getOpenId() const { return m_openid; }
            inline const char* getOpenKey() const { return m_openkey; }
            inline const char* getClientIp() const { return m_clientIp; } 
            void setOpenId(const std::string& openid, bool load = false);
            inline void setOpenKey(const std::string& openkey) { strncpy(m_openkey, openkey.c_str(), 256); m_openkey[256] = '\0';} 
            inline void setClientIp(const std::string& clientIp) 
            {                                           
                if (inet_addr(clientIp.c_str()) == INADDR_NONE)
                {
                    strncpy(m_clientIp, "0.0.0.0", 16);
                }
                else
                {   
                    strncpy(m_clientIp, clientIp.c_str(), 256);
                    m_clientIp[255] = '\0';
                }
            }

            //serverNo
            inline UInt16 getServerNo() { return 0 ;/*static_cast<UInt16>(getId() >> 48);*/ }

            //log 
            void udpLog(const char* str1, const char* str2, const char* str3, const char* str4, 
                    const char* str5, const char* str6, const char* type, UInt32 count = 1);
            void udpLog(UInt8 platform, const char* str1, const char* str2, const char* str3, const char* str4,
                    const char* str5, const char* str6, const char* type, UInt32 count = 1);
            void udpLog(UInt32 type, UInt32 id, UInt32 num, UInt32 price, const char* op); 

            //
            inline const std::string& getName(){ return _playerData.name; }
            
            //Fighter
            void addFighter(Fighter * fgt, bool = true, bool = false); 

            //BattleSimulator
            void PutFighters( Battle::BattleSimulator& bsim, int side, bool fullhp ,UInt16 fighterId);

            //Function
            void Login();

            //login
            inline void setId(std::string id){ _id = id;}
            inline std::string getId(){ return _id;}
            inline void setAccounts(std::string accounts) { _accounts = accounts;}
            inline std::string getAccounts(){return _accounts;}
            inline void setPassword(std::string password) {_password = password;}

        private:
            std::string _id;
            std::string _accounts;
            std::string _password;
            bool _isOnline;  
            AtomicVal<int> _session;
            char m_domain[256+1]; 
            char m_openid[256+1];
            char m_openkey[256+1];
            char m_clientIp[256+1];


            //功能类
            VarSystem* m_pVars;
            Package* m_Package;
            FriendManager * m_friendMgr;
            PlayerData _playerData;
    };
    typedef GGlobalObjectManagerT<Player, UInt64> GlobalPlayers;

    typedef GGlobalObjectManagerIStringT<Player> GlobalNamedPlayers;  
    extern GlobalNamedPlayers globalPlayers;
    extern GlobalNamedPlayers globalOnlinePlayers;
    extern GlobalNamedPlayers globalNamedPlayers;
    extern GlobalNamedPlayers globalAccountsPlayers;
}
#endif // PLAYER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */
