--����Ľ�������
function Task_Accept_00000158()
	local player = GetPlayer();
	if player:GetLev() < 75 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(158) or task:HasCompletedTask(158) or task:HasSubmitedTask(158) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000158()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 75 then
		return false;
	end
	if task:HasAcceptedTask(158) or task:HasCompletedTask(158) or task:HasSubmitedTask(158) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000158()
	if GetPlayer():GetTaskMgr():HasCompletedTask(158) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000158(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(158) == npcId and Task_Accept_00000158 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 158
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "天狐渡劫";
	elseif task:GetTaskSubmitNpc(158) == npcId then
		if Task_Submit_00000158() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 158
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "天狐渡劫";
		elseif task:HasAcceptedTask(158) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 158
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "天狐渡劫";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000158_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "如今在东海有一位道友正要渡劫，她原本乃是一天狐，是秦氏姐妹的母亲，已经有数位我们正道中人前去助她渡劫，小友你不如也去东海见识一下。";
	action.m_ActionMsg = "嗯，我这就去。";
	return action;
end

function Task_00000158_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "多谢诸位道友相助。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000158_step_table = {
		[1] = Task_00000158_step_01,
		[10] = Task_00000158_step_10,
		};

function Task_00000158_step(step)
	if Task_00000158_step_table[step] ~= nil then
		return Task_00000158_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000158_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000158() then
		return false;
	end
	if not task:AcceptTask(158) then
		return false;
	end
	task:AddTaskStep(158);
	return true;
end



--�ύ����
function Task_00000158_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(158) then
		return false;
	end


	player:AddExp(80000);
	return true;
end

--��������
function Task_00000158_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(158);
end
