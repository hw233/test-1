function Monster_00004202(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(60403) then
		task:AddTaskStep2(60403,1,num);
	end
	if task:HasAcceptedTask(30301) then
		task:AddTaskStep2(30301,1,num);
	end
	if task:HasAcceptedTask(30302) then
		task:AddTaskStep2(30302,1,num);
	end
	if task:HasAcceptedTask(70504) then
		task:AddTaskStep2(70504,1,num);
	end
	if task:HasAcceptedTask(60302) then
		local itemNum = package:GetItemNum(15003,1);
		if itemNum < 12 then
			itemNum = 12 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15003, itemNum, 1) ~= nil then
				task:AddTaskStep2(60302,1,itemNum);
			end
		end
	end
end