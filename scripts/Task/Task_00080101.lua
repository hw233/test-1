--����Ľ�������
function Task_Accept_00080101()
	local player = GetPlayer();
	if player:GetLev() < 45 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80101) or task:HasCompletedTask(80101) or (task:HasSubmitedTask(80101) and (GetSharpDay(task:GetTaskEndTime(80101)) == GetSharpDay(os.time()))) then 
		return false;
	end
	local otherTasks = { 80104, 80105, 80106, 80108, 80109, 80110, 80114, 80115, 80116, 80117, 80118, 80119 };
	for i, v in pairs(otherTasks) do
		if task:HasAcceptedTask(v) or task:HasCompletedTask(v) or (task:HasSubmitedTask(v) and (GetSharpDay(task:GetTaskEndTime(v)) == GetSharpDay(os.time()))) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080101()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 45 then
		return false;
	end
	if task:HasAcceptedTask(80101) or task:HasCompletedTask(80101) or (task:HasSubmitedTask(80101) and (GetSharpDay(task:GetTaskEndTime(80101)) == GetSharpDay(os.time()))) then 
		return false;
	end
	local otherTasks = { 80104, 80105, 80106, 80108, 80109, 80110, 80114, 80115, 80116, 80117, 80118, 80119 };
	for i, v in pairs(otherTasks) do
		if task:HasAcceptedTask(v) or task:HasCompletedTask(v) or (task:HasSubmitedTask(v) and (GetSharpDay(task:GetTaskEndTime(v)) == GetSharpDay(os.time()))) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00080101()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80101) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080101(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80101) == npcId and Task_Accept_00080101 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80101
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "焚骨窟魔王之邪髅王";
	elseif task:GetTaskSubmitNpc(80101) == npcId then
		if Task_Submit_00080101() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80101
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "焚骨窟魔王之邪髅王";
		elseif task:HasAcceptedTask(80101) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80101
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "焚骨窟魔王之邪髅王";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080101_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你是来修炼的吗？";
	action.m_ActionMsg = "什么才是真正的修炼？";
	return action;
end

function Task_00080101_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 3;
	action.m_NpcMsg = "真正的修炼是不断的改善心智，从而达到超越自我。";
	action.m_ActionMsg = "那我现在该做什么？";
	return action;
end

function Task_00080101_step_03()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我想你已经去过焚骨窟了，那里住着很多魔王，你的修炼将从除恶开始";
	action.m_ActionMsg = "我想我已经明白了，我这就去。";
	return action;
end

function Task_00080101_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "看来你的修炼进行得很顺利。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080101_step_table = {
		[1] = Task_00080101_step_01,
		[2] = Task_00080101_step_02,
		[3] = Task_00080101_step_03,
		[10] = Task_00080101_step_10,
		};

function Task_00080101_step(step)
	if Task_00080101_step_table[step] ~= nil then
		return Task_00080101_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080101_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080101() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80101) then
		return false;
	end
	local otherTasks = { 80104, 80105, 80106, 80108, 80109, 80110, 80114, 80115, 80116, 80117, 80118, 80119 };
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
function Task_00080101_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80101) then
		return false;
	end


	player:AddExp(20000);
	player:getCoin(10000);
	return true;
end

--��������
function Task_00080101_abandon()
	local task = GetPlayer():GetTaskMgr();
	if task:AbandonTask(80101) then
		if not task:TaskExist(80102) and not task:TaskExist(80103) then
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
