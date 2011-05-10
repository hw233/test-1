function Monster_00004234(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80050) then
		task:AddTaskStep2(80050,1,num);
	end
	if task:HasAcceptedTask(61202) then
		task:AddTaskStep2(61202,1,num);
	end
	if task:HasAcceptedTask(30801) then
		local itemNum = package:GetItemNum(15059,1);
		if itemNum < 12 then
			itemNum = 12 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15059, itemNum, 1) ~= nil then
				task:AddTaskStep2(30801,1,itemNum);
			end
		end
	end
end