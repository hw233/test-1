function Monster_00005018(num)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();
	if task:HasAcceptedTask(517) then
		task:AddTaskStep2(517,1,num);
	end
	if task:HasAcceptedTask(567) then
		task:AddTaskStep2(567,1,num);
	end
	if task:HasAcceptedTask(614) then
		task:AddTaskStep2(614,1,num);
	end
	if task:HasAcceptedTask(634) then
		task:AddTaskStep2(634,1,num);
	end
	if task:HasAcceptedTask(542) then
		task:AddTaskStep2(542,1,num);
	end
	if task:HasAcceptedTask(587) then
		task:AddTaskStep2(587,1,num);
	end
	if task:HasAcceptedTask(58) then
		task:AddTaskStep2(58,1,num);
	end
	if task:HasAcceptedTask(129) then
		task:AddTaskStep2(129,1,num);
	end
end