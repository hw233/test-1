
alter table `item_template` add column `vLev` smallint(6) NOT NULL DEFAULT '1' COMMENT '价值等级' after reqLev;

