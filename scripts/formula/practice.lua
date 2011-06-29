
capfactor = 0.16
goldfactor = 0.2
prabase = 5
potentialbase = 5

-- TODO: 山头据点
addons = {
    [1] = 0.2,
    [2] = 0.2,
    [3] = 0.1,
    [4] = 0.1,
    [5] = 0.1,
    [6] = 0.1,
    [7] = 0
}

function calcPracticeInc( fgt )
    if fgt == nil then
        return 0
    end

    place = fgt:getPracticePlace();
    if not place or place > 7 then
        return 0
    end
    placeadd = addons[place]
    if placeadd == nil then
        placeadd = 0
    end

    if not fgt:isGoldPractice() then
        goldfactor = 0
    end

    if place ~= 7 then
        clantechaddon = fgt:getClanTechAddon(place)
    else
        clantechaddon = 0
    end

    capacity = fgt:getCapacity()
    if capacity < 5 then
        capacity = 5
    end

    -- 穴道加成 + (资质 - 5) * 0.16 + 钱加成 + 山头加成
    return fgt:getAcuPraAdd() + (prabase * (1 + (capacity - 5) * capfactor + goldfactor + placeadd + clantechaddon)) + 0.5
end

