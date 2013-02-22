#ifndef LB_NAME_TMPL_H
#define LB_NAME_TMPL_H

#include "Script/Script.h"

namespace GObject
{
#define MAX_SKILL_ID 133
#define MAX_LEVELS   4
#define MAX_COLORS   4
#define MAX_LB_TYPES 3
#define MAX_ATTR_ID  12

#define LEVEL_UNIT   10

#define MIN_LEVEL    70
#define MIN_COLOR    2

    class ItemLingbao;

    class LBNameTmpl
    {
    public:
        LBNameTmpl();
        void load(const char * = NULL);
        inline void setFilename(const char * fn) {_filename = fn;}

        void setSkillPrevfix(UInt16 skillId, const char* str);
        void setSkillSuffix(UInt16 skillId, const char* str);
        void setColorPrefix(UInt8 lv, UInt8 color, UInt8 lbType, const char* str);
        void setAttrPrefix(UInt8 lv, UInt8 attrId, const char* str);
        void setLBName(UInt8 lv, UInt8 lbType, const char* str);

        void getLBName(std::string& name, ItemLingbao* lb);
    private:
        std::string _filename;

        std::string _skillPrefix[MAX_SKILL_ID+1];
        std::string _skillSuffix[MAX_SKILL_ID+1];

        std::string _colorPrefix[MAX_LEVELS][MAX_COLORS][MAX_LB_TYPES];
        std::string _attrPrefix[MAX_LEVELS][MAX_ATTR_ID+1];
        std::string _lbName[MAX_LEVELS][MAX_LB_TYPES];
    };

    extern LBNameTmpl lbnameTmpl;
}


namespace Script
{
    class LBNameTmplScript:
        public Script
    {
    public:
        LBNameTmplScript(GObject::LBNameTmpl* lbnametmpl);
    };

}

#endif  //LB_NAME_TMPL_H
