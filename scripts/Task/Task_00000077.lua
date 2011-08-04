--����Ľ�������
function Task_Accept_00000077()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(77) or task:HasCompletedTask(77) or task:HasSubmitedTask(77) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000077()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(77) or task:HasCompletedTask(77) or task:HasSubmitedTask(77) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000077()
	if GetPlayer():GetTaskMgr():HasCompletedTask(77) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000077(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(77) == npcId and Task_Accept_00000077 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 77
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "义救湘英";
	elseif task:GetTaskSubmitNpc(77) == npcId then
		if Task_Submit_00000077() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 77
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "义救湘英";
		elseif task:HasAcceptedTask(77) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 77
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "义救湘英";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000077_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "莫非你就是戴家那帮小子请来的什么剑仙？看你这副乳臭味干的样子如何是咱们庄主请来的神仙郭道长的对手。";
	action.m_ActionMsg = "郭老道很厉害吗？";
	return action;
end

function Task_00000077_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "什么郭老道，要称郭仙长，方才你们戴家的小女娃来捣乱，就被仙长他老人家出手擒下关在螺丝湾里。劝你还是不要来趟戴家的这潭浑水比较好。";
	action.m_ActionMsg = "有这种事……";
	return action;
end

function Task_00000077_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "多谢少侠出手相救。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000077_step_table = {
		[1] = Task_00000077_step_01,
		[2] = Task_00000077_step_02,
		[10] = Task_00000077_step_10,
		};

function Task_00000077_step(step)
	if Task_00000077_step_table[step] ~= nil then
		return Task_00000077_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000077_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000077() then
		return false;
	end
	if not task:AcceptTask(77) then
		return false;
	end
	task:AddTaskStep(77);
	return true;
end



--�ύ����
function Task_00000077_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(77) then
		return false;
	end


	player:AddExp(4444);
	return true;
end

--��������
function Task_00000077_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(77);
end
