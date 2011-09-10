function Monster_00005002(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(601) then
		task:AddTaskStep2(601,1,num);
	end
	if task:HasAcceptedTask(651) then
		task:AddTaskStep2(651,1,num);
	end
	if task:HasAcceptedTask(12) then
		local itemNum = package:GetItemNum(801,1);
		if itemNum < 1 then
			itemNum = 1 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(801, itemNum, 1) ~= nil then
				task:AddTaskStep2(12,1,itemNum);
			end
		end
	end
end