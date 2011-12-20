--����Ľ�������
function Task_Accept_00000177()
	local player = GetPlayer();
	if player:GetLev() < 85 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(177) or task:HasCompletedTask(177) or task:HasSubmitedTask(177) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000177()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 85 then
		return false;
	end
	if task:HasAcceptedTask(177) or task:HasCompletedTask(177) or task:HasSubmitedTask(177) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000177()
	if GetPlayer():GetTaskMgr():HasCompletedTask(177) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000177(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(177) == npcId and Task_Accept_00000177 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 177
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "半边大师";
	elseif task:GetTaskSubmitNpc(177) == npcId then
		if Task_Submit_00000177() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 177
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "半边大师";
		elseif task:HasAcceptedTask(177) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 177
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "半边大师";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000177_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠修行速度真快，果然天赋异禀，方才武当半边大师飞剑传书找你，让去武当一行，似乎有什么事找你。";
	action.m_ActionMsg = "是吗，我这就去看看。";
	return action;
end

function Task_00000177_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠我们又见面了";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000177_step_table = {
		[1] = Task_00000177_step_01,
		[10] = Task_00000177_step_10,
		};

function Task_00000177_step(step)
	if Task_00000177_step_table[step] ~= nil then
		return Task_00000177_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000177_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000177() then
		return false;
	end
	if not task:AcceptTask(177) then
		return false;
	end
	task:AddTaskStep(177);
	return true;
end



--�ύ����
function Task_00000177_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(177) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--��������
function Task_00000177_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(177);
end
