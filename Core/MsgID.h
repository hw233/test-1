#ifndef _MSGID_H_
#define _MSGID_H_

#include "Config.h"

namespace REQ
{
    /**保持连接*/
    const UInt8 KEEP_ALIVE          = 0x00;
    /**自动重连*/
    const UInt8 RECONNECT           = 0x01;
    /**成就*/
    const UInt8 ACHIEVEMENT         = 0xF0;// 0x0C
    /**阵营选择*/
    const UInt8 CAMPS_CHOICE        = 0xF1;// 0x0D
    /**登陆*/
    const UInt8 LOGIN               = 0xE0;// 0x10
    /**创建角色*/                              
    const UInt8 CREATE_ROLE         = 0xE1;// 0x11
    /**新手引导步骤*/                           
    const UInt8 NEW_HAND_STEP       = 0xE3;// 0x13
    /**请求玩家信息*/                          
    const UInt8 USER_INFO           = 0xE4;// 0x14
    /**BUFFER信息改变*/                        
    const UInt8 STATE               = 0xE5;// 0x15
    /** ??? */                                  
    const UInt8 STATE_CHANGE        = 0x16;
    /**查询书商*/                               
    const UInt8 BOOK_LIST           = 0xEC;// 0x1A
    /**买书*/                                 
    const UInt8 BOOK_BUY            = 0xED;// 0x1B
    /**读阵型*/                               
    const UInt8 READ_BATTLE         = 0xEE;// 0x1D
    /**改变阵型*/                             
    const UInt8 BATTLE_CHECK        = 0xEF;// 0x1E
    /**装备改变*/
    const UInt8 CHANGE_EQ           = 0xD0;// 0x21
    /**雇用散仙*/                             
    const UInt8 HIRE_HERO           = 0xD1;// 0x22
    /**散仙信息*/                             
    const UInt8 HERO_INFO           = 0xD2;// 0x23
    /**酒馆列表*/                             
    const UInt8 HOTEL_LIST          = 0xD3;// 0x26
    /**解雇散仙*/                             
    const UInt8 FIRE_HERO           = 0xD4;// 0x27
    /**接收散仙*/                             
    const UInt8 ACCEPT_FIGHTER      = 0xD5;// 0x28
    /**加血*/
    const UInt8 ADD_HP              = 0xDE;// 0x29
    /**潜力洗炼*/
    const UInt8 POTENTIAL           = 0xDF;// 0x2C
    /**请求修炼武将列表*/
    //const UInt8 TRAIN_FIGHTER_LIST  = 0x2D;
    /**加速取消修炼散仙*/
    const UInt8 TRAIN_FIGHTER_OP    = 0x37;
    /**背包列表信息*/
    const UInt8 PACK_INFO           = 0xC0;// 0x30
    /**背包添加到九疑鼎*/                      
    const UInt8 PACK_TRIPOD         = 0xC1;// 0x31
    /**背包卖出*/                              
    const UInt8 PACK_SELL           = 0xC2;// 0x32
    /**背包使用*/                              
    const UInt8 PACK_USE            = 0xC3;// 0x33
    /**背包扩展*/                              
    const UInt8 PACK_EXTEND         = 0xC4;// 0x34
    /**在线奖励*/
    const UInt8 REWARD              = 0xCF;// 0x38
    /**九疑鼎*/
    const UInt8 TRIPOD_INFO         = 0xCE;// 0x39
    /**探险信息*/
    const UInt8 LUCKYDRAW_INFO      = 0x3D;
    /**探险*/
    const UInt8 LUCKYDRAW           = 0x3E;
    /**装备强化*/
    const UInt8 EQ_ENHANCE          = 0xB0;// 0x40
    /**装备打孔*/                               
    const UInt8 EQ_PUNCH            = 0xB1;// 0x41
    /**宝石镶嵌*/                               
    const UInt8 EQ_EMBED            = 0xB2;// 0x43
    /**宝石拆卸*/                               
    const UInt8 EQ_UN_EMBED         = 0xB3;// 0x44
    /**单个分解*/                               
    const UInt8 EQ_UN_SINGLE        = 0x45;
    /**转换装备*/                               
    const UInt8 EQ_EXCHANGE         = 0x46;
    /**装备洗炼*/                               
    const UInt8 EQ_PURIFY           = 0xB4;// 0x47
    /**批量分解*/                               
    const UInt8 EQ_DECOMPOSE        = 0xB5;// 0x49
    /**置换部位*/
    const UInt8 EQ_EXCHANGE_POS     = 0x4A;
    /**宝石批量合成*/
    const UInt8 GEM_UPGRADE         = 0xB6;// 0x4B
    /**装备激活 */
    const UInt8 EQ_ACTIVE           = 0x4C;
    /**地图获取据点信息*/
    const UInt8 MAP_LOCATE          = 0xA0;// 0x51
    /**地图传送*/
    const UInt8 MAP_TRANSPORT       = 0xA1;// 0x52
    /**进入离开通天塔*/
    const UInt8 BABEL_JOIN          = 0xAA;// 0x58
    /**请求通天塔信息*/                       
    const UInt8 BABEL_UPDATE        = 0xAB;// 0x59
    /**开始通天塔战斗*/                       
    const UInt8 BABEL_START         = 0xAC;// 0x5A
    /**开始自动通天塔战斗*/                   
    const UInt8 BABEL_AUTO_START    = 0xAD;// 0x5B
    /**立即完成通天塔战斗*/                   
    const UInt8 BABEL_END           = 0xAE;// 0x5C
    /**日常*/                                 
    const UInt8 DAILY               = 0xAF;// 0x5F
    /**换名字*/
    //const UInt8 CHANGE_NAME         = 0x60;
    /**加入国战*/
    const UInt8 CAMPS_WAR_JOIN      = 0x90;// 0x62
    /**副本数据*/
    const UInt8 COPY_DATA           = 0x94;// 0x67
    /**阵图数据*/
    const UInt8 FORMATION_DATA      = 0x95;// 0x68
    /**自动副本*/
    const UInt8 AUTO_COPY           = 0x96;// ??
    /**自动阵图*/
    const UInt8 AUTO_FRONTMAP       = 0x98;// ??
    /**请求战报*/
    const UInt8 FIGHT_REPORT        = 0x9A;// 0x6C
    /**打怪*/                                 
    const UInt8 ATTACK_NPC          = 0x9B;// 0x61
    /**挂机*/                                 
    const UInt8 TASK_HOOK           = 0x9D;// 0x6D
    /**挂机停止*/                             
    const UInt8 TASK_HOOK_STOP      = 0x9E;// 0x6E
    /**挂机加速*/                             
    const UInt8 TASK_HOOK_ADD       = 0x9F;// 0x6F
    /**发起切磋请求*/
    const UInt8 LANCHCHALLENGE      = 0x72;
    /**回复切磋*/
    const UInt8 REQUESTCHALLENGE    = 0x73;
    /**战斗退出*/
    const UInt8 FIGHT_EXIT          = 0x9C;// 0x77
    /**帮派战*/
    const UInt8 CLAN_BATTLE         = 0x79;
    /***/
    const UInt8 CLANCITYBATTLELIST  = 0x7A;
    /**帮派动态信息*/
    const UInt8 CLAN_INFO           = 0x7C;
    /**获取战报信息请求*/
    const UInt8 CLAN_BATTLE_INFO    = 0x7D;
    /**点击NPC对话*/
    const UInt8 DIALOG_START        = 0x50;// 0x80
    /**点击交互动作*/
    const UInt8 NPC_INTERACT        = 0x51;// 0x81
    /**当前任务列表请求*/
    const UInt8 CURR_TASK_LIST      = 0x52;// 0x82
    /**当前可接任务列表请求*/
    const UInt8 CURR_AVAILABLE_TASK = 0x53;// 0x83
    /**玩家接受、提交、放弃某任务请求*/
    const UInt8 TASK_ACTION         = 0x54;// 0x85
    /**护送任务玩家到达可触发战斗据点*/
    const UInt8 CONVEYBATTLE        = 0x88;
    /**采集任务物品请求*/
    const UInt8 COLLECTNPCACTION    = 0x89;
    /**循环任务刷新*/
    const UInt8 TASK_CYC_REFRESH    = 0x57;// 0x8B
    /**自动完成日常任务*/
    const UInt8 DAYTASKAUTOCOMPLETED= 0x8C;
    /**???*/
    const UInt8 AUTO_COMPLETED_TASK = 0x8D;
    /**循环任务进度请求*/
    const UInt8 QUERYDAYTASK        = 0x8F;
    /**请求帮派列表信息*/
    const UInt8 CLAN_LIST           = 0x60;// 0x90
    /**请求自己帮派信息*/                      
    const UInt8 CLAN_SELF           = 0x61;// 0x91
    /**帮派创建*/                              
    const UInt8 CLAN_CREATE         = 0x62;// 0x92
    /**申请加入帮派*/                          
    const UInt8 CLAN_APPLY          = 0x63;// 0x93
    /**帮派成员操作*/                          
    const UInt8 CLAN_OPERATE        = 0x64;// 0x94
    /**帮派信息改变*/                          
    const UInt8 CLAN_INFO_CHANGE    = 0x65;// 0x95
    /**请求某个帮派信息*/                      
    const UInt8 CLAN_REQ_OTHER      = 0x66;// 0x96
    /**请求帮派成员列表信息*/                  
    const UInt8 CLAN_PLAYER_LIST    = 0x67;// 0x97
    /**请求帮派科技信息*/
    const UInt8 CLAN_SKILL          = 0x69;// 0x99
    /**帮派驻地信息*/
    const UInt8 CLAN_BUILD          = 0x6A;// 0x9B
    /**帮会仓库*/
    const UInt8 CLAN_PACKAGE        = 0x6B;// 0x6B
    /**帮会仓库记录*/
    const UInt8 CLAN_PACKAGE_RECORD = 0x6C;
    /**请求宗族奖励物品*/
    const UInt8 CLANREWARD          = 0x9C;
    /**领取宗族奖励*/
    const UInt8 GET_CLANREWARD      = 0xFB;// 0x9D
    /**分配宗族奖励物品*/
    const UInt8 ALLOCATECLANREWARD  = 0x9E;
    /**宗族分配记录请求*/
    const UInt8 CLANALLOCRECORD     = 0x9F;
    /**邮件ID列表*/
    const UInt8 MAIL_ID_LIST        = 0x40;// 0xA6
    /**邮件信息列表*/
    const UInt8 MAIL_LIST           = 0x41;// 0xA0
    /**邮件内容*/
    const UInt8 MAIL_CONTENT        = 0x42;// 0xA1
    /**邮件删除*/
    const UInt8 MAIL_DELETE         = 0x43;// 0xA2
    /**邮件发送*/
    const UInt8 MAIL_SEND           = 0x44;// 0xA3
    /**邮件信息改变*/
    const UInt8 MAIL_CHANGE         = 0x46;// 0xA5
    /**活跃度请求*/
    const UInt8 ACTIVITY_LIST       = 0x4A; 
    /**活跃度领取奖励*/
    const UInt8 ACTIVITY_REWARD     = 0x4B;
    /**关系列表请求*/
    const UInt8 FRIEND_LIST         = 0xD8;// 0xA8
    /**关系列表操作*/
    const UInt8 FRIEND_ACTION       = 0xD9;// 0xA9
    /**商城信息请求*/
    const UInt8 STORE_LIST          = 0xDC;// 0xB0
    /**商城购买*/
    const UInt8 STORE_BUY           = 0xDD;// 0xB1
    /**请求交易列表信息*/
    const UInt8 TRADE_LIST          = 0x80;// 0xC0
    /**根据交易ID请求交易内容*/                
    const UInt8 TRADE_DATA          = 0x81;// 0xC1
    /**发起交易*/                              
    const UInt8 TRADE_LAUNCH        = 0x82;// 0xC2
    /**回复交易*/                              
    const UInt8 TRADE_REPLY         = 0x83;// 0xC3
    /**确认取消和删除交易状态操作*/            
    const UInt8 TRADE_OPERATE       = 0x84;// 0xC4
    /**请求寄售列表*/                          
    const UInt8 SALE_LIST           = 0x85;// 0xC5
    /**发起寄售*/                              
    const UInt8 SALE_SELL           = 0x86;// 0xC6
    /**购买或下架指定物品*/                    
    const UInt8 SALE_OP             = 0x87;// 0xC7
    /**英雄岛*/
    const UInt8 HERO_ISLAND         = 0x97;
    /**设置密码*/
    const UInt8 OP_PWD              = 0xCA;
    /***/
    const UInt8 PWD_QUESTION        = 0xCB;
    /***/
    const UInt8 PWD_LOCK            = 0xFA;// 0xCE
    /**斗剑场信息*/
    const UInt8 ARENA_INFO          = 0xC5;// 0xD0
    /**斗剑场挑战*/
    //const UInt8 ARENA_FIGHT_INFO    = 0xC6;// 0xD1
    /**???*/
    const UInt8 ATHLETICS_CHALLENGE = 0xC6;//0xF8;// 0xD2
    /**刷新历练*/
    const UInt8 ATHLETICS_REFRESH_MARTIAL = 0xC7;
    /**斗剑领取奖励*/
    const UInt8 ATHLETICS_GET_AWARD = 0xC8;

    /**斗剑翻页*/
    const UInt8 ATHLETICS_PAGING    = 0xC9;
    /**???*/
    const UInt8 ATTACK_BLOCKBOSS    = 0xD6;
    /**修炼地信息*/
    const UInt8 PLACE_INFO          = 0x30;// 0xE0
    /**修炼地翻页*/
    const UInt8 PRACTICE_PAGE_CHANGE= 0x31;// 0xE1
    /**修炼地挑战*/
    const UInt8 PLACE_ROB           = 0x32;// 0xE2
    /**修炼地占领*/
    const UInt8 PLACE_OCCUPY        = 0x33;// 0xE3
    /**开始修炼*/
    const UInt8 PRACTICE_START      = 0x34;// 0xE4
    /**停止修炼*/
    const UInt8 PRACTICE_STOP       = 0x35;// 0xE5
    /**修炼加速*/
    const UInt8 PRACTICE_HOOK_ADD   = 0x36;
    /**排行榜*/
    const UInt8 SORT_LIST           = 0xDA;// 0xE8
    /**玩家个人排行信息*/
    const UInt8 SORT_PERSON         = 0xDB;// 0xE9
    /**跨服战信息请求*/
    const UInt8 SERVER_ARENA_INFO   = 0xEA;
    /**请求对阵表*/
    const UInt8 SERVER_ARENA_ELIM   = 0xEB;
    /**???*/
    const UInt8 SERVER_ARENA_OP     = 0xEC;
    /**聊天*/
    const UInt8 CHAT                = 0x20;// 0xF0
    /**私聊*/
    const UInt8 WHISPER             = 0x21;// 0xF1
    /**寻找玩家*/
    const UInt8 CHECK_USER          = 0x22;// 0xF2
    /**炫耀*/
    const UInt8 FLAUNT              = 0x23;// 0xF3
    /**BUG投诉*/
    const UInt8 BUG                 = 0x25;// 0xF8
    /***领取国庆礼包*/
    const UInt8 YD_GETPACKS         = 0x2D;
    /**黄钻信息*/
    const UInt8 YD_INFO             = 0x2E;
    /**黄钻奖励领取*/
    const UInt8 YD_AWARD_RCV        = 0x2F;

    /**非战斗时信息请求*/
    const UInt8 CLAN_RANKBATTLE_REQINIT = 0x59;
    /**战斗时信息请求*/
    const UInt8 CLAN_RANKBATTLE_REQ     = 0x5A;
    /**帮会战排名列表*/
    const UInt8 CLAN_RANKBATTLE_SORTLIST = 0x5B;
    
    /** 法宝精炼*/
    const UInt8 EQ_TRUMP_UPGRADE    = 0xB7;
    /** 法宝升阶*/
    const UInt8 EQ_TRUMP_L_ORDER    = 0xB8;
    
    /***装备升级*/
    const UInt8 EQ_UPGRADE          = 0xB9;
    
    // const UInt8 GETBOX              = 0xD4;
    // const UInt8 BLOCKBOSS           = 0xD5;

    /** 组队副本*/
    const UInt8 TEAM_COPY_REQ       = 0x99;

    /** 战报数据*/
    const UInt8 FIGHT_REPORT2       = 0x0F;
}

namespace REP
{
    const UInt8 KEEP_ALIVE          = 0x00;// 0x00
    const UInt8 RECONNECT           = 0x01;// 帐号被封
    const UInt8 ENTER_ARENA         = 0x02;
    const UInt8 LINEUP_CHANGE       = 0x03;
    const UInt8 NEXT_ARENA          = 0x04;
    const UInt8 CHKMARK             = 0x05;
    const UInt8 ACHIEVEMENT         = 0xF0;// 0x0C
    const UInt8 CAMP_SELECT         = 0xF1;// 0x0D
    const UInt8 BE_DISCONNECT       = 0xF2;// 0x0E
    const UInt8 ALERT_MSG           = 0xF3;// 0x0F

    const UInt8 LOGIN               = 0xE0;// 0x10
    const UInt8 NEW_CHARACTER       = 0xE1;// 0x11
    const UInt8 WALLOW_VERIFY       = 0xE2;// 0x12
    const UInt8 GUIDE_RESPONSE_STEP = 0xE3;// 0x13
    const UInt8 USER_INFO           = 0xE4;// 0x14
    const UInt8 USER_INFO_CHANGE    = 0xE5;// 0x15
    const UInt8 STATE_CHANGE        = 0x16;
    const UInt8 BOOK_SHOP_LIST      = 0xEC;// 0x1A
    const UInt8 ARENAPRILIMINARY    = 0xE9;// 0xED
    const UInt8 BOOK_SHOP_BUY       = 0xED;// 0x1B
    const UInt8 RANK_DATA           = 0xEE;// 0x1D
    const UInt8 RANK_SETTING        = 0xEF;// 0x1E

    const UInt8 CHANGE_EQUIPMENT    = 0xD0;// 0x21
    const UInt8 HOTEL_PUB_HIRE      = 0xD1;// 0x22
    const UInt8 FIGHTER_INFO        = 0xD2;// 0x23
    const UInt8 HOTEL_PUB_LIST      = 0xD3;// 0x26
    const UInt8 FIGHTER_DISMISS     = 0xD4;// 0x27
    const UInt8 FIGHTER_ACCEPT      = 0xD5;// 0x28
    const UInt8 PUB_LIST            = 0x2A;
    const UInt8 POTENCIAL           = 0xDF;// 0x2C

    const UInt8 TRAIN_FIGHTER_OP    = 0x37;

    const UInt8 PACK_INFO           = 0xC0;// 0x30
    const UInt8 PACK_USE            = 0xC3;// 0x33
    const UInt8 PACK_EXTEND         = 0xC4;// 0x34
    const UInt8 REWARD_DRAW         = 0xCF;// 0x38
    const UInt8 TRIPOD_INFO         = 0xCE;// 0x39
    const UInt8 LUCKYDRAW_INFO      = 0x3D;
    const UInt8 LUCKYDRAW           = 0x3E;
    const UInt8 LUCKYDRAW_OTH       = 0x3F;

    const UInt8 EQ_TO_STRONG        = 0xB0;// 0x40
    const UInt8 EQ_TO_PUNCH         = 0xB1;// 0x41
    const UInt8 EQ_EMBED            = 0xB2;// 0x43
    const UInt8 EQ_UN_EMBED         = 0xB3;// 0x44
    const UInt8 EQ_UN_SINGLE        = 0x45;
    const UInt8 EQ_EXCHANGE         = 0x46;
    const UInt8 EQ_PURIFY           = 0xB4;// 0x47
    const UInt8 EQ_BATCH_DECOMPOSE  = 0xB5;// 0x49
    const UInt8 EQ_EXCHANGE_POS     = 0x4A;
    const UInt8 GEM_BATCH_UPGRADE   = 0xB6;// 0x4B
    const UInt8 EQ_ACTIVE           = 0x4C;
    const UInt8 EQ_TRUMP_UPGRADE    = 0xB7;
    const UInt8 EQ_TRUMP_L_ORDER    = 0xB8;
    const UInt8 EQ_UPGRADE          = 0xB9;

    const UInt8 CITY_INSIDE_MOVE    = 0xA0;// 0x51
    const UInt8 MAP_TRANSPORT       = 0xA1;// 0x52
    const UInt8 MAP_TRANSPORT_UPDATE= 0xA2;// 0x53
    const UInt8 MAP_SAMPLEUSER      = 0xA3;// 0x54
    const UInt8 MAP_POINT_JOIN      = 0xA4;// 0x55
    const UInt8 COPY_JOIN           = 0xAA;// 0x58
    const UInt8 COPY_DATA_UPDATE    = 0xAB;// 0x59
    const UInt8 COPY_FIGHT_RESULT   = 0xAC;// 0x5A
    const UInt8 COPY_AUTO_FIGHT     = 0xAD;// 0x5B
    const UInt8 COPY_END_FIGHT      = 0xAE;// 0x5C
    const UInt8 DAILY_DATA          = 0xAF;// 0x5F

    //const UInt8 CHANGE_NAME         = 0x60;
    const UInt8 COUNTRY_WAR_JOIN    = 0x90;// 0x62
    const UInt8 COUNTRY_WAR_PROCESS = 0x91;// 0x63
    const UInt8 COUNTRY_WAR_RESULT  = 0x92;// 0x64
    const UInt8 COUNTRY_WAR_STRING  = 0x93;// 0x66
    const UInt8 COPY_INFO           = 0x94;// 0x67
    const UInt8 FORMATTON_INFO      = 0x95;// 0x68
    const UInt8 AUTO_COPY           = 0x96;// ??
    const UInt8 AUTO_FRONTMAP       = 0x98;// ??
    const UInt8 FIGHT_START         = 0x9A;// 0x6C
    const UInt8 ATTACK_NPC          = 0x9B;// 0x61
    const UInt8 TASK_RESPONSE_HOOK  = 0x9D;// 0x6D

    const UInt8 LANCHCHALLENGE      = 0x72;
    const UInt8 REQUESTCHALLENGE    = 0x73;
    const UInt8 CLAN_TECH           = 0x78;
    const UInt8 CLAN_BATTLE         = 0x79;
    const UInt8 CLAN_OPEN           = 0x7A;
    const UInt8 CLAN_BATTLE_END     = 0x7B;
    const UInt8 CLAN_INFO           = 0x7C;
    const UInt8 CLAN_DINFO_UPDATE   = 0x7D;

    const UInt8 DIALOG_START        = 0x50;// 0x80
    const UInt8 DIALOG_INTERACTION  = 0x51;// 0x81
    const UInt8 GET_TASK_LIST       = 0x52;// 0x82
    const UInt8 GET_USABLE_TASK     = 0x53;// 0x83
    const UInt8 PLAYER_ABANDON_TASK = 0x54;// 0x85
    const UInt8 UPDATE_TASK_PROCESS = 0x55;// 0x86
    const UInt8 NEW_TASK            = 0x56;// 0x87

    const UInt8 CONVEYBATTLE        = 0x88;
    const UInt8 COLLECTNPCACTION    = 0x89;
    const UInt8 TASK_CYC_STATE      = 0x8A;

    const UInt8 TASK_CYC_FRESH      = 0x57;// 0x8B

    const UInt8 AUTO_COMPLETED_TASK = 0x8D;
    const UInt8 QUERYDAYTASK        = 0x8F;

    const UInt8 CLAN_REQ_LIST       = 0x60;// 0x90
    const UInt8 CLAN_REQ_USER       = 0x61;// 0x91
    const UInt8 CLAN_CREATE         = 0x62;// 0x92
    const UInt8 CLAN_JOIN_IN        = 0x63;// 0x93
    const UInt8 CLAN_MEMBER_OPERATE = 0x64;// 0x94
    const UInt8 CLAN_INFO_CHANGE    = 0x65;// 0x95
    const UInt8 CLAN_REQ_ITEM       = 0x66;// 0x96
    const UInt8 CLAN_MEMBER_LIST    = 0x67;// 0x97
    const UInt8 CLAN_INFO_UPDATE    = 0x68;// 0x98
    const UInt8 CLAN_SKILL          = 0x69;// 0x99
    const UInt8 CLAN_BUILD          = 0x6A;// 0x9B
    const UInt8 CLAN_PACKAGE        = 0x6B;//
    const UInt8 CLAN_PACKAGE_RECORD = 0x6C;

    const UInt8 HERO_ISLAND         = 0x97;
    const UInt8 CLANREWARD          = 0x9C;
    const UInt8 GET_CLANREWARD      = 0x9D;
    const UInt8 ALLOCATECLANREWARD  = 0x9E;
    const UInt8 CLANALLOCRECORD     = 0x9F;

    const UInt8 MAIL_ID_LIST        = 0x40;// 0xA6
    const UInt8 MAIL_LIST           = 0x41;// 0xA0
    const UInt8 MAIL_CONTENTS       = 0x42;// 0xA1
    const UInt8 MAIL_DELETE         = 0x43;// 0xA2
    const UInt8 MAIL_SEND           = 0x44;// 0xA3
    const UInt8 MAIL_NEW            = 0x45;// 0xA4
    const UInt8 MAIL_CHANGE         = 0x46;// 0xA5

    /**活跃度请求*/
    const UInt8 ACTIVITY_LIST       = 0x4A; 
    /**活跃度领取奖励*/
    const UInt8 ACTIVITY_REWARD     = 0x4B;
 
    const UInt8 FRIEND_LIST         = 0xD8;// 0xA8
    const UInt8 FRIEND_ACTION       = 0xD9;// 0xA9

    const UInt8 STORE_LIST          = 0xDC;// 0xB0
    const UInt8 STORE_BUY           = 0xDD;// 0xB1
    const UInt8 STORE_LIST_EXCHANGE = 0xDE;// ??

    const UInt8 TRADE_LIST          = 0x80;// 0xC0
    const UInt8 TRADE_DATA          = 0x81;// 0xC1
    const UInt8 TRADE_LAUNCH        = 0x82;// 0xC2
    const UInt8 TRADE_OPERATE       = 0x84;// 0xC4
    const UInt8 SALE_LIST           = 0x85;// 0xC5
    const UInt8 SALE_SELL           = 0x86;// 0xC6
    const UInt8 OP_PWD              = 0xCA;
    const UInt8 SECOND_PWD          = 0xCB;
    const UInt8 PWD_DAILOG          = 0xCC;
    const UInt8 PWD_LOCK            = 0xCE;

    const UInt8 ARENA_IFNO          = 0xC5;// 0xD0
    const UInt8 FIGHT_INFO_CHANGE   = 0xC6;// 0xD1

    const UInt8 PRACTICE_PLACE_IFNO = 0x30;// 0xE0
    const UInt8 PRACTICE_PAGE       = 0x31;// 0xE1
    const UInt8 PRACTICE_ROB        = 0x32;// 0xE2
    const UInt8 PRACTICE_OCCUPY     = 0x33;// 0xE3
    const UInt8 PRACTICE_START      = 0x34;// 0xE4
    const UInt8 PRACTICE_STOP       = 0x35;// 0xE5
    const UInt8 PRACTICE_HOOK_ADD   = 0x36;

    const UInt8 SORT_LIST           = 0xDA;// 0xE8
    const UInt8 SORT_PERSONAL       = 0xDB;// 0xE9
    const UInt8 SERVER_ARENA_INFO   = 0xEA;
    const UInt8 SERVER_ARENA_ELIM   = 0xEB;

    const UInt8 CHAT                = 0x20;// 0xF0
    const UInt8 CHAT_PRIVATE        = 0x21;// 0xF1
    const UInt8 FIND_USER           = 0x22;// 0xF2
    const UInt8 FLAUNT_GOOD         = 0x23;// 0xF3
    const UInt8 SYSTEM_INFO         = 0x24;// 0xF7
    const UInt8 BUG_INFO            = 0x25;// 0xF8
    const UInt8 YD_GETPACKS         = 0x2D;
    const UInt8 YD_INFO             = 0x2E;
    const UInt8 YD_AWARD_RCV        = 0x2F;

    const UInt8 SERVER_ARENA_OP     = 0xEC;

    const UInt8 ATHLETICS_CHALLENGE = 0xF5;// 0xD2
    const UInt8 WINSTREAK           = 0xF6;// 0xD3
    const UInt8 GETBOX              = 0xF7;// 0xD4
    const UInt8 BLOCKBOSS           = 0xF9;// 0xD5

    const UInt8 CLAN_RANKBATTLE_REPINIT = 0x59;  //非战斗时信息返回
    const UInt8 CLAN_RANKBATTLE_REP     = 0x5A;  //战斗时信息返回
    const UInt8 CLAN_RANKBATTLE_SORTLIST = 0x5B; //帮会战排名列表
    
    const UInt8 TEAM_COPY_REQ       = 0x99;

    /** 战报数据*/
    const UInt8 FIGHT_REPORT2       = 0x0F;
}

namespace SPEQ
{
    const UInt8 SOCK_ADDR           = 0x02;
    const UInt8 PLAYERIDAUTH        = 0xFC;// 0xFD
    const UInt8 WORLDANNOUNCE       = 0xFD;// 0xFE
    const UInt16 USERRECHARGE       = 0x105;// 0x100
    const UInt16 KILLUSER           = 0x104;// 0x101
    const UInt16 LOCKUSER           = 0x100;// 0x102
    const UInt16 UNLOCKUSER         = 0x101;// 0x103
    const UInt16 GMHANDLERFROMBS    = 0x102;// 0x104
    const UInt16 MAILFROMBS         = 0x107;// 0x105
    const UInt16 BANCHATFROMBS      = 0x103;// 0x106
    const UInt16 ADDITEMFROMBS      = 0x108;// 0x107
    const UInt16 BATTLEREPORT       = 0x106;// 0x110
    const UInt16 RERECHARGE         = 0x109;//
    const UInt16 ONLINE             = 0x10A;//
    const UInt16 SETLEVEL           = 0x10B;//
    const UInt16 ADDITEMTOALL       = 0x10C;//
    const UInt16 SETPROPS           = 0x10D;//
    const UInt16 SETMONEY           = 0x10E;//
    const UInt16 LOADLUA            = 0x10F;//
    const UInt16 SETVIPL            = 0x110;//
    const UInt16 CLSTASK            = 0x111;//
    const UInt16 SALE_ONOFF         = 0x112;// 交易开关
    const UInt16 PLAYERINFO         = 0x113;
    const UInt16 WBOSS              = 0x114;
    const UInt16 ONLINEPF           = 0x115;
    const UInt16 ADDITEMFROMBSBYID  = 0x116;
    const UInt16 ADDFIGHTER         = 0x117;
    const UInt16 GETMONEY           = 0x118;
}

namespace SPEP
{
    const UInt8 LOCKUSER            = 0x00;
    const UInt8 UNLOCKUSER          = 0x01;
    const UInt8 GMHANDLERFROMBS     = 0x02;
    const UInt8 USERRECHARGE        = 0x05;
    const UInt8 MAILFROMBS          = 0x07;
    const UInt8 BANCHATFROMBS       = 0x03;
    const UInt8 ADDITEMFROMBS       = 0x08;
    const UInt8 RERECHARGE          = 0x09;//
    const UInt8 ONLINE              = 0x0A;//
    const UInt8 SETLEVEL            = 0x0B;//
    const UInt8 ADDITEMTOALL        = 0x0C;//
    const UInt8 SETPROPS            = 0x0D;//
    const UInt8 SETMONEY            = 0x0E;//
    const UInt8 LOADLUA             = 0x0F;//
    const UInt8 SETVIPL             = 0x10;//
    const UInt8 CLSTASK             = 0x11;//
    const UInt8 PLAYERIDAUTH        = 0xFC;// 0xFD
    const UInt8 SALE_ONOFF          = 0x12;// 交易开关
    const UInt8 PLAYERINFO          = 0x13;
    const UInt8 WBOSS               = 0x14;
    const UInt8 ONLINEPF            = 0x15;
    const UInt8 ADDITEMFROMBSBYID   = 0x16;
    const UInt8 ADDFIGHTER          = 0x17;
    const UInt8 GETMONEY            = 0x18;
}

#endif // _MSGID_H_

