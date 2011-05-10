--����Ľ�������
function Task_Accept_00031001()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 57 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31001) or task:HasCompletedTask(31001) or task:HasSubmitedTask(31001) then
		return false;
	end
	if not task:HasSubmitedTask(30906) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031001()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 57 then
		return false;
	end
	if task:HasAcceptedTask(31001) or task:HasCompletedTask(31001) or task:HasSubmitedTask(31001) then
		return false;
	end
	if not task:HasSubmitedTask(30906) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00031001()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31001) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031001(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31001) == npcId and Task_Accept_00031001 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31001
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "如烟身世";
	elseif task:GetTaskSubmitNpc(31001) == npcId then
		if Task_Submit_00031001() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31001
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "如烟身世";
		elseif task:HasAcceptedTask(31001) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31001
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "如烟身世";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031001_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你好，能在这人烟稀少的寒冷之地见到我们魔族的勇士真是太好了。";
	action.m_ActionMsg = "美女，你怎会孤身一人在这险恶的山海大陆的寒冷寺庙里？";
	return action;
end

function Task_00031001_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我叫如烟，跟勇士你一样也是来自魔族，可是几年前我被迫嫁给了天族在山海大陆的大使蒙不凡，并且跟随他来到了山海大陆，可是几天前我出门迷路了，找不到他，只能躲在这寺庙中等待好心人的救助。";
	action.m_ActionMsg = "真是一位可怜的女子。";
	return action;
end

function Task_00031001_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "太好了，总算让我等到了一个能来救助我的人了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031001_step_table = {
		[1] = Task_00031001_step_01,
		[2] = Task_00031001_step_02,
		[10] = Task_00031001_step_10,
		};

function Task_00031001_step(step)
	if Task_00031001_step_table[step] ~= nil then
		return Task_00031001_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031001_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031001() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31001) then
		return false;
	end
	task:AddTaskStep(31001);
	return true;
end



--�ύ����
function Task_00031001_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(31001) then
		return false;
	end


	player:AddExp(84000);
	player:getCoin(84000);
	player:getTael(20);
	return true;
end

--��������
function Task_00031001_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31001);
end
