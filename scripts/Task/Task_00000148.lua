--����Ľ�������
function Task_Accept_00000148()
	local player = GetPlayer();
	if player:GetLev() < 65 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(148) or task:HasCompletedTask(148) or task:HasSubmitedTask(148) then
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
function Task_Can_Accept_00000148()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 65 then
		return false;
	end
	if task:HasAcceptedTask(148) or task:HasCompletedTask(148) or task:HasSubmitedTask(148) then
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
function Task_Submit_00000148()
	if GetPlayer():GetTaskMgr():HasCompletedTask(148) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000148(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(148) == npcId and Task_Accept_00000148 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 148
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "无华氏";
	elseif task:GetTaskSubmitNpc(148) == npcId then
		if Task_Submit_00000148() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 148
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "无华氏";
		elseif task:HasAcceptedTask(148) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 148
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "无华氏";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000148_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这无华氏乃是上古山民之君，后来其子被轩辕黄帝囚禁困死，因为伤心爱子身死，不就也即逝去。后来他手下权臣北车在这庐山无华袕内，为他父子筑了一座巨大的墓穴，谁知他们竟然在墓内得了灵域地气成为妖孽，如今这些妖孽得了轩辕异宝，蠢蠢欲动，得早些铲除他们。";
	action.m_ActionMsg = "师太，我这就去三尸墓将他们消灭。";
	return action;
end

function Task_00000148_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."，这只妖尸想来已经修炼千年之久，今日终于死于你手。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000148_step_table = {
		[1] = Task_00000148_step_01,
		[10] = Task_00000148_step_10,
		};

function Task_00000148_step(step)
	if Task_00000148_step_table[step] ~= nil then
		return Task_00000148_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000148_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000148() then
		return false;
	end
	if not task:AcceptTask(148) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000148_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(148) then
		return false;
	end


	player:AddExp(100000);
	return true;
end

--��������
function Task_00000148_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(148);
end
