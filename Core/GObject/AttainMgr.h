#ifndef _ATTAINMGR_H_
#define _ATTAINMGR_H_

#include <string>
#include <vector>


namespace GObject
{

#define _ATTAINED_FINISHED_ static_cast<UInt32>(0xFFFFFFFF)
#define _ATTAINED_          static_cast<UInt32>(0xFFFFFFFE)

    class Player;

    struct AttainData
    {
        UInt64 ownerId;
        UInt16 attainId;
        UInt32 status;
        UInt32 updatetime;
    };

    class AttainMgr
    {
        typedef std::map<UInt32, AttainData*> AttainList;
        typedef AttainList::const_iterator attain_c_iterator;
        typedef AttainList::iterator attain_iterator;

    public:
        AttainMgr(Player* pl);
        ~AttainMgr();

    public:
        void LoadAttain(AttainData* attainData);

    public:
        bool HasFinishAttainment(UInt16 attainId);
        bool HasAttained(UInt16 attainId);
        bool CanAttain(UInt16 attainId);
        void UpdateAttainment(UInt16 attainId, UInt32 status);
        UInt32 getAttainStatus(UInt16 attainId);
        void MidnightCheckAttain();

    private:
        Player* m_PlayerOwner;
        AttainList m_AttainList;
    };
}

#endif
