--4_03_04 比武试炼
function OperationTaskAction0_0001()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local taskId;
	if GetPlayerData(6) == 0 then
		taskId = 40304;
	else
		taskId = 50305;
	end
	if task:HasAcceptedTask(taskId) then
		task:AddTaskStep(taskId);
	end
end

--4_03_05 实力的较量
function OperationTaskAction0_0002()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local taskId;
	if GetPlayerData(6) == 0 then
		taskId = 40305;
	else
		taskId = 50306;
	end
	if task:HasAcceptedTask(taskId) then
		task:AddTaskStep(taskId);
	end	
end

--
function OperationTaskAction0_0003()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local taskId;
	if GetPlayerData(6) == 0 then
		taskId = 40409;
	else
		taskId = 50409;
	end	
	if task:HasAcceptedTask(taskId) then
		task:AddTaskStep(taskId);
	end
end

--竞技
function OperationTaskAction0_0004()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(80111) then
		task:AddTaskStep(80111);
	end	
end

--送喜好品
function OperationTaskAction0_0005()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(80112) then
		task:AddTaskStep(80112);
	end		
end

--加入宗族
function OperationTaskAction0_0006()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if task:HasAcceptedTask(62206) then
		task:AddTaskStep(62206);
	end
end


local OperationTaskAction0_Table = {
	[1] = OperationTaskAction0_0001,
	[2] = OperationTaskAction0_0002,
	[3] = OperationTaskAction0_0003,
	[4] = OperationTaskAction0_0004,
	[5] = OperationTaskAction0_0005,
	[6] = OperationTaskAction0_0006,
};

function RunOperationTaskAction0(op)
	if OperationTaskAction0_Table[op] ~= nil then
		return OperationTaskAction0_Table[op]();
	end
	return false;
end

--------------------------------------------------------
--------------------------------------------------------


--4_05_05 提炼自我
function OperationTaskAction1_0001(param1)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if GetPlayerData(6) == 0 then
		taskId = 40505;
	else
		taskId = 50505;
	end	
	if task:HasAcceptedTask(taskId) then
		task:AddTaskStep2(taskId, param1, 1);
	end
end

--国战任务
function OperationTaskAction1_0002(param1)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local taskId;
--	print("res = " .. param1);
	if param1 == 1 then
		--胜利
		if GetPlayerData(6) == 0 then
			taskId = 80202;
		else
			taskId = 80302;
		end
		if task:HasAcceptedTask(taskId) then
			task:AddTaskStep(taskId);
		end
	end
	if GetPlayerData(6) == 0 then
		taskId = 80201;
	else
		taskId = 80301;
	end	
	if task:HasAcceptedTask(taskId) then
		task:AddTaskStep(taskId);
	end
end


local OperationTaskAction1_Table = {
	[1] = OperationTaskAction1_0001,
	[2] = OperationTaskAction1_0002,
};

function RunOperationTaskAction1(op, param1)
	if OperationTaskAction1_Table[op] ~= nil then
		return OperationTaskAction1_Table[op](param1);
	end
	return false;
end

------------------------------------------------------
------------------------------------------------------
--一次副本任务
function OperationTaskAction3_0001(param1, param2, param3)
	if param1 == 1 then
		local player = GetPlayer();
		local task = player:GetTaskMgr();
		if task:HasAcceptedTask(62204) then
			task:AddTaskStep(62204);
		end			
	end
end

--
function OperationTaskAction3_0002(_id, difficulty, level)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	--print("OperationTaskAction3_0002 " .. _id .. " " .. difficulty .. " " .. level);
	if _id == 1 and difficulty == 0 then
		if task:HasSubmitedTask(62204) and task:HasAcceptedTask(62205) then
			task:AddTaskStep4(62205, 1, level+1);
		end
	end
	if _id == 1 and difficulty == 1 and level == 5 then
		if task:HasAcceptedTask(80101) then
			task:AddTaskStep(80101);
		end		
	end
	if _id == 1 and difficulty == 1 and level == 10 then
		if task:HasAcceptedTask(80102) then
			task:AddTaskStep(80102);
		end			
	end
	if _id == 1 and difficulty == 1 and level == 15 then
		if task:HasAcceptedTask(80103) then
			task:AddTaskStep(80103);
		end			
	end	
	if _id == 2 and difficulty == 1 and level == 5 then
		if task:HasAcceptedTask(80104) then
			task:AddTaskStep(80104);
		end			
	end	
	if _id == 2 and difficulty == 1 and level == 12 then
		if task:HasAcceptedTask(80105) then
			task:AddTaskStep(80105);
		end			
	end
	if _id == 2 and difficulty == 1 and level == 17 then
		if task:HasAcceptedTask(80106) then
			task:AddTaskStep(80106);
		end			
	end		
	if _id == 3 and difficulty == 1 and level == 12 then
		if task:HasAcceptedTask(80107) then
			task:AddTaskStep(80107);
		end			
	end
	if _id == 3 and difficulty == 1 and level == 19 then
		if task:HasAcceptedTask(80108) then
			task:AddTaskStep(80108);
		end			
	end
	if _id == 3 and difficulty == 1 and level == 38 then
		if task:HasAcceptedTask(80109) then
			task:AddTaskStep(80109);
		end			
	end	
	if _id == 3 and difficulty == 1 and level == 44 then
		if task:HasAcceptedTask(80110) then
			task:AddTaskStep(80110);
		end			
	end	
	if _id == 4 and difficulty == 1 and level == 5 then
		if task:HasAcceptedTask(80114) then
			task:AddTaskStep(80114);
		end
	end
	if _id == 4 and difficulty == 1 and level == 15 then
		if task:HasAcceptedTask(80115) then
			task:AddTaskStep(80115);
		end		
	end
	if _id == 4 and difficulty == 1 and level == 20 then
		if task:HasAcceptedTask(80116) then
			task:AddTaskStep(80116);
		end		
	end	
	if _id == 5 and difficulty == 1 and level == 5 then
		if task:HasAcceptedTask(80117) then
			task:AddTaskStep(80117);
		end		
	end		
	if _id == 5 and difficulty == 1 and level == 10 then
		if task:HasAcceptedTask(80118) then
			task:AddTaskStep(80118);
		end		
	end	
	if _id == 5 and difficulty == 1 and level == 20 then
		if task:HasAcceptedTask(80119) then
			task:AddTaskStep(80119);
		end		
	end	
end

local OperationTaskAction3_Table = {
	[1] = OperationTaskAction3_0001,
	[2] = OperationTaskAction3_0002,
};

function RunOperationTaskAction3(op, param1, param2, param3)
	if OperationTaskAction3_Table[op] ~= nil then
		return OperationTaskAction3_Table[op](param1, param2, param3);
	end
	return false;
end



------------------------------------------------------
------------------------------------------------------
--一次副本任务
function OperationTaskAction2_0001(param1, param2)
	if param1 == 1 then
		local player = GetPlayer();
		local task = player:GetTaskMgr();
		if task:HasAcceptedTask(60) then
			task:AddTaskStep(60);
		end			
	end
end

--
function OperationTaskAction2_0002(_id, level)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	--print("OperationTaskAction2_0002 " .. _id .. " " .. difficulty .. " " .. level);
	if _id == 1 and level == 5 then
		if task:HasAcceptedTask(60) then
			task:AddTaskStep(60);
		end		
	end
end

local OperationTaskAction2_Table = {
	[1] = OperationTaskAction2_0001,
	[2] = OperationTaskAction2_0002,
};

function RunOperationTaskAction2(op, param1, param2)
	if OperationTaskAction2_Table[op] ~= nil then
		return OperationTaskAction2_Table[op](param1, param2);
	end
	return false;
end


