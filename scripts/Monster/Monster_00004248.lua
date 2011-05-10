function Monster_00004248(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(30901) then
		task:AddTaskStep2(30901,1,num);
	end
	if task:HasAcceptedTask(30902) then
		task:AddTaskStep2(30902,1,num);
	end
	if task:HasAcceptedTask(80053) then
		local itemNum = package:GetItemNum(15091,1);
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
			if realItemNum ~= 0 and package:AddItem(15091,realItemNum,1) ~= nil then
				task:AddTaskStep2(80053,1,realItemNum);
			end
		end
	end
end