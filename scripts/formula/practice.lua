
potfactor = 0.16
goldfactor = 0.2
prabase = 5

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

    placeadd = addons[fgt:getPracticePlace()];
    if placeadd == nil then
        placeadd = 0
    end

    if not fgt:isGoldPractice() then
        goldfactor = 0
    end
    -- 穴道加成 + (资质 - 5) * 0.16 + 钱加成 + 山头加成
    return fgt:getAcuPraAdd() + (prabase * (1 + (fgt:getPotential() - 5) * potfactor + goldfactor + placeadd)) + 0.5
end

