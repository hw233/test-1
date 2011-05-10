--����Ľ�������
function Task_Accept_00031503()
	local player = GetPlayer();
	if player:GetLev() < 65 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31503) or task:HasCompletedTask(31503) or task:HasSubmitedTask(31503) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31502) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31502) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031503()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 65 then
		return false;
	end
	if task:HasAcceptedTask(31503) or task:HasCompletedTask(31503) or task:HasSubmitedTask(31503) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31502) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31502) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031503()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31503) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031503(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31503) == npcId and Task_Accept_00031503 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31503
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "与吕布对话";
	elseif task:GetTaskSubmitNpc(31503) == npcId then
		if Task_Submit_00031503() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31503
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "与吕布对话";
		elseif task:HasAcceptedTask(31503) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31503
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "与吕布对话";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031503_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "请问你是谁？来找我做什么？";
	action.m_ActionMsg = "我是来自"..GetStateName(GetPlayer():getCountry()).."的一位勇士，想问你为什么投靠贪婪无度的夜摩盟？";
	return action;
end

function Task_00031503_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "唉，又一个来问我这个问题的人，这样的选择也是无奈，最初莫名流落到山海大陆，落魄多日，是夜叉王发现并帮助了我，他许了我各种优越的条件，只要我效忠他，并且率部队听从他的指挥就行，可是后来我才知道他贪婪的阴谋和各种罪恶的行径。但我已效忠他了，不想背负背叛的罪名啊。";
	action.m_ActionMsg = "虽然是出于无奈，但是帮助这样的人你不会难过吗？";
	return action;
end

function Task_00031503_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，我也是没有办法，我是在不明真相的情况下作了这样的选择的。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031503_step_table = {
		[1] = Task_00031503_step_01,
		[2] = Task_00031503_step_02,
		[10] = Task_00031503_step_10,
		};

function Task_00031503_step(step)
	if Task_00031503_step_table[step] ~= nil then
		return Task_00031503_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031503_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031503() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31503) then
		return false;
	end
	task:AddTaskStep(31503);
	return true;
end



--�ύ����
function Task_00031503_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(31503) then
		return false;
	end


	player:AddExp(180000);
	player:getCoin(156000);
	player:getTael(30);
	return true;
end

--��������
function Task_00031503_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31503);
end
