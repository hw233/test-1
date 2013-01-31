--[[
-- 糖果
qixitmpl:setQixiItem(9194)
-- 称号卡
qixitmpl:setTitleItem(9195)
-- 分手提示信息编码
qixitmpl:setDivorceMsgCode(1033)
qixitmpl:setRoamingFunc("onRoamingWansheng")

qixitmpl:setScoreTitle(4054)
qixitmpl:setScoreContent(4055)
qixitmpl:setMinAwardScore(300)
-- 2000分：五行精金(515)x7，凝神易筋丹(509)x7，补髓益元丹(507)x7
qixitmpl:addScoreAwards(2000, 515, 7)
qixitmpl:addScoreAwards(2000, 509, 7)
qixitmpl:addScoreAwards(2000, 507, 7)
-- 1000分：太乙精金(503)x7，万象元神(8000)x7,宝石保护符(513)x7
qixitmpl:addScoreAwards(1000, 503, 7)
qixitmpl:addScoreAwards(1000, 8000, 7)
qixitmpl:addScoreAwards(1000, 513, 7)
-- 800分：天赋洗炼丹(516)x7，天赋保护符(547)x7
qixitmpl:addScoreAwards(800, 516, 7)
qixitmpl:addScoreAwards(800, 547, 7)
-- 600分：洗炼符(500)x7,洗炼保护符(501)x7
qixitmpl:addScoreAwards(600, 500, 7)
qixitmpl:addScoreAwards(600, 501, 7)
-- 300分：太乙真金(502)x7，初级打孔石(510)x7，粗制拆卸石(504)x7
qixitmpl:addScoreAwards(300, 502, 7)
qixitmpl:addScoreAwards(300, 510, 7)
qixitmpl:addScoreAwards(300, 504, 7)

qixitmpl:setPartnerTitle(4052)
qixitmpl:setPartnerContent(4053)
-- 拥有冒险搭档：技能符文熔炼诀(1325)x7
qixitmpl:addPartnerAwards(1325, 7)

qixitmpl:setLBTitle(4050)
qixitmpl:setLBContent(4051)
qixitmpl:setMaxLBAwardPos(99)
-- 第1名：对应性别称号卡（男：冒险达人，女：大冒险家），技能符文熔炼诀(1325)x30
qixitmpl:addLBAwards(1, 9195, 1)
qixitmpl:addLBAwards(1, 1325, 30)
-- 第2名：技能符文熔炼诀(1325)x25
qixitmpl:addLBAwards(2, 1325, 25)
-- 第3名：技能符文熔炼诀(1325)x20
qixitmpl:addLBAwards(3, 1325, 20)
-- 第4-10名：技能符文熔炼诀(1325)x15
qixitmpl:addLBAwards(10, 1325, 15)
-- 第11-49名：技能符文熔炼诀(1325)x10
qixitmpl:addLBAwards(49, 1325, 10)
-- 第50-99名：技能符文熔炼诀(1325)x5
qixitmpl:addLBAwards(99, 1325, 5)
--]]


-- 蜀山之恋
qixitmpl:setQixiItem(9355)
-- 称号卡
qixitmpl:setTitleItem(9347)
-- 分手提示信息编码
qixitmpl:setDivorceMsgCode(1035)
qixitmpl:setRoamingFunc("onRoamingQingRen")

qixitmpl:setScoreTitle(4066)
qixitmpl:setScoreContent(4067)
qixitmpl:setMinAwardScore(50)
-- 1000分：五行精金(515)x7，法灵精金(134)x7，技能符文熔炼诀(1325)x7
qixitmpl:addScoreAwards(1000, 515, 7)
qixitmpl:addScoreAwards(1000, 134, 7)
qixitmpl:addScoreAwards(1000, 1325, 7)
-- 800分：太乙精金(503)x7，五行真金(514)x7,高级打孔石(512)x7
qixitmpl:addScoreAwards(800, 503, 7)
qixitmpl:addScoreAwards(800, 514, 7)
qixitmpl:addScoreAwards(800, 512, 7)
-- 600分：洗炼符(500)x7，中级打孔石(511)x7，法灵结晶(1523)x7
qixitmpl:addScoreAwards(600, 500, 7)
qixitmpl:addScoreAwards(600, 511, 7)
qixitmpl:addScoreAwards(600, 133, 7)
-- 200分：高级挂机加速符(56)x7，修为加速符(57)x7，银票(15)x7
qixitmpl:addScoreAwards(200, 56, 7)
qixitmpl:addScoreAwards(200, 57, 7)
qixitmpl:addScoreAwards(200, 15, 7)
-- 50分：太乙真金(502)x7，初级打孔石(510)x7，粗制拆卸石(504)x7
qixitmpl:addScoreAwards(50, 502, 7)
qixitmpl:addScoreAwards(50, 510, 7)
qixitmpl:addScoreAwards(50, 504, 7)

qixitmpl:setPartnerTitle(4058)
qixitmpl:setPartnerContent(4059)
-- 拥有冒险搭档：技能符文熔炼诀(1325)x7
qixitmpl:addPartnerAwards(1325, 7)

qixitmpl:setLBTitle(4056)
qixitmpl:setLBContent(4057)
qixitmpl:setMaxLBAwardPos(99)
-- 第1名：对应性别称号卡（男：罗密欧，女：朱丽叶），技能符文熔炼诀(1325)x30
qixitmpl:addLBAwards(1, 9347, 1)
qixitmpl:addLBAwards(1, 1325, 30)
-- 第2名：技能符文熔炼诀(1325)x25
qixitmpl:addLBAwards(2, 1325, 25)
-- 第3名：技能符文熔炼诀(1325)x20
qixitmpl:addLBAwards(3, 1325, 20)
-- 第4-10名：技能符文熔炼诀(1325)x15
qixitmpl:addLBAwards(10, 1325, 15)
-- 第11-49名：技能符文熔炼诀(1325)x10
qixitmpl:addLBAwards(49, 1325, 10)
-- 第50-99名：技能符文熔炼诀(1325)x5
qixitmpl:addLBAwards(99, 1325, 5)

