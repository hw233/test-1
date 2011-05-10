function Monster_00004264(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(31201) then
		task:AddTaskStep2(31201,1,num);
	end
	if task:HasAcceptedTask(80058) then
		local itemNum = package:GetItemNum(15093,1);
		if itemNum < 30 then
			itemNum = 30 - itemNum;
			if itemNum > num then itemNum = num; end
			local realItemNum = 0;
			for i = 1, num do
				if math.random(1, 100) <= 10 then
					realItemNum = realItemNum + 1;
					if realItemNum >= itemNum then
						break;
					end
				end
			end
			if realItemNum ~= 0 and package:AddItem(15093,realItemNum,1) ~= nil then
				task:AddTaskStep2(80058,1,realItemNum);
			end
		end
	end
end