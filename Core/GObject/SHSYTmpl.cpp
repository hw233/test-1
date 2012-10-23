#include "Config.h"
#include "SHSYTmpl.h"

#ifndef _WIN32
#include <sys/ioctl.h> // for ioctl
#include <net/if.h> // for struct ifreq, IF_NAMESIZE
#endif

namespace GObject
{
    SHSYTmpl::SHSYTmpl() : _level(0), _potential(0), _soulMax(0), _filename("scripts/shsytmpl.lua")
    {
    }

    void SHSYTmpl::load(const char * scriptPath)
    {
        Script::SHSYTmplScript script(this);
        if(scriptPath != NULL)
        {
            script.runScript(scriptPath);
            return;
        }
        script.doFile(_filename.c_str());
    }

    SHSYTmpl shsyTmpl;
}


namespace Script
{
    SHSYTmplScript::SHSYTmplScript( GObject::SHSYTmpl * shsytmpl )
    {
        class_add<GObject::SHSYTmpl>("SHSYTmpl");
        class_def<GObject::SHSYTmpl>("setStrength", &GObject::SHSYTmpl::setStrength);
        class_def<GObject::SHSYTmpl>("setPhysique", &GObject::SHSYTmpl::setPhysique);
        class_def<GObject::SHSYTmpl>("setAgility", &GObject::SHSYTmpl::setAgility);
        class_def<GObject::SHSYTmpl>("setIntelligence", &GObject::SHSYTmpl::setIntelligence);
        class_def<GObject::SHSYTmpl>("setWill", &GObject::SHSYTmpl::setWill);
        class_def<GObject::SHSYTmpl>("setSoul", &GObject::SHSYTmpl::setSoul);
        class_def<GObject::SHSYTmpl>("setAura", &GObject::SHSYTmpl::setAura);
        class_def<GObject::SHSYTmpl>("setAuraMax", &GObject::SHSYTmpl::setAuraMax);
        class_def<GObject::SHSYTmpl>("setAttack", &GObject::SHSYTmpl::setAttack);
        class_def<GObject::SHSYTmpl>("setMagAtk", &GObject::SHSYTmpl::setMagAtk);
        class_def<GObject::SHSYTmpl>("setDefend", &GObject::SHSYTmpl::setDefend);
        class_def<GObject::SHSYTmpl>("setMagdef", &GObject::SHSYTmpl::setMagdef);
        class_def<GObject::SHSYTmpl>("setHP", &GObject::SHSYTmpl::setHP);
        class_def<GObject::SHSYTmpl>("setTough", &GObject::SHSYTmpl::setTough);
        class_def<GObject::SHSYTmpl>("setAction", &GObject::SHSYTmpl::setAction);
        class_def<GObject::SHSYTmpl>("setHitrate", &GObject::SHSYTmpl::setHitrate);
        class_def<GObject::SHSYTmpl>("setEvade", &GObject::SHSYTmpl::setEvade);
        class_def<GObject::SHSYTmpl>("setCritical", &GObject::SHSYTmpl::setCritical);
        class_def<GObject::SHSYTmpl>("setCriticalDmg", &GObject::SHSYTmpl::setCriticalDmg);
        class_def<GObject::SHSYTmpl>("setPierce", &GObject::SHSYTmpl::setPierce);
        class_def<GObject::SHSYTmpl>("setCounter", &GObject::SHSYTmpl::setCounter);
        class_def<GObject::SHSYTmpl>("setMagRes", &GObject::SHSYTmpl::setMagRes);
        class_def<GObject::SHSYTmpl>("setLevel", &GObject::SHSYTmpl::setLevel);
        class_def<GObject::SHSYTmpl>("setPotential", &GObject::SHSYTmpl::setPotential);
        class_def<GObject::SHSYTmpl>("setSoulMax", &GObject::SHSYTmpl::setSoulMax);
        set("shsytmpl", shsytmpl);
    }
}

