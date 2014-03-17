#ifndef _MOFANG_H_
#define _MOFANG_H_
#include "Config.h"
#include <vector>
#include <map>
#include "Common/Stream.h"

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
        JIGUANYU = 1,       // 机关玉
        JIGUANQI            // 机关器 
    };

    enum
    {
        WIND_ELEMENT = 1,   // 风 
        FOREST_ELEMENT,     // 林
        FIRE_ELEMENT,       // 火
        MOUNTAIN_ELEMENT    // 山
    };

    #define NORMAL_TUZHI   100000
    #define SPECIAL_TUZHI  200000
    #define JGS_MAXEXP     1525500
    #define JGS_MAXLVL     60

    class Player;
	class Fighter;
    class JiguanData;

    struct DBJiguanshu;
    struct DBJiguanyu;
    struct DBTuzhi;
    struct DBKeyin;
    struct DBZhenwei;

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
        void AddKeyinFromDB(DBKeyin &);
        void AddZhenweiFromDB(DBZhenwei &);

        void randTuzhi(UInt16 num);
        void addJGSExp(UInt32 exp);
        UInt32 addTuzhi(UInt32 tuzhiId, bool mark = false);
        void makejiguan(UInt32 tuzhiId, UInt8 type, UInt8 mark);                        // 制造机关
        void equipJG(UInt32 jgId, UInt8 pos, UInt8 mark);                               // 装备机关
        bool checkPoint(UInt32 jgId, UInt8 pos, UInt8 mark, std::vector<UInt8> &);      // 检测是否能够装备
        void addJGYAttr(GData::AttrExtra& ae);                                          // 机关玉添加属性
        UInt16 useJGQskill();                                                           // 使用机关器技能
        void upgradeJGS();                                                              // 机关术升级
        void sendMoFangInfo(UInt8 mark = 0);                                            // 墨方信息
        void zhenweiInfo(Stream& st);                                                   // 震位信息
        void quickMakejiguan(UInt32 tuzhiId, UInt8 mark);                               // 一键制造机关玉
        void upgradeKY(UInt8 keyinId, UInt8 mark);                                      // 刻篆
        void quickUpgradeKY(UInt8 keyinId, UInt8 mark);                                 // 一键刻篆
        void changeMoney(UInt8 mark);                                                   // 壁凤墨转换为磐龙墨
        void addMoney(UInt32 toolId, UInt32 count);
        void addKYAttr(GData::AttrExtra& ae);                                           // 刻印添加属性
        void checkKey(UInt16 keyId, UInt8 mark);                                                     
        UInt32 yizhiyue(UInt16 keyId);                                                  // 义之鈅
        UInt32 renzhiyue(UInt16 keyId);                                                 // 仁之鈅
        inline bool findEquipJG(UInt32 jgId);
        inline bool findNoEquipJG(UInt32 jgId);
        inline bool findKey(UInt16 keyId);

        /**GM**/
        void addjiguanyu(UInt32 jiguanyuId);
    private:

        std::map<UInt32, UInt8> m_equipJG;  // 装备的机关

        std::map<UInt32, UInt8> m_tuzhi;    // 记录图纸所对应的熟练度

        std::map<UInt8, UInt8> m_kyLvl;     // 记录刻印当前等级

        std::map<UInt8, UInt32> m_kyCurExp; // 记录刻印当前经验

        std::map<UInt16, UInt8> m_zhenwei;  // 记录机关密钥所对应的状态

        std::vector<UInt32> m_jg;           // 记录未装备机关

        int m_grids[49];                    // 记录网格占用情况

        int m_kyQuality[4];                 // 刻印品质

        Jiguanshu m_jiguanshu;

        Player* m_owner;
    };
}

#endif
