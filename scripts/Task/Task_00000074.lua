--����Ľ�������
function Task_Accept_00000074()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(74) or task:HasCompletedTask(74) or task:HasSubmitedTask(74) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(69) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(69) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(69) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000074()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(74) or task:HasCompletedTask(74) or task:HasSubmitedTask(74) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(69) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(69) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(69) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000074()
	if GetPlayer():GetTaskMgr():HasCompletedTask(74) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000074(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(74) == npcId and Task_Accept_00000074 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 74
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "探路先锋";
	elseif task:GetTaskSubmitNpc(74) == npcId then
		if Task_Submit_00000074() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 74
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "探路先锋";
		elseif task:HasAcceptedTask(74) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 74
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "探路先锋";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000074_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "少侠愿意帮助戴家场对付妖人，真是太好了，可是方才和少侠一起来得小伙子许超不见了。下人们告诉我说他要去探查一下吕村的动静，一个人跑去了鱼神洞，这可就不妙了。";
	action.m_ActionMsg = "怎么回事？";
	return action;
end

function Task_00000074_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "鱼神洞有不少吕村的妖人驻扎，他们想打通这条山洞好真奔我们戴家场而来，我怕徐超出什么意外，少侠还得尽快去找他。";
	action.m_ActionMsg = "好，我这就去。";
	return action;
end

function Task_00000074_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "想不到鱼神洞里如此险恶，我刚来就被这些恶人困在此处。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000074_step_table = {
		[1] = Task_00000074_step_01,
		[2] = Task_00000074_step_02,
		[10] = Task_00000074_step_10,
		};

function Task_00000074_step(step)
	if Task_00000074_step_table[step] ~= nil then
		return Task_00000074_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000074_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000074() then
		return false;
	end
	if not task:AcceptTask(74) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000074_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(74) then
		return false;
	end


	player:AddExp(17000);
	return true;
end

--��������
function Task_00000074_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(74);
end
