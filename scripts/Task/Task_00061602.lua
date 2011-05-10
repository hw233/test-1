--����Ľ�������
function Task_Accept_00061602()
	local player = GetPlayer();
	if player:GetLev() < 61 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61602) or task:HasCompletedTask(61602) or task:HasSubmitedTask(61602) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61601) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61601) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061602()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 61 then
		return false;
	end
	if task:HasAcceptedTask(61602) or task:HasCompletedTask(61602) or task:HasSubmitedTask(61602) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61601) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61601) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061602()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61602) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061602(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61602) == npcId and Task_Accept_00061602 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61602
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "摩睺的目的";
	elseif task:GetTaskSubmitNpc(61602) == npcId then
		if Task_Submit_00061602() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61602
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "摩睺的目的";
		elseif task:HasAcceptedTask(61602) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61602
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "摩睺的目的";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061602_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这位勇士带着这小册子来找我，是什么用意啊？";
	action.m_ActionMsg = "你是在装吗？这小册子不就是你写的吗？";
	return action;
end

function Task_00061602_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是我写的又怎么样呢？";
	action.m_ActionMsg = "你是想利用这些巨人，教他们邪恶的行为而达到控制他们的目的吧。";
	return action;
end

function Task_00061602_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你怎么能这么说，这是毫无根据的吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061602_step_table = {
		[1] = Task_00061602_step_01,
		[2] = Task_00061602_step_02,
		[10] = Task_00061602_step_10,
		};

function Task_00061602_step(step)
	if Task_00061602_step_table[step] ~= nil then
		return Task_00061602_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061602_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061602() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61602) then
		return false;
	end
	task:AddTaskStep(61602);
	return true;
end



--�ύ����
function Task_00061602_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(61602) then
		return false;
	end


	player:AddExp(135000);
	player:getCoin(115000);
	return true;
end

--��������
function Task_00061602_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61602);
end
