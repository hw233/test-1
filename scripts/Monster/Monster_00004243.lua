function Monster_00004243(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(80052) then
		task:AddTaskStep2(80052,1,num);
	end
	if task:HasAcceptedTask(30704) then
		task:AddTaskStep2(30704,1,num);
	end
end