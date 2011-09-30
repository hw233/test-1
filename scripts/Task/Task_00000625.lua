--����Ľ�������
function Task_Accept_00000625()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(625) or task:HasCompletedTask(625) or task:HasSubmitedTask(625) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000625()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(625) or task:HasCompletedTask(625) or task:HasSubmitedTask(625) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000625()
	if GetPlayer():GetTaskMgr():HasCompletedTask(625) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000625(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(625) == npcId and Task_Accept_00000625 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 625
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "为恶剑侠";
	elseif task:GetTaskSubmitNpc(625) == npcId then
		if Task_Submit_00000625() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 625
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "为恶剑侠";
		elseif task:HasAcceptedTask(625) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 625
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
function Task_00000625_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "之前听说慈云寺来了不少恶人，今日见百姓来告状才发觉竟是真事，有一个叫毛太的家伙时常来郊外骚扰良家妇女，还希望"..GetPlayerName(GetPlayer()).."去将这个毛太除掉。";
	action.m_ActionMsg = "为民除害是我辈本色，小可去去就回。";
	return action;
end

function Task_00000625_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."你真是身手不凡啊。 ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000625_step_table = {
		[1] = Task_00000625_step_01,
		[10] = Task_00000625_step_10,
		};

function Task_00000625_step(step)
	if Task_00000625_step_table[step] ~= nil then
		return Task_00000625_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000625_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000625() then
		return false;
	end
	if not task:AcceptTask(625) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000625_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(625) then
		return false;
	end


	player:AddExp(3000);
	return true;
end

--��������
function Task_00000625_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(625);
end
