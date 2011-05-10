--����Ľ�������
function Task_Accept_00080109()
	local player = GetPlayer();
	if player:GetLev() < 80 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80109) or task:HasCompletedTask(80109) or (task:HasSubmitedTask(80109) and (GetSharpDay(task:GetTaskEndTime(80109)) == GetSharpDay(os.time()))) then 
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
function Task_Can_Accept_00080109()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 80 then
		return false;
	end
	if task:HasAcceptedTask(80109) or task:HasCompletedTask(80109) or (task:HasSubmitedTask(80109) and (GetSharpDay(task:GetTaskEndTime(80109)) == GetSharpDay(os.time()))) then 
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
function Task_Submit_00080109()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80109) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080109(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80109) == npcId and Task_Accept_00080109 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80109
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "困仙魔王之双煞称霸";
	elseif task:GetTaskSubmitNpc(80109) == npcId then
		if Task_Submit_00080109() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80109
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "困仙魔王之双煞称霸";
		elseif task:HasAcceptedTask(80109) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80109
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "困仙魔王之双煞称霸";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080109_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你的进步让我惊讶，接下来的修炼会更艰难，我相信你能行。";
	action.m_ActionMsg = "我想变得更加强大，这样就能为族里的英雄，保护我的族人。";
	return action;
end

function Task_00080109_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你继续修炼吧，我在此等候你。";
	action.m_ActionMsg = "我定能拿下卫青和霍去病的人头来见你。";
	return action;
end

function Task_00080109_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你即将成为英勇无比的人。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080109_step_table = {
		[1] = Task_00080109_step_01,
		[2] = Task_00080109_step_02,
		[10] = Task_00080109_step_10,
		};

function Task_00080109_step(step)
	if Task_00080109_step_table[step] ~= nil then
		return Task_00080109_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080109_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080109() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80109) then
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
function Task_00080109_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80109) then
		return false;
	end


	player:AddExp(180000);
	player:getCoin(150000);
	return true;
end

--��������
function Task_00080109_abandon()
	local task = GetPlayer():GetTaskMgr();
	if task:AbandonTask(80109) then
		if not task:TaskExist(80108) and not task:TaskExist(80110) then
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
