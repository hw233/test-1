function Monster_00004257(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80056) then
		task:AddTaskStep2(80056,1,num);
	end
	if task:HasAcceptedTask(61502) then
		local itemNum = package:GetItemNum(15014,1);
		if itemNum < 1 then
			itemNum = 1 - itemNum;
			if itemNum > num then itemNum = num; end
			if package:AddItem(15014, itemNum, 1) ~= nil then
				task:AddTaskStep2(61502,1,itemNum);
			end
		end
	end
	if task:HasCompletedTask(61601) or task:HasAcceptedTask(61601) or task:HasSubmitedTask(61601) then
		return false;
	end
	if package:ExistItem(15015) then
		return false;
	end
	if package:AddItem(15015, 1, 1) == nil then
		return false;
	end	
	return true;
end