
#ifndef GDATAMANAGER_H_
#define GDATAMANAGER_H_
namespace GObject
{

}
namespace GData
{
    class GDataManager
    {
        public:
            static bool LoadAllData();

        public:
            static bool LoadExpData(); 
            static bool LoadItemTypeData();
            static bool LoadSkillConditionData();
            static bool LoadSkillScopeData();
            static bool LoadSkillEffectData();
            static bool LoadFighterBase();
            static bool LoadSkillData();
            static bool LoadSkillBuff();
            static bool LoadMapInfo();
        private:

    };
}
#endif // GDATAMANAGER_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

