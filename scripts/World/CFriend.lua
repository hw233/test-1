
local items = {
    {{15,5,1}},
    {{9010,1,1},},
    {{56,2,1}, {15,2,1}},
    {{56,2,1}, {15,2,1}},
    {{56,5,1}, {15,5,1}, {1600,1,1}},
    {{56,10,1}, {15,10,1}},
    {{56,10,1}, {15,10,1}},
    {{57,1,1}, {502,1,1}},
    {{57,2,1}, {502,2,1}, {1605,1,1}},
    {{57,2,1}, {502,2,1}, {514,5,1}},
    {{57,5,1}, {502,5,1}, {515,2,1}},
    {{57,10,1}, {502,10,1}, {515,5,1}},
    {{57,10,1}, {502,10,1}, {515,5,1}},
    {{514,1,1}, {503,1,1}},
    {{514,2,1}, {503,2,1}, {1610,1,1}},
    {{514,2,1}, {503,2,1}, {515,1,1}},
    {{514,3,1}, {503,5,1}, {515,2,1}},
    {{514,4,1}, {503,10,1}, {515,5,1}},
    {{514,5,1}, {503,10,1}, {515,5,1}},
    {{507,1,1}, {503,1,1}},
    {{507,2,1}, {503,2,1}, {1607,1,1}},
    {{507,2,1}, {503,2,1}, {514,5,1}},
    {{507,5,1}, {503,5,1}, {515,2,1}},
    {{507,10,1}, {503,10,1}, {515,5,1}},
    {{507,10,1}, {503,10,1}, {515,5,1}},
    {{509,1,1}, {503,1,1}},
    {{509,2,1}, {503,2,1}, {1621,1,1}},
    {{509,2,1}, {503,2,1}, {514,5,1}},
    {{509,5,1}, {503,5,1}, {515,2,1}},
    {{509,10,1}, {503,10,1}, {515,5,1}},
    {{509,10,1}, {503,10,1}, {515,5,1}},
};

function onGetCFriendAward(player, idx)
    if idx == 0 then
        return false
    end

    if items[idx] == nil then
        return false
    end

    local package = player:GetPackage()
    if package:GetRestPackageSize() < #items[idx] then 
        player:sendMsgCode(2, 1011, 0)
        return false
    end  

    for k, v in pairs(items[idx]) do
        package:Add(v[1], v[2], v[3])
    end  
    return true 
end
