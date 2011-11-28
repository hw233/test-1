--����Ľ�������
function Task_Accept_00000159()
	local player = GetPlayer();
	if player:GetLev() < 75 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(159) or task:HasCompletedTask(159) or task:HasSubmitedTask(159) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(158) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(158) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(158) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000159()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 75 then
		return false;
	end
	if task:HasAcceptedTask(159) or task:HasCompletedTask(159) or task:HasSubmitedTask(159) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(158) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(158) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(158) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000159()
	if GetPlayer():GetTaskMgr():HasCompletedTask(159) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000159(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(159) == npcId and Task_Accept_00000159 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 159
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "宝相夫人";
	elseif task:GetTaskSubmitNpc(159) == npcId then
		if Task_Submit_00000159() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 159
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "宝相夫人";
		elseif task:HasAcceptedTask(159) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 159
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "宝相夫人";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000159_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我昔年炼制的白眉针杀戮太重，命中注定会有这么一劫，虽然百年来我静心修道，不再造杀孽，不过这天劫仍然如期而至。如今我专心渡劫，不能分心，还望少侠帮助除掉那鄢什，此人修炼的诡异魔宫，操纵僵尸，你要小心对付才是。";
	action.m_ActionMsg = "嗯，我知道了。";
	return action;
end

function Task_00000159_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "多谢诸位道友相助。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000159_step_table = {
		[1] = Task_00000159_step_01,
		[10] = Task_00000159_step_10,
		};

function Task_00000159_step(step)
	if Task_00000159_step_table[step] ~= nil then
		return Task_00000159_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000159_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000159() then
		return false;
	end
	if not task:AcceptTask(159) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000159_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(159) then
		return false;
	end


	player:AddExp(100000);
	return true;
end

--��������
function Task_00000159_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(159);
end
