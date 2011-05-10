--����Ľ�������
function Task_Accept_00090123()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90123) or task:HasCompletedTask(90123) or task:HasSubmitedTask(90123) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90122) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90122) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090123()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90123) or task:HasCompletedTask(90123) or task:HasSubmitedTask(90123) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90122) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90122) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00090123()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90123) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090123(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90123) == npcId and Task_Accept_00090123 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90123
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "疏忽";
	elseif task:GetTaskSubmitNpc(90123) == npcId then
		if Task_Submit_00090123() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90123
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "疏忽";
		elseif task:HasAcceptedTask(90123) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90123
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "疏忽";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090123_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "居然有此事？";
	action.m_ActionMsg = "是的，那些狼人与黑风怪已勾结多时，将军也是刚发现，命我去杀了些狼人，因其数目较多，没有杀尽。";
	return action;
end

function Task_00090123_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我实在太轻敌了，原以为那些狼人根本成不了气候，一直防范黑风怪。却没想让那些枯木狼人影响了夜摩城的整个进攻计划。";
	action.m_ActionMsg = "不用太自责，我们现在反攻还来得及。";
	return action;
end

function Task_00090123_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我们必须教训那些怪物。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090123_step_table = {
		[1] = Task_00090123_step_01,
		[2] = Task_00090123_step_02,
		[10] = Task_00090123_step_10,
		};

function Task_00090123_step(step)
	if Task_00090123_step_table[step] ~= nil then
		return Task_00090123_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090123_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090123() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90123) then
		return false;
	end
	task:AddTaskStep(90123);
	return true;
end



--�ύ����
function Task_00090123_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90123) then
		return false;
	end


	player:AddExp(700000);
	player:getCoin(316000);
	return true;
end

--��������
function Task_00090123_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90123);
end
