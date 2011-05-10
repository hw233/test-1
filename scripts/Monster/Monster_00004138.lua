function Monster_00004138(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(10505) then
		task:AddTaskStep2(10505,1,num);
	end
	if task:HasAcceptedTask(10501) then
		task:AddTaskStep2(10501,1,num);
	end
	if GetPlayerData(6) == 0 then
		if task:HasCompletedTask(60101) or task:HasAcceptedTask(60101) or task:HasSubmitedTask(60101) then
			return false;
		end	
		if package:ExistItem(15001) then
			return false;
		end
		if package:AddItem(15001, 1, 1) == nil then
			return false;
		end			
	end
	return true;
end