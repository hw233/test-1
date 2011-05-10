function Monster_00004307(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(32001) then
		task:AddTaskStep2(32001,1,num);
	end
	if task:HasAcceptedTask(32005) then
		task:AddTaskStep2(32005,1,num);
	end
	if task:HasAcceptedTask(32003) then
		local itemNum = package:GetItemNum(15083,1);
		if itemNum < 1 then
			itemNum = 1 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15083, itemNum, 1) ~= nil then
				task:AddTaskStep2(32003,1,itemNum);
			end
		end
	end
end