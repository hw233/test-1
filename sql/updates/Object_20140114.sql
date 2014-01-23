alter TABLE `married_couple` drop column `both_onlinetime`; 
UPDATE `married_couple` SET `pet_level` = 1 WHERE `pet_level` = 0;

