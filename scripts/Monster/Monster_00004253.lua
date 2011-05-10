function Monster_00004253(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(61403) then
		task:AddTaskStep2(61403,1,num);
	end
end