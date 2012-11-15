#ifndef DAYSRANK_H_
#define DAYSRANK_H_

#include <map>
#include <list>
#include <string>
#include <string.h>
#include "Common/Platform.h"
#include "Common/Mutex.h"
#include "MsgHandler/CountryMsgStruct.h"

#define DAYS_RANK_TYPE  5
#define DAYS_RANK_COUNT 10
using namespace std;

namespace GObject
{
    class Player;

    struct PlayerRankInfo
    {
        //0:斗剑 1:等级 2:财富 3:PK(荣誉和声望) 4:战斗力
        int rank[DAYS_RANK_TYPE][DAYS_RANK_COUNT];
        PlayerRankInfo()
        {
            clear();
        }

        std::string GetNextSection(std::string& strString , char cSeperator)
        {
            std::string strRet;
            int nIndex=(int)strString.find(cSeperator);
            if(nIndex>=0)
            {
                strRet=strString.substr(0,nIndex);//Section
                strString=strString.substr(nIndex+1);
            }
            else
            {
                strRet=strString;
                strString="";
            }
            return strRet;
        }
        void clear()
        {
            memset(this, 0, sizeof(PlayerRankInfo));
        }
        void push_back(int value, UInt8 type = 0)
        {
            if (type >= 5)
                return;
            int* p = rank[type];
            memcpy(&p[0], &p[1], sizeof(int)*(DAYS_RANK_COUNT-1));
            p[DAYS_RANK_COUNT-1] = value;
        }
        string toString(UInt8 type = 0)
        {
            string str;
            if (type >= 5)
                return str;
            int* p = rank[type];
            for (int i = 0; i < DAYS_RANK_COUNT; ++i)
            {
                char s[10] = {0};
                if (i == 0 )
                    sprintf(s, "%d", p[i]);
                else
                    sprintf(s, ",%d", p[i]);
                str += s;
            }
            return str;
        }
        void parseFromDB(std::string& str, UInt8 type = 0)
        {
            if (type >= 5)
                return;
            int i = 0;
            std::string s = GetNextSection(str, ',');
            while (!s.empty() && i < DAYS_RANK_COUNT)
            {
                 rank[type][i++] =  atoi(s.c_str());
                 s = GetNextSection(str, ','); 
            }
        }
    };

    class DaysRank 
    {
    public:
		static DaysRank& instance()
		{
		    static DaysRank s;
			return s;
		}
    public:
        DaysRank();
        void Init();
        void LoadFromDB();
        void process(bool toDB = true);
        void updateDaysValue(daysValueRankMsg * msg);
        PlayerRankInfo* getDaysRank(UInt64 playerId);
//    GObject::DaysRank::instance().getDaysRank(playerId);

    private:
        std::map<UInt64, PlayerRankInfo> m_playerDaysRank;
        FastMutex _opMutex;
    };
}

#endif

