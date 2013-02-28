#include "Config.h"
#include "LBNameTmpl.h"
#include "Item.h"
#include "GObjectManager.h"

#ifndef _WIN32
#include <sys/ioctl.h> // for ioctl
#include <net/if.h> // for struct ifreq, IF_NAMESIZE
#endif

namespace GObject
{
    LBNameTmpl::LBNameTmpl() : _filename("scripts/lbnametmpl.lua")
    {
    }

    void LBNameTmpl::load(const char * scriptPath)
    {
        Script::LBNameTmplScript script(this);
        if(scriptPath != NULL)
        {
            script.runScript(scriptPath);
            return;
        }
        script.doFile(_filename.c_str());
    }


    void LBNameTmpl::setSkillPrevfix(UInt16 skillId, const char* str)
    {
        if(skillId > MAX_SKILL_ID)
            return;
        _skillPrefix[skillId] = str;
    }

    void LBNameTmpl::setSkillSuffix(UInt16 skillId, const char* str)
    {
        if(skillId > MAX_SKILL_ID)
            return;
        _skillSuffix[skillId] = str;
    }

    void LBNameTmpl::setColorPrefix(UInt8 lv, UInt8 color, UInt8 lbType, const char* str)
    {
        UInt8 lvIdx = (lv - MIN_LEVEL)/LEVEL_UNIT;
        UInt8 colorIdx = (color - 1);
        UInt8 lbIdx = lbType - 1;

        if(lvIdx > MAX_LEVELS || colorIdx > MAX_COLORS || lbIdx > MAX_LB_TYPES)
            return;
        _colorPrefix[lvIdx][colorIdx][lbIdx] = str;
    }

    void LBNameTmpl::setAttrPrefix(UInt8 lv, UInt8 attrId, const char* str)
    {
        UInt8 lvIdx = (lv - MIN_LEVEL)/LEVEL_UNIT;
        if(lvIdx > MAX_LEVELS || attrId > MAX_ATTR_ID)
            return;

        _attrPrefix[lvIdx][attrId] = str;
    }

    void LBNameTmpl::setLBName(UInt8 lv, UInt8 lbType, const char* str)
    {
        UInt8 lvIdx = (lv - MIN_LEVEL)/LEVEL_UNIT;
        UInt8 lbIdx = lbType - 1;

        if(lvIdx > MAX_LEVELS || lbIdx > MAX_LB_TYPES)
            return;
        _lbName[lvIdx][lbIdx] = str;
    }


    void LBNameTmpl::getLBName(std::string& name, ItemLingbao* lb)
    {
        if(!lb)
            return;

        ItemLingbaoAttr& lba = lb->getLingbaoAttr();
        UInt8 lv = lb->getReqLev();
        UInt8 lvIdx = (lv - MIN_LEVEL)/LEVEL_UNIT;
        UInt8 lbIdx = lb->GetItemType().subClass - Item_LBling;
        if(lvIdx > MAX_LEVELS || lbIdx > MAX_LB_TYPES)
            return;

        std::string prefix("");
        std::string suffix("");


        if(lba.tongling == 0)
        {
            UInt8 colorIdx = lba.lbColor - MIN_COLOR;
            if(colorIdx > MAX_COLORS)
                colorIdx = 0;
            prefix = _colorPrefix[lvIdx][colorIdx][lbIdx];
            suffix = _lbName[lvIdx][lbIdx];
        }
        else
        {
            if(lba.skill[0] != 0 && lba.skill[0] <= MAX_SKILL_ID)
            {
                prefix += _skillPrefix[lba.skill[0]];
                suffix += _skillSuffix[lba.skill[0]];
            }

            if(lba.skill[1] != 0 && lba.skill[1] <= MAX_SKILL_ID)
            {
                prefix += _skillPrefix[lba.skill[1]];
                suffix += _skillSuffix[lba.skill[1]];
            }

            if(prefix.empty())
            {
                stLBAttrConf& lbAttrConf = GObjectManager::getLBAttrConf();
                float maxValueP = 0;
                UInt8 attrId = 0;
                for(int i = 0; i < 4; ++ i)
                {
                    if(lba.type[i] == 0)
                        continue;
                    float maxVal = lbAttrConf.getAttrMax(lv, lbIdx, lba.type[i]-1);
                    float valueP = lba.value[i] / maxVal;
                    if(valueP > maxValueP)
                    {
                        maxValueP = valueP;
                        attrId = lba.type[i];
                    }
                }
                prefix = _attrPrefix[lvIdx][attrId];
            }

            if(suffix.empty())
                suffix = _lbName[lvIdx][lbIdx];
        }

        name = prefix + suffix;
    }


    LBNameTmpl lbnameTmpl;
}


namespace Script
{
    LBNameTmplScript::LBNameTmplScript( GObject::LBNameTmpl * lbnametmpl )
    {
        class_add<GObject::LBNameTmpl>("LBNameTmpl");
        class_def<GObject::LBNameTmpl>("setSkillPrevfix", &GObject::LBNameTmpl::setSkillPrevfix);
        class_def<GObject::LBNameTmpl>("setSkillSuffix", &GObject::LBNameTmpl::setSkillSuffix);
        class_def<GObject::LBNameTmpl>("setColorPrefix", &GObject::LBNameTmpl::setColorPrefix);
        class_def<GObject::LBNameTmpl>("setAttrPrefix", &GObject::LBNameTmpl::setAttrPrefix);
        class_def<GObject::LBNameTmpl>("setLBName", &GObject::LBNameTmpl::setLBName);
        set("lbnametmpl", lbnametmpl);
    }
}

