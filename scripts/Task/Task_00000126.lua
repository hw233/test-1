--����Ľ�������
function Task_Accept_00000126()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(126) or task:HasCompletedTask(126) or task:HasSubmitedTask(126) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(125) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(125) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(125) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000126()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(126) or task:HasCompletedTask(126) or task:HasSubmitedTask(126) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(125) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(125) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(125) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000126()
	if GetPlayer():GetTaskMgr():HasCompletedTask(126) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000126(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(126) == npcId and Task_Accept_00000126 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 126
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "众志成城";
	elseif task:GetTaskSubmitNpc(126) == npcId then
		if Task_Submit_00000126() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 126
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "众志成城";
		elseif task:HasAcceptedTask(126) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 126
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "众志成城";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000126_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "修道指路漫长，稍有不慎就可能陷入万劫不复之地，一个人的力量毕竟有限，若可以召集一群志同道合的修道者相互扶持，将会好很多， 所以"..GetPlayerName(GetPlayer()).."你可以加入一个帮派或者自己召集散仙建立一个帮派。";
	action.m_ActionMsg = "让我考虑考虑！";
	return action;
end

function Task_00000126_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "孺子可教，"..GetPlayerName(GetPlayer()).."你不但侠肝义胆，而且虚怀若谷啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000126_step_table = {
		[1] = Task_00000126_step_01,
		[10] = Task_00000126_step_10,
		};

function Task_00000126_step(step)
	if Task_00000126_step_table[step] ~= nil then
		return Task_00000126_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000126_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000126() then
		return false;
	end
	if not task:AcceptTask(126) then
		return false;
	end
	task:AddTaskStep(126);
	return true;
end



--�ύ����
function Task_00000126_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(126) then
		return false;
	end


	player:AddExp(5000);
	return true;
end

--��������
function Task_00000126_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(126);
end
