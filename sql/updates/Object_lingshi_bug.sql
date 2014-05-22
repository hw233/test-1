 update lingshiAttr,fighter set lingshiAttr.exp = 324 where lingshiAttr.level > 0 and lingshiAttr.level < 9 and fighter.level >= 85 and fighter.lingshi like concat("%",lingshiAttr.id,"%");

 update lingshiAttr,fighter set lingshiAttr.exp = 2128 where lingshiAttr.level > 9 and lingshiAttr.level < 19 and fighter.level >= 85 and fighter.lingshi like concat("%",lingshiAttr.id,"%");

 update lingshiAttr,fighter set lingshiAttr.exp = 6612 where lingshiAttr.level > 19 and lingshiAttr.level < 29 and fighter.level >= 85 and fighter.lingshi like concat("%",lingshiAttr.id,"%");

 update lingshiAttr,fighter set lingshiAttr.exp = 14976 where lingshiAttr.level > 29 and lingshiAttr.level < 39 and fighter.level >= 85 and fighter.lingshi like concat("%",lingshiAttr.id,"%");

 update lingshiAttr,fighter set lingshiAttr.exp = 28420 where lingshiAttr.level > 39 and lingshiAttr.level < 49 and fighter.level >= 85 and fighter.lingshi like concat("%",lingshiAttr.id,"%");

 update lingshiAttr,fighter set lingshiAttr.exp = 48144 where lingshiAttr.level > 49 and lingshiAttr.level < 59 and fighter.level >= 85 and fighter.lingshi like concat("%",lingshiAttr.id,"%");

 update lingshiAttr,fighter set lingshiAttr.exp = 75348 where lingshiAttr.level > 59 and lingshiAttr.level < 69 and fighter.level >= 85 and fighter.lingshi like concat("%",lingshiAttr.id,"%");

 update lingshiAttr,fighter set lingshiAttr.exp = 111232 where lingshiAttr.level > 69 and lingshiAttr.level < 79 and fighter.level >= 85 and fighter.lingshi like concat("%",lingshiAttr.id,"%");

 update lingshiAttr,fighter set lingshiAttr.exp = 155998 where lingshiAttr.level > 79 and lingshiAttr.level < 89 and fighter.level >= 85 and fighter.lingshi like concat("%",lingshiAttr.id,"%");

 update lingshiAttr,fighter set lingshiAttr.level = lingshiAttr.level - (lingshiAttr.level % 10) + 9 where fighter.level >= 85 and (lingshiAttr.level % 10) != 9 and fighter.lingshi like concat("%",lingshiAttr.id,"%");
