function Monster_00004268(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80060) then
		task:AddTaskStep2(80060,1,num);
	end
	if task:HasAcceptedTask(61704) then
		task:AddTaskStep2(61704,1,num);
	end
	if task:HasAcceptedTask(31302) then
		local itemNum = package:GetItemNum(15068,1);
		if itemNum < 1 then
			itemNum = 1 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15068, itemNum, 1) ~= nil then
				task:AddTaskStep2(31302,1,itemNum);
			end
		end
	end
end