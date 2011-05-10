--����Ľ�������
function Task_Accept_00030705()
	local player = GetPlayer();
	if player:GetLev() < 49 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30705) or task:HasCompletedTask(30705) or task:HasSubmitedTask(30705) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30704) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30704) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030705()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 49 then
		return false;
	end
	if task:HasAcceptedTask(30705) or task:HasCompletedTask(30705) or task:HasSubmitedTask(30705) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30704) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30704) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030705()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30705) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030705(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30705) == npcId and Task_Accept_00030705 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30705
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "冰冷的祖言";
	elseif task:GetTaskSubmitNpc(30705) == npcId then
		if Task_Submit_00030705() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30705
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "冰冷的祖言";
		elseif task:HasAcceptedTask(30705) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30705
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "冰冷的祖言";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030705_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "做的很好勇士，看来你也是真心与夜摩盟为敌的，不过我们不像那些罗嗦的东山派，你愿意与我们镇邪派为伍，就帮我们战斗，不愿意就走开。";
	action.m_ActionMsg = "……";
	return action;
end

function Task_00030705_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "在镇子外面的黎朝废墟，有我们的镇邪护卫，你愿意就去找他们，为对抗夜摩盟付出一份力量，不愿意我们就再见了。";
	action.m_ActionMsg = "你还真是坏脾气，战斗是我最大的乐趣.";
	return action;
end

function Task_00030705_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "太好了，祖言帮主派人来帮助我们了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030705_step_table = {
		[1] = Task_00030705_step_01,
		[2] = Task_00030705_step_02,
		[10] = Task_00030705_step_10,
		};

function Task_00030705_step(step)
	if Task_00030705_step_table[step] ~= nil then
		return Task_00030705_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030705_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030705() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30705) then
		return false;
	end
	task:AddTaskStep(30705);
	return true;
end



--�ύ����
function Task_00030705_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30705) then
		return false;
	end


	player:AddExp(52000);
	player:getCoin(40000);
	return true;
end

--��������
function Task_00030705_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30705);
end
