--����Ľ�������
function Task_Accept_00000107()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(107) or task:HasCompletedTask(107) or task:HasSubmitedTask(107) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000107()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(107) or task:HasCompletedTask(107) or task:HasSubmitedTask(107) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000107()
	if GetPlayer():GetTaskMgr():HasCompletedTask(107) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000107(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(107) == npcId and Task_Accept_00000107 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 107
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "107";
	elseif task:GetTaskSubmitNpc(107) == npcId then
		if Task_Submit_00000107() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 107
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "107";
		elseif task:HasAcceptedTask(107) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 107
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "107";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000107_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "齐漱溟让你去对话";
	action.m_ActionMsg = "齐漱溟让你去对话";
	return action;
end

function Task_00000107_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你好";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000107_step_table = {
		[1] = Task_00000107_step_01,
		[10] = Task_00000107_step_10,
		};

function Task_00000107_step(step)
	if Task_00000107_step_table[step] ~= nil then
		return Task_00000107_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000107_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000107() then
		return false;
	end
	if not task:AcceptTask(107) then
		return false;
	end
	task:AddTaskStep(107);
	return true;
end



--�ύ����
function Task_00000107_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(107) then
		if DayTaskAward(0) then
			task:DayTaskSubmit(107);
			return true;
		end
	end

	return false;
end

--��������
function Task_00000107_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(107);
end
