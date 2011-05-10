function Monster_00004311(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(62103) then
		task:AddTaskStep2(62103,1,num);
	end
	if task:HasAcceptedTask(80075) then
		local itemNum = package:GetItemNum(15099,1);
		if itemNum < 80 then
			itemNum = 80 - itemNum;
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
			if realItemNum ~= 0 and package:AddItem(15099,realItemNum,1) ~= nil then
				task:AddTaskStep2(80075,1,realItemNum);
			end
		end
	end
end