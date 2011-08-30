function Monster_00005071(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(94) then
		local itemNum = package:GetItemNum(809,1);
		if itemNum < 2 then
			itemNum = 2 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(809, itemNum, 1) ~= nil then
				task:AddTaskStep2(94,1,itemNum);
			end
		end
	end
end