function Monster_00004209(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(70305) then
		task:AddTaskStep2(70305,1,num);
	end
	if task:HasAcceptedTask(70315) then
		task:AddTaskStep2(70315,1,num);
	end
	if task:HasAcceptedTask(60402) then
		local itemNum = package:GetItemNum(15004,1);
		if itemNum < 12 then
			itemNum = 12 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15004, itemNum, 1) ~= nil then
				task:AddTaskStep2(60402,1,itemNum);
			end
		end
	end
	if task:HasAcceptedTask(60501) then
		local itemNum = package:GetItemNum(15006,1);
		if itemNum < 20 then
			itemNum = 20 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15006, itemNum, 1) ~= nil then
				task:AddTaskStep2(60501,1,itemNum);
			end
		end
	end
end