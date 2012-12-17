alter table `job_hunter` add column `slotVal1` tinyint(3) unsigned NOT NULL DEFAULT '0'after `stepCount`;
alter table `job_hunter` add column `slotVal2` tinyint(3) unsigned NOT NULL DEFAULT '0'after `slotVal1`;
alter table `job_hunter` add column `slotVal3` tinyint(3) unsigned NOT NULL DEFAULT '0'after `slotVal2`;
