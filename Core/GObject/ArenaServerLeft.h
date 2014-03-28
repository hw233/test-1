#ifndef _ARENASERVERLEFT_H_
#define _ARENASERVERLEFT_H_
#include "Common/Stream.h"
#include "GObjectManager.h"
#include "Common/BinaryReader.h"
#include "Common/TimeUtil.h"
#include "GObject/Player.h"
#include "GObject/World.h"
namespace GObject
{
#define SERVERLEFT_MAX_MEMBER 5
    struct SLPlayerData
    {
        SLPlayerData()(Player * pl = NULL ,UInt32 bp = NULL):player(pl),battlePoint(bp){}
        UInt64 player[SERVERLEFT_MAX_MEMBER];
        UInt32 battlePoint[SERVERLEFT_MAX_MEMBER];
    };
    struct PairPlayerReport
    {
        PairPlayerReport() : won(0), battleId(0) {}
        //PlayerInfo pInfo1;
        //PlayerInfo pInfo2;
        UInt8 won;
        UInt32 battleId;
        std::string name1;
        std::string name2;

        inline void readReport(BinaryReader& brd)
        {
            //pInfo1.readInfo(brd);
            //pInfo2.readInfo(brd);
            brd >> won >> battleId;
            brd >> name1 >> name2;
        }
        inline void appendReport(Stream& st)
        {
            st << won << battleId;
            st << name1 << name2;
            //st << pInfo1.name << pInfo1.mainFid << pInfo1.battlePoint;
            //st << pInfo2.name << pInfo2.mainFid << pInfo2.battlePoint;
        }
    };
    struct PairServerReport
    {
        PairServerReport() : channelId(0), serverId(0), otherChannelId(0), otherServerId(0), won(0xFF) {}
        int channelId;
        int serverId;
        int clanId ;
        int otherChannelId;
        int otherServerId;
        int clanOtherId ;
        std::string serverName;
        std::string clanName ; 
        std::string otherServerName;
        std::string otherClanName ;
        UInt8 won;  //1:serverId胜 0:otherServerId胜
        std::vector<PairPlayerReport> ppReportVec;

        inline void readServerReport(BinaryReader& brd)
        {
            UInt8 tmpWin = 0;
            brd >> tmpWin;
            if(tmpWin == 0)
                won = 1;
            else
                won = 0;
            brd >> channelId >> serverId >> clanId ;
            brd >>otherChannelId >> otherServerId >>clanOtherId;
            brd >> serverName >> otherServerName;
            brd >> clanName >> otherClanName;
            UInt8 size = 0;
            brd >> size;
            for(UInt8 i = 0; i < size; ++ i)
            {
                PairPlayerReport ppr;
                ppr.readReport(brd);
                ppReportVec.push_back(ppr);
            }
        }
        inline void appendServerReport(Stream& st)
        {
            st << serverId << serverName << otherServerId << otherServerName;
            UInt8 size = ppReportVec.size();
            st << won << size;
            for(UInt8 i = 0; i < size; ++ i)
            {
                ppReportVec[i].appendReport(st);
            }
        }
    };
    struct SLClanInfo
    {
        SLClanInfo(){} 
        std::map<UInt8 , SLPlayerData >  _occupyLeft;
        std::vector<PairServerReport > _pairServerVec;
    };
    class ServerLeftMgr
    {
    public :
        ServerLeftMgr():_loaded(false){}
	    inline bool active() { return _loaded  }
        inline UInt16 getSession() { return _session; }
    };
}

#endif // _ARENASERVERLEFT_H_

