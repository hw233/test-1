--����Ľ�������
function Task_Accept_00000575()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(575) or task:HasCompletedTask(575) or task:HasSubmitedTask(575) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000575()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(575) or task:HasCompletedTask(575) or task:HasSubmitedTask(575) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000575()
	if GetPlayer():GetTaskMgr():HasCompletedTask(575) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000575(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(575) == npcId and Task_Accept_00000575 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 575
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "正道弃徒";
	elseif task:GetTaskSubmitNpc(575) == npcId then
		if Task_Submit_00000575() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 575
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "正道弃徒";
		elseif task:HasAcceptedTask(575) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 575
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "正道弃徒";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000575_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "长沙郊外有一个恶人，之前是万里飞虹冬元奇的弟子，因为心术不正被逐出师门，如今在长沙郊外为恶，"..GetPlayerName(GetPlayer()).."你这就去将这个恶人除掉。";
	action.m_ActionMsg = "弟子遵命，我去去就回。";
	return action;
end

function Task_00000575_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."你真是身手不凡啊。 ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000575_step_table = {
		[1] = Task_00000575_step_01,
		[10] = Task_00000575_step_10,
		};

function Task_00000575_step(step)
	if Task_00000575_step_table[step] ~= nil then
		return Task_00000575_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000575_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000575() then
		return false;
	end
	if not task:AcceptTask(575) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000575_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(575) then
		return false;
	end


	return true;
end

--��������
function Task_00000575_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(575);
end
