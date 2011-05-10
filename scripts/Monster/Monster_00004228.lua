function Monster_00004228(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(30701) then
		local itemNum = package:GetItemNum(15046,1);
		if itemNum < 1 then
			itemNum = 1 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15046, itemNum, 1) ~= nil then
				task:AddTaskStep2(30701,1,itemNum);
			end
		end
	end
	local package = player:GetPackage();
	if task:HasAcceptedTask(80046) then
		local itemNum = package:GetItemNum(15088,1);
		if itemNum < 15 then
			itemNum = 15 - itemNum;
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
			if realItemNum ~= 0 and package:AddItem(15088,realItemNum,1) ~= nil then
				task:AddTaskStep2(80046,1,realItemNum);
			end
		end
	end
end