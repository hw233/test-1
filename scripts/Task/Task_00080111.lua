--����Ľ�������
function Task_Accept_00080111()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80111) or task:HasCompletedTask(80111) or (task:HasSubmitedTask(80111) and (GetSharpDay(task:GetTaskEndTime(80111)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080111()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(80111) or task:HasCompletedTask(80111) or (task:HasSubmitedTask(80111) and (GetSharpDay(task:GetTaskEndTime(80111)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080111()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80111) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080111(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80111) == npcId and Task_Accept_00080111 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80111
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "英雄争霸";
	elseif task:GetTaskSubmitNpc(80111) == npcId then
		if Task_Submit_00080111() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80111
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "英雄争霸";
		elseif task:HasAcceptedTask(80111) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80111
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "英雄争霸";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080111_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "去竞技场掠夺别人吧，想成为英雄就千万不要手软。";
	action.m_ActionMsg = "你看我可是那心软之人？我们需要的正是掠夺的野心。";
	return action;
end

function Task_00080111_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，情况如何？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080111_step_table = {
		[1] = Task_00080111_step_01,
		[10] = Task_00080111_step_10,
		};

function Task_00080111_step(step)
	if Task_00080111_step_table[step] ~= nil then
		return Task_00080111_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080111_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080111() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80111) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00080111_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80111) then
		return false;
	end


	player:AddExp(30000);
	player:getTael(50);
	return true;
end

--��������
function Task_00080111_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80111);
end
