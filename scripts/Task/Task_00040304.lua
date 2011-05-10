--����Ľ�������
function Task_Accept_00040304()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40304) or task:HasCompletedTask(40304) or task:HasSubmitedTask(40304) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040304()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(40304) or task:HasCompletedTask(40304) or task:HasSubmitedTask(40304) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040304()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40304) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040304(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40304) == npcId and Task_Accept_00040304 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40304
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "比武试炼";
	elseif task:GetTaskSubmitNpc(40304) == npcId then
		if Task_Submit_00040304() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40304
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "比武试炼";
		elseif task:HasAcceptedTask(40304) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40304
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "比武试炼";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040304_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."，你该进入实战训练了，去竞技场吧，那里有我们天族无数的勇士在和魔族对抗， 你去见识见识吧。";
	action.m_ActionMsg = "我不会让天族失望的。";
	return action;
end

function Task_00040304_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我等你的好消息，用你的实力去证明你自己吧。";
	action.m_ActionMsg = "请放心。";
	return action;
end

function Task_00040304_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "现在，你看到我们天族的力量有多么强大了吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040304_step_table = {
		[1] = Task_00040304_step_01,
		[2] = Task_00040304_step_02,
		[10] = Task_00040304_step_10,
		};

function Task_00040304_step(step)
	if Task_00040304_step_table[step] ~= nil then
		return Task_00040304_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040304_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040304() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40304) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00040304_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40304) then
		return false;
	end


	player:AddExp(2000);
	player:getCoin(10000);
	player:getTael(120);
	return true;
end

--��������
function Task_00040304_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40304);
end
