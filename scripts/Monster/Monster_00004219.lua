function Monster_00004219(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(30408) then
		local itemNum = package:GetItemNum(15039,1);
		if itemNum < 12 then
			itemNum = 12 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15039, itemNum, 1) ~= nil then
				task:AddTaskStep2(30408,1,itemNum);
			end
		end
	end
end