--����Ľ�������
function Task_Accept_00000027()
	local player = GetPlayer();
	if player:GetLev() < 22 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(27) or task:HasCompletedTask(27) or task:HasSubmitedTask(27) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(26) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(26) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(26) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000027()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 22 then
		return false;
	end
	if task:HasAcceptedTask(27) or task:HasCompletedTask(27) or task:HasSubmitedTask(27) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(26) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(26) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(26) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000027()
	if GetPlayer():GetTaskMgr():HasCompletedTask(27) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000027(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(27) == npcId and Task_Accept_00000027 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 27
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "切磋武艺";
	elseif task:GetTaskSubmitNpc(27) == npcId then
		if Task_Submit_00000027() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 27
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "切磋武艺";
		elseif task:HasAcceptedTask(27) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 27
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "切磋武艺";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000027_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你看起来不怎么样嘛，又不高又不壮，可是师父说你成就不凡，武功也不错，我们可不服气哦，要不要来跟我们比试一下。";
	action.m_ActionMsg = "那就恭敬不如从命了哦。";
	return action;
end

function Task_00000027_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."你果然高明，我们算是彻底的服气了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000027_step_table = {
		[1] = Task_00000027_step_01,
		[10] = Task_00000027_step_10,
		};

function Task_00000027_step(step)
	if Task_00000027_step_table[step] ~= nil then
		return Task_00000027_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000027_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000027() then
		return false;
	end
	if not task:AcceptTask(27) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000027_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(27) then
		return false;
	end


	player:AddExp(3333);
	return true;
end

--��������
function Task_00000027_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(27);
end
