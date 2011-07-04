--����Ľ�������
function Task_Accept_00000110()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(110) or task:HasCompletedTask(110) or task:HasSubmitedTask(110) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000110()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(110) or task:HasCompletedTask(110) or task:HasSubmitedTask(110) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000110()
	if GetPlayer():GetTaskMgr():HasCompletedTask(110) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000110(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(110) == npcId and Task_Accept_00000110 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 110
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "110";
	elseif task:GetTaskSubmitNpc(110) == npcId then
		if Task_Submit_00000110() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 110
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "110";
		elseif task:HasAcceptedTask(110) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 110
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "110";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000110_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "齐漱溟让你去对话";
	action.m_ActionMsg = "齐漱溟让你去对话";
	return action;
end

function Task_00000110_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "齐漱溟让你去对话";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000110_step_table = {
		[1] = Task_00000110_step_01,
		[10] = Task_00000110_step_10,
		};

function Task_00000110_step(step)
	if Task_00000110_step_table[step] ~= nil then
		return Task_00000110_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000110_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000110() then
		return false;
	end
	if not task:AcceptTask(110) then
		return false;
	end
	task:AddTaskStep(110);
	return true;
end



--�ύ����
function Task_00000110_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(110) then
		if DayTaskAward(0) then
			task:DayTaskSubmit(110);
			return true;
		end
	end

	return false;
end

--��������
function Task_00000110_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(110);
end
