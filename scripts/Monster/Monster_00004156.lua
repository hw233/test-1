function Monster_00004156(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(50402) then
		task:AddTaskStep2(50402,1,num);
	end
	if task:HasAcceptedTask(50103) then
		local itemNum = package:GetItemNum(15048,1);
		if itemNum < 1 then
			itemNum = 1 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15048, itemNum, 1) ~= nil then
				task:AddTaskStep2(50103,1,itemNum);
			end
		end
	end
end