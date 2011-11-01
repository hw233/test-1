--����Ľ�������
function Task_Accept_00000146()
	local player = GetPlayer();
	if player:GetLev() < 65 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(146) or task:HasCompletedTask(146) or task:HasSubmitedTask(146) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000146()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 65 then
		return false;
	end
	if task:HasAcceptedTask(146) or task:HasCompletedTask(146) or task:HasSubmitedTask(146) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000146()
	if GetPlayer():GetTaskMgr():HasCompletedTask(146) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000146(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(146) == npcId and Task_Accept_00000146 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 146
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "妖尸戎敦";
	elseif task:GetTaskSubmitNpc(146) == npcId then
		if Task_Submit_00000146() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 146
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "妖尸戎敦";
		elseif task:HasAcceptedTask(146) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 146
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "妖尸戎敦";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000146_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "妖尸戎敦乃是无华氏之子，他禀天地乖戾之气而生，自幼即具神力，能手搏飞龙，生裂犀象。上古的时候因为和蚩尤氏勾结，被轩辕黄帝囚禁，后来他与其父在这三尸墓中得灵域地气成了气候，从此为恶庐山。";
	action.m_ActionMsg = "师太，我这就去三尸墓将他们消灭。";
	return action;
end

function Task_00000146_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."，这只妖尸想来已经修炼千年之久，今日终于死于你手。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000146_step_table = {
		[1] = Task_00000146_step_01,
		[10] = Task_00000146_step_10,
		};

function Task_00000146_step(step)
	if Task_00000146_step_table[step] ~= nil then
		return Task_00000146_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000146_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000146() then
		return false;
	end
	if not task:AcceptTask(146) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000146_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(146) then
		return false;
	end


	player:AddExp(80000);
	return true;
end

--��������
function Task_00000146_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(146);
end
