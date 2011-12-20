--����Ľ�������
function Task_Accept_00000183()
	local player = GetPlayer();
	if player:GetLev() < 85 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(183) or task:HasCompletedTask(183) or task:HasSubmitedTask(183) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(182) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(182) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(182) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000183()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 85 then
		return false;
	end
	if task:HasAcceptedTask(183) or task:HasCompletedTask(183) or task:HasSubmitedTask(183) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(182) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(182) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(182) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000183()
	if GetPlayer():GetTaskMgr():HasCompletedTask(183) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000183(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(183) == npcId and Task_Accept_00000183 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 183
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "武当败类";
	elseif task:GetTaskSubmitNpc(183) == npcId then
		if Task_Submit_00000183() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 183
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "武当败类";
		elseif task:HasAcceptedTask(183) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 183
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "武当败类";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000183_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "林莽是我的同门师兄弟，他目前就在丹江口处，是我们武当的耻辱，少侠去解决他吧。";
	action.m_ActionMsg = "是吗，我这就去除掉这个败类。";
	return action;
end

function Task_00000183_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠真是修为深厚，天赋异禀啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000183_step_table = {
		[1] = Task_00000183_step_01,
		[10] = Task_00000183_step_10,
		};

function Task_00000183_step(step)
	if Task_00000183_step_table[step] ~= nil then
		return Task_00000183_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000183_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000183() then
		return false;
	end
	if not task:AcceptTask(183) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000183_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(183) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--��������
function Task_00000183_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(183);
end
