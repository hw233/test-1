alter table `sh_global` add column `timeEnd` int(11) NOT NULL DEFAULT '0' after `progress`;
alter table `sh_global` add column `timeBegin` int(11) NOT NULL DEFAULT '0' after `progress`;
alter table `sh_global` add column `open` tinyint(3) NOT NULL DEFAULT '0' after `progress`;
