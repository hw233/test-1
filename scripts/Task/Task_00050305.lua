--����Ľ�������
function Task_Accept_00050305()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50305) or task:HasCompletedTask(50305) or task:HasSubmitedTask(50305) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050305()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(50305) or task:HasCompletedTask(50305) or task:HasSubmitedTask(50305) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050305()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50305) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050305(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50305) == npcId and Task_Accept_00050305 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50305
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "比武试炼";
	elseif task:GetTaskSubmitNpc(50305) == npcId then
		if Task_Submit_00050305() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50305
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "比武试炼";
		elseif task:HasAcceptedTask(50305) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50305
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
function Task_00050305_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."，你该进入实战训练了，去竞技场吧，那里有我们魔族无数的勇士在和天族对抗， 你去见识见识吧。";
	action.m_ActionMsg = "我不会让魔族失望的。";
	return action;
end

function Task_00050305_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我等你的好消息，用你的实力去证明你自己吧。";
	action.m_ActionMsg = "请放心。";
	return action;
end

function Task_00050305_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "现在，你看到我们魔族的力量有多么强大了吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050305_step_table = {
		[1] = Task_00050305_step_01,
		[2] = Task_00050305_step_02,
		[10] = Task_00050305_step_10,
		};

function Task_00050305_step(step)
	if Task_00050305_step_table[step] ~= nil then
		return Task_00050305_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050305_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050305() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50305) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00050305_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50305) then
		return false;
	end


	player:AddExp(2000);
	player:getCoin(10000);
	player:getTael(120);
	return true;
end

--��������
function Task_00050305_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50305);
end
