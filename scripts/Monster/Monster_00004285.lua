function Monster_00004285(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(61902) then
		task:AddTaskStep2(61902,1,num);
	end
	if task:HasAcceptedTask(80067) then
		local itemNum = package:GetItemNum(15096,1);
		if itemNum < 50 then
			itemNum = 50 - itemNum;
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
			if realItemNum ~= 0 and package:AddItem(15096,realItemNum,1) ~= nil then
				task:AddTaskStep2(80067,1,realItemNum);
			end
		end
	end
end