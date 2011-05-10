--����Ľ�������
function Task_Accept_00080302()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80302) or task:HasCompletedTask(80302) or (task:HasSubmitedTask(80302) and (GetSharpDay(task:GetTaskEndTime(80302)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080302()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(80302) or task:HasCompletedTask(80302) or (task:HasSubmitedTask(80302) and (GetSharpDay(task:GetTaskEndTime(80302)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080302()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80302) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080302(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80302) == npcId and Task_Accept_00080302 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80302
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "峥嵘国战";
	elseif task:GetTaskSubmitNpc(80302) == npcId then
		if Task_Submit_00080302() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80302
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "峥嵘国战";
		elseif task:HasAcceptedTask(80302) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80302
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "峥嵘国战";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080302_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "想要做长久的英雄是很难的，风水总是轮流转。";
	action.m_ActionMsg = "就凭天族那贪婪的嘴脸？只是甘露给了他们威力，我去灭了他们的威风。";
	return action;
end

function Task_00080302_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "果然我没看错人。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080302_step_table = {
		[1] = Task_00080302_step_01,
		[10] = Task_00080302_step_10,
		};

function Task_00080302_step(step)
	if Task_00080302_step_table[step] ~= nil then
		return Task_00080302_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080302_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080302() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80302) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00080302_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80302) then
		return false;
	end


	player:AddExp(50000);
	player:getTael(100);
	return true;
end

--��������
function Task_00080302_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80302);
end
