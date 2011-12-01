function Monster_00000001(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(84) then
		local itemNum = package:GetItemNum(1,1);
		if itemNum < 1 then
			itemNum = 1 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(1, itemNum, 1) ~= nil then
				task:AddTaskStep2(84,1,itemNum);
			end
		end
	end
	local package = player:GetPackage();
	if task:HasAcceptedTask(85) then
		local itemNum = package:GetItemNum(1,1);
		if itemNum < 1 then
			itemNum = 1 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(1, itemNum, 1) ~= nil then
				task:AddTaskStep2(85,1,itemNum);
			end
		end
	end
end