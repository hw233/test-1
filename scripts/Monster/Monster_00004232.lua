function Monster_00004232(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(61102) then
		task:AddTaskStep2(61102,1,num);
	end
	if task:HasAcceptedTask(80048) then
		task:AddTaskStep2(80048,1,num);
	end
	if task:HasAcceptedTask(61101) then
		local itemNum = package:GetItemNum(15010,1);
		if itemNum < 1 then
			itemNum = 1 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15010, itemNum, 1) ~= nil then
				task:AddTaskStep2(61101,1,itemNum);
			end
		end
	end
end