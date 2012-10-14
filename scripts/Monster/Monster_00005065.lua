function Monster_00005065(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(83) then
		local itemNum = package:GetItemNum(808,1);
		if itemNum < 1 then
			itemNum = 1 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(808, itemNum, 1) ~= nil then
				task:AddTaskStep2(83,1,itemNum);
			end
		end
	end
end