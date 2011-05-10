UPDATE `clan` SET `rank` = (SELECT CASE
WHEN 60 <= `fighter`.`level` && `fighter`.`level` < 70 THEN 2
WHEN 70 <= `fighter`.`level` && `fighter`.`level` < 75 THEN 3
WHEN 75 <= `fighter`.`level` && `fighter`.`level` < 80 THEN 4
WHEN 80 <= `fighter`.`level` && `fighter`.`level` < 82 THEN 5
WHEN 82 <= `fighter`.`level` && `fighter`.`level` < 84 THEN 6
WHEN 84 <= `fighter`.`level` && `fighter`.`level` < 86 THEN 7
WHEN 86 <= `fighter`.`level` && `fighter`.`level` < 88 THEN 8
WHEN 88 <= `fighter`.`level` && `fighter`.`level` < 90 THEN 9
WHEN 90 <= `fighter`.`level` && `fighter`.`level` < 92 THEN 10
WHEN 92 <= `fighter`.`level` && `fighter`.`level` < 94 THEN 11
WHEN 94 <= `fighter`.`level` && `fighter`.`level` < 96 THEN 12
WHEN 96 <= `fighter`.`level` && `fighter`.`level` < 98 THEN 13
WHEN 98 <= `fighter`.`level` && `fighter`.`level` < 100 THEN 14
WHEN 100 <= `fighter`.`level`  THEN 15
ELSE 1
END
FROM `fighter`
JOIN `player` on `fighter`.`id` <= 6 and `player`.`id` = `fighter`.`playerId`
ORDER BY level DESC
LIMIT 19, 1) WHERE `id` = 0;
