--����Ľ�������
function Task_Accept_00080110()
	local player = GetPlayer();
	if player:GetLev() < 80 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80110) or task:HasCompletedTask(80110) or (task:HasSubmitedTask(80110) and (GetSharpDay(task:GetTaskEndTime(80110)) == GetSharpDay(os.time()))) then 
		return false;
	end
	local otherTasks = { 80101, 80102, 80103, 80104, 80105, 80106, 80114, 80115, 80116, 80117, 80118, 80119 };
	for i, v in pairs(otherTasks) do
		if task:HasAcceptedTask(v) or task:HasCompletedTask(v) or (task:HasSubmitedTask(v) and (GetSharpDay(task:GetTaskEndTime(v)) == GetSharpDay(os.time()))) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080110()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 80 then
		return false;
	end
	if task:HasAcceptedTask(80110) or task:HasCompletedTask(80110) or (task:HasSubmitedTask(80110) and (GetSharpDay(task:GetTaskEndTime(80110)) == GetSharpDay(os.time()))) then 
		return false;
	end
	local otherTasks = { 80101, 80102, 80103, 80104, 80105, 80106, 80114, 80115, 80116, 80117, 80118, 80119 };
	for i, v in pairs(otherTasks) do
		if task:HasAcceptedTask(v) or task:HasCompletedTask(v) or (task:HasSubmitedTask(v) and (GetSharpDay(task:GetTaskEndTime(v)) == GetSharpDay(os.time()))) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00080110()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80110) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080110(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80110) == npcId and Task_Accept_00080110 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80110
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "困仙魔王之秦仙人";
	elseif task:GetTaskSubmitNpc(80110) == npcId then
		if Task_Submit_00080110() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80110
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "困仙魔王之秦仙人";
		elseif task:HasAcceptedTask(80110) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80110
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "困仙魔王之秦仙人";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080110_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "秦仙人是困仙阵最后一个大魔王，他比以往你遇到过的魔怪都要厉害百倍，要加倍小心。";
	action.m_ActionMsg = "不管他有多厉害我都会让他死于我的刀下。";
	return action;
end

function Task_00080110_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你的修炼已经到了较高境界，但切记骄傲。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080110_step_table = {
		[1] = Task_00080110_step_01,
		[10] = Task_00080110_step_10,
		};

function Task_00080110_step(step)
	if Task_00080110_step_table[step] ~= nil then
		return Task_00080110_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080110_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080110() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80110) then
		return false;
	end
	local otherTasks = { 80101, 80102, 80103, 80104, 80105, 80106, 80114, 80115, 80116, 80117, 80118, 80119 };
	for i, v in pairs(otherTasks) do
		if i ~= #otherTasks then
			task:DelCanAcceptTask2(v, false);
		else
			task:DelCanAcceptTask2(v, true);
		end
	end
	return true;
end



--�ύ����
function Task_00080110_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80110) then
		return false;
	end


	player:AddExp(200000);
	player:getCoin(180000);
	return true;
end

--��������
function Task_00080110_abandon()
	local task = GetPlayer():GetTaskMgr();
	if task:AbandonTask(80110) then
		if not task:TaskExist(80108) and not task:TaskExist(80109) then
			local otherTasks = { 80101, 80102, 80103, 80104, 80105, 80106, 80108, 80109, 80110, 80114, 80115, 80116, 80117, 80118, 80119};
			for i, v in pairs(otherTasks) do
				if task:TaskCanAccept(v) then
					task:AddCanAcceptTask2(v, false);
				end
			end
			task:SendCanAcceptTaskInfor();
		end
		return true;
	end
	return false;
end
