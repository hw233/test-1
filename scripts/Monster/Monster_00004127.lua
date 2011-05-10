function Monster_00004127(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(10302) then
		local itemNum = package:GetItemNum(15021,1);
		if itemNum < 1 then
			itemNum = 1 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15021, itemNum, 1) ~= nil then
				task:AddTaskStep2(10302,1,itemNum);
			end
		end
	end
end