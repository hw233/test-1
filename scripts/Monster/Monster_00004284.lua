function Monster_00004284(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80066) then
		task:AddTaskStep2(80066,1,num);
	end
	if task:HasAcceptedTask(31501) then
		local itemNum = package:GetItemNum(15074,1);
		if itemNum < 1 then
			itemNum = 1 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15074, itemNum, 1) ~= nil then
				task:AddTaskStep2(31501,1,itemNum);
			end
		end
	end
end