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
    enum
    {
        TUZHI_GREEN = 2,     //绿色
        TUZHI_BLUE,          //蓝色
        TUZHI_PURPLE,        //紫色
        TUZHI_YELLOW         //橙色
    };

    enum
    {
        NOOCCUPY_MOFANG = 0, // 没有占用墨方方格
        OCCUPY_MOFANG        // 占用墨方方格
    };

    enum
    {
        EQUIP_JG = 2,        // 装备机关
        DISMANT_JG = 3,      // 拆除机关
    };

    enum
    {
        JIGUANYU = 0,       // 机关玉
        JIGUANQI            // 机关器 
    };

    #define NORMAL_TUZHI   100000
    #define SPECIAL_TUZHI  200000
    #define JGS_MAXEXP     1313000

    class Player;
	class Fighter;
    class JiguanData;

    struct DBJiguanshu;
    struct DBJiguanyu;
    struct DBTuzhi;
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

        void AddJGSFromDB(DBJiguanshu &);
        void AddJGYFromDB(DBJiguanyu &);
        void AddTuzhiFromDB(DBTuzhi &);

        void randTuzhi(UInt8 num);
        void addJGSExp(UInt32 exp);
        UInt32 addTuzhi(UInt32 tuzhiId, bool mark = false);
        void makejiguan(UInt32 tuzhiId, UInt8 type, UInt8 mark);                        // 制造机关
        void equipJG(UInt32 jgId, UInt8 pos, UInt8 mark);                               // 装备机关
        bool checkPoint(UInt32 jgId, UInt8 pos, UInt8 mark, std::vector<UInt8> &);      // 检测是否能够装备
        void addJGYAttr(GData::AttrExtra& ae);                                          // 添加机关玉属性
        UInt16 useJGQskill();                                                           // 使用机关器技能
        void upgradeJGS();                                                              // 机关术升级
        void sendMoFangInfo(UInt8 mark = 0 /*0:获取墨方信息；1：制作机关玉；2：装备机关；3：拆卸机关 */);
        inline UInt32 findEquipJG(UInt8 pos);
        inline bool findNoEquipJG(UInt32 jgId);
    private:

        std::map<UInt8, UInt32> m_equipJG;  // 记录墨方坐标点所装备的机关

        std::map<UInt32, UInt8> m_tuzhi;   // 记录图纸所对应的熟练度

        std::vector<UInt32> m_jg;           // 记录未装备机关

        int m_grids[49];                 // 记录网格占用情况

        Jiguanshu m_jiguanshu;

        Player* m_owner;
    };
}

#endif
