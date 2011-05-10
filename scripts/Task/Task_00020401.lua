--����Ľ�������
function Task_Accept_00020401()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 22 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20401) or task:HasCompletedTask(20401) or task:HasSubmitedTask(20401) then
		return false;
	end
	if not task:HasSubmitedTask(20305) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020401()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 22 then
		return false;
	end
	if task:HasAcceptedTask(20401) or task:HasCompletedTask(20401) or task:HasSubmitedTask(20401) then
		return false;
	end
	if not task:HasSubmitedTask(20305) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020401()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20401) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020401(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20401) == npcId and Task_Accept_00020401 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20401
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "拜见难陀罗";
	elseif task:GetTaskSubmitNpc(20401) == npcId then
		if Task_Submit_00020401() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20401
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "拜见难陀罗";
		elseif task:HasAcceptedTask(20401) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20401
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "拜见难陀罗";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020401_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我的勇士，我已经从王后那里听说了你的事迹，知道你是一个勇敢而有潜力的新人，我知道你在对付荒郊的采花怪兽的时候受了伤，我想我有责任来指导你如何获得更加强大的力量。";
	action.m_ActionMsg = "请告诉我吧。";
	return action;
end

function Task_00020401_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "打开阵型，选择使用3人阵型，以后再遇到危险的情景，这个阵型会帮助你获得更好的攻防能力。现在回到前面让你损伤惨重的采花怪兽那里，再次把他们打败，练习这个阵型的使用吧。";
	action.m_ActionMsg = "让我来试试阵型到底有怎样的威力。";
	return action;
end

function Task_00020401_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "感受到阵型的强大力量了吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020401_step_table = {
		[1] = Task_00020401_step_01,
		[2] = Task_00020401_step_02,
		[10] = Task_00020401_step_10,
		};

function Task_00020401_step(step)
	if Task_00020401_step_table[step] ~= nil then
		return Task_00020401_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020401_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020401() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20401) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00020401_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(20401) then
		return false;
	end


	player:AddExp(2200);
	player:getCoin(2700);
	return true;
end

--��������
function Task_00020401_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20401);
end
