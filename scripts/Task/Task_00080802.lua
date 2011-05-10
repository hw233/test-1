--����Ľ�������
function Task_Accept_00080802()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80802) or task:HasCompletedTask(80802) or task:HasSubmitedTask(80802) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80801) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80801) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080802()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80802) or task:HasCompletedTask(80802) or task:HasSubmitedTask(80802) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80801) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80801) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00080802()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80802) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080802(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80802) == npcId and Task_Accept_00080802 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80802
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "胜利的消息";
	elseif task:GetTaskSubmitNpc(80802) == npcId then
		if Task_Submit_00080802() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80802
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "胜利的消息";
		elseif task:HasAcceptedTask(80802) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80802
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "胜利的消息";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080802_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你去向王仁宫的寒尧汇报此处战况，让他放心。";
	action.m_ActionMsg = "我定会如实禀报。";
	return action;
end

function Task_00080802_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我一直在这等你，我的勇士。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080802_step_table = {
		[1] = Task_00080802_step_01,
		[10] = Task_00080802_step_10,
		};

function Task_00080802_step(step)
	if Task_00080802_step_table[step] ~= nil then
		return Task_00080802_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080802_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080802() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80802) then
		return false;
	end
	task:AddTaskStep(80802);
	return true;
end



--�ύ����
function Task_00080802_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80802) then
		return false;
	end


	player:AddExp(725000);
	player:getCoin(318000);
	return true;
end

--��������
function Task_00080802_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80802);
end
