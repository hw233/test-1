function Monster_00004157(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(50403) then
		local itemNum = package:GetItemNum(15049,1);
		if itemNum < 1 then
			itemNum = 1 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15049, itemNum, 1) ~= nil then
				task:AddTaskStep2(50403,1,itemNum);
			end
		end
	end
end