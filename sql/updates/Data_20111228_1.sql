
alter table `skill_effect` add column `atkreduce`  float(10,4) NOT NULL DEFAULT '0.0000'  after `magres`;
alter table `skill_effect` add column `magatkreduce`  float(10,4) NOT NULL DEFAULT '0.0000'  after `atkreduce`;

