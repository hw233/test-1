function Monster_00004113(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(40103) then
		local itemNum = package:GetItemNum(15055,1);
		if itemNum < 1 then
			itemNum = 1 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15055, itemNum, 1) ~= nil then
				task:AddTaskStep2(40103,1,itemNum);
			end
		end
	end
end