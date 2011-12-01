--����Ľ�������
function Task_Accept_00000078()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(78) or task:HasCompletedTask(78) or task:HasSubmitedTask(78) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(52) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(52) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(52) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000078()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(78) or task:HasCompletedTask(78) or task:HasSubmitedTask(78) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(52) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(52) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(52) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000078()
	if GetPlayer():GetTaskMgr():HasCompletedTask(78) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000078(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(78) == npcId and Task_Accept_00000078 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 78
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "峨眉访客";
	elseif task:GetTaskSubmitNpc(78) == npcId then
		if Task_Submit_00000078() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 78
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "峨眉访客";
		elseif task:HasAcceptedTask(78) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 78
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "峨眉访客";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000078_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "有劳少侠了为我们戴家场如此奔波了，方才有一位峨眉的前辈高人来访，说有事找你，现下他正在戴家的大院里等候，少侠还是快快去吧。";
	action.m_ActionMsg = "是吗，我这就去。";
	return action;
end

function Task_00000078_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "想不到我昔日教授的恶徒现在竟然在此为恶。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000078_step_table = {
		[1] = Task_00000078_step_01,
		[10] = Task_00000078_step_10,
		};

function Task_00000078_step(step)
	if Task_00000078_step_table[step] ~= nil then
		return Task_00000078_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000078_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000078() then
		return false;
	end
	if not task:AcceptTask(78) then
		return false;
	end
	task:AddTaskStep(78);
	return true;
end



--�ύ����
function Task_00000078_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(78) then
		return false;
	end


	player:AddExp(10000);
	return true;
end

--��������
function Task_00000078_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(78);
end
