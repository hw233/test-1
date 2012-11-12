#ifndef  _ACTIVITYMGR_H
#define  _ACTIVITYMGR_H
class Stream;
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
        AtyHeroIsland,//英雄岛
        AtyBoss,      //世界BOSS
        AtyCountryWar,//阵营战
        AtyClanWar,   //帮派战
        AtyAthletics, //斗剑
        AtySignIn,    //每日签到
        AtyMaxFlag,      //标志记录的个数

        AtyBegin      = 99,
        AtyShimenTask = 100,//师门任务
        AtyYamenTask, //衙门任务
        AtyClanTask,  //帮会任务
        AtyDungeon,   //决战之地
        AtyCopy,      //副本
        AtyFormation, //阵图
        AtyTaskHook,  //挂机加速
        AtyPSpeed,    //修炼加速
        AtyGroupCopy, //组队副本
        AtyTownDeamon, //锁妖塔
        AtyShuoShuo, //取发表说说奖励
        AtyInvited, //领取好友邀请奖励
        AtyEnd,
    };

#define SIGNIN_RECORD 31
    struct ActivityItem
    {
        UInt32  overTime;//月结束时间
        UInt32  scores;  //玩家签到总积分
        UInt32  propsID; //刷新出来的道具ID，待兑换
        UInt16  signRecord[SIGNIN_RECORD]; //每日签到记录(一整月)
        UInt32  awardID; //上线领取奖励物品ID
        UInt32  point;   //活跃点数
        UInt32  award;   //领取的奖励
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
        void Reset(UInt32 aid = 0, UInt32 ot = 0, UInt32 s = 0, UInt32 pid = 0)
        {
            memset(this, 0, sizeof(ActivityItem));
            if(aid)
                awardID = aid;
            if(ot)
                overTime = ot;
            if(s)
                scores = s;
            point = 10;
            propsID = 29;
            if(pid)
                propsID = pid;
        }
        /*
        void Reset(UInt32 ot = 0, UInt32 s = 0, UInt32 pid = 0)
        {
            memset(this, 0, sizeof(ActivityItem));
            if(ot)
                overTime = ot;
            if(s)
                scores = s;
            propsID = 29;
            if(pid)
                propsID = pid;
        }
        */
    };
    struct stRtyReward
    {
        UInt32 id;
        UInt8 num;
    };

    typedef std::vector<stRtyReward>  RtyRewards;
    typedef std::vector<RtyRewards> OnlineRewards;
//    struct stOnlineReward
  //  {
     //   Rty
   // };
    struct DBActivityData;
    class ActivityMgr
    {
        private :
            enum{
                AtyOnlineFree = 1, //是否已经用去免费领取
                AtyOnlineReward = 2, //是否已经领取了
                AtyReward1 = 4,  //60
                AtyReward2 = 8,  //80
                AtyReward3 = 16,  //100
                AtyReward4 = 32, //120
                AtyReward5 = 64, //160
            };
            Player*  _owner;
            ActivityItem  _item;
            RtyRewards    _onlineReward;
            //std::Map<UInt32, UInt32> _randmap;  //上线奖励中， 多种奖中励随机一个的，需要记录这个随机值。
            //func
            UInt32 GetRandomReward() ;
            //std::vector<stOnlineReward> GetOnlineReward();
            void GetOnlineReward(UInt8 cnt);
            UInt8 GetOnlineRewardGetNum();
            UInt32 GetOnlineRewardNum();
            void SendOnlineReward(Stream& s);
            void SendActivityInfo(Stream& s);
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
           void  AddRewardFlag(UInt32 flag, bool db = true);


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

           /**
            * @brief s->c  刷新列表
            */
           void ActivityList(UInt8 type);

           /**
            * @brief 增加签到积分
            */
            void AddScores(UInt32 v);
            void SubScores(UInt32 v);
            UInt32 GetScores() { return _item.scores;}
            UInt32 GetPropsID() { return _item.propsID;}
            UInt16 GetOneDayRecord(UInt8 day);
            void SetPropsID(UInt32 id = 0);
            void AddSignTime(UInt8 day = 0);
            void ActivitySignIn();
            void RefreshProps();
            void ExchangeProps();
            void SendActivityInfo();
    };

}
#endif
