--放生仙宠对应的龙元凤髓数值
local converts = {
    [0] = {     --绿色仙宠
        [1] = 200,     --龙元
        [2] = 200,     --凤髓
        [3] = 0,        --好感度
    },
    [1] = {     --蓝色仙宠
        [1] = 1200,
        [2] = 1200,
        [3] = 0,
    },
    [2] = {     --紫色仙宠
        [1] = 7500,
        [2] = 7500,
        [3] = 7,
    },
    [3] = {     --橙色仙宠(无)
        [1] = 0,
        [2] = 0,
        [3] = 0,
    },
    [4] = {     --暗金仙宠(无)
        [1] = 0,
        [2] = 0,
        [3] = 0,
    },
}

greenPet = {501, 504, 507, 510}     --绿色仙宠id  【注:设置为全局变量】
bluePet = {502, 505, 508, 511}      --蓝色仙宠id  【注:设置为全局变量】
purplePet = {503, 506, 509, 512}    --紫色仙宠id  【注:设置为全局变量】

function exchangPurplePet(player)
    if nil == player then
        return 0
    end
    local like = player:GetVar(300) - 15
    if like < 0 then
        return 0
    end
    local id = purplePet[math.random(1, #purplePet)]
    player:SetVar(300, like)
    return id
end

function getPetColorFromId(petId)
    for _, val in pairs(greenPet) do
        if petId == val then
            return 0
        end
    end
    for _, val in pairs(bluePet) do
        if petId == val then
            return 1
        end
    end
    for _, val in pairs(purplePet) do
        if petId == val then
            return 2
        end
    end
    return 10
end

--仙宠
function onSeekFairypetAwardAndSucceed(step, isConvert)
    local longYuan = {
        [1] = {25, 75},
        [2] = {100, 200},
        [3] = {200, 300},
        [4] = {300, 400},
        [5] = {400, 500},
    }
    local fengSui = {
        [1] = {25, 75},
        [2] = {100, 200},
        [3] = {200, 300},
        [4] = {300, 400},
        [5] = {400, 500},
    }
    local petProb = {
        [1] = 0,
        [2] = 1667,
        [3] = 417,
        [4] = {1000, 1834},
        [5] = 3000,
    }
    local result =  --返回结果
    {
        ["longyuan"] = 0,   --龙元数
        ["fengsui"] = 0,    --凤髓数
        ["greenId"] = 0,    --绿色仙宠id
        ["blueId"] = 0,     --蓝色仙宠id
        ["like"] = 0,       --好感度(绿色)
        ["succeed"] = 0,    --是否成功晋级下一游历池
        ["convert1"] = 0,   --放生仙宠获得龙元数
        ["convert2"] = 0,   --放生仙宠获得凤髓数
    }
    if nil == step or step < 1 or step > 5 then
        step = 1
    end
    result.longyuan = math.random(longYuan[step][1], longYuan[step][2])
    result.fengsui = math.random(fengSui[step][1], fengSui[step][2])
    local r = math.random(1, 10000)
    if 2 == step and r <= petProb[step] then
        result.greenId = greenPet[math.random(1, #greenPet)]
        if isConvert > 0 then
            result.convert1 = converts[0][1]
            result.convert2 = converts[0][2]
        end
    end
    if 3 == step and r <= petProb[step] then
        result.blueId = bluePet[math.random(1, #bluePet)]
        if isConvert > 0 then
            result.convert1 = converts[1][1]
            result.convert2 = converts[1][2]
        end
    end
    if 4 == step then
        if r <= petProb[step][1] then
            result.like = 1
        end
        if r > petProb[step][1] and r <= petProb[step][2] then
            result.blueId = bluePet[math.random(1, #bluePet)]
            if isConvert > 0 then
                result.convert1 = converts[1][1]
                result.convert2 = converts[1][2]
            end
        end
    end
    if 5 == step and r <= petProb[step] then
        result.like = 1
    end
    --是否成功晋级下一游历池
    local prob = {7500, 6666, 5000, 5000, 10000}
    r = math.random(1, 10000)
    if r <= prob[step] then
        result.succeed = 1
    else
        result.succeed = 0
    end
    return result
end

function getConvertPetValue(petId)
    local result = {
        ["longyuan"] = 0,
        ["fengsui"] = 0,
        ["like"] = 0,
    }
    if nil == petId then
        return result
    end
    local color = getPetColorFromId(petId)
    if nil == color or nil == converts[color] then
        return result
    end
    result.longyuan = converts[color][1]
    result.fengsui = converts[color][2]
    result.like = converts[color][3]
    return result
end


local petGemsId = {
    [1]  = {13000,13020,13040,13060,13080,13100,13120,13140,13160,13180,13200,13220},
    [2]  = {13001,13021,13041,13061,13081,13101,13121,13141,13161,13181,13201,13221},
    [3]  = {13002,13022,13042,13062,13082,13102,13122,13142,13162,13182,13202,13222},
    [4]  = {13003,13023,13043,13063,13083,13103,13123,13143,13163,13183,13203,13223},
    [5]  = {13004,13024,13044,13064,13084,13104,13124,13144,13164,13184,13204,13224},
    [6]  = {13005,13025,13045,13065,13085,13105,13125,13145,13165,13185,13205,13225},
    [7]  = {13006,13026,13046,13066,13086,13106,13126,13146,13166,13186,13206,13226},
    [8]  = {13007,13027,13047,13067,13087,13107,13127,13147,13167,13187,13207,13227},
    [9]  = {13008,13028,13048,13068,13088,13108,13128,13148,13168,13188,13208,13228},
    [10] = {13009,13029,13049,13069,13089,13109,13129,13149,13169,13189,13209,13229},
    [11] = {13010,13030,13050,13070,13090,13110,13130,13150,13170,13190,13210,13230},
    [12] = {13011,13031,13051,13071,13091,13111,13131,13151,13171,13191,13211,13231},
    [13] = {13012,13032,13052,13072,13092,13112,13132,13152,13172,13192,13212,13232},
    [14] = {13013,13033,13053,13073,13093,13113,13133,13153,13173,13193,13213,13233},
    [15] = {13014,13034,13054,13074,13094,13114,13134,13154,13174,13194,13214,13234},
    [16] = {13015,13035,13055,13075,13095,13115,13135,13155,13175,13195,13215,13235},
    [17] = {13016,13036,13056,13076,13096,13116,13136,13156,13176,13196,13216,13236},
    [18] = {13017,13037,13057,13077,13097,13117,13137,13157,13177,13197,13217,13237},
    [19] = {13018,13038,13058,13078,13098,13118,13138,13158,13178,13198,13218,13238},
    [20] = {13019,13039,13059,13079,13099,13119,13139,13159,13179,13199,13219,13239},
}

function getpetGemIdByMerge(reqLev)
    if nil == reqLev or nil == petGemsId[reqLev] then
        return 0
    end
    local rnd = math.random(1, #petGemsId[reqLev])
    return petGemsId[reqLev][rnd]
end

