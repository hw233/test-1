#ifndef _RACEBATTLE_H_
#define _RACEBATTLE_H_

namespace GObject
{
#define RACEBATTLE_LOCATION 0xFFFF
    class RaceBattle
    {
        public:
            RaceBattle();
            ~RaceBattle();
            void RaceBattleCheck(UInt32 time);
            UInt16 getLocation() { return RACEBATTLE_LOCATION; }
            void enterPos(Player* pl, UInt8 pos);
            void autoBattle(Player* pl);
            void cancelBattle(Player* pl);
            void freshContinueWinRank(Player* pl);
            void getAward(Player* pl);
            void readBattleReport(Player* pl, UInt32 reportId);
            void requestMatch(Player* pl);
        private:
            UInt8 _status; //0（未开启），1（17：50~20：00），2（20：00~20：30）
    };

    extern RaceBattle* gRaceBattle;

} //namespace GObject

#endif

