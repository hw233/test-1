function Monster_00004238(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(61301) then
		task:AddTaskStep2(61301,1,num);
	end
	if task:HasAcceptedTask(80051) then
		local itemNum = package:GetItemNum(15090,1);
		if itemNum < 20 then
			itemNum = 20 - itemNum;
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
			if realItemNum ~= 0 and package:AddItem(15090,realItemNum,1) ~= nil then
				task:AddTaskStep2(80051,1,realItemNum);
			end
		end
	end
end