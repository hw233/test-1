#include "Config.h"
#include "QixiTmpl.h"

#ifndef _WIN32
#include <sys/ioctl.h> // for ioctl
#include <net/if.h> // for struct ifreq, IF_NAMESIZE
#endif

namespace GObject
{
    QixiTmpl::QixiTmpl() : _filename("scripts/qixitmpl.lua")
    {
    }

    void QixiTmpl::load(const char * scriptPath)
    {
        Script::QixiTmplScript script(this);
        if(scriptPath != NULL)
        {
            script.runScript(scriptPath);
            return;
        }
        script.doFile(_filename.c_str());
    }


    void QixiTmpl::addScoreAwards(UInt32 score, UInt32 itemId, UInt32 itemCnt)
    {
        MailPackage::MailItem item;
        item.id = itemId;
        item.count = itemCnt;
        UInt32 size = _scoreAwards.size();
        bool find = false;
        for(size_t i = 0; i < size; ++ i)
        {
            if(score == _scoreAwards[i].score)
            {
                find = true;
                _scoreAwards[i].items.push_back(item);
                break;
            }
        }

        if(!find)
        {
            ScoreAward sa;
            sa.score = score;
            sa.items.push_back(item);
            _scoreAwards.push_back(sa);
        }
    }

    void QixiTmpl::addPartnerAwards(UInt32 itemId, UInt32 itemCnt)
    {
        MailPackage::MailItem item;
        item.id = itemId;
        item.count = itemCnt;
        _partnerAward.push_back(item);
    }

    void QixiTmpl::addLBAwards(UInt32 pos, UInt32 itemId, UInt32 itemCnt)
    {
        MailPackage::MailItem item;
        item.id = itemId;
        item.count = itemCnt;
        UInt32 size = _lbAwards.size();
        bool find = false;
        for(size_t i = 0; i < size; ++ i)
        {
            if(pos == _lbAwards[i].lbPos)
            {
                find = true;
                _lbAwards[i].items.push_back(item);
                break;
            }
        }

        if(!find)
        {
            LBAward lba;
            lba.lbPos = pos;
            lba.items.push_back(item);
            _lbAwards.push_back(lba);
        }
    }

    QixiTmpl qixiTmpl;
}


namespace Script
{
    QixiTmplScript::QixiTmplScript( GObject::QixiTmpl * qixitmpl )
    {
        class_add<GObject::QixiTmpl>("QixiTmpl");
        class_def<GObject::QixiTmpl>("setQixiItem", &GObject::QixiTmpl::setQixiItem);
        class_def<GObject::QixiTmpl>("setTitleItem", &GObject::QixiTmpl::setTitleItem);
        class_def<GObject::QixiTmpl>("setDivorceMsgCode", &GObject::QixiTmpl::setDivorceMsgCode);
        class_def<GObject::QixiTmpl>("setRoamingFunc", &GObject::QixiTmpl::setRoamingFunc);

        class_def<GObject::QixiTmpl>("setScoreTitle", &GObject::QixiTmpl::setScoreTitle);
        class_def<GObject::QixiTmpl>("setScoreContent", &GObject::QixiTmpl::setScoreContent);
        class_def<GObject::QixiTmpl>("setMinAwardScore", &GObject::QixiTmpl::setMinAwardScore);
        class_def<GObject::QixiTmpl>("addScoreAwards", &GObject::QixiTmpl::addScoreAwards);

        class_def<GObject::QixiTmpl>("setPartnerTitle", &GObject::QixiTmpl::setPartnerTitle);
        class_def<GObject::QixiTmpl>("setPartnerContent", &GObject::QixiTmpl::setPartnerContent);
        class_def<GObject::QixiTmpl>("addPartnerAwards", &GObject::QixiTmpl::addPartnerAwards);

        class_def<GObject::QixiTmpl>("setLBTitle", &GObject::QixiTmpl::setLBTitle);
        class_def<GObject::QixiTmpl>("setLBContent", &GObject::QixiTmpl::setLBContent);
        class_def<GObject::QixiTmpl>("addLBAwards", &GObject::QixiTmpl::addLBAwards);
        class_def<GObject::QixiTmpl>("setMaxLBAwardPos", &GObject::QixiTmpl::setMaxLBAwardPos);
        set("qixitmpl", qixitmpl);
    }
}

