--����Ľ�������
function Task_Accept_00000141()
	local player = GetPlayer();
	if player:GetLev() < 65 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(141) or task:HasCompletedTask(141) or task:HasSubmitedTask(141) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(140) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(140) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(140) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000141()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 65 then
		return false;
	end
	if task:HasAcceptedTask(141) or task:HasCompletedTask(141) or task:HasSubmitedTask(141) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(140) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(140) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(140) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000141()
	if GetPlayer():GetTaskMgr():HasCompletedTask(141) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000141(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(141) == npcId and Task_Accept_00000141 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 141
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "食人猿妖";
	elseif task:GetTaskSubmitNpc(141) == npcId then
		if Task_Submit_00000141() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 141
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "食人猿妖";
		elseif task:HasAcceptedTask(141) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 141
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "食人猿妖";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000141_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "对付三尸墓里的僵尸到是不急，我还约了其他的正道朋友来助拳，少侠你不如先帮老身解决庐山百姓的一大难题。你看那鬼见愁悬崖危壁，居住着一群食人猿妖，经常三五成群出来袭击当地百姓，我乃出家人，不宜沾染血腥，还得少侠你出出力了。";
	action.m_ActionMsg = "师太莫愁，我这就去消灭它们。";
	return action;
end

function Task_00000141_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."，白眉大师派你来帮助我真是太好了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000141_step_table = {
		[1] = Task_00000141_step_01,
		[10] = Task_00000141_step_10,
		};

function Task_00000141_step(step)
	if Task_00000141_step_table[step] ~= nil then
		return Task_00000141_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000141_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000141() then
		return false;
	end
	if not task:AcceptTask(141) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000141_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(141) then
		return false;
	end


	player:AddExp(70000);
	return true;
end

--��������
function Task_00000141_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(141);
end
