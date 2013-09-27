#ifndef  _STRENGTHENMGR_H
#define  _STRENGTHENMGR_H
class Stream;
namespace GObject
{

    class Player;
    enum
    {
        SthShimenTask = 0,  //师门任务
        SthTaskHook,  //挂机
        SthHookUse,  //使用挂机加速符
        SthHookSpeed,  //挂机加速
        SthDungeon,   //决战之地
        SthBoss,      //世界BOSS
        SthCountryWar,//阵营战
        SthPractice,  //修炼
        SthPUse,    //使用修炼加速符
        SthPSpeed,    //修炼加速
        SthCopy = 10, //副本
        SthClanWar,   //帮派战
        SthYamenTask, //衙门任务
        SthAthletics1, //斗剑天梯
        SthAthletics2, //斗剑历练
        SthOpenPurpleBox, //打开紫色变强秘宝
        SthTripodFire, //炉火
        SthFormation, //阵图
        SthTownDeamon, //封印锁妖塔
        SthHeroIsland,//英雄岛
        SthGroupCopy = 20, //组队副本
        SthShuoShuo, //领取发表说说奖励
        SthInvited, //领取好友邀请奖励
        SthEnchant,   //装备强化
        SthSplit,     //装备分解
        SthForge,     //装备洗练
        SthOpenSocket, //装备打孔
        SthAttachGem, //宝石镶嵌
        SthMergeGem, //宝石合成
        SthDetachGem, //宝石拆卸
        SthTrumpEnchant = 30,   //法宝强化
        SthTrumpLOrder,   //法宝升阶
        SthTrumpUpgrade,   //法宝熔炼
        SthCittaUpgrade,   //升级心法
        SthGenius,   //天赋洗炼
        SthPotential,   //潜力洗炼
        SthCapacity = 36,   //资质洗炼
        SthCHTownDeamon, //镇守聚魂阵
        SthActSignIn,    //活跃度签到
        SthChallenge,    //切磋
        SthMSCG = 40,   //墨守成规
        SthClanTask,  //帮派任务
        SthLastDay,  //末日之战
        SthClanSpirit, //神魔之树
        SthPrayTree,  //许愿树
        SthSerachMo,    //寻墨 
        SthFuwenJIe,   //符文解封
        SthFuwenRong,  //符文熔炼
        SthSkillUp,   //个人技能提升
        SthStoreFix, //晶石融合
        SthXingchen, //星辰描绘
        SthPinJieUp, //品阶进阶
        SthGenguUp, //根骨提升
        SthSpiritEat, //元神吞噬
        SthSpiritUp, //元神升级
        SthNeiDanUp, //内丹升级
        SthJingPoFix,//精魄合成
        SthEquipLH, //装备炼化
        SthEquipSpirit,//装备注灵
        SthGuJiSpirit, //古籍唤灵
        SthBaoJuSpirit, //宝具通灵
        SthMaxFlag,      //标志记录的个数
    };
#define STRONGTYPE_CNT 2
#define STRONGBOX_CNT 6
    struct StrongItem
    {
        UInt32 overTime; //天结束时间
        UInt8  souls;   //变强之魂总数
        UInt8  soulId;   //变强之魂id
        UInt8  flag[SthMaxFlag];
        UInt32 greenBox[STRONGTYPE_CNT][STRONGBOX_CNT];
        UInt32 blueBox[STRONGTYPE_CNT][STRONGBOX_CNT];
        UInt32 purpleBox[STRONGTYPE_CNT][STRONGBOX_CNT];
        UInt32 orangeBox[STRONGTYPE_CNT][STRONGBOX_CNT];
        StrongItem()
        {
            Reset();
        }
        void Reset(UInt32 ot = 0)
        {
            memset(this, 0, sizeof(StrongItem));
            if(ot)
                overTime = ot;
        }
    };
    struct DBStrengthenData;
    class StrengthenMgr 
    {
        private :
            Player*  _owner;
            StrongItem _item;
        public:
            StrengthenMgr(Player* player);
            ~StrengthenMgr();
            UInt8 GetFlag(UInt8 idx); 
            UInt8 GetSouls();
            UInt8 GetSoulId(){return _item.soulId;}
            void SetSoulId(UInt8 id = 0){_item.soulId = id;}
            void AddSouls(UInt8 v);
            void LoadFromDB(DBStrengthenData& data) ;
            void UpdateToDB();
            void UpdateFlag(UInt8 idx, UInt8 v);
            bool CheckTimeOver(UInt32 now = TimeUtil::Now());
            void SendStrengthenInfo();
            void SendStrengthLevelInfo();
            void SendStrengthenRank();
            void SetStrengthLevelInfo(UInt8 type);
            void SendOpenChestsInfo(UInt8 boxId, UInt8 index);
            void EveryDayRoar();
            bool CanOpenGreenBox(UInt8 type, UInt32 onlineTime);
            bool CanOpenBlueBox(UInt8 type, UInt32 useTael);
            bool CanOpenPurpleBox(UInt8 type, UInt32 useCoupon);
            bool CanOpenOrangeBox(UInt8 type, UInt32 useGold);
            void CheckeFixedSouls(UInt8 v);
            void SendFlickerToClient();
            
            void GMClearSthAll(){_item.Reset(); UpdateToDB();} //For GM
    };

}
#endif
