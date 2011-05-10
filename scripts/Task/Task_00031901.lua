--����Ľ�������
function Task_Accept_00031901()
	local player = GetPlayer();
	if player:GetLev() < 70 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31901) or task:HasCompletedTask(31901) or task:HasSubmitedTask(31901) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31805) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31805) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031901()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 70 then
		return false;
	end
	if task:HasAcceptedTask(31901) or task:HasCompletedTask(31901) or task:HasSubmitedTask(31901) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31805) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31805) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031901()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31901) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031901(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31901) == npcId and Task_Accept_00031901 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31901
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "遇到貂蝉";
	elseif task:GetTaskSubmitNpc(31901) == npcId then
		if Task_Submit_00031901() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31901
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "遇到貂蝉";
		elseif task:HasAcceptedTask(31901) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31901
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "遇到貂蝉";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031901_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士"..GetPlayerName(GetPlayer()).."，可让我等到你了，我费尽千辛万苦穿越到山海大陆来追随吕布，却不曾想，还未见到他的面，却得到了是你杀掉他的消息。";
	action.m_ActionMsg = "你可是美女貂蝉？";
	return action;
end

function Task_00031901_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "就是我，我与吕布缘分未尽，所以我追随他到现在所在的地方来了，你最好能告诉我，为什么要这样做。";
	action.m_ActionMsg = "唉真的是一个误会。";
	return action;
end

function Task_00031901_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士不是我有意对你无理的，可是对我来你，你就是杀死我的爱人的凶手。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031901_step_table = {
		[1] = Task_00031901_step_01,
		[2] = Task_00031901_step_02,
		[10] = Task_00031901_step_10,
		};

function Task_00031901_step(step)
	if Task_00031901_step_table[step] ~= nil then
		return Task_00031901_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031901_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031901() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31901) then
		return false;
	end
	task:AddTaskStep(31901);
	return true;
end



--�ύ����
function Task_00031901_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(31901) then
		return false;
	end


	player:AddExp(230000);
	player:getCoin(217500);
	player:getTael(40);
	return true;
end

--��������
function Task_00031901_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31901);
end
