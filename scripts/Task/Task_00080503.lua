--����Ľ�������
function Task_Accept_00080503()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80503) or task:HasCompletedTask(80503) or task:HasSubmitedTask(80503) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80502) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80502) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080503()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80503) or task:HasCompletedTask(80503) or task:HasSubmitedTask(80503) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80502) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80502) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00080503()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80503) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080503(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80503) == npcId and Task_Accept_00080503 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80503
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "剿灭黑风寨";
	elseif task:GetTaskSubmitNpc(80503) == npcId then
		if Task_Submit_00080503() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80503
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "剿灭黑风寨";
		elseif task:HasAcceptedTask(80503) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80503
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "剿灭黑风寨";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080503_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "黑风寨情况如何？";
	action.m_ActionMsg = "他们果然与夜摩盟勾结，而且势力越来越大，据说连冰妖都参与了他们的计划。";
	return action;
end

function Task_00080503_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "果真如此？那不能再放纵他们任意而为，你去剿杀他们的巢穴。";
	action.m_ActionMsg = "但愿不会引来更大的危机。";
	return action;
end

function Task_00080503_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这就是杀鸡儆猴，看那些冰妖和其他小怪还敢不敢勾结夜摩盟。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080503_step_table = {
		[1] = Task_00080503_step_01,
		[2] = Task_00080503_step_02,
		[10] = Task_00080503_step_10,
		};

function Task_00080503_step(step)
	if Task_00080503_step_table[step] ~= nil then
		return Task_00080503_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080503_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080503() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80503) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00080503_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80503) then
		return false;
	end


	player:AddExp(525000);
	player:getCoin(302000);
	return true;
end

--��������
function Task_00080503_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80503);
end
