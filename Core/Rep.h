
namespace REP
{
    const UInt8 KEEP_ALIVE          = 0x00;
    const UInt8 RECONNECT           = 0x01;
    const UInt8 ENTER_ARENA         = 0x02;
    const UInt8 LINEUP_CHANGE       = 0x03;
    const UInt8 ACHIEVEMENT         = 0x0C;
    const UInt8 CAMP_SELECT         = 0x0D;
    const UInt8 BE_DISCONNECT       = 0x0E;
    const UInt8 ALERT_MSG           = 0x0F;

    const UInt8 LOGIN               = 0x10;
    const UInt8 NEW_CHARACTER       = 0x11;
    const UInt8 WALLOW_VERIFY       = 0x12;
    const UInt8 GUIDE_RESPONSE_STEP = 0x13;
    const UInt8 USER_INFO           = 0x14;
    const UInt8 USER_INFO_CHANGE    = 0x15;
    const UInt8 STATE_CHANGE        = 0x16;
    const UInt8 BOOK_SHOP_LIST      = 0x1A;
    const UInt8 BOOK_SHOP_BUY       = 0x1B;
    const UInt8 RANK_DATA           = 0x1D;
    const UInt8 RANK_SETTING        = 0x1E;

    const UInt8 CHANGE_EQUIPMENT    = 0x21;
    const UInt8 HOTEL_PUB_HIRE      = 0x22;
    const UInt8 FIGHTER_INFO        = 0x23;
    const UInt8 HOTEL_PUB_LIST      = 0x26;
    const UInt8 FIGHTER_DISMISS     = 0x27;
    const UInt8 FIGHTER_ACCEPT      = 0x28;
    //const UInt8 PUB_LIST            = 0x2A;
    const UInt8 POTENCIAL           = 0x2C;
    const UInt8 TRAIN_FIGHTER_LIST  = 0x2D;
    const UInt8 TRAIN_FIGHTER_OP    = 0x2E;

    const UInt8 PACK_INFO           = 0x30;
    const UInt8 PACK_USE            = 0x33;
    const UInt8 PACK_EXTEND         = 0x34;
    const UInt8 REWARD_DRAW         = 0x38;
    const UInt8 TRIPOD_INFO         = 0x39;
    const UInt8 LUCKYDRAW_INFO      = 0x3D;
    const UInt8 LUCKYDRAW           = 0x3E;
    const UInt8 LUCKYDRAW_OTH       = 0x3F;

    const UInt8 EQ_TO_STRONG        = 0x40;
    const UInt8 EQ_TO_PUNCH         = 0x41;
    const UInt8 MERGE_GEM           = 0x42;
    const UInt8 EQ_EMBED            = 0x43;
    const UInt8 EQ_UN_EMBED         = 0x44;
    const UInt8 EQ_UN_SINGLE        = 0x45;
    const UInt8 EQ_EXCHANGE         = 0x46;
    const UInt8 EQ_PURIFY           = 0x47;
    const UInt8 EQ_BATCH_DECOMPOSE  = 0x49;
    const UInt8 EQ_EXCHANGE_POS     = 0x4A;
    const UInt8 GEM_BATCH_UPGRADE   = 0x4B;
    const UInt8 EQ_ACTIVE           = 0x4C;

    const UInt8 CITY_OUTSIDE_MOVE   = 0x50;
    const UInt8 CITY_INSIDE_MOVE    = 0x51;
    const UInt8 MAP_TRANSPORT       = 0x52;
    const UInt8 MAP_TRANSPORT_UPDATE= 0x53;
    const UInt8 MAP_SAMPLEUSER      = 0x54;
    const UInt8 MAP_POINT_JOIN      = 0x55;

    const UInt8 COPY_JOIN           = 0x58;
    const UInt8 COPY_DATA_UPDATE    = 0x59;
    const UInt8 COPY_FIGHT_RESULT   = 0x5A;
    const UInt8 COPY_AUTO_FIGHT     = 0x5B;
    const UInt8 COPY_END_FIGHT      = 0x5C;

    const UInt8 DAILY_DATA          = 0x5F;

    const UInt8 CHANGE_NAME         = 0x60;
    const UInt8 ATTACK_NPC          = 0x61;
    const UInt8 COUNTRY_WAR_JOIN    = 0x62;
    const UInt8 COUNTRY_WAR_PROCESS = 0x63;
    const UInt8 COUNTRY_WAR_RESULT  = 0x64;
    const UInt8 COUNTRY_WAR_STRING  = 0x66;
    const UInt8 COPY_INFO           = 0x67;
    const UInt8 FORMATTON_INFO      = 0x68;
    const UInt8 FIGHT_START         = 0x6C;
    const UInt8 TASK_RESPONSE_HOOK  = 0x6D;

    const UInt8 LANCHCHALLENGE      = 0x72;
    const UInt8 REQUESTCHALLENGE    = 0x73;
    const UInt8 CLAN_TECH           = 0x78;
    const UInt8 CLAN_BATTLE         = 0x79;
    const UInt8 CLAN_OPEN           = 0x7A;
    const UInt8 CLAN_BATTLE_END     = 0x7B;
    const UInt8 CLAN_INFO           = 0x7C;
    const UInt8 CLAN_DINFO_UPDATE   = 0x7D;

    const UInt8 DIALOG_START        = 0x80;
    const UInt8 DIALOG_INTERACTION  = 0x81;
    const UInt8 GET_TASK_LIST       = 0x82;
    const UInt8 GET_USABLE_TASK     = 0x83;
    const UInt8 PLAYER_ABANDON_TASK = 0x85;
    const UInt8 UPDATE_TASK_PROCESS = 0x86;
    const UInt8 NEW_TASK            = 0x87;
    const UInt8 CONVEYBATTLE        = 0x88;
    const UInt8 COLLECTNPCACTION    = 0x89;
    const UInt8 TASK_CYC_STATE      = 0x8A;
    const UInt8 TASK_CYC_FRESH      = 0x8B;
    const UInt8 QUERYDAYTASK        = 0x8F;

    const UInt8 CLAN_REQ_LIST       = 0x90;
    const UInt8 CLAN_REQ_USER       = 0x91;
    const UInt8 CLAN_CREATE         = 0x92;
    const UInt8 CLAN_JOIN_IN        = 0x93;
    const UInt8 CLAN_MEMBER_OPERATE = 0x94;
    const UInt8 CLAN_INFO_CHANGE    = 0x95;
    const UInt8 CLAN_REQ_ITEM       = 0x96;
    const UInt8 CLAN_MEMBER_LIST    = 0x97;
    const UInt8 CLAN_INFO_UPDATE    = 0x98;
    const UInt8 CLAN_SKILL          = 0x99;
    const UInt8 CLAN_BUILD          = 0x9B;
    const UInt8 CLANREWARD          = 0x9C;
    const UInt8 GET_CLANREWARD      = 0x9D;
    const UInt8 ALLOCATECLANREWARD  = 0x9E;
    const UInt8 CLANALLOCRECORD     = 0x9F;

    const UInt8 MAIL_LIST           = 0xA0;
    const UInt8 MAIL_CONTENTS       = 0xA1;
    const UInt8 MAIL_DELETE         = 0xA2;
    const UInt8 MAIL_SEND           = 0xA3;
    const UInt8 MAIL_NEW            = 0xA4;
    const UInt8 MAIL_CHANGE         = 0xA5;
    const UInt8 MAIL_ID_LIST        = 0xA6;
    const UInt8 FRIEND_LIST         = 0xA8;
    const UInt8 FRIEND_ACTION       = 0xA9;
    const UInt8 FRIEND_STATUS       = 0xAB;

    const UInt8 STORE_LIST          = 0xB0;
    const UInt8 STORE_BUY           = 0xB1;

    const UInt8 TRADE_LIST          = 0xC0;
    const UInt8 TRADE_DATA          = 0xC1;
    const UInt8 TRADE_LAUNCH        = 0xC2;
    const UInt8 TRADE_OPERATE       = 0xC4;
    const UInt8 SALE_LIST           = 0xC5;
    const UInt8 SALE_SELL           = 0xC6;
    const UInt8 OP_PWD              = 0xCA;
    const UInt8 SECOND_PWD          = 0xCB;
    const UInt8 PWD_DAILOG          = 0xCC;
    const UInt8 PWD_LOCK            = 0xCE;

    const UInt8 ARENA_IFNO          = 0xD0;
    const UInt8 FIGHT_INFO          = 0xD1;

    const UInt8 PRACTICE_PLACE_IFNO = 0xE0;
    const UInt8 PRACTICE_PAGE       = 0xE1;
    const UInt8 PRACTICE_ROB        = 0xE2;
    const UInt8 PRACTICE_OCCUPY     = 0xE3;
    const UInt8 PRACTICE_START      = 0xE4;
    const UInt8 PRACTICE_STOP       = 0xE5;
    const UInt8 SORT_LIST           = 0xE8;
    const UInt8 SORT_PERSONAL       = 0xE9;
    const UInt8 SERVER_ARENA_INFO   = 0xEA;
    const UInt8 SERVER_ARENA_ELIM   = 0xEB;

    const UInt8 CHAT                = 0xF0;
    const UInt8 CHAT_PRIVATE        = 0xF1;
    const UInt8 FIND_USER           = 0xF2;
    const UInt8 FLAUNT_GOOD         = 0xF3;
    const UInt8 SYSTEM_INFO         = 0xF7;
    const UInt8 BUG_INFO            = 0xF8;
}

namespace SPEP
{
    const UInt8 LOCKUSER            = 0x02;
    const UInt8 UNLOCKUSER          = 0x03;
    const UInt8 GMHANDLERFROMBS     = 0x04;
    const UInt8 MAILFROMBS          = 0x05;
    const UInt8 BANCHATFROMBS       = 0x06;
    const UInt8 ADDITEMFROMBS       = 0x07;
}

