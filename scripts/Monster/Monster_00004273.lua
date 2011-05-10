function Monster_00004273(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80063) then
		task:AddTaskStep2(80063,1,num);
	end
	if task:HasAcceptedTask(61702) then
		local itemNum = package:GetItemNum(15016,1);
		if itemNum < 1 then
			itemNum = 1 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15016, itemNum, 1) ~= nil then
				task:AddTaskStep2(61702,1,itemNum);
			end
		end
	end
end