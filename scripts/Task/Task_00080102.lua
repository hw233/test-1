--����Ľ�������
function Task_Accept_00080102()
	local player = GetPlayer();
	if player:GetLev() < 50 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80102) or task:HasCompletedTask(80102) or (task:HasSubmitedTask(80102) and (GetSharpDay(task:GetTaskEndTime(80102)) == GetSharpDay(os.time()))) then 
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
function Task_Can_Accept_00080102()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 50 then
		return false;
	end
	if task:HasAcceptedTask(80102) or task:HasCompletedTask(80102) or (task:HasSubmitedTask(80102) and (GetSharpDay(task:GetTaskEndTime(80102)) == GetSharpDay(os.time()))) then 
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
function Task_Submit_00080102()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80102) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080102(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80102) == npcId and Task_Accept_00080102 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80102
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "焚骨窟魔王之阿罗多";
	elseif task:GetTaskSubmitNpc(80102) == npcId then
		if Task_Submit_00080102() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80102
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "焚骨窟魔王之阿罗多";
		elseif task:HasAcceptedTask(80102) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80102
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "焚骨窟魔王之阿罗多";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080102_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "不同的修炼目标会产生不同的修炼结果。";
	action.m_ActionMsg = "是的，我需要得到更多的灵气。";
	return action;
end

function Task_00080102_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你可以将焚骨窟当作修炼的战场，在那里收集你所需要的灵气,因为那里,唉,有那么多战士的亡魂.";
	action.m_ActionMsg = "战争是可怕的,却也是无可避免的啊,但愿他们会安息.";
	return action;
end

function Task_00080102_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "修行状况如何?";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080102_step_table = {
		[1] = Task_00080102_step_01,
		[2] = Task_00080102_step_02,
		[10] = Task_00080102_step_10,
		};

function Task_00080102_step(step)
	if Task_00080102_step_table[step] ~= nil then
		return Task_00080102_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080102_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080102() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80102) then
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
function Task_00080102_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80102) then
		return false;
	end


	player:AddExp(30000);
	player:getCoin(20000);
	return true;
end

--��������
function Task_00080102_abandon()
	local task = GetPlayer():GetTaskMgr();
	if task:AbandonTask(80102) then
		if not task:TaskExist(80101) and not task:TaskExist(80103) then
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
