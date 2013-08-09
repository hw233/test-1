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
    class JiguanData;

    struct Jiguanshu
    {
        Jiguanshu() : curLvl(0), curExp(0){}

        UInt32 curLvl;      // 机关术当前等级
        UInt32 curExp;      // 机关术当前经验
    };

    class MoFang
    {
    public:
        MoFang(Player* pl);

        void Init();
        void makejiguan(UInt32 itemId, UInt8 type, UInt8 mark);   // 制造机关
        void upgradeJGS();                                        // 机关术升级
        void equipJG(UInt32 jgId, UInt8 pos, UInt8 mark);         // 装备机关
        void dismantleJG(UInt8 pos, UInt8 mark);                  // 拆除机关
        bool checkEquip(UInt32 jgId);                             // 检测是否能够装备
        void addJGYAttr(GData::AttrExtra& ae);                    // 添加机关玉属性
        UInt16 useJGQskill();                                     // 使用机关器技能
        void sendMoFangInfo(UInt8 mark = 0 /*0:获取墨方信息；1：制作机关玉；2：装备机关；3：拆卸机关 */);
    private:

        map<UInt32 jgId, UInt8 pos> m_equipJG;              /*pos:1~49：表示装备在相应的格子*/

        map<UInt32 tuzhiId, UInt32 curProficient> m_tuzhi;  //记录图纸所对应的熟练度

        vector<UInt32 jgId> m_jg;                           //记录未装备机关

        UInt16 m_grids[49];                                 // 记录网格占用情况

        Jiguanshu m_jiguanshu;

        Player* m_owner;
    }；
}

#endif
