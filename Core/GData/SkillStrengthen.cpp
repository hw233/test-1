#include "SkillStrengthen.h"


namespace GData
{

    const SkillStrengthenEffect* SkillStrengthenBase::getEffect(UInt16 cond, Int16 type)
    {
        int cnt = effect.size();
        for(int i = 0; i < cnt; ++ i)
        {
            if(effect[i]->cond == cond && effect[i]->type == type)
                return effect[i];
        }

        return NULL;
    }

}

