function Monster_00004208(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(30403) then
		local itemNum = package:GetItemNum(15036,1);
		if itemNum < 12 then
			itemNum = 12 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15036, itemNum, 1) ~= nil then
				task:AddTaskStep2(30403,1,itemNum);
			end
		end
	end
end