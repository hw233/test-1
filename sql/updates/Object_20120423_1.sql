alter table `recharge` add column `time` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP after `status`;
