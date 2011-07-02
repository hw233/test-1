--����Ľ�������
function Task_Accept_00000104()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(104) or task:HasCompletedTask(104) or task:HasSubmitedTask(104) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000104()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(104) or task:HasCompletedTask(104) or task:HasSubmitedTask(104) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000104()
	if GetPlayer():GetTaskMgr():HasCompletedTask(104) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000104(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(104) == npcId and Task_Accept_00000104 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 104
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "为恶剑侠";
	elseif task:GetTaskSubmitNpc(104) == npcId then
		if Task_Submit_00000104() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 104
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "为恶剑侠";
		elseif task:HasAcceptedTask(104) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 104
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "为恶剑侠";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000104_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "之前听说慈云寺来了不少恶人，今日见百姓来告状才发觉竟是真事，有一个叫毛太的家伙时常来郊外骚扰良家妇女，还希望<name>去将这个毛太除掉。";
	action.m_ActionMsg = "为民除害是我辈本色，小可去去就回。";
	return action;
end

function Task_00000104_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "<name>你真是身手不凡啊。 ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000104_step_table = {
		[1] = Task_00000104_step_01,
		[10] = Task_00000104_step_10,
		};

function Task_00000104_step(step)
	if Task_00000104_step_table[step] ~= nil then
		return Task_00000104_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000104_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000104() then
		return false;
	end
	if not task:AcceptTask(104) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000104_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(104) then
		if DayTaskAward(0) then
			task:DayTaskSubmit(104);
			return true;
		end
	end

	return false;
end

--��������
function Task_00000104_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(104);
end
