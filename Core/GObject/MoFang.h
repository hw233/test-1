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
        GEAR_COMMON = 1,    // 普通机关库
        GEAR_SPECIAL        // 特殊机关库
    };

    enum
    {
        WIND_ELEMENT = 1,   // 风 
        FOREST_ELEMENT,     // 林
        FIRE_ELEMENT,       // 火
        MOUNTAIN_ELEMENT    // 山
    };

    //机关库套件统计
    enum
    {
        JIGUANKU_A = 401,
        JIGUANKU_B = 501,
        JIGUANKU_C = 601,
        JIGUANKU_D = 701,
        JIGUANKU_E = 801,
        JIGUANKU_F = 901,
        JIGUANKU_G = 1001,
        JIGUANKU_H = 1101,
        JIGUANKU_I = 1201,
        JIGUANKU_J = 1301,
        JIGUANKU_K = 1401
    };

    #define NORMAL_TUZHI        100000
    #define SPECIAL_TUZHI       200000
    #define JGS_MAXEXP          1525500
    #define JGS_MAXLVL          60
    #define EQUIP_SCHEME_MAX    3
    #define EQUIP_GRID_MAX      49

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
        void AddGearFromDB(UInt16 gearId, UInt8 mark);

        void randTuzhi(UInt16 num);
        void addJGSExp(UInt32 exp);
        UInt32 addTuzhi(UInt32 tuzhiId, bool mark = false);
        void makejiguan(UInt32 tuzhiId, UInt8 type, UInt8 mark);                                      // 制造机关
        void selectSchmem(UInt8 scheme);                                                              // 选择方案
        void equipJG(UInt32 jgId, UInt8 pos, UInt8 mark);                                             // 装备机关
        bool checkPoint(UInt32 jgId, UInt8 pos, UInt8 mark, UInt8 scheme, std::vector<UInt8> &);      // 检测是否能够装备
        void addJGYAttrFromSchmem(GData::AttrExtra& ae);                                // 机关玉根据方案添加属性
        void addJGYAttr(GData::AttrExtra& ae, UInt32 jgyId);                            // 机关玉添加属性
        //void useJGQskill();                                                           // 使用机关器技能
        void upgradeJGS();                                                              // 机关术升级
        void sendMoFangInfo(UInt8 mark = 0);                                            // 墨方信息
        void zhenweiInfo(Stream& st);                                                   // 震位信息
        void quickMakejiguan(UInt32 tuzhiId, UInt8 mark);                               // 一键制造机关玉
        void upgradeKY(UInt8 keyinId, UInt8 mark);                                      // 刻篆
        void quickUpgradeKY(UInt8 keyinId, UInt8 mark);                                 // 一键刻篆
        void changeMoney(UInt16 num, UInt8 mark);                                       // 壁凤墨转换为磐龙墨
        void addMoney(UInt32 toolId, UInt32 count);
        void addKYAttr(GData::AttrExtra& ae);                                           // 刻印添加属性
        void checkKey(UInt16 keyId, UInt8 mark);                                                     
        UInt32 yizhiyue(UInt16 keyId);                                                  // 义之鈅
        UInt32 renzhiyue(UInt16 keyId);                                                 // 仁之鈅
        inline bool findEquipJG(UInt32 jgId);
        inline bool specialFindEquipJG(UInt32 jgId);
        inline bool findTotalJG(UInt32 jgId);
        inline bool findNoEquipJG(UInt32 jgId);
        inline bool findKey(UInt16 keyId);

        /**GM**/
        void addjiguanyu(UInt32 jiguanyuId);

    public:
        void sendCommonGearInfo();
        void sendSpecialGearInfo();
        void sendZYValue(Stream& st);
        void makeGear(UInt16 gearId, UInt8 mark);
        bool makeCommonGear(UInt16 gearId);
        bool makeSpecialGear(UInt16 gearId);
        bool checkCommonGear(UInt16 gearId);
        bool checkSpecialGear(UInt16 gearId);
        void addGearAttr(GData::AttrExtra& ae);

    private:

        std::map<UInt32, UInt8> m_equipJGA;  // A装备的机关

        std::map<UInt32, UInt8> m_equipJGB;  // B装备的机关

        std::map<UInt32, UInt8> m_equipJGC;  // C装备的机关

        std::map<UInt32, UInt8> m_tuzhi;    // 记录图纸所对应的熟练度

        std::map<UInt8, UInt8> m_kyLvl;     // 记录刻印当前等级

        std::map<UInt8, UInt32> m_kyCurExp; // 记录刻印当前经验

        std::map<UInt16, UInt8> m_zhenwei;  // 记录机关密钥所对应的状态

        std::vector<UInt32> m_jg;           // 记录未装备机关

        std::vector<UInt16> m_commonGear;   // 记录普通机关库已经制造成功的零件

        std::vector<UInt16> m_specialGear;  // 记录特殊机关库已经制造成功的零件

        int m_grids[EQUIP_SCHEME_MAX][EQUIP_GRID_MAX]; // 记录网格占用情况[方案][占用情况]

        int m_kyQuality[4];                 // 刻印品质

        Jiguanshu m_jiguanshu;

        Player* m_owner;
    };
}

#endif
