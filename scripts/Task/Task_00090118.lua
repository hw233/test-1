--����Ľ�������
function Task_Accept_00090118()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90118) or task:HasCompletedTask(90118) or task:HasSubmitedTask(90118) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90117) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90117) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090118()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90118) or task:HasCompletedTask(90118) or task:HasSubmitedTask(90118) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90117) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90117) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00090118()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90118) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090118(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90118) == npcId and Task_Accept_00090118 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90118
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "旧事";
	elseif task:GetTaskSubmitNpc(90118) == npcId then
		if Task_Submit_00090118() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90118
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "旧事";
		elseif task:HasAcceptedTask(90118) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90118
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "旧事";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090118_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "看来他还是忘不了旧事。";
	action.m_ActionMsg = "什么旧事？";
	return action;
end

function Task_00090118_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "（叹气），不提也罢。既然他不愿意来，那这儿还得请勇士多留阵子，帮我们扫清敌寇。";
	action.m_ActionMsg = "这些都是小事，只是对将军和吕先生之间的旧事很好奇，难到是为了貂蝉？我想将军不是好色之人。";
	return action;
end

function Task_00090118_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "请勇士不要多想。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090118_step_table = {
		[1] = Task_00090118_step_01,
		[2] = Task_00090118_step_02,
		[10] = Task_00090118_step_10,
		};

function Task_00090118_step(step)
	if Task_00090118_step_table[step] ~= nil then
		return Task_00090118_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090118_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090118() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90118) then
		return false;
	end
	task:AddTaskStep(90118);
	return true;
end



--�ύ����
function Task_00090118_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90118) then
		return false;
	end


	player:AddExp(650000);
	player:getCoin(312000);
	return true;
end

--��������
function Task_00090118_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90118);
end
