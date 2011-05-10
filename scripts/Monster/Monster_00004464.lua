function Monster_00004464(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80601) then
		task:AddTaskStep2(80601,1,num);
	end
	if task:HasAcceptedTask(80602) then
		task:AddTaskStep2(80602,1,num);
	end
end