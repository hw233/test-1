#ifndef _NEWQUESTIONSTABLE_H_
#define _NEWQUESTIONSTABLE_H_

#include <map>
#include "Config.h"
#include "GDataDBExecHelper.h"

namespace GData
{

class NewQuestionsData
{
    public:
        struct newquestionsInfo
        {
            UInt16 id;       // 问题Id
            UInt8 lvl;       // 问题星级
            UInt8 answer;    // 答案

            newquestionsInfo(): id(0), lvl(0), answer(0) {}
        };

    private:
        std::map<UInt16, newquestionsInfo> _newquestionsInfo;

    public:
        void setNewQuestionsInfo(DBNewQuestionsConfig &);

        newquestionsInfo * getNewQuestionsInfo(UInt16 id);
};

extern NewQuestionsData newquestionsData;

}

#endif
