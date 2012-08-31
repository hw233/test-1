CREATE TABLE IF NOT EXISTS `tianjie` (                                                                             
    `id` int(10) unsigned NOT NULL AUTO_INCREMENT,                                                       
    `is_opened` tinyint(1) DEFAULT '0' COMMENT '是否已开启',                                             
    `is_execute` tinyint(1) DEFAULT '0' COMMENT '是否正在进行天劫事件',                                  
    `is_finish` tinyint(1) DEFAULT '0' COMMENT '是否天劫事件已达成',                                     
    `is_ok` tinyint(1) DEFAULT '0' COMMENT '是否BOSS已击杀 ',                                            
    `level` int(11) DEFAULT '0' COMMENT '天劫等级',                                                      
    `rate` int(11) DEFAULT '0' COMMENT '天劫进度 1,2,3,4',                                               
    `opentime` datetime DEFAULT NULL COMMENT '天劫事件开启时间',                                         
    `r1_killed` char(32) DEFAULT NULL COMMENT '事件1怪物击杀数',                                         
    `r2_donated` char(32) DEFAULT NULL COMMENT '事件2捐款数',                                            
    `r3_copyid` int(11) DEFAULT '0' COMMENT '事件3天雷阵层数',                                           
    `r4_day` int(11) DEFAULT '0' COMMENT '打BOSS的天数',                                                 
    PRIMARY KEY (`id`),                                                                                  
    UNIQUE KEY `level_only` (`level`)                                                                    
) ENGINE=InnoDB AUTO_INCREMENT=1 DEFAULT CHARSET=utf8 CHECKSUM=1 DELAY_KEY_WRITE=1 ROW_FORMAT=DYNAMIC;
