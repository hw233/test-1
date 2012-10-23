
CREATE TABLE IF NOT EXISTS `sh_global` (                
    `session` int(11) NOT NULL DEFAULT '1',
    `nextTime` int(11) NOT NULL DEFAULT '0',
    `progress` tinyint(3) NOT NULL DEFAULT '0',
    `cls1TowerLevel` smallint(6) NOT NULL DEFAULT '0',
    `cls2TowerLevel` smallint(6) NOT NULL DEFAULT '0',
    `cls3TowerLevel` smallint(6) NOT NULL DEFAULT '0'
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


CREATE TABLE IF NOT EXISTS `sh_stage_progress` (                
    `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `gpId` tinyint(3) NOT NULL DEFAULT '0',
    `cls` tinyint(3) NOT NULL DEFAULT '0',
    `round` tinyint(3) NOT NULL DEFAULT '0',
    `progress` tinyint(3) NOT NULL DEFAULT '0',
    PRIMARY KEY (`type`, `gpId`, `cls`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `sh_final_player` (                
    `gpId` tinyint(3) NOT NULL DEFAULT '0',
    `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `playerId` bigint(20) unsigned NOT NULL,
    `pos` tinyint(3) NOT NULL DEFAULT '0',
    `final3pos` tinyint(3) NOT NULL DEFAULT '0',
    `stage_idx` tinyint(3) NOT NULL DEFAULT '0',
    `cls` tinyint(3) NOT NULL DEFAULT '0',
    PRIMARY KEY (`gpId`, `type`, `playerId`, `cls`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `sh_elimination` (                
    `gpId` tinyint(3) NOT NULL DEFAULT '0',
    `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `cls` tinyint(3) NOT NULL DEFAULT '0',
    `stage_idx` tinyint(3) NOT NULL DEFAULT '0',
    `id` tinyint(3) NOT NULL DEFAULT '0',
    `won` varchar(255) NOT NULL DEFAULT '',
    `battleId` varchar(255) NOT NULL DEFAULT '',
    PRIMARY KEY (`gpId`, `type`, `cls`, `stage_idx`, `id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `sh_preliminary` (                
    `gpId` tinyint(3) NOT NULL DEFAULT '0',
    `cls` tinyint(3) NOT NULL DEFAULT '0',
    `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `won` tinyint(3) NOT NULL DEFAULT '0',
    `player1` bigint(20) unsigned NOT NULL,
    `player2` bigint(20) unsigned NOT NULL,
    `battleTime` int(11) NOT NULL,
    `battleId` int(11) NOT NULL,
    PRIMARY KEY (`gpId`, `cls`, `type`, `battleId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `sh_towerbattle` (                
    `player` bigint(20) unsigned NOT NULL,
    `cls` tinyint(3) NOT NULL DEFAULT '0',
    `won` tinyint(3) NOT NULL DEFAULT '0',
    `level` smallint(6) NOT NULL DEFAULT '0',
    `turns` int(11) NOT NULL DEFAULT '0',
    `battleId` int(11) NOT NULL,
    `npcId` int(11) NOT NULL,
    PRIMARY KEY (`player`, `cls`, `battleId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `sh_player` (
    `playerId` bigint(20) unsigned NOT NULL,
    `entered` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `teamId` int(11) NOT NULL DEFAULT '0',
    `tLevel` smallint(6) NOT NULL DEFAULT '0',
    `tTurns` int(11) NOT NULL DEFAULT '0',
    `tLastTurns` smallint(6) NOT NULL DEFAULT '0',
    `score` int(11) NOT NULL,
    PRIMARY KEY (`playerId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `sh_fighter` (
    `id` int(11) NOT NULL,
    `playerId` bigint(20) unsigned NOT NULL,
    `potential` float(5,2) NOT NULL DEFAULT '1.00',
    `capacity` float(5,2) NOT NULL DEFAULT '1.00',
    `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `level` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `soulMax` int(11) NOT NULL,
    `citta` varchar(255) NOT NULL,
    `trump` varchar(255) NOT NULL,
    `skillstrengthen` varchar(255) NOT NULL,
    PRIMARY KEY (`id`, `playerId`, `type`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `sh_fighter_attr_extra` (
    `fighterId` int(10) NOT NULL,
    `playerId` bigint(20) unsigned NOT NULL,
    `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `strength` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `physique` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `agility` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `intelligence` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `will` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `soul` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `aura` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `auraMax` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `attack` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `magatk` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `defend` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `magdef` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `hp` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `tough` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `action` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `hitrate` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `evade` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `critical` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `criticaldmg` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `pierce` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `counter` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `magres` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `strengthP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `physiqueP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `agilityP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `intelligenceP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `willP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `soulP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `auraP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `auraMaxP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `attackP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `magatkP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `defendP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `magdefP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `hpP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `toughP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `actionP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `hitrateP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `evadeP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `criticalP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `criticaldmgP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `pierceP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `counterP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `magresP` float(10,4) unsigned NOT NULL DEFAULT '0.00',
    `hitrlvl` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `evdlvl` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `crilvl` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `pirlvl` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `counterlvl` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `mreslvl` float(10,2) unsigned NOT NULL DEFAULT '0.00',
    `toughlvl` float(10,2) unsigned NOT NULL DEFAULT '0.00',
  PRIMARY KEY (`fighterId`,`playerId`, `type`),
  KEY `playerId` (`playerId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `sh_fighter_attr2` (
    `fighterId` int(10) NOT NULL,
    `playerId` bigint(20) unsigned NOT NULL,
    `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
    `soulExtraAura` tinyint(3) unsigned NOT NULL,
    `soulAuraLeft` tinyint(3) unsigned NOT NULL,
    `elixir_strength` int(10) NOT NULL,
    `elixir_physique` int(10) NOT NULL,
    `elixir_agility` int(10) NOT NULL,
    `elixir_intelligence` int(10) NOT NULL,
    `elixir_will` int(10) NOT NULL,
    `elixir_soul` int(10) NOT NULL,
    `elixir_attack` int(10) NOT NULL,
    `elixir_defend` int(10) NOT NULL,
    `elixir_critical` int(10) NOT NULL,
    `elixir_pierce` int(10) NOT NULL,
    `elixir_evade` int(10) NOT NULL,
    `elixir_counter` int(10) NOT NULL,
    `elixir_tough` int(10) NOT NULL,
    `elixir_action` int(10) NOT NULL,
  PRIMARY KEY (`fighterId`,`playerId`, `type`),
  KEY `playerId` (`playerId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `sh_supporter` (
    `playerId` bigint(20) unsigned NOT NULL,
    `flag` tinyint(3) NOT NULL DEFAULT '0',
    `class1PlayerId` bigint(20) unsigned NOT NULL,
    `class2PlayerId` bigint(20) unsigned NOT NULL,
    `class3PlayerId` bigint(20) unsigned NOT NULL,
  PRIMARY KEY (`playerId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

CREATE TABLE IF NOT EXISTS `sh_candidate` (
    `playerId` bigint(20) unsigned NOT NULL,
    `jobClass` tinyint(3) NOT NULL DEFAULT '0',
    `supportedCount` int(11) NOT NULL DEFAULT '0',
    `pos` tinyint(3) NOT NULL DEFAULT '0',
  PRIMARY KEY (`playerId`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


