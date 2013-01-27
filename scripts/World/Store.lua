
-- 限时折扣
testTm = { ['year'] = 2012, ['month'] = 8, ['day'] = 14, ['hour'] = 12, ['min'] = 0, ['sec'] = 0 };

local discount_items = {
    465,466,516,547,57,56,500,15,501,503,505,506,508,511,512,513,514,515,517,
};

-- itemID, discountType, limitCount, beginTime, endTime, priceDiscount
specialDiscount1 = {
    discount_items[1],
    4,
    8,
    os.time(testTm),
    os.time(testTm) + 86400,
    13,
    23,
}
specialDiscount2 = {
    discount_items[6],
    4,
    8,
    os.time(testTm),
    os.time(testTm) + 86400,
    12,
    24,
}
specialDiscount3 = {
    discount_items[6],
    5,
    27,
    os.time(testTm),
    os.time(testTm) + 86400,
    12,
    25,
}
specialDiscount4 = {
    discount_items[6],
    6,
    28,
    os.time(testTm),
    os.time(testTm) + 86400,
    12,
    26,
}

specialDiscount ={
    specialDiscount1, specialDiscount2, specialDiscount3, specialDiscount4,
}

function resetDiscount()
    local store = GetStore()

    local discount_num = {
        1,2,4,
    }

    local discounts = {
        3,5,8,
    }

    local discount_items = {
        465,466,516,547,57,56,500,15,501,503,505,506,508,511,512,513,514,515,517,
    };

    store:clearNormalDiscount()
    for d = 1, #discounts do
        local nr = math.random(1, #discount_num)
        local num = discount_num[nr]
        print("num :" .. num)
        for i = 1, num do
            local n = math.random(1, #discount_items)
            print("id: " .. discount_items[n] .. " discount: " .. discounts[d])
            store:addNormalDiscount(discount_items[n], discounts[d], num)
            table.remove(discount_items, n);
        end
    end
    store:addSpecialDiscount()
end

function discount(store)
    local tm1 = { ['year'] = 2012, ['month'] = 12, ['day'] = 7, ['hour'] = 0, ['min'] = 0, ['sec'] = 0 };

    local now = os.time() + 30
    local t1 = os.time(tm1)

    local day = 86400
    local week = 604800

    if now >= t1 and now < t1 + 3*day then
        store:add(5,9279,1000)
        store:add(2,9280,10)
        store:add(2,9281,20)
        store:add(2,9282,50)
    end
end


function loadStore()
    local store = GetStore()
    local now = os.time() + 30

    store:clearSpecialDiscount()

    --if ((now % 86400) > 30) then
        store:clear()

        discount(store)

        if getDragonKingAct() then
            store:add(2, 9337, 10) --游龙令
        end
        if isfashion_shop() then
            store:add(2, 1700, 599) --仙墨笔
            store:add(2, 1701, 599) --折翼扇
            store:add(2, 1702, 599) --西方之焰
            store:add(2, 1703, 599) --玫瑰情缘
            store:add(2, 1709, 599) --圣诞萌萌的糖
            store:add(2, 1710, 599) --圣诞老人的包袱
        end
        store:add(2, 1704, 599) --轩辕双剑
        store:add(2, 1705, 599) --伏羲古琴

        if is0111_0117() then
            store:add(5,9279,1000)
            store:add(2,9280,10)
            store:add(2,9281,20)
            store:add(2,9282,50)
        end

        if getSnowAct() then
            store:add(2, 9275, 10)
        end
        if getGoldSnakeAct() then
            store:add(2, 9314, 20)
        end

        if is1221_1227() then
            store:add(2, 1709, 599) --圣诞萌萌的糖
            store:add(2, 1710, 599) --圣诞老人的包袱
        end

        if is1116_1122() then
            store:add(2, 9215, 10) --逍遥礼包
        end
        if is0921_0924() then
            store:add(2, 1700, 599) --仙墨笔
            store:add(2, 1701, 599) --折翼扇
            store:add(2, 1702, 599) --西方之焰
            store:add(2, 1703, 599) --玫瑰情缘
            store:add(2, 1704, 599) --轩辕双剑
            store:add(2, 1705, 599) --伏羲古琴
            store:add(2, 1707, 599) --蓝钻超人
            store:add(2,509,80)
            store:add(2,507,80)
        end
        if is0926_0927() then
        store:add(2,493,10)
        store:add(2,494,20)
        store:add(2,495,50)
        end
        --store:add(2,1706,599)
        --store:add(2,9163,10)
        -- store:add(2,1706,599)
        store:add(2,1325,50)
        if is7_16_8_15() then
            store:add(2,1704,999)
            store:add(2,1705,999)
        end

        if is7_13_16() then
            store:add(2,9088,10)
        end

        if is7_14_15() then
            store:add(2,5007,950)
            store:add(2,5037,950)
            store:add(2,5027,950)
            store:add(2,9087,100)
        end

        if is7_10_15() then
            store:add(2,9085,100)
            store:add(2,9086,200)
        end

        if is4_21() then
            store:add(2,493,10)
            store:add(2,494,20)
            store:add(2,495,50)
            store:add(5,492,1000)
        end

        if getMayDay() then
            store:add(2,496,10)
            store:add(2,497,10)
        end

        if is7_1() then
            store:add(2,1702,500)
            store:add(2,1703,500)
        end

        if is0808_0814() then
            store:add(2,9120,400)
        end

        if getQixi() then
            store:add(2,9122,10)
        end

        if getWansheng() then
            store:add(2,9194,10)
        end

        if getGuoqing() then
            store:add(2,9179,30)
        end

		store:add(2,9338,50)
		store:add(2,9349,10)
		store:add(2,9310,30)
		store:add(2,9308,10)
		store:add(2,9310,30)
        store:add(2,9285,10)
        store:add(2,9123,50)
        store:add(2,551,10)
        store:add(2,134,50)
        store:add(2,9082,5)
        store:add(2,549,80)
        store:add(2,33,10)
        store:add(2,8000,15)
        store:add(2,72,720)
        store:add(2,78,120)
        store:add(2,79,200)
        store:add(2,80,450)
        store:add(2,81,110)
        store:add(2,9012,10)
        store:add(2,9013,10)
        store:add(2,9014,10)
        store:add(2,9015,10)
        store:add(2,9016,10)
        store:add(2,9035,10)
        store:add(2,548,1)
        store:add(2,465,20)
        store:add(2,466,10)
        store:add(2,440,50)
        store:add(2,516,30)
        store:add(2,547,15)
        store:add(2,47,100)
        store:add(2,49,80)
        store:add(2,50,90)
        store:add(2,57,10)
        store:add(2,56,10)
        store:add(2,500,10)
        store:add(2,15,10)
        store:add(2,16,10)
        store:add(2,501,15)
        store:add(2,503,20)
        store:add(2,505,15)
        store:add(2,506,10)
        store:add(2,508,10)
        store:add(2,511,5)
        store:add(2,512,10)
        store:add(2,513,15)
        store:add(2,514,10)
        store:add(2,515,80)
        store:add(2,517,10)
        store:add(3,5005,100)
        store:add(3,5015,100)
        store:add(3,5025,100)
        store:add(3,5035,100)
        store:add(3,5045,100)
        store:add(3,5055,100)
        store:add(3,5065,200)
        store:add(3,5075,150)
        store:add(3,5085,200)
        store:add(3,5095,200)
        store:add(3,5105,200)
        store:add(3,5115,200)
        store:add(3,5125,200)
        store:add(3,5135,150)
        store:add(3,5145,200)
        store:add(3,5003,10)
        store:add(3,5013,10)
        store:add(3,5023,10)
        store:add(3,5033,10)
        store:add(3,5043,10)
        store:add(3,5053,10)
        store:add(3,5063,20)
        store:add(3,5073,15)
        store:add(3,5083,20)
        store:add(3,5093,20)
        store:add(3,5103,20)
        store:add(3,5113,20)
        store:add(3,5123,20)
        store:add(3,5133,15)
        store:add(3,5143,20)
        store:add(4,57,10)
        store:add(4,511,5)
        store:add(4,56,10)
        store:add(4,500,10)
        store:add(4,503,20)
        store:add(4,505,15)
        store:add(4,5003,10)
        store:add(4,5013,10)
        store:add(4,5023,10)
        store:add(4,5033,10)
        store:add(4,5043,10)
        store:add(4,5053,10)
        store:add(4,5063,20)
        store:add(4,5073,15)
        store:add(4,5083,20)
        store:add(4,5093,20)
        store:add(4,5103,20)
        store:add(4,5113,20)
        store:add(4,5123,20)
        store:add(4,5133,15)
        if is0926_0927() then
            store:add(5,492,1000)
        end
        store:add(4,5143,20)
		store:add(5,9307,200)
		store:add(5,9309,500)
        store:add(5,550,1000)
        store:add(5,9081,500)
        store:add(5,9231,2000)
        store:add(5,489,2000)
        store:add(5,490,2000)
        store:add(5,491,2000)
        store:add(5,502,1000)
        store:add(5,504,1000)
        store:add(5,510,800)
        store:add(5,55,400)
        store:add(5,5001,300)
        store:add(5,5011,300)
        store:add(5,5021,300)
        store:add(5,5031,300)
        store:add(5,5041,300)
        store:add(5,5051,300)
        store:add(5,5061,500)
        store:add(5,5071,400)
        store:add(5,5081,500)
        store:add(5,5091,500)
        store:add(5,5101,500)
        store:add(5,5111,500)
        store:add(5,5121,500)
        store:add(5,5131,400)
        store:add(5,5141,500)
        store:add(6,1504,3000)
        store:add(6,1522,10000)
        store:add(6,74,12000)
        store:add(6,76,2000)
        store:add(6,15,500)
        store:add(6,503,800)
        store:add(6,514,1000)
        store:add(6,48,600)
        store:add(6,49,1000)
        store:add(6,50,1000)
        store:add(6,51,600)
        store:add(6,506,1000)
        store:add(6,508,1000)
        store:add(6,507,2000)
        store:add(6,509,2000)
        store:add(7,1511,6000)
        store:add(7,1520,15000)
        store:add(7,73,18000)
        store:add(7,75,4500)
        store:add(7,77,4500)
        store:add(7,133,1000)
        store:add(7,2545,3000)
        store:add(7,2546,3000)
        store:add(7,2548,3000)
        store:add(7,2549,3000)
        store:add(7,2553,3000)
        store:add(7,2554,3000)
        store:add(7,2556,3000)
        store:add(7,2557,3000)
        store:add(7,2561,3000)
        store:add(7,2562,3000)
        store:add(7,2564,3000)
        store:add(7,2565,3000)
        store:add(7,2569,4000)
        store:add(7,2570,4000)
        store:add(7,2572,4000)
        store:add(7,2573,4000)
        store:add(7,2577,4000)
        store:add(7,2578,4000)
        store:add(7,2580,4000)
        store:add(7,2581,4000)
        store:add(7,2585,4000)
        store:add(7,2586,4000)
        store:add(7,2588,4000)
        store:add(7,2589,4000)
        store:add(7,2593,5000)
        store:add(7,2594,5000)
        store:add(7,2596,5000)
        store:add(7,2597,5000)
        store:add(7,2601,5000)
        store:add(7,2602,5000)
        store:add(7,2604,5000)
        store:add(7,2605,5000)
        store:add(7,2609,5000)
        store:add(7,2610,5000)
        store:add(7,2612,5000)
        store:add(7,2613,5000)
        store:add(7,2617,6000)
        store:add(7,2618,6000)
        store:add(7,2620,6000)
        store:add(7,2621,6000)
        store:add(7,2625,6000)
        store:add(7,2626,6000)
        store:add(7,2628,6000)
        store:add(7,2629,6000)
        store:add(7,2633,6000)
        store:add(7,2634,6000)
        store:add(7,2636,6000)
        store:add(7,2637,6000)
        store:addExchange(8,519,526,2)
        store:addExchange(8,520,527,2)
        store:addExchange(8,521,528,2)
        store:addExchange(8,522,529,2)
        store:addExchange(8,523,530,2)
        store:addExchange(8,524,531,2)
	store:addExchange(8,525,532,2)
        store:addExchange(8,527,526,10)
        store:addExchange(8,528,527,10)
        store:addExchange(8,529,528,10)
        store:addExchange(8,530,529,10)
        store:addExchange(8,531,530,10)
        store:addExchange(8,532,531,10)
        store:add(11,9232,9000)
        store:add(11,9233,9000)
        store:add(11,9234,9000)
        store:add(11,9037,9000)
        store:add(11,9038,9000)
        store:add(11,9039,9000)
        store:add(11,9040,9000)
        store:add(11,9041,9000)
        store:add(11,9042,9000)
        store:add(11,9043,9000)
        store:add(11,9044,9000)
        store:add(11,9045,9000)
        store:add(11,9046,9000)
        store:add(11,9047,9000)
        store:add(11,9048,9000)
        store:add(11,9049,9000)
        store:add(12,1535,20000)
        store:add(12,1640,10000)
        store:add(12,1641,10000)
        store:add(12,1642,10000)
        store:add(12,1643,10000)
        store:add(12,1644,10000)
        store:add(12,1645,10000)
        store:add(13,1293,3000)
        store:add(13,1294,3000)
        store:add(13,1295,3000)
        store:add(13,1296,4000)
        store:add(13,1297,4000)
        store:add(13,1298,4000)
        store:add(13,1299,5000)
        store:add(13,1300,5000)
        store:add(13,1301,5000)
        store:add(13,1302,6000)
        store:add(13,1303,6000)
        store:add(13,1304,6000)
        store:add(14,507,2000)
        store:add(14,509,2000)
        store:add(15,5041,300)
        store:add(15,5051,300)

    --end

    if store:needResetDiscount() then
        print ("needResetDiscount")
        resetDiscount()
    else
        print ("Don't needResetDiscount")
    end
    store:storeDiscount()


    store:update()

end

function GetSpecialDiscount ()
    return specialDiscount
end

