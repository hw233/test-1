--����Ľ�������
function Task_Accept_00000143()
	local player = GetPlayer();
	if player:GetLev() < 65 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(143) or task:HasCompletedTask(143) or task:HasSubmitedTask(143) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(142) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(142) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(142) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000143()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 65 then
		return false;
	end
	if task:HasAcceptedTask(143) or task:HasCompletedTask(143) or task:HasSubmitedTask(143) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(142) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(142) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(142) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000143()
	if GetPlayer():GetTaskMgr():HasCompletedTask(143) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000143(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(143) == npcId and Task_Accept_00000143 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 143
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "洞犀山";
	elseif task:GetTaskSubmitNpc(143) == npcId then
		if Task_Submit_00000143() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 143
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "洞犀山";
		elseif task:HasAcceptedTask(143) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 143
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "洞犀山";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000143_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "真是一波未平，一波又起，才消灭了妖道叶培，洞犀山又来了妖人尉迟元，这个妖人魔宫精湛，少侠你要小心对付才是。";
	action.m_ActionMsg = "师太莫愁，我这就去消灭他们。";
	return action;
end

function Task_00000143_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."，少侠真是剑术高强，一身正气啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000143_step_table = {
		[1] = Task_00000143_step_01,
		[10] = Task_00000143_step_10,
		};

function Task_00000143_step(step)
	if Task_00000143_step_table[step] ~= nil then
		return Task_00000143_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000143_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000143() then
		return false;
	end
	if not task:AcceptTask(143) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000143_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(143) then
		return false;
	end


	player:AddExp(70000);
	return true;
end

--��������
function Task_00000143_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(143);
end