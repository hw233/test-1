ALTER TABLE `task_instance` DROP  PRIMARY KEY;
ALTER TABLE `task_instance` ADD CONSTRAINT PK_task_instance PRIMARY KEY(`taskId`, `ownerId`);