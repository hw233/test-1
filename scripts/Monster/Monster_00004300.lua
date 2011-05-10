function Monster_00004300(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(31803) then
		local itemNum = package:GetItemNum(15075,1);
		if itemNum < 12 then
			itemNum = 12 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15075, itemNum, 1) ~= nil then
				task:AddTaskStep2(31803,1,itemNum);
			end
		end
	end
	local package = player:GetPackage();
	if task:HasAcceptedTask(80072) then
		local itemNum = package:GetItemNum(15098,1);
		if itemNum < 70 then
			itemNum = 70 - itemNum;
			if itemNum > num then itemNum = num; end
			local realItemNum = 0;
			for i = 1, num do
				if math.random(1, 100) <= 10 then
					realItemNum = realItemNum + 1;
					if realItemNum >= itemNum then
						break;
					end
				end
			end
			if realItemNum ~= 0 and package:AddItem(15098,realItemNum,1) ~= nil then
				task:AddTaskStep2(80072,1,realItemNum);
			end
		end
	end
end