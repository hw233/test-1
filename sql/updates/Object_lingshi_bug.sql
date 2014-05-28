 update lingshiAttr set exp = 324 where level > 0 and level < 9;
 update lingshiAttr set exp = 2128 where level > 9 and level < 19;
 update lingshiAttr set exp = 6612 where level > 19 and level < 29;
 update lingshiAttr set exp = 14976 where level > 29 and level < 39;
 update lingshiAttr set exp = 28420 where level > 39 and level < 49;
 update lingshiAttr set exp = 48144 where level > 49 and level < 59;
 update lingshiAttr set exp = 75348 where level > 59 and level < 69;
 update lingshiAttr set exp = 111232 where level > 69 and level < 79;
 update lingshiAttr set exp = 155998 where level > 79 and level < 89;
 update lingshiAttr set level = level - (level % 10) + 9 where (level % 10) != 9 and level < 90;

