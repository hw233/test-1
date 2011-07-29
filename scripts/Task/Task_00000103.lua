--����Ľ�������
function Task_Accept_00000103()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(103) or task:HasCompletedTask(103) or task:HasSubmitedTask(103) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(102) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(102) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(102) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000103()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(103) or task:HasCompletedTask(103) or task:HasSubmitedTask(103) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(102) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(102) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(102) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000103()
	if GetPlayer():GetTaskMgr():HasCompletedTask(103) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000103(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(103) == npcId and Task_Accept_00000103 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 103
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "青牛剑";
	elseif task:GetTaskSubmitNpc(103) == npcId then
		if Task_Submit_00000103() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 103
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "青牛剑";
		elseif task:HasAcceptedTask(103) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 103
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "青牛剑";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000103_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这青牛剑我一共炼制了七把，尚未见血，"..GetPlayerName(GetPlayer()).."这柄青牛剑我就赠送与你，拿去助那金针圣母兵解吧，虽然她当年玄女针毒辣异常，但也罪不当绝。";
	action.m_ActionMsg = "多谢前辈赐剑。";
	return action;
end

function Task_00000103_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "多谢少侠，有青牛剑助我兵解，我总算可以避开此次劫难了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000103_step_table = {
		[1] = Task_00000103_step_01,
		[10] = Task_00000103_step_10,
		};

function Task_00000103_step(step)
	if Task_00000103_step_table[step] ~= nil then
		return Task_00000103_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000103_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000103() then
		return false;
	end
	if not task:AcceptTask(103) then
		return false;
	end
	task:AddTaskStep(103);
	return true;
end



--�ύ����
function Task_00000103_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(103) then
		return false;
	end


	player:AddExp(54555);
	return true;
end

--��������
function Task_00000103_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(103);
end
