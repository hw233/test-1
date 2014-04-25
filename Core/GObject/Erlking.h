#ifndef _ERLKING_H_
#define _ERLKING_H_
#include "Config.h"
#include "Common/Stream.h"

namespace GObject
{
    #define ERLKING_MAX_COPY_NUM   40

    enum
    {
        ERLKING_MARKA = 0,   // 未解锁未通关标志 
        ERLKING_MARKB,       // 已解锁未通关标志
        ERLKING_MARKC,       // 已解锁已通关标志
    };

    class Player;

    class Erlking
    {
    public:
        Erlking(Player* pl);

    private:
        UInt8 GetsurplusNum();

        UInt8 GetErlkingStatus(UInt8 copyId);

        void SetsurplusNum(UInt8 num = 1);

        void SetErlkingStatus(UInt8 copyId, UInt8 type=0);

        void SendErlkingInfo(Stream& st);

    public:
        void ErlkingInfo();

        void BuyPassNum();

        void StartBattle(UInt8 copyId);

        void AutoBattle(UInt8 copyId, UInt8 num);

        Player* m_owner;
    };
}

#endif
