#ifndef _MOFANG_H_
#define _MOFANG_H_
#include "Config.h"
#include <vector>
#include <map>

namespace GData
{
    class AttrExtra;
}

namespace GObject
{
    class Player;
    class ItemJiguan;
    class MoFang
    {
    public:
        MoFang(Player* pl);
        void Init();
        void addLearnedPaper(UInt8 paperId);
        void addLearningPaper(UInt8 paperId, UInt8 value);
        void EquipJg(ItemJiguan* jg, UInt8 idx);
        void analysisItem(std::vector<UInt32>& items); // 钻研
        void makeJiguan(UInt8 paperId);                // 制作机关
        void learnPaper(UInt8 paperId, UInt8 value);   // 学习图纸进度
        void addAttr(GData::AttrExtra& ae);            // 属性
        UInt16 getSkill();                             // 技能
    private:
        std::vector<UInt8> m_papers_learned;      // 已学会<图纸id>
        std::map<UInt8, UInt8> m_papers_learning; // 未学会<图纸id, 学习进度>

        ItemJiguan* m_jg[2];                      // 机关装备位
        Player* m_owner;
    };
}

#endif
