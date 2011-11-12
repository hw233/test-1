
alter table `skill_effect` add column `crrdam` varchar(255) NOT NULL DEFAULT '' after `addmag`; 
alter table `skill_effect` add column `addcrr` float(10,4) NOT NULL DEFAULT '0.0000' after `crrdam`; 

