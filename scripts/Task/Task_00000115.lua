--����Ľ�������
function Task_Accept_00000115()
	local player = GetPlayer();
	if player:GetLev() < 60 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(115) or task:HasCompletedTask(115) or task:HasSubmitedTask(115) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(114) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(114) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(114) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000115()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 60 then
		return false;
	end
	if task:HasAcceptedTask(115) or task:HasCompletedTask(115) or task:HasSubmitedTask(115) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(114) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(114) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(114) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000115()
	if GetPlayer():GetTaskMgr():HasCompletedTask(115) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000115(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(115) == npcId and Task_Accept_00000115 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 115
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "水涧朱果";
	elseif task:GetTaskSubmitNpc(115) == npcId then
		if Task_Submit_00000115() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 115
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "水涧朱果";
		elseif task:HasAcceptedTask(115) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 115
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "水涧朱果";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000115_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."你知道不，离咱们马熊洞不远的地方有一处孤峰，孤峰里的一处水涧中生长着一种朱果，乃是天地奇珍，对修道之人大有裨益，只是那里被一只木魃占据，那朱果虽好却是不好轻易摘取，你看……";
	action.m_ActionMsg = "我看出来了，你的哈喇子都流到地上了。";
	return action;
end

function Task_00000115_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这下发达了，以后可以天天吃朱果了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000115_step_table = {
		[1] = Task_00000115_step_01,
		[10] = Task_00000115_step_10,
		};

function Task_00000115_step(step)
	if Task_00000115_step_table[step] ~= nil then
		return Task_00000115_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000115_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000115() then
		return false;
	end
	if not task:AcceptTask(115) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000115_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(115) then
		return false;
	end


	player:AddExp(34000);
	return true;
end

--��������
function Task_00000115_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(115);
end
