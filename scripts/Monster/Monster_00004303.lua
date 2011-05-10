function Monster_00004303(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(31903) then
		local itemNum = package:GetItemNum(15076,1);
		if itemNum < 12 then
			itemNum = 12 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15076, itemNum, 1) ~= nil then
				task:AddTaskStep2(31903,1,itemNum);
			end
		end
	end
end