--����Ľ�������
function Task_Accept_00000628()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(628) or task:HasCompletedTask(628) or task:HasSubmitedTask(628) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000628()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(628) or task:HasCompletedTask(628) or task:HasSubmitedTask(628) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000628()
	if GetPlayer():GetTaskMgr():HasCompletedTask(628) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000628(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(628) == npcId and Task_Accept_00000628 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 628
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "作恶多端";
	elseif task:GetTaskSubmitNpc(628) == npcId then
		if Task_Submit_00000628() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 628
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "作恶多端";
		elseif task:HasAcceptedTask(628) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 628
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "作恶多端";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000628_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "成都前段时间治安真得很成问题，有一些慈云寺出来的妖人在附近为恶，祸害百姓，他们都是传说中的剑仙一流，我们普通捕快奈何不了他们，还希望"..GetPlayerName(GetPlayer()).."去将这些恶人除掉。";
	action.m_ActionMsg = "为民除害是我辈本色，小可去去就回。";
	return action;
end

function Task_00000628_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."你真是身手不凡啊。 ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000628_step_table = {
		[1] = Task_00000628_step_01,
		[10] = Task_00000628_step_10,
		};

function Task_00000628_step(step)
	if Task_00000628_step_table[step] ~= nil then
		return Task_00000628_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000628_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000628() then
		return false;
	end
	if not task:AcceptTask(628) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000628_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(628) then
		return false;
	end


	player:AddExp(3000);
	return true;
end

--��������
function Task_00000628_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(628);
end
