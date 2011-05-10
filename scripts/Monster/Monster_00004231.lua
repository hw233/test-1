function Monster_00004231(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80047) then
		local itemNum = package:GetItemNum(15089,1);
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
			if realItemNum ~= 0 and package:AddItem(15089,realItemNum,1) ~= nil then
				task:AddTaskStep2(80047,1,realItemNum);
			end
		end
	end
	local package = player:GetPackage();
	if task:HasAcceptedTask(30802) then
		local itemNum = package:GetItemNum(15061,1);
		if itemNum < 12 then
			itemNum = 12 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15061, itemNum, 1) ~= nil then
				task:AddTaskStep2(30802,1,itemNum);
			end
		end
	end
end