--����Ľ�������
function Task_Accept_00070304()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70304) or task:HasCompletedTask(70304) or task:HasSubmitedTask(70304) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00070304()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(70304) or task:HasCompletedTask(70304) or task:HasSubmitedTask(70304) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	if task:GetDayTaskCurrTaskId(1003) ~= 70304 then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00070304()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70304) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00070304(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70304) == npcId and Task_Accept_00070304 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70304
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "今天不打怪明天空悲切";
	elseif task:GetTaskSubmitNpc(70304) == npcId then
		if Task_Submit_00070304() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70304
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "今天不打怪明天空悲切";
		elseif task:HasAcceptedTask(70304) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70304
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "今天不打怪明天空悲切";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00070304_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "据专家测算，山崩崖的雷电恶龙再次出现了，真是死性不改啊，怎么办，只能你去了，不要浪费这修炼的好时机啊，今天不不去杀掉他们，明天空悲切啊。";
	action.m_ActionMsg = "少来不努力，老大徒悲伤。这个道理我是明白的。";
	return action;
end

function Task_00070304_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你今天已经杀掉了那些雷电恶龙，明天是不用空悲切了，不过明天还是要杀掉他们，这样后天才不用空悲切啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070304_step_table = {
		[1] = Task_00070304_step_01,
		[10] = Task_00070304_step_10,
		};

function Task_00070304_step(step)
	if Task_00070304_step_table[step] ~= nil then
		return Task_00070304_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00070304_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070304() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70304) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00070304_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(70304) then
		if DayTaskAward(1003) then
			task:DayTaskSubmit(70304);
			return true;
		end
	end

	return false;
end

--��������
function Task_00070304_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70304);
end
