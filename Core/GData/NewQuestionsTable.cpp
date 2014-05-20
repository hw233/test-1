#include "NewQuestionsTable.h"

namespace GData
{
    NewQuestionsData newquestionsData;

    void NewQuestionsData::setNewQuestionsInfo(DBNewQuestionsConfig & data)
    {
        newquestionsInfo info;

        info.id = data.id;
        info.lvl = data.lvl;
        info.answer = data.answer;

        _newquestionsInfo.insert(std::make_pair(info.id, info));
    }

    NewQuestionsData::newquestionsInfo * NewQuestionsData::getNewQuestionsInfo(UInt16 id)
    {
        std::map<UInt16, newquestionsInfo>::iterator iter = _newquestionsInfo.find(id);
        if(iter != _newquestionsInfo.end())
            return &(iter->second);

        return NULL;
    }
}
