
alter table `clan_skill_template` drop column `hp`;
alter table `clan_skill_template` drop column `attack`;
alter table `clan_skill_template` drop column `defend`;
alter table `clan_skill_template` drop column `magatk`;
alter table `clan_skill_template` drop column `magdef`;

alter table `clan_skill_template` add column `value`  float(10,4) NOT NULL after `needs`;

