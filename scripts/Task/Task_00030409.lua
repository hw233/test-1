--����Ľ�������
function Task_Accept_00030409()
	local player = GetPlayer();
	if player:GetLev() < 39 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30409) or task:HasCompletedTask(30409) or task:HasSubmitedTask(30409) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30408) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30408) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030409()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 39 then
		return false;
	end
	if task:HasAcceptedTask(30409) or task:HasCompletedTask(30409) or task:HasSubmitedTask(30409) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30408) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30408) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030409()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30409) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030409(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30409) == npcId and Task_Accept_00030409 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30409
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "试用武器";
	elseif task:GetTaskSubmitNpc(30409) == npcId then
		if Task_Submit_00030409() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30409
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "试用武器";
		elseif task:HasAcceptedTask(30409) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30409
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "试用武器";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030409_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这位勇士，拿到新的武器，该去试试了吧，离我这茅屋不远处的黑烟谷，总是有一群疯狂狼骑在那里，如果你战斗安排和武器使用得当，击败他们应该不成问题，去试试看吧。";
	action.m_ActionMsg = "好的。";
	return action;
end

function Task_00030409_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "打完了记得回来找我，让我知道你能很好的使用这把武器，对付这些烦人的怪物了。";
	action.m_ActionMsg = "难道你还在怀疑我的实力吗？那好吧.";
	return action;
end

function Task_00030409_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我想那些狼妖看到你马上就害怕了吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030409_step_table = {
		[1] = Task_00030409_step_01,
		[2] = Task_00030409_step_02,
		[10] = Task_00030409_step_10,
		};

function Task_00030409_step(step)
	if Task_00030409_step_table[step] ~= nil then
		return Task_00030409_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030409_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030409() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30409) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00030409_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30409) then
		return false;
	end


	player:AddExp(8000);
	player:getCoin(12000);
	return true;
end

--��������
function Task_00030409_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30409);
end
