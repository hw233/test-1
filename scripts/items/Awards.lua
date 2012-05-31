
function RunSSDTAward(player, opt)
    if player == nil then
        return false;
    end

    local package = player:GetPackage();

	if package:GetRestPackageSize() < 1 then		
		player:sendMsgCode(2, 1011, 0);
		return false;
	end

    if opt > 3 then
        return false;
    end

    if opt == 0 then
        local gem = {5002, 5012, 5022, 5032, 5042, 5052, 5062, 5072, 5082, 5092, 5102, 5112, 5122, 5132, 5142}
        local g = math.random(1, #gem)
        package:AddItem(gem[g], 1, true, 0, 39);
    else
        local items = { {56, 2}, {503, 2}, {507, 2} }
        package:AddItem(items[opt][1], items[opt][2], true, 0, 39);
    end

    return true;
end

function RunHappyAward(player, opt)
    if player == nil then
        return 0;
    end

    local package = player:GetPackage();

	if package:GetRestPackageSize() < 5 then		
		player:sendMsgCode(2, 1011, 0);
		return 0;
	end

    if opt > 5 then
        return 0;
    end

    local itemId = 0;
    if opt == 0 then
        local chance = {100, 212, 338, 456, 1461, 3470, 5981, 10000}
        local item = {9022, 509, 507, 515, 503, 514, 56, 15}
        local g = math.random(1, 10000)
        for i = 1, #chance do 
            if g <= chance[i] then
                package:AddItem(item[i], 1, true, true, 40);
                itemId = item[i]

                if i <=4 then
                    Broadcast(0x27, "[p:"..player:getCountry()..":"..player:getPName().."]在歡樂大轉盤中幸運地獲得了[4:"..itemId.."]x1")
                end
                break
            end
        end
    else
        local items = { 
            { 56, 1, 57, 1, 15, 1, 508, 1, 506, 1 },
            { 500, 3, 511, 1, 501, 1, 517, 2 },
            { 5005, 1, 514, 1, 465, 1 },
            { 516, 2, 503, 2, 512, 2, 466, 2 },
            { 507, 1, 509, 1, 9021, 1 },
        }

        itemId = 1;
        for i = 1, #items[opt], 2 do
            if items[opt][i] == 5005 then
                local gem = {5005, 5015, 5025, 5035, 5045, 5055, 5065, 5075, 5085, 5095, 5105, 5115, 5125, 5135, 5145}
                local g = math.random(1, #gem)
                package:AddItem(gem[g], 1, true, 0, 40);
            else
                package:AddItem(items[opt][i], items[opt][i+1], true, 0, 40);
            end
        end
    end

    return itemId;
end


