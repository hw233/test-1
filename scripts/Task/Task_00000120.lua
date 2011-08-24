--����Ľ�������
function Task_Accept_00000120()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(120) or task:HasCompletedTask(120) or task:HasSubmitedTask(120) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000120()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(120) or task:HasCompletedTask(120) or task:HasSubmitedTask(120) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000120()
	if GetPlayer():GetTaskMgr():HasCompletedTask(120) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000120(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(120) == npcId and Task_Accept_00000120 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 120
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "百禽道人";
	elseif task:GetTaskSubmitNpc(120) == npcId then
		if Task_Submit_00000120() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 120
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "百禽道人";
		elseif task:HasAcceptedTask(120) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 120
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "百禽道人";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000120_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000120_step_table = {
		[10] = Task_00000120_step_10,
		};

function Task_00000120_step(step)
	if Task_00000120_step_table[step] ~= nil then
		return Task_00000120_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000120_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000120() then
		return false;
	end
	if not task:AcceptTask(120) then
		return false;
	end
	task:AddTaskStep(120);
	return true;
end



--�ύ����
function Task_00000120_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(120) then
		return false;
	end


	return true;
end

--��������
function Task_00000120_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(120);
end
