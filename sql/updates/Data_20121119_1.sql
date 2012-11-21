alter table `skill_effect` modify column `state` smallint(5) NOT NULL DEFAULT 0;
alter table `skill_effect` modify column `immune` smallint(5) NOT NULL DEFAULT 0;
alter table `skill_effect` modify column `disperse` smallint(5) NOT NULL DEFAULT 0;


alter table `skill_effect` add column `efv` varchar(255) NOT NULL DEFAULT '' after `magatkreduce`;
alter table `skill_effect` add column `efl` varchar(255) NOT NULL DEFAULT '' after `magatkreduce`;
alter table `skill_effect` add column `eft` varchar(255) NOT NULL DEFAULT '' after `magatkreduce`;

