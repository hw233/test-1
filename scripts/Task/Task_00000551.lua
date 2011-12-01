--����Ľ�������
function Task_Accept_00000551()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(551) or task:HasCompletedTask(551) or task:HasSubmitedTask(551) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000551()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(551) or task:HasCompletedTask(551) or task:HasSubmitedTask(551) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000551()
	if GetPlayer():GetTaskMgr():HasCompletedTask(551) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000551(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(551) == npcId and Task_Accept_00000551 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 551
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "教诲";
	elseif task:GetTaskSubmitNpc(551) == npcId then
		if Task_Submit_00000551() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 551
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "教诲";
		elseif task:HasAcceptedTask(551) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 551
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "教诲";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000551_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."我们昆仑派源远流长，也是天下正道气运所在，作为昆仑弟子一定要时刻自省其身。你师叔韦少少入昆仑已久，受训颇严，今日你就去师叔处聆听教诲吧。";
	action.m_ActionMsg = "谨遵掌教之令。";
	return action;
end

function Task_00000551_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我看你心存正气，将来必然会将我派发扬光大。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000551_step_table = {
		[1] = Task_00000551_step_01,
		[10] = Task_00000551_step_10,
		};

function Task_00000551_step(step)
	if Task_00000551_step_table[step] ~= nil then
		return Task_00000551_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000551_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000551() then
		return false;
	end
	if not task:AcceptTask(551) then
		return false;
	end
	task:AddTaskStep(551);
	return true;
end



--�ύ����
function Task_00000551_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(551) then
		return false;
	end


	return true;
end

--��������
function Task_00000551_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(551);
end
