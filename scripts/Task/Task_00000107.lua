--����Ľ�������
function Task_Accept_00000107()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(107) or task:HasCompletedTask(107) or task:HasSubmitedTask(107) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(106) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(106) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(106) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000107()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(107) or task:HasCompletedTask(107) or task:HasSubmitedTask(107) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(106) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(106) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(106) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000107()
	if GetPlayer():GetTaskMgr():HasCompletedTask(107) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000107(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(107) == npcId and Task_Accept_00000107 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 107
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "留人寨";
	elseif task:GetTaskSubmitNpc(107) == npcId then
		if Task_Submit_00000107() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 107
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "留人寨";
		elseif task:HasAcceptedTask(107) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 107
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "留人寨";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000107_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "在不远处有一处留人寨，乃是五毒天王列霸多手下的据点之一，他们与绿袍勾结已久，专门劫掠过往行人，我听说绿袍和列霸多都喜欢生啖活人，实在是令人发指，这些为虎作伥的妖人一个都不能留，"..GetPlayerName(GetPlayer()).."你这就去除掉他们吧。";
	action.m_ActionMsg = "这些禽兽不如的妖人，我一定不放过他们。";
	return action;
end

function Task_00000107_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这些邪派魔头勾结在一起，到是要小心点应付才好。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000107_step_table = {
		[1] = Task_00000107_step_01,
		[10] = Task_00000107_step_10,
		};

function Task_00000107_step(step)
	if Task_00000107_step_table[step] ~= nil then
		return Task_00000107_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000107_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000107() then
		return false;
	end
	if not task:AcceptTask(107) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000107_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(107) then
		return false;
	end


	player:AddExp(4444);
	return true;
end

--��������
function Task_00000107_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(107);
end
