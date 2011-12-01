--����Ľ�������
function Task_Accept_00000075()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(75) or task:HasCompletedTask(75) or task:HasSubmitedTask(75) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(74) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(74) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(74) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000075()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(75) or task:HasCompletedTask(75) or task:HasSubmitedTask(75) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(74) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(74) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(74) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000075()
	if GetPlayer():GetTaskMgr():HasCompletedTask(75) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000075(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(75) == npcId and Task_Accept_00000075 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 75
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "妖人之祸";
	elseif task:GetTaskSubmitNpc(75) == npcId then
		if Task_Submit_00000075() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 75
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "妖人之祸";
		elseif task:HasAcceptedTask(75) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 75
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "妖人之祸";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000075_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "想不到鱼神洞里如此险恶，若不是少侠搭救，我差点就出不来了。不过刚刚我在暗处听他们交谈，吕村请来了一个厉害的妖道和南疆的妖人，听说很难对付。";
	action.m_ActionMsg = "有这种事？";
	return action;
end

function Task_00000075_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嗯，少侠赶快回去戴家场告诉大家吧，要尽早商量对策。";
	action.m_ActionMsg = "你身上的伤没事吧？";
	return action;
end

function Task_00000075_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这样厉害的妖人，让我们这些普通的江湖武夫怎么对付才好？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000075_step_table = {
		[1] = Task_00000075_step_01,
		[2] = Task_00000075_step_02,
		[10] = Task_00000075_step_10,
		};

function Task_00000075_step(step)
	if Task_00000075_step_table[step] ~= nil then
		return Task_00000075_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000075_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000075() then
		return false;
	end
	if not task:AcceptTask(75) then
		return false;
	end
	task:AddTaskStep(75);
	return true;
end



--�ύ����
function Task_00000075_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(75) then
		return false;
	end


	player:AddExp(12000);
	return true;
end

--��������
function Task_00000075_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(75);
end
