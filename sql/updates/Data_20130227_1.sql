alter table `skill_effect` add column `hppec` float(10,4) NOT NULL DEFAULT '0.0000' COMMENT '最大生命值的百分比' after `efv`;
alter table `skill_effect` add column `maxhpdampec` float(10,4) NOT NULL DEFAULT '0.0000' COMMENT '最大生命值的百分比扣血上限（取最高攻击力的百分比）' after `hppec`;
