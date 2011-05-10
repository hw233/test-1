--����Ľ�������
function Task_Accept_00070301()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70301) or task:HasCompletedTask(70301) or task:HasSubmitedTask(70301) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00070301()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(70301) or task:HasCompletedTask(70301) or task:HasSubmitedTask(70301) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	if task:GetDayTaskCurrTaskId(1003) ~= 70301 then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00070301()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70301) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00070301(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70301) == npcId and Task_Accept_00070301 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70301
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "大侠中的战斗机";
	elseif task:GetTaskSubmitNpc(70301) == npcId then
		if Task_Submit_00070301() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70301
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "大侠中的战斗机";
		elseif task:HasAcceptedTask(70301) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70301
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "大侠中的战斗机";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00070301_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "侠中之大，就是要不畏任何危险和困难。瞿卢岭的绝世林正是一个能够证明你的地方，打败那里的怨灵射手，你就是大侠中的战斗机。";
	action.m_ActionMsg = "那些小家伙你也让我去打?那好吧.";
	return action;
end

function Task_00070301_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "膜拜啊，大侠中的战斗机。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070301_step_table = {
		[1] = Task_00070301_step_01,
		[10] = Task_00070301_step_10,
		};

function Task_00070301_step(step)
	if Task_00070301_step_table[step] ~= nil then
		return Task_00070301_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00070301_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070301() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70301) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00070301_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(70301) then
		if DayTaskAward(1003) then
			task:DayTaskSubmit(70301);
			return true;
		end
	end

	return false;
end

--��������
function Task_00070301_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70301);
end
