--����Ľ�������
function Task_Accept_00000081()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(81) or task:HasCompletedTask(81) or task:HasSubmitedTask(81) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(80) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000081()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(81) or task:HasCompletedTask(81) or task:HasSubmitedTask(81) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(80) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000081()
	if GetPlayer():GetTaskMgr():HasCompletedTask(81) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000081(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(81) == npcId and Task_Accept_00000081 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 81
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "代惩劣徒";
	elseif task:GetTaskSubmitNpc(81) == npcId then
		if Task_Submit_00000081() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 81
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "代惩劣徒";
		elseif task:HasAcceptedTask(81) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 81
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "代惩劣徒";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000081_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "那恶人罗九数年前曾在我门下学剑，也怪我一时不查，竟没看出他品行不端。后来我见他心性歹毒就将他逐出师门，并且警告他不要仗着所学的剑法为恶。";
	action.m_ActionMsg = "真是人面兽心的家伙。";
	return action;
end

function Task_00000081_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "谁知他竟然违背我立下的训条，在这长沙城外作恶，可是毕竟我和他啊师徒一场，这除恶之事就交给你去做吧。";
	action.m_ActionMsg = "好吧，不过前辈你也不要自责了。";
	return action;
end

function Task_00000081_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "想不到我昔年一时失查，竟然酿成今日之大祸……哎！";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000081_step_table = {
		[1] = Task_00000081_step_01,
		[2] = Task_00000081_step_02,
		[10] = Task_00000081_step_10,
		};

function Task_00000081_step(step)
	if Task_00000081_step_table[step] ~= nil then
		return Task_00000081_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000081_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000081() then
		return false;
	end
	if not task:AcceptTask(81) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000081_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(81) then
		return false;
	end


	player:AddExp(2222);
	return true;
end

--��������
function Task_00000081_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(81);
end
