
function RunSSDTAward(opt)
    local player = GetPlayer();
    local package = player:GetPackage();

	if package:GetRestPackageSize() < 1 then		
		player:sendMsgCode(2, 1011, 0);
		return false;
	end

    if(opt > 3)
        return false;

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


