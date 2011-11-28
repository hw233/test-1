--����Ľ�������
function Task_Accept_00000523()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(523) or task:HasCompletedTask(523) or task:HasSubmitedTask(523) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000523()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(523) or task:HasCompletedTask(523) or task:HasSubmitedTask(523) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000523()
	if GetPlayer():GetTaskMgr():HasCompletedTask(523) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000523(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(523) == npcId and Task_Accept_00000523 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 523
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "化外妖道";
	elseif task:GetTaskSubmitNpc(523) == npcId then
		if Task_Submit_00000523() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 523
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "化外妖道";
		elseif task:HasAcceptedTask(523) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 523
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "化外妖道";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000523_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "长沙郊外有一些恶人，受了魔教中人的蛊惑，在长沙为恶，他们还请来了化外的妖道使用邪法，"..GetPlayerName(GetPlayer()).."我看就派你去一程，铲除了这些恶人。";
	action.m_ActionMsg = "几个恶人不在话下，我去去就回。";
	return action;
end

function Task_00000523_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."你果然是身手不凡。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000523_step_table = {
		[1] = Task_00000523_step_01,
		[10] = Task_00000523_step_10,
		};

function Task_00000523_step(step)
	if Task_00000523_step_table[step] ~= nil then
		return Task_00000523_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000523_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000523() then
		return false;
	end
	if not task:AcceptTask(523) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000523_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(523) then
		return false;
	end


	return true;
end

--��������
function Task_00000523_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(523);
end
