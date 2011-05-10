--����Ľ�������
function Task_Accept_00020302()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 19 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20302) or task:HasCompletedTask(20302) or task:HasSubmitedTask(20302) then
		return false;
	end
	if not task:HasSubmitedTask(20301) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020302()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 19 then
		return false;
	end
	if task:HasAcceptedTask(20302) or task:HasCompletedTask(20302) or task:HasSubmitedTask(20302) then
		return false;
	end
	if not task:HasSubmitedTask(20301) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020302()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20302) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020302(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20302) == npcId and Task_Accept_00020302 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20302
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "文隣之墓";
	elseif task:GetTaskSubmitNpc(20302) == npcId then
		if Task_Submit_00020302() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20302
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "文隣之墓";
		elseif task:HasAcceptedTask(20302) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20302
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "文隣之墓";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020302_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "文隣之墓  香音王后敬";
	action.m_ActionMsg = "难道跟香音王后有关？看来我要去找一下王后。";
	return action;
end

function Task_00020302_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，你是为何而来？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020302_step_table = {
		[1] = Task_00020302_step_01,
		[10] = Task_00020302_step_10,
		};

function Task_00020302_step(step)
	if Task_00020302_step_table[step] ~= nil then
		return Task_00020302_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020302_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020302() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20302) then
		return false;
	end
	task:AddTaskStep(20302);
	return true;
end



--�ύ����
function Task_00020302_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(20302) then
		return false;
	end


	player:AddExp(1800);
	player:getCoin(1900);
	return true;
end

--��������
function Task_00020302_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20302);
end
