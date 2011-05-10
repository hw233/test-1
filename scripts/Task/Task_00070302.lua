--����Ľ�������
function Task_Accept_00070302()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70302) or task:HasCompletedTask(70302) or task:HasSubmitedTask(70302) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00070302()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(70302) or task:HasCompletedTask(70302) or task:HasSubmitedTask(70302) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	if task:GetDayTaskCurrTaskId(1003) ~= 70302 then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00070302()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70302) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00070302(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70302) == npcId and Task_Accept_00070302 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70302
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "一个都不能留";
	elseif task:GetTaskSubmitNpc(70302) == npcId then
		if Task_Submit_00070302() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70302
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "一个都不能留";
		elseif task:HasAcceptedTask(70302) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70302
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "一个都不能留";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00070302_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "西来山谷的僵尸射手制造的麻烦越来越多了，不仅是我，许多周围的人也不断受到麻烦，我需要你挺身而出来，去观音山的西来山谷解决他们，无论如何一个都不能留。";
	action.m_ActionMsg = "我定能将他们杀个片甲不留，你放心！";
	return action;
end

function Task_00070302_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你真的把那些僵尸射手打的一个都没有留下？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070302_step_table = {
		[1] = Task_00070302_step_01,
		[10] = Task_00070302_step_10,
		};

function Task_00070302_step(step)
	if Task_00070302_step_table[step] ~= nil then
		return Task_00070302_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00070302_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070302() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70302) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00070302_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(70302) then
		if DayTaskAward(1003) then
			task:DayTaskSubmit(70302);
			return true;
		end
	end

	return false;
end

--��������
function Task_00070302_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70302);
end
