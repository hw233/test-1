function Monster_00004270(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(70309) then
		task:AddTaskStep2(70309,1,num);
	end
	if task:HasAcceptedTask(80061) then
		local itemNum = package:GetItemNum(15094,1);
		if itemNum < 40 then
			itemNum = 40 - itemNum;
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
			if realItemNum ~= 0 and package:AddItem(15094,realItemNum,1) ~= nil then
				task:AddTaskStep2(80061,1,realItemNum);
			end
		end
	end
end