function Monster_00004187(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(20505) then
		task:AddTaskStep2(20505,1,num);
	end
	if task:HasAcceptedTask(20501) then
		task:AddTaskStep2(20501,1,num);
	end
	if task:HasAcceptedTask(50904) then
		task:AddTaskStep2(50904,1,num);
	end
	if task:HasAcceptedTask(20501) then
		task:AddTaskStep2(20501,1,num);
	end
	if GetPlayerData(6) == 1 then
		if task:HasCompletedTask(60201) or task:HasAcceptedTask(60201) or task:HasSubmitedTask(60201) then
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