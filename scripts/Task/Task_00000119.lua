--����Ľ�������
function Task_Accept_00000119()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(119) or task:HasCompletedTask(119) or task:HasSubmitedTask(119) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(118) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(118) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(118) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000119()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(119) or task:HasCompletedTask(119) or task:HasSubmitedTask(119) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(118) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(118) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(118) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000119()
	if GetPlayer():GetTaskMgr():HasCompletedTask(119) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000119(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(119) == npcId and Task_Accept_00000119 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 119
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "冰蚕";
	elseif task:GetTaskSubmitNpc(119) == npcId then
		if Task_Submit_00000119() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 119
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "冰蚕";
		elseif task:HasAcceptedTask(119) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 119
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "冰蚕";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000119_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "莽苍山这处冰穴真是世所罕见，在冰穴深处孕育着一只数百年道行的冰蚕，阴寒无比，若被那些邪道人士获得，练成邪功就是一场灾难了，"..GetPlayerName(GetPlayer()).."不如你去冰穴内将它降服。";
	action.m_ActionMsg = "嗯，我这就去会会这只冰蚕。";
	return action;
end

function Task_00000119_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这个冰蚕真是天地奇珍。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000119_step_table = {
		[1] = Task_00000119_step_01,
		[10] = Task_00000119_step_10,
		};

function Task_00000119_step(step)
	if Task_00000119_step_table[step] ~= nil then
		return Task_00000119_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000119_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000119() then
		return false;
	end
	if not task:AcceptTask(119) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000119_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(119) then
		return false;
	end


	player:AddExp(2222);
	return true;
end

--��������
function Task_00000119_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(119);
end
