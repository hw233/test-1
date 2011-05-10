function Monster_00004356(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80033) then
		task:AddTaskStep2(80033,1,num);
	end
end