function Monster_00004227(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(30604) then
		task:AddTaskStep2(30604,1,num);
	end
	if task:HasAcceptedTask(80045) then
		local itemNum = package:GetItemNum(15087,1);
		if itemNum < 15 then
			itemNum = 15 - itemNum;
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
			if realItemNum ~= 0 and package:AddItem(15087,realItemNum,1) ~= nil then
				task:AddTaskStep2(80045,1,realItemNum);
			end
		end
	end
end