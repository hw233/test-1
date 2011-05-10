function Monster_00004472(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80801) then
		task:AddTaskStep2(80801,1,num);
	end
end