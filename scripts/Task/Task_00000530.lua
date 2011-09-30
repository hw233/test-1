--����Ľ�������
function Task_Accept_00000530()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(530) or task:HasCompletedTask(530) or task:HasSubmitedTask(530) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000530()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(530) or task:HasCompletedTask(530) or task:HasSubmitedTask(530) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000530()
	if GetPlayer():GetTaskMgr():HasCompletedTask(530) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000530(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(530) == npcId and Task_Accept_00000530 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 530
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "采花大盗";
	elseif task:GetTaskSubmitNpc(530) == npcId then
		if Task_Submit_00000530() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 530
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "采花大盗";
		elseif task:HasAcceptedTask(530) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 530
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "采花大盗";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000530_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "最近颇有些妖人在成都作乱，成都治安真得很成问题，听说有几个大户人家的小姐都被那采花大盗糟蹋，成都的官府对此束手无策，听说是剑侠一流在作案，"..GetPlayerName(GetPlayer()).."你去将这个淫贼捉拿归案。";
	action.m_ActionMsg = "我绝不会放过这些采花贼！";
	return action;
end

function Task_00000530_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."你真是身手不凡啊。 ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000530_step_table = {
		[1] = Task_00000530_step_01,
		[10] = Task_00000530_step_10,
		};

function Task_00000530_step(step)
	if Task_00000530_step_table[step] ~= nil then
		return Task_00000530_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000530_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000530() then
		return false;
	end
	if not task:AcceptTask(530) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000530_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(530) then
		return false;
	end


	return true;
end

--��������
function Task_00000530_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(530);
end
