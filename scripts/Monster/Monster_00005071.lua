function Monster_00005071(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(37) then
		task:AddTaskStep2(37,1,num);
	end
	if task:HasAcceptedTask(94) then
		local itemNum = package:GetItemNum(809,1);
		if itemNum < 2 then
			itemNum = 2 - itemNum;
			if itemNum > num then itemNum = num; end
			local realItemNum = 0;
			for i = 1, num do
				if math.random(1, 100) <= 40 then
					realItemNum = realItemNum + 1;
					if realItemNum >= itemNum then
						break;
					end
				end
			end
			if realItemNum ~= 0 and package:AddItem(809,realItemNum,1) ~= nil then
				task:AddTaskStep2(94,1,realItemNum);
			end
		end
	end
end