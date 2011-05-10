--����Ľ�������
function Task_Accept_00070306()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70306) or task:HasCompletedTask(70306) or task:HasSubmitedTask(70306) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00070306()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(70306) or task:HasCompletedTask(70306) or task:HasSubmitedTask(70306) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	if task:GetDayTaskCurrTaskId(1003) ~= 70306 then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00070306()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70306) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00070306(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70306) == npcId and Task_Accept_00070306 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70306
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "狼魔头子只是个传说";
	elseif task:GetTaskSubmitNpc(70306) == npcId then
		if Task_Submit_00070306() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70306
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "狼魔头子只是个传说";
		elseif task:HasAcceptedTask(70306) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70306
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "狼魔头子只是个传说";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00070306_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，走过那片王舍城废墟，你可曾听说，有一个屠魔英雄，他再也没有回来过。为何片片白云为他落泪，为何阵阵风儿，为他诉说，他真的是个英雄，还是只是个传说？去杀掉王舍城废墟黑风岗的狼魔首领，也许你就明白了到底什么事传说。";
	action.m_ActionMsg = "我去杀掉王舍城废墟黑风岗的狼魔首领后，也能成为英雄。";
	return action;
end

function Task_00070306_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "到底狼魔是传说，还是英雄是传说呢？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070306_step_table = {
		[1] = Task_00070306_step_01,
		[10] = Task_00070306_step_10,
		};

function Task_00070306_step(step)
	if Task_00070306_step_table[step] ~= nil then
		return Task_00070306_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00070306_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070306() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70306) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00070306_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(70306) then
		if DayTaskAward(1003) then
			task:DayTaskSubmit(70306);
			return true;
		end
	end

	return false;
end

--��������
function Task_00070306_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70306);
end
