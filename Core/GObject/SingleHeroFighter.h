#ifndef _SINGLEHEROFIGHTER_H_
#define _SINGLEHEROFIGHTER_H_

#include "Fighter.h"

namespace GObject
{

//#define SH_FIGHTER_STRATEGY_CD 900
#define SH_FIGHTER_STRATEGY_CD 60
#define SH_SY_SKILL_LEVEL      9

    class SingleHeroFighter
    {
    public:

        static GData::AttrExtra _attrExtraEquipTemplate[4];

        SingleHeroFighter(Player* pl, UInt8 type);
        SingleHeroFighter(Player* pl, Fighter* fgt, UInt8 type);
        SingleHeroFighter(Player* pl, UInt8 cls, UInt8 type);

        ~SingleHeroFighter();

        inline UInt8 getClass() { return _fgt == NULL ? 0 : _fgt->getClass(); }
        inline UInt16 getId() { return _fgt == NULL ? 0 : _fgt->getId(); }
        inline UInt16 getLevel() { return _fgt == NULL ? 0 : _fgt->getLevel(); }
        inline UInt32 getCurrentHP() { return _fgt == NULL ? 0 : _fgt->getCurrentHP(); }
        inline UInt32 getMaxHP() { return _fgt == NULL ? 0 : _fgt->getMaxHP();}
        inline void   setCurrentHP(UInt32 hp) 
        { 
            if (_fgt)
                _fgt->setCurrentHP(hp, false); 
        }


        void setAttrExtraEquip(GData::AttrExtra& attr);
        void setAttr2(UInt8 soulExtraAura, UInt8 soulAuraLeft, ElixirAttr& attr);

        void setPotential(float potential);
        void setCapacity(float capacity);
        void setLevel(UInt8 level);

        void setUpCittas(std::string& citta);
        void setUpTrumps(std::string& trump);
        void setUpSS(std::string& skillstrengthen);
        void setSoulMax(Int32 soulMax);


        UInt8 getCittasNum();
        UInt8 getTrumpsNum();

        bool offTrump(const std::vector<const GData::SkillBase*>& trumps);
        bool upTrump(const std::vector<const GData::SkillBase*>& trumps);
        bool upCitta( UInt16 citta, int idx );
        void saveStrategy(std::vector<UInt16>& trumps, std::vector<UInt16>& cittas);
        void showStrategy();

        void writeDB();
        void strTrump(std::string& str);
        void strSS(std::string& str);
        void strCitta(std::string& str);

        template <typename T>
        bool value2string(T* values, int size, std::string& str)
        {
            if (!values || !size)
                return true; // XXX: will be set to ''

            char buf[1024] = {0};
            char* pbuf = buf;
            char* pend = &buf[sizeof(buf)-1];
            for (int i = 0; i < size; ++i)
            {
                pbuf += snprintf(pbuf, pend - pbuf, "%u", values[i]);
                if (i < size - 1)
                    pbuf += snprintf(pbuf, pend - pbuf, ",");
            }

            if (pbuf != buf)
                str = buf;
            return true;
        }

        inline Fighter* getFighter() { return _fgt; }
        inline void setFighter(Fighter* fgt) { _fgt = fgt; }
        inline void setName(const std::string& name) { _fgt->setName(name); }

        inline UInt16 getTLevel() const { return _tLevel; }
        inline UInt32 getTTurns() const { return _tTurns; }
        inline UInt32 getTLastTurns() const { return _tLastTurns; }

        inline void setTLevel(UInt16 level) { _tLevel = level; }
        inline void setTTurns(UInt32 turns) { _tTurns = turns; }
        inline void setTLastTurns(UInt32 lastTurns) { _tLastTurns = lastTurns; }

        inline void stepTLevel() { ++_tLevel; }
        inline void addTTurns(UInt32 turns) { _tTurns += turns; }
        void saveTowerProgress();

        inline Player* getPlayer() const { return _player; }
        bool checkCittas(std::vector<UInt16>& cittas);
        bool checkTrumps(std::vector<UInt16>& trumps);

    private:
        UInt32 _cd;

        Player* _player;
        Fighter* _fgt;

        UInt8 _type;
        UInt8 _level;
        Int32 _soulMax;

        float _potential;
        float _capacity;
        UInt16 _citta[CITTA_UPMAX];
        UInt16 _trump[TRUMP_UPMAX];

        std::map<UInt16, UInt16> _ss;

        GData::AttrExtra _attrExtraEquip;
        ElixirAttr _elixirattr;
        UInt8 _soulExtraAura;
        UInt8 _soulAuraLeft;

        UInt16 _tLevel;
        UInt32 _tTurns;
        UInt32 _tLastTurns;
    };
}

#endif

