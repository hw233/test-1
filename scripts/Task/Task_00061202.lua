--����Ľ�������
function Task_Accept_00061202()
	local player = GetPlayer();
	if player:GetLev() < 51 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61202) or task:HasCompletedTask(61202) or task:HasSubmitedTask(61202) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61201) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61201) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061202()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 51 then
		return false;
	end
	if task:HasAcceptedTask(61202) or task:HasCompletedTask(61202) or task:HasSubmitedTask(61202) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61201) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61201) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061202()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61202) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061202(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61202) == npcId and Task_Accept_00061202 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61202
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "清除伏地龙";
	elseif task:GetTaskSubmitNpc(61202) == npcId then
		if Task_Submit_00061202() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61202
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "清除伏地龙";
		elseif task:HasAcceptedTask(61202) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61202
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "清除伏地龙";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061202_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士先别急着走，我本来以为漂流谷是一个有着清澈的流水的很漂亮的山谷，没想到我发现那里盘踞了许多可恶的伏地龙，完全破坏了这儿的风景，让一切是那么的格格不入。";
	action.m_ActionMsg = "看来又要让我去清除那些伏地龙了。";
	return action;
end

function Task_00061202_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嗯勇士你说对了，我正有此意，不把这一切收拾干净，我会一直不踏实的。";
	action.m_ActionMsg = "真是败给你了，我只好去帮你了。";
	return action;
end

function Task_00061202_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你还真是挺厉害的，清除这些怪物对你来说完全不在话下嘛";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061202_step_table = {
		[1] = Task_00061202_step_01,
		[2] = Task_00061202_step_02,
		[10] = Task_00061202_step_10,
		};

function Task_00061202_step(step)
	if Task_00061202_step_table[step] ~= nil then
		return Task_00061202_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061202_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061202() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61202) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00061202_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(61202) then
		return false;
	end


	player:AddExp(60000);
	player:getCoin(47000);
	return true;
end

--��������
function Task_00061202_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61202);
end
