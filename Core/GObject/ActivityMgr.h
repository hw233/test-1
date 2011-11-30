#ifndef  _ACTIVITYMGR_H
#define  _ACTIVITYMGR_H
namespace GObject
{

    class Player;
    enum
    {
        AtyPractice,  //修炼
        AtyTripodFire,//点火
        AtyBarRef,    //酒店刷新
        AtyBookStore, //书店刷新
        AtyEnchant,   //装备强化
        AtySplit,     //装备分解
        AtyForge,     //装备洗练
        AtyBuy,       //购买物品
        AtyLongTime,  //在线时间足够长
        AtyMaxFlag,      //标志记录的个数

                AtyShimenTask = 1000,//师门任务
                AtyYamenTask, //衙门任务
                AtyClanTask,  //帮会任务
                AtyDungeon,   //决战之地
                AtyCopy,      //副本
                AtyFormation, //阵图
                AtyAthletics, //斗剑
                AtyGroupCopy, //组队副本
                AtyHeroIsland,//英雄岛
                AtyBoss,      //世界BOSS
                AtyCountryWar,//阵营战
                AtyClanWar,   //帮派战
                AtyTaskHook,  //挂机加速
                AtyPSpeed,    //修炼加速
    };

          
    struct ActivityItem
    {
        UInt32  overTime;//结束时间
        UInt32  awardID; //上线领取奖励物品ID
        UInt32  point;  //活跃点数
        UInt32  award;  //领取的奖励
        /*
        UInt8   practice;//修炼次数
        UInt8   tripodFire;//点火次数
        UInt8   barRefresh;//酒馆刷新
        UInt8   bookStore; //书店刷新
        UInt8   enchant;  //装备强化
        UInt8   split;    //装备分解
        UInt8   forge;    //装备洗练
        UInt8   buy;      //商城购买
*/
        UInt8   flag[AtyMaxFlag];
        ActivityItem()
        {
            Reset();
        }
        void Reset(UInt32 aid = 0, UInt32 ot = 0)
        {
            memset(this, 0, sizeof(ActivityItem));
            if(aid)
                awardID = aid;
            if(ot)
                overTime = ot;
        }
    };
    struct DBActivityData;
    class ActivityMgr
    {
        private :
            enum{
                AtyOnlineReward = 1,
                AtyReward1 = 2,  //60
                AtyReward2 = 4,  //80
                AtyReward3 = 8,  //100
                AtyReward4 = 16, //120
                AtyReward5 = 32, //160
            };
            Player*  _owner;
            ActivityItem  _item;

            //func
            UInt32 GetRandomReward() ;
            std::vector<UInt32> GetOnlineReward();
        public:
            ActivityMgr(Player* player);
            ~ActivityMgr();
           // only in script func
           void AddPoint(UInt32 v);
           UInt32 GetPoint(){return _item.point;}
           UInt8  GetFlag(UInt32 idx);  

           /**
            * @brief 领取奖励后置领取位
            */
           void  AddRewardFlag(UInt32 flag);


           //data base
           void LoadFromDB(DBActivityData& data) ;
           void UpdateToDB();

           /**
            * @brief  更新活跃点数依赖的标志位
            */
           void  UpdateFlag(UInt32 idx, UInt8 v);
           /**
            * @brief  确定时间是否重置
            */
           bool CheckTimeOver();
           /**
            * @brief  c->s  领取奖励
            */
           void  GetReward(UInt32 flag);
           /**
            * @brief  c->s  改变上线奖励
            */
           void  ChangeOnlineReward();
    };

}
#endif
