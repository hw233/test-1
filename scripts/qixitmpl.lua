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
qixitmpl:setQixiItem(9122)
-- 称号卡
qixitmpl:setTitleItem(9992)
-- 分手提示信息编码
qixitmpl:setDivorceMsgCode(1035)
qixitmpl:setRoamingFunc("onRoamingQueqiao")

qixitmpl:setScoreTitle(4066)
qixitmpl:setScoreContent(4067)
qixitmpl:setMinAwardScore(200)
-- 
qixitmpl:addScoreAwards(32000, 1732, 1)
qixitmpl:addScoreAwards(32000, 9076, 5)
qixitmpl:addScoreAwards(32000, 515, 5)
qixitmpl:addScoreAwards(32000, 1325, 5)
qixitmpl:addScoreAwards(32000, 9425, 5)
-- 
qixitmpl:addScoreAwards(16000, 9498, 5)
qixitmpl:addScoreAwards(16000, 1126, 5)
qixitmpl:addScoreAwards(16000, 9338, 5)
qixitmpl:addScoreAwards(16000, 134, 5)
qixitmpl:addScoreAwards(16000, 16001, 5)
-- 
qixitmpl:addScoreAwards(8000, 8000, 5)
qixitmpl:addScoreAwards(8000, 9600, 5)
qixitmpl:addScoreAwards(8000, 9414, 5)
qixitmpl:addScoreAwards(8000, 9457, 5)
-- 1000分：五行精金(515)x7，凝神易筋丹(509)x7，补髓益元丹(507)x7
qixitmpl:addScoreAwards(5000, 16037, 1)
qixitmpl:addScoreAwards(5000, 9424, 7)
qixitmpl:addScoreAwards(5000, 9603, 7)
qixitmpl:addScoreAwards(5000, 516, 7)
-- 800分：橙色星辰旗(1126)x7，仙缘石(9371)x7,高级打孔石(512)x7
qixitmpl:addScoreAwards(2500, 551, 3)
qixitmpl:addScoreAwards(2500, 503, 3)
qixitmpl:addScoreAwards(2500, 517, 3)
qixitmpl:addScoreAwards(2500, 547, 3)
-- 600分：洗炼符(500)x7，中级打孔石(511)x7，法灵结晶(133)x7
qixitmpl:addScoreAwards(1200, 501, 3)
qixitmpl:addScoreAwards(1200, 512, 3)
qixitmpl:addScoreAwards(1200, 9123, 5)
-- 200分：高级挂机加速符(56)x7，修为加速符(57)x7，银票(15)x7
qixitmpl:addScoreAwards(600, 500, 3)
qixitmpl:addScoreAwards(600, 513, 3)
qixitmpl:addScoreAwards(600, 15, 5)
-- 50分：太乙真金(502)x7，初级打孔石(510)x7，蓝色符文熔炼诀(1327)x7
qixitmpl:addScoreAwards(200, 56, 3)
qixitmpl:addScoreAwards(200, 57, 3)
qixitmpl:addScoreAwards(200, 133, 3)

qixitmpl:setPartnerTitle(4058)
qixitmpl:setPartnerContent(4059)
-- 拥有冒险搭档：技能符文熔炼诀(1325)x7
qixitmpl:addPartnerAwards(134, 7)
qixitmpl:addPartnerAwards(9123, 7)

qixitmpl:setLBTitle(4056)
qixitmpl:setLBContent(4057)
qixitmpl:setMaxLBAwardPos(99)
-- 第1名：对应性别称号卡（男：迢迢牵牛星，女：皎皎河汉女），技能符文熔炼诀(1325)x30
qixitmpl:addLBAwards(1, 9992, 1)
qixitmpl:addLBAwards(1, 1126, 30)
qixitmpl:addLBAwards(1, 9498, 30)
qixitmpl:addLBAwards(1, 1325, 30)
qixitmpl:addLBAwards(1, 9457, 30)
-- 第2名：技能符文熔炼诀(1325)x25
qixitmpl:addLBAwards(2, 1126, 25)
qixitmpl:addLBAwards(2, 9498, 25)
qixitmpl:addLBAwards(2, 1325, 25)
qixitmpl:addLBAwards(2, 9457, 25)
-- 第3名：技能符文熔炼诀(1325)x20
qixitmpl:addLBAwards(3, 1126, 20)
qixitmpl:addLBAwards(3, 9498, 20)
qixitmpl:addLBAwards(3, 1325, 20)
qixitmpl:addLBAwards(3, 9457, 20)
-- 第4-10名：技能符文熔炼诀(1325)x15
qixitmpl:addLBAwards(10, 1126, 15)
qixitmpl:addLBAwards(10, 9498, 15)
qixitmpl:addLBAwards(10, 1325, 15)
qixitmpl:addLBAwards(10, 9457, 15)
-- 第11-49名：技能符文熔炼诀(1325)x10
qixitmpl:addLBAwards(49, 1126, 10)
qixitmpl:addLBAwards(49, 9498, 10)
qixitmpl:addLBAwards(49, 1325, 10)
qixitmpl:addLBAwards(49, 9457, 10)
-- 第50-99名：技能符文熔炼诀(1325)x5
qixitmpl:addLBAwards(99, 1126, 5)
qixitmpl:addLBAwards(99, 9498, 5)
qixitmpl:addLBAwards(99, 1325, 5)
qixitmpl:addLBAwards(99, 9457, 5)

