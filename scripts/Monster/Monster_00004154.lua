function Monster_00004154(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(20407) then
		local itemNum = package:GetItemNum(15030,1);
		if itemNum < 1 then
			itemNum = 1 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15030, itemNum, 1) ~= nil then
				task:AddTaskStep2(20407,1,itemNum);
			end
		end
	end
end