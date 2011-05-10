function Monster_00004250(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80054) then
		task:AddTaskStep2(80054,1,num);
	end
	if task:HasAcceptedTask(61401) then
		local itemNum = package:GetItemNum(15013,1);
		if itemNum < 10 then
			itemNum = 10 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15013, itemNum, 1) ~= nil then
				task:AddTaskStep2(61401,1,itemNum);
			end
		end
	end
end