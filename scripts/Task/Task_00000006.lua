--����Ľ�������
function Task_Accept_00000006()
	local player = GetPlayer();
	if player:GetLev() < 6 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(6) or task:HasCompletedTask(6) or task:HasSubmitedTask(6) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000006()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 6 then
		return false;
	end
	if task:HasAcceptedTask(6) or task:HasCompletedTask(6) or task:HasSubmitedTask(6) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(5) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000006()
	if GetPlayer():GetTaskMgr():HasCompletedTask(6) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000006(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(6) == npcId and Task_Accept_00000006 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 6
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "阵法图册";
	elseif task:GetTaskSubmitNpc(6) == npcId then
		if Task_Submit_00000006() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 6
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "阵法图册";
		elseif task:HasAcceptedTask(6) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 6
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "阵法图册";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000006_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "咦，这几本古籍在我这里真是明珠蒙尘了，这本阵法图册看起来玄奥无比，小生我是肉眼凡胎自然不了解你们剑侠之流……不过周大哥吩咐的事情我一定妥善安排，你回去告诉周大哥一声，让他放心就是。";
	action.m_ActionMsg = "好，我这就去转告他。";
	return action;
end

function Task_00000006_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "如此，我就放心了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000006_step_table = {
		[1] = Task_00000006_step_01,
		[10] = Task_00000006_step_10,
		};

function Task_00000006_step(step)
	if Task_00000006_step_table[step] ~= nil then
		return Task_00000006_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000006_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000006() then
		return false;
	end
	if not task:AcceptTask(6) then
		return false;
	end
	task:AddTaskStep(6);
	return true;
end



--�ύ����
function Task_00000006_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(6) then
		return false;
	end


	player:AddExp(2000);
	return true;
end

--��������
function Task_00000006_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(6);
end
