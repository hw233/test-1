
#ifndef HEROMEMO_H_
#define HEROMEMO_H_

#include "Config.h"

namespace GObject
{

class Player;

enum MemoType
{
/******* 散仙篇 *******/

// 入门
    // 获得经验
    // 挂机
    // 挂机加速
    // 主将45级 
// 进阶 
    // 招募散仙
    // 招募一个绿色散仙
    // 招募一个蓝色散仙
    // 招募一个紫色散仙
    // 散仙出战
    // 出战5名将
    // 使用奇门阵
    // 使用七绝锁云阵
// 大师
    // 潜力洗炼
    // 一次潜力洗炼
    // 潜力洗炼到1.1
    // 潜力洗炼到1.2
    // 资质洗炼
    // 进行一次资质洗炼
    // 将主将的资质洗炼至6.5
    // 将主将的资质洗炼至7
    // 天赋洗炼
    // 进行一次天赋洗炼
    // 洗炼出蓝色天赋
    // 洗炼出紫色天赋
// 传奇

/****** 心法篇 ******/

// 入门
    // 获得修为
    // 在无主之地修炼
    // 一个散仙累计获得20000修为
    // 一个散仙累计获得50000修为
    // 打通穴道
    // 进行一次冲穴
    // 打通肩井穴
    // 打通神藏穴
// 进阶
    // 心法学习
    // 学习一本心法
    // 装备一本心法
    // 升级一本心法至九层
// 大师
    // 心法组合
    // 一位散仙装备三本技能心法
    // 一位职业为儒的散仙装备身法入门篇+七煞诀+无相智之章
    // 一位职业为道的散仙装备锻骨术+三元心法+无相心经
// 传奇
    // 心法获得
    // 获得一本剑术入门篇
    // 获得一本无量心经
    // 获得一本锻骨术

/****** 炼器篇 ******/

// 入门
    // 装备强化
    // 将一件装备强化至+2
    // 将一件装备强化至+4
    // 将一件装备强化至+6
    // 装备分解
    // 分解装备获得太乙真金
    // 分解装备获得陨铁
    // 分解装备获得太乙精金
// 进阶
    // 装备洗炼
    // 进行一次装备洗炼
    // 洗炼出蓝色属性
    // 洗炼出紫色属性
    // 装备炼化
    // 进行一次炼化
    // 进行4次炼化
    // 进行8次炼化
// 大师
    // 装备打孔
    // 进行一次打孔
    // 在一件装备上打两个孔
    // 在一件装备上打三个孔
    // 宝石镶嵌
    // 镶嵌一颗宝石
    // 拆卸一颗宝石
    // 镶嵌一颗三级宝石
// 传奇
    // 法宝强化
    // 将一件法宝强化至+1
    // 将一件法宝强化至+2
    // 将一件法宝强化至+4
    // 法宝熔炼
    // 将一件法宝熔炼至1阶
    // 将一件法宝熔炼至2阶
    // 将一件法宝熔炼至3阶

/****** 技能篇 ******/
// 入门
    // 主动技能
    // 一个散仙装备一个主动技能
    // 一个散仙装备两个主动技能
    // 一个散仙装备三个主动技能
// 进阶
    // 无双技能
    // 一个散仙装备一个无双法宝
    // 一个散仙装备一次无双技能
    // 一个散仙使用一次无双技能
// 大师
    // 被动技能
    // 一个散仙装备一个被动法宝
    // 一个散仙使用一次被动技能
    // 一个散仙装备两个被动法宝

/****** 除魔篇 ******/
// 入门
    // 决战之地
    // 在任意决战之地战斗一次
    // 通关魏家场
    // 通关水火风雷魔阵
// 进阶
    // 单人副本
    // 在任意副本单人挑战模式战斗一次
    // 通关慈云寺单人挑战模式
    // 通关青螺魔宫单人挑战模式
// 大师
    // 组队副本
    // 在任意副本组队模式战斗一次
    // 通关慈云寺组队简单模式
    // 通关慈云寺组队困难模式
    // 阵法挑战
    // 在任意阵法战斗一次
    // 通七绝锁云阵
    // 通四象元灵阵
// 传奇
    // 世界boss
    // 攻击一次五鬼天王尚和阳
    // 攻击一次绝阵者封印
    // 攻击一次世界boss元神

/****** 斗剑篇 ******/
// 入门
    // 斗剑历练
    // 进行一次斗剑历练
    // 在一次斗剑历练中获得胜利
    // 获得一个斗剑历练宝箱
// 进阶
    // 斗剑天梯
    // 在一次斗剑天梯挑战中获得胜利
    // 提升10个排名
    // 提升100个排名
    // 阵营战
    // 参加一次阵营战
    // 在阵营战中获得一次胜利
    // 在阵营战中获得两连胜
// 大师
    // 英雄岛
    // 参加一次英雄岛
    // 在英雄岛中击败一名指定身份玩家
    // 在英雄岛中抽取一次宝箱
// 传奇
    // 帮派战
    // 参加一次帮派战
    // 在帮派战中获得一次战斗胜利
    // 赢得一场帮派战的胜利

/****** 人脉篇 ******/
// 入门
    // 好友邀请
    // 与1名玩家成为好友
    // 与5名玩家成为好友
    // 与10名玩家成为好友
// 进阶
    // 加入或建立一个帮派
    // 捐献一次帮派科技
    // 帮派技能身法增强达到3级
// 大师
// 传奇

};

class Player;

enum MemoChapter
{
    MC_FIGHTER = 0,
    MC_CITTA,
    MC_FORGE,
    MC_SKILL,
    MC_SLAYER,
    MC_ATHLETICS,
    MC_CONTACTS,
    MC_MAX
};

enum MemoDiff
{
    MD_STARTED = 0,
    MD_ADVANCED,
    MD_MASTER,
    MD_LEGEND,
    MD_MAX
};

class MemoItem
{
public:
    MemoItem()
    {
        memset(m_item, 0x00, sizeof(m_item));
    }

    inline UInt32 size() const { return sizeof(m_item); }

    const UInt8& operator[](UInt32 idx) const { return m_item[idx];}
    UInt8& operator[](UInt32 idx) { return m_item[idx];}

private:
    UInt8 m_item[3];
};

class Memo
{
public:
    Memo() {}
    ~Memo() {}

    inline UInt32 size() const { return m_items.size(); }
    inline void resize(UInt32 size) { m_items.resize(size); }

    const MemoItem& operator[](UInt32 idx) const { return m_items[idx]; }
    MemoItem& operator[](UInt32 idx)
    {
        if (idx >= m_items.size())
            m_items.resize(idx+1);
        return m_items[idx];
    }

private:
    std::vector<MemoItem> m_items;
};

class HeroMemo
{
public:
    HeroMemo(Player* player);
    ~HeroMemo();

    void setMemo(UInt8 chapter, UInt8 diff, UInt8 group, UInt8 item, UInt8 value);
    UInt8 getMemo(UInt8 chapter, UInt8 diff, UInt8 group, UInt8 item);

    inline UInt16 getHeroSoul() const { return m_heroSoul; }

    void sendHeroMemoInfo();
    void getAward(UInt8 idx);

    void updateToDB();
    void loadFromDB(const char* awards, const char* memos);

    static bool testCanGetAward(Player* player, UInt8 idx);
    static void addMaxSoul(UInt16 soul);

private:
    static std::vector<UInt16> m_maxSoul;

private:
    Player* m_owner;
    UInt16 m_heroSoul;
    Memo m_memos[MC_MAX][MD_MAX];
    std::vector<UInt8> m_awards;
    UInt8 m_maxAwardIdx;
};

} // namespace GObject

#endif // HEROMEMO_H_

/* vim: set ai si nu sm smd hls is ts=4 sm=4 bs=indent,eol,start */

