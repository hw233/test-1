--����Ľ�������
function Task_Accept_00000541()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(541) or task:HasCompletedTask(541) or task:HasSubmitedTask(541) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000541()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(541) or task:HasCompletedTask(541) or task:HasSubmitedTask(541) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000541()
	if GetPlayer():GetTaskMgr():HasCompletedTask(541) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000541(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(541) == npcId and Task_Accept_00000541 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 541
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "飞龙师太";
	elseif task:GetTaskSubmitNpc(541) == npcId then
		if Task_Submit_00000541() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 541
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "飞龙师太";
		elseif task:HasAcceptedTask(541) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 541
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "飞龙师太";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000541_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "成都不远处的桂花山是一处美景，不过山中有五色云雾，寻常人轻易不得入内，听说有个室外高人隐居在山中，布下了五色云雾。前不久有个奇怪的道姑来成都，似乎要去福仙潭找什么乌风草，与本地居民发生争吵，辣手屠灭了一个村子，你速速去桂花山将这个邪恶道姑消灭。";
	action.m_ActionMsg = "竟然有这种凶恶之人！";
	return action;
end

function Task_00000541_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "昔年我与飞龙师太也有一面之缘，却想不到她如今已堕落成邪派之人。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000541_step_table = {
		[1] = Task_00000541_step_01,
		[10] = Task_00000541_step_10,
		};

function Task_00000541_step(step)
	if Task_00000541_step_table[step] ~= nil then
		return Task_00000541_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000541_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000541() then
		return false;
	end
	if not task:AcceptTask(541) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000541_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(541) then
		return false;
	end


	return true;
end

--��������
function Task_00000541_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(541);
end
