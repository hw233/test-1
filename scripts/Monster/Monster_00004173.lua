function Monster_00004173(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(20401) then
		task:AddTaskStep2(20401,1,num);
	end
	if task:HasAcceptedTask(20304) then
		local itemNum = package:GetItemNum(15029,1);
		if itemNum < 1 then
			itemNum = 1 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15029, itemNum, 1) ~= nil then
				task:AddTaskStep2(20304,1,itemNum);
			end
		end
	end
end