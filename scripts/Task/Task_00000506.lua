--����Ľ�������
function Task_Accept_00000506()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(506) or task:HasCompletedTask(506) or task:HasSubmitedTask(506) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000506()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(506) or task:HasCompletedTask(506) or task:HasSubmitedTask(506) then
		return false;
	end
	if player:ColorTaskOutOf(0) then
		return false
	end
	return true;
end


--�����������
function Task_Submit_00000506()
	if GetPlayer():GetTaskMgr():HasCompletedTask(506) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000506(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(506) == npcId and Task_Accept_00000506 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 506
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "水贼头目";
	elseif task:GetTaskSubmitNpc(506) == npcId then
		if Task_Submit_00000506() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 506
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "水贼头目";
		elseif task:HasAcceptedTask(506) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 506
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "水贼头目";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000506_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "成都前段时间治安真得很成问题，有一股水贼在附近为恶，他们的头目是个叫李刚的家伙，残暴异常，"..GetPlayerName(GetPlayer()).."速速去将这个恶人铲除。";
	action.m_ActionMsg = "为民除害是我辈本色，弟子去去就回。";
	return action;
end

function Task_00000506_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."你真是身手不凡啊。 ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000506_step_table = {
		[1] = Task_00000506_step_01,
		[10] = Task_00000506_step_10,
		};

function Task_00000506_step(step)
	if Task_00000506_step_table[step] ~= nil then
		return Task_00000506_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000506_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000506() then
		return false;
	end
	if not task:AcceptTask(506) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000506_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(506) then
		return false;
	end


	player:AddExp(2222);
	return true;
end

--��������
function Task_00000506_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(506);
end
