--����Ľ�������
function Task_Accept_00090102()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90102) or task:HasCompletedTask(90102) or task:HasSubmitedTask(90102) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90101) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90101) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090102()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90102) or task:HasCompletedTask(90102) or task:HasSubmitedTask(90102) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90101) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90101) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00090102()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90102) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090102(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90102) == npcId and Task_Accept_00090102 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90102
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "美好的期望";
	elseif task:GetTaskSubmitNpc(90102) == npcId then
		if Task_Submit_00090102() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90102
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "美好的期望";
		elseif task:HasAcceptedTask(90102) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90102
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "美好的期望";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090102_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "战争何时休！";
	action.m_ActionMsg = "大家都希望世界是和平的，可那些妖魔鬼怪总要破坏世间和平。";
	return action;
end

function Task_00090102_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "放下屠刀，立地成佛，方可和平。";
	action.m_ActionMsg = "我会将长老的话带到。";
	return action;
end

function Task_00090102_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "谁也不想常年征战，谁都想天伦之乐。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090102_step_table = {
		[1] = Task_00090102_step_01,
		[2] = Task_00090102_step_02,
		[10] = Task_00090102_step_10,
		};

function Task_00090102_step(step)
	if Task_00090102_step_table[step] ~= nil then
		return Task_00090102_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090102_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090102() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90102) then
		return false;
	end
	task:AddTaskStep(90102);
	return true;
end



--�ύ����
function Task_00090102_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90102) then
		return false;
	end


	player:AddExp(500000);
	player:getCoin(300000);
	return true;
end

--��������
function Task_00090102_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90102);
end
