
namespace REQ
{
    /**保持连接*/
    const UInt8 KEEP_ALIVE          = 0x00;
    /**自动重连*/
    const UInt8 RECONNECT           = 0x01;
    /**成就*/
    const UInt8 ACHIEVEMENT         = 0x0C;
    /**阵营选择*/
    const UInt8 CAMPS_CHOICE        = 0x0D;
    /**登陆*/
    const UInt8 LOGIN               = 0x10;
    /**创建角色*/
    const UInt8 CREATE_ROLE         = 0x11;
    /**新手引导步骤*/
    const UInt8 NEW_HAND_STEP       = 0x13;
    /**请求玩家信息*/
    const UInt8 USER_INFO           = 0x14;
    /**BUFFER信息改变*/
    const UInt8 STATE               = 0x15;
    /** ??? */
    const UInt8 STATE_CHANGE        = 0x16;
    /**查询书商*/
    const UInt8 BOOK_LIST           = 0x1A;
    /**买书*/
    const UInt8 BOOK_BUY            = 0x1B;
    /**读阵型*/
    const UInt8 READ_BATTLE         = 0x1D;
    /**改变阵型*/
    const UInt8 BATTLE_CHECK        = 0x1E;
    /**装备改变*/
    const UInt8 CHANGE_EQ           = 0x21;
    /**雇用散仙*/
    const UInt8 HIRE_HERO           = 0x22;
    /**散仙信息*/
    const UInt8 HERO_INFO           = 0x23;
    /**酒馆列表*/
    const UInt8 HOTEL_LIST          = 0x26;
    /**解雇散仙*/
    const UInt8 FIRE_HERO           = 0x27;
    /**接收散仙*/
    const UInt8 ACCEPT_FIGHTER      = 0x28;
    /**加血*/
    const UInt8 ADD_HP              = 0x29;
    /**潜力洗炼*/
    const UInt8 POTENTIAL           = 0x2C;
    /**请求修炼武将列表*/
    const UInt8 TRAIN_FIGHTER_LIST  = 0x2D;
    /**加速取消修炼散仙*/
    const UInt8 TRAIN_FIGHTER_OP    = 0x2E;
    /**背包列表信息*/
    const UInt8 PACK_INFO           = 0x30;
    /**背包添加到九疑鼎*/
    const UInt8 PACK_TRIPOD         = 0x31;
    /**背包卖出*/
    const UInt8 PACK_SELL           = 0x32;
    /**背包使用*/
    const UInt8 PACK_USE            = 0x33;
    /**背包扩展*/
    const UInt8 PACK_EXTEND         = 0x34;
    /**在线奖励*/
    const UInt8 REWARD              = 0x38;
    /**九疑鼎*/
    const UInt8 TRIPOD_INFO         = 0x39;
    /**探险信息*/
    const UInt8 LUCKYDRAW_INFO      = 0x3D;
    /**探险*/
    const UInt8 LUCKYDRAW           = 0x3E;
    /**装备强化*/
    const UInt8 EQ_ENHANCE          = 0x40;
    /**装备打孔*/
    const UInt8 EQ_PUNCH            = 0x41;
    const UInt8 MERGE_GEM           = 0x42;
    /**宝石镶嵌*/
    const UInt8 EQ_EMBED            = 0x43;
    /**宝石拆卸*/
    const UInt8 EQ_UN_EMBED         = 0x44;
    /**单个分解*/
    const UInt8 EQ_UN_SINGLE        = 0x45;
    /**转换装备*/
    const UInt8 EQ_EXCHANGE         = 0x46;
    /**装备洗炼*/
    const UInt8 EQ_PURIFY           = 0x47;
    /**批量分解*/
    const UInt8 EQ_DECOMPOSE        = 0x49;
    /**置换部位*/
    const UInt8 EQ_EXCHANGE_POS     = 0x4A;
    /**宝石批量合成*/
    const UInt8 GEM_UPGRADE         = 0x4B;
    /**装备激活 */
    const UInt8 EQ_ACTIVE           = 0x4C;
    const UInt8 CITY_OUTSIDE_MOVE   = 0x50;
    /**地图获取据点信息*/
    const UInt8 MAP_LOCATE          = 0x51;
    /**地图传送*/
    const UInt8 MAP_TRANSPORT       = 0x52;
    /**进入离开通天塔*/
    const UInt8 BABEL_JOIN          = 0x58;
    /**请求通天塔信息*/
    const UInt8 BABEL_UPDATE        = 0x59;
    /**开始通天塔战斗*/
    const UInt8 BABEL_START         = 0x5A;
    /**开始自动通天塔战斗*/
    const UInt8 BABEL_AUTO_START    = 0x5B;
    /**立即完成通天塔战斗*/
    const UInt8 BABEL_END           = 0x5C;
    /**日常*/
    const UInt8 DAILY               = 0x5F;
    /**换名字*/
    const UInt8 CHANGE_NAME         = 0x60;
    /**打怪*/
    const UInt8 ATTACK_NPC          = 0x61;
    /**加入国战*/
    const UInt8 CAMPS_WAR_JOIN      = 0x62;
    /**副本数据*/
    const UInt8 COPY_DATA           = 0x67;
    /**阵图数据*/
    const UInt8 FORMATION_DATA      = 0x68;
    /**请求战报*/
    const UInt8 FIGHT_REPORT        = 0x6C;
    /**挂机*/
    const UInt8 TASK_HOOK           = 0x6D;
    /**挂机加速*/
    const UInt8 TASK_HOOK_ADD       = 0x6F;
    /**挂机停止*/
    const UInt8 TASK_HOOK_STOP      = 0x6E;
    /**点击NPC对话*/
    const UInt8 DIALOG_START        = 0x80;
    /**发起切磋请求*/
    const UInt8 LANCHCHALLENGE      = 0x72;
    /**回复切磋*/
    const UInt8 REQUESTCHALLENGE    = 0x73;
    /**战斗退出*/
    const UInt8 FIGHT_EXIT          = 0x77;
    /**帮派战*/
    const UInt8 CLAN_BATTLE         = 0x79;
    /***/
    const UInt8 CLANCITYBATTLELIST  = 0x7A;
    /**帮派动态信息*/
    const UInt8 CLAN_INFO           = 0x7C;
    /**获取战报信息请求*/
    const UInt8 CLAN_BATTLE_INFO    = 0x7D;
    /**点击交互动作*/
    const UInt8 NPC_INTERACT        = 0x81;
    /**当前任务列表请求*/
    const UInt8 CURR_TASK_LIST      = 0x82;
    /**当前可接任务列表请求*/
    const UInt8 CURR_AVAILABLE_TASK = 0x83;
    /**玩家接受、提交、放弃某任务请求*/
    const UInt8 TASK_ACTION         = 0x85;
    /**护送任务玩家到达可触发战斗据点*/
    const UInt8 CONVEYBATTLE        = 0x88;
    /**采集任务物品请求*/
    const UInt8 COLLECTNPCACTION    = 0x89;
    /**循环任务刷新*/
    const UInt8 TASK_CYC_REFRESH    = 0x8B;
    /**自动完成日常任务*/
    const UInt8 DAYTASKAUTOCOMPLETED= 0x8C;
    /**循环任务进度请求*/
    const UInt8 QUERYDAYTASK        = 0x8F;
    /**请求帮派列表信息*/
    const UInt8 CLAN_LIST           = 0x90; 
    /**请求自己帮派信息*/
    const UInt8 CLAN_SELF           = 0x91;
    /**帮派创建*/
    const UInt8 CLAN_CREATE         = 0x92; 
    /**申请加入帮派*/
    const UInt8 CLAN_APPLY          = 0x93; 
    /**帮派成员操作*/
    const UInt8 CLAN_OPERATE        = 0x94; 
    /**帮派信息改变*/
    const UInt8 CLAN_INFO_CHANGE    = 0x95; 
    /**请求某个帮派信息*/
    const UInt8 CLAN_REQ_OTHER      = 0x96; 
    /**请求帮派成员列表信息*/
    const UInt8 CLAN_PLAYER_LIST    = 0x97; 
    /**请求帮派科技信息*/
    const UInt8 CLAN_SKILL          = 0x99; 
    /**帮派驻地信息*/
    const UInt8 CLAN_BUILD          = 0x9B;
    /**请求宗族奖励物品*/
    const UInt8 CLANREWARD          = 0x9C;
    /**领取宗族奖励*/
    const UInt8 GET_CLANREWARD      = 0x9D;
    /**分配宗族奖励物品*/
    const UInt8 ALLOCATECLANREWARD  = 0x9E;
    /**宗族分配记录请求*/
    const UInt8 CLANALLOCRECORD     = 0x9F;
    /**邮件ID列表*/
    const UInt8 MAIL_ID_LIST        = 0xA6;
    /**邮件信息列表*/
    const UInt8 MAIL_LIST           = 0xA0;
    /**邮件内容*/
    const UInt8 MAIL_CONTENT        = 0xA1;
    /**邮件删除*/
    const UInt8 MAIL_DELETE         = 0xA2;
    /**邮件发送*/
    const UInt8 MAIL_SEND           = 0xA3;
    /**邮件信息改变*/
    const UInt8 MAIL_CHANGE         = 0xA5;
    /**关系列表请求*/
    const UInt8 FRIEND_LIST         = 0xA8;
    /**关系列表操作*/
    const UInt8 FRIEND_ACTION       = 0xA9;
    /**好友动态*/
    const UInt8 FRIEND_STATUS       = 0xAB;
    /**商城信息请求*/
    const UInt8 STORE_LIST          = 0xB0;
    /**商城购买*/
    const UInt8 STORE_BUY           = 0xB1;
    /**请求交易列表信息*/
    const UInt8 TRADE_LIST          = 0xC0;
    /**根据交易ID请求交易内容*/
    const UInt8 TRADE_DATA          = 0xC1 ;
    /**发起交易*/
    const UInt8 TRADE_LAUNCH        = 0xC2;
    /**回复交易*/
    const UInt8 TRADE_REPLY         = 0xC3;
    /**确认取消和删除交易状态操作*/
    const UInt8 TRADE_OPERATE       = 0xC4;
    /**请求寄售列表*/
    const UInt8 SALE_LIST           = 0xC5;
    /**发起寄售*/
    const UInt8 SALE_SELL           = 0xC6;
    /**购买或下架指定物品*/
    const UInt8 SALE_OP             = 0xC7;
    /**设置密码*/
    const UInt8 OP_PWD              = 0xCA;
    /***/
    const UInt8 PWD_QUESTION        = 0xCB;
    /***/
    const UInt8 PWD_LOCK            = 0xCE;
    /**斗剑场信息*/
    const UInt8 ARENA_INFO          = 0xD0;
    /**斗剑场挑战*/
    const UInt8 ARENA_FIGHT_INFO    = 0xD1;
    /**修炼地信息*/
    const UInt8 PLACE_INFO          = 0xE0;
    /**修炼地翻页*/
    const UInt8 PRACTICE_PAGE_CHANGE= 0xE1;
    /**修炼地挑战*/
    const UInt8 PLACE_ROB           = 0xE2;
    /**修炼地占领*/
    const UInt8 PLACE_OCCUPY        = 0xE3;
    /**开始修炼*/
    const UInt8 PRACTICE_START      = 0xE4;
    /**停止修炼*/
    const UInt8 PRACTICE_STOP       = 0xE5;
    /**排行榜*/
    const UInt8 SORT_LIST           = 0xE8;
    /**玩家个人排行信息*/
    const UInt8 SORT_PERSON         = 0xE9;
    /**跨服战信息请求*/
    const UInt8 SERVER_ARENA_INFO   = 0xEA;
    /**请求对阵表*/
    const UInt8 SERVER_ARENA_ELIM   = 0xEB;
    /**聊天*/
    const UInt8 CHAT                = 0xF0;
    /**私聊*/
    const UInt8 WHISPER             = 0xF1;
    /**寻找玩家*/
    const UInt8 CHECK_USER          = 0xF2;
    /**炫耀*/
    const UInt8 FLAUNT              = 0xF3;
    /**BUG投诉*/
    const UInt8 BUG                 = 0xF8;
}

namespace SPEQ
{
    //const UInt8 LOCKUSER            = 0x102;
    const UInt8 SOCK_ADDR           = 0x02;
    const UInt8 PLAYERIDAUTH        = 0xFD;
    const UInt8 WORLDANNOUNCE       = 0xFE;
    const UInt16 USERRECHARGE       = 0x100;
    const UInt16 KILLUSER           = 0x101;
    const UInt16 LOCKUSER           = 0x102;
    const UInt16 UNLOCKUSER         = 0x103;
    const UInt16 GMHANDLERFROMBS    = 0x104;
    const UInt16 MAILFROMBS         = 0x105;
    const UInt16 BANCHATFROMBS      = 0x106;
    const UInt16 ADDITEMFROMBS      = 0x107;
    const UInt16 BATTLEREPORT       = 0x110;
}

