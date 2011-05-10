--����Ľ�������
function Task_Accept_00030507()
	local player = GetPlayer();
	if player:GetLev() < 42 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30507) or task:HasCompletedTask(30507) or task:HasSubmitedTask(30507) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30506) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30506) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030507()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 42 then
		return false;
	end
	if task:HasAcceptedTask(30507) or task:HasCompletedTask(30507) or task:HasSubmitedTask(30507) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30506) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30506) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030507()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30507) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030507(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30507) == npcId and Task_Accept_00030507 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30507
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "笑而不语";
	elseif task:GetTaskSubmitNpc(30507) == npcId then
		if Task_Submit_00030507() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30507
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "笑而不语";
		elseif task:HasAcceptedTask(30507) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30507
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "笑而不语";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030507_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "哈哈，你给我带来了如影送来的美酒啊，那我可要珍藏好。";
	action.m_ActionMsg = "如影还想让我问你，他前面受到刺客的偷袭，是不是你去救她的。";
	return action;
end

function Task_00030507_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "啊哈哈。";
	action.m_ActionMsg = "如果在下没猜错，应该是你，我代他多谢你的救命之恩。";
	return action;
end

function Task_00030507_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "再会啦勇士，你走你的山海路，我喝我的寂寞酒。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030507_step_table = {
		[1] = Task_00030507_step_01,
		[2] = Task_00030507_step_02,
		[10] = Task_00030507_step_10,
		};

function Task_00030507_step(step)
	if Task_00030507_step_table[step] ~= nil then
		return Task_00030507_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030507_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030507() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30507) then
		return false;
	end
	task:AddTaskStep(30507);
	return true;
end



--�ύ����
function Task_00030507_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30507) then
		return false;
	end


	player:AddExp(20000);
	player:getCoin(17500);
	player:getTael(30);
	return true;
end

--��������
function Task_00030507_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30507);
end
