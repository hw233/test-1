#ifndef _JIGUANDATA_H_
#define _JIGUANDATA_H_

#include <map>
#include "Config.h"
#include "GDataDBExecHelper.h"

namespace GData
{

class JiguanData
{
    public:
        struct jiguanyuInfo
        {
            std::string name;   // 机关玉名字
            UInt32 jiguanyuId;  // 机关玉Id
            UInt8 mark;         // 类型
            UInt8 quality;      // 品质 
            UInt8 attrType;     // 属性类型(机关玉)
            UInt8 element;      // 元素
            float attrValue;    // 属性值(机关玉)
            UInt32 molding[9];  // 造型
            UInt16 skillId;     // 技能(机关器)

            jiguanyuInfo(): name(""), jiguanyuId(0), quality(0), attrType(0), element(0), attrValue(0.00), skillId(0) { memset(molding, 0, sizeof(molding)); }
        };

        struct jiguanshuInfo
        {
            UInt8 jgshuLvl;     // 机关术等级
            UInt32 totalExp;    // 升到下个等级所需经验
            UInt32 needExp;         
            float attrValue1;   // 风属性加成 
            float attrValue2;   // 林属性加成 
            float attrValue3;   // 火属性加成 
            float attrValue4;   // 山属性加成 

            jiguanshuInfo(): jgshuLvl(0), totalExp(0), needExp(0), attrValue1(0.00), attrValue2(0.00), attrValue3(0.00), attrValue4(0.00) {}
        };

        struct tuzhiInfo
        {
            std::string name;   // 图纸名字
            UInt32 tuzhiId;     // 图纸Id
            UInt8 quality;      // 图纸品质
            UInt8 needjgsLvl;   // 需要机关术等级
            UInt32 jiguanyuId;  // 生产出的机关玉Id

            tuzhiInfo(): name(""), tuzhiId(0), quality(0), needjgsLvl(0), jiguanyuId(0) {}
        };

        struct keyinInfo
        {
            std::string name;   // 刻印名字
            UInt8 keyinId;      // 刻印Id
            UInt8 keyinLvl;     // 刻印等级
            UInt8 quality;      // 品质 
            UInt8 attrType;     // 属性类型(刻印)
            UInt32 attrValue;   // 属性值(刻印)
            UInt32 materialA;   // 消耗壁凰墨值
            UInt32 materialB;   // 消耗磐龙墨值
            UInt32 maxValue;    // 达到升级最大值

            keyinInfo(): name(""), keyinId(0), keyinLvl(0), quality(0), attrType(0), attrValue(0), materialA(0), materialB(0), maxValue(0) {}
        };

        struct zhenweiInfo
        {
            UInt16 keyId;       // 机关密钥Id
            std::string name;   // 机关密钥名字
            UInt8 type;         // 收集类型
            UInt32 collect1;    // 收集1
            UInt32 collect2;    // 收集2
            UInt32 collect3;    // 收集3
            UInt32 collect4;    // 收集4
            UInt32 collect5;    // 收集5
            UInt32 collect6;    // 收集6
            UInt32 collect7;    // 收集7
            UInt32 collect8;    // 收集8
            UInt32 award;       // 奖励

            zhenweiInfo(): keyId(0), name(""), type(0), collect1(0), collect2(0), collect3(0), collect4(0), collect5(0), collect6(0), collect7(0), collect8(0), award(0) {}
        };

        struct needComponent
        {
            UInt16 needComponentAId; // 需要元件A
            UInt16 needComponentBId; // 需要元件B

            needComponent(): needComponentAId(0), needComponentBId(0) {}
        };

        struct needPart
        {
            UInt16 lastSuiteId;     // 前一个套件Id
            UInt16 needPartAId;     // 需要部件A
            UInt16 needPartBId;     // 需要部件B
            UInt16 needPartCId;     // 需要部件C

            needPart(): needPartAId(0), needPartBId(0), needPartCId(0) {}
        };
        
        struct gearInfo
        {
            UInt16 partId;          // 零件Id
            UInt32 ZYKNum;          // 需要紫云矿数量
            UInt32 ZYMNum;          // 需要紫云木数量
            UInt32 ZYSNum;          // 需要紫云索数量
            UInt32 ZYLFNum;         // 需要紫云炼符数量
            UInt32 taelNum;         // 需要银币数量
            float attrValueA;       // 物攻
            float attrValueB;       // 法功
            float attrValueC;       // 物防
            float attrValueD;       // 法防
            float attrValueE;       // 生命
            float attrValueF;       // 身法
            float attrValueG;       // 挂机经验

            gearInfo(): partId(0), ZYKNum(0), ZYMNum(0), ZYSNum(0), ZYLFNum(0), taelNum(0), attrValueA(0.00), attrValueB(0.00), attrValueC(0.00), attrValueD(0.00), attrValueE(0.00), attrValueF(0.00), attrValueG(0.00) {}
        };

    private:
        std::map<UInt32, jiguanyuInfo> _jiguanyuInfo;
        std::map<UInt8, jiguanshuInfo> _jiguanshuInfo;
        std::map<UInt32, tuzhiInfo> _tuzhiInfo;
        std::map<UInt8, std::vector<UInt32>> _lvltuzhiInfo;
        std::map<UInt8, std::map<UInt8, keyinInfo>> _keyinInfo;
        std::map<UInt16, zhenweiInfo> _zhenweiInfo;
        std::map<UInt16, UInt8> _tempType;
        std::map<UInt16, needComponent> _makePart;
        std::map<UInt16, needPart> _makeSuite;
        std::map<UInt16, gearInfo> _gearInfo;
    public:
        void setJiguanyuInfo(DBJiguanyuConfig &);

        void setJiguanshuInfo(DBJiguanshuConfig &);

        void settuzhiInfo(DBTuzhiConfig &);

        void setkeyinInfo(DBKeyinConfig &);

        void setZhenweiInfo(DBZhenweiConfig &);

        void setGearInfo(DBGearConfig &);

        void setGearTreeInfo(DBGearTreeConfig &);

        UInt32 getTuzhiId(UInt8 lvl);

        jiguanyuInfo * getJiguanyuInfo(UInt32 jgyuId);

        jiguanshuInfo * getJiguanshuInfo(UInt8 lvl);

        tuzhiInfo * getTuzhiInfo(UInt32 tuzhiId);

        jiguanshuInfo * getUpgradeInfo(UInt32 curExp);

        keyinInfo * getKeyinInfo(UInt8 keyinId, UInt8 keyinLvl);

        zhenweiInfo * getZhenweiInfo(UInt16 keyId);

        UInt8 getGearType(UInt16 gearId);

        needComponent * getMakePartInfo(UInt16 gearId);

        needPart * getMakeSuiteInfo(UInt16 gearId);

        gearInfo * getGearInfo(UInt16 gearId);

        inline UInt16 getJGMYCount(){return _zhenweiInfo.size();}
};

extern JiguanData jiguanData;

}

#endif
