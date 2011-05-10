--����Ľ�������
function Task_Accept_00080103()
	local player = GetPlayer();
	if player:GetLev() < 55 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80103) or task:HasCompletedTask(80103) or (task:HasSubmitedTask(80103) and (GetSharpDay(task:GetTaskEndTime(80103)) == GetSharpDay(os.time()))) then 
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
function Task_Can_Accept_00080103()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 55 then
		return false;
	end
	if task:HasAcceptedTask(80103) or task:HasCompletedTask(80103) or (task:HasSubmitedTask(80103) and (GetSharpDay(task:GetTaskEndTime(80103)) == GetSharpDay(os.time()))) then 
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
function Task_Submit_00080103()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80103) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080103(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80103) == npcId and Task_Accept_00080103 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80103
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "焚骨窟魔王之赶尸教主";
	elseif task:GetTaskSubmitNpc(80103) == npcId then
		if Task_Submit_00080103() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80103
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "焚骨窟魔王之赶尸教主";
		elseif task:HasAcceptedTask(80103) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80103
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "焚骨窟魔王之赶尸教主";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080103_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "路漫漫其修远兮，勇士，辛苦你了。";
	action.m_ActionMsg = "我为求强大而努力。";
	return action;
end

function Task_00080103_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "强大须从点滴做起。";
	action.m_ActionMsg = "谨遵教诲！ ";
	return action;
end

function Task_00080103_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你的修炼让我很满意！";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080103_step_table = {
		[1] = Task_00080103_step_01,
		[2] = Task_00080103_step_02,
		[10] = Task_00080103_step_10,
		};

function Task_00080103_step(step)
	if Task_00080103_step_table[step] ~= nil then
		return Task_00080103_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080103_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080103() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80103) then
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
function Task_00080103_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80103) then
		return false;
	end


	player:AddExp(50000);
	player:getCoin(30000);
	return true;
end

--��������
function Task_00080103_abandon()
	local task = GetPlayer():GetTaskMgr();
	if task:AbandonTask(80103) then
		if not task:TaskExist(80101) and not task:TaskExist(80102) then
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
