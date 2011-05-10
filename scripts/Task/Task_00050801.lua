--����Ľ�������
function Task_Accept_00050801()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 25 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50801) or task:HasCompletedTask(50801) or task:HasSubmitedTask(50801) then
		return false;
	end
	if not task:HasSubmitedTask(50703) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050801()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 25 then
		return false;
	end
	if task:HasAcceptedTask(50801) or task:HasCompletedTask(50801) or task:HasSubmitedTask(50801) then
		return false;
	end
	if not task:HasSubmitedTask(50703) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050801()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50801) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050801(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50801) == npcId and Task_Accept_00050801 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50801
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "郭子仪求助";
	elseif task:GetTaskSubmitNpc(50801) == npcId then
		if Task_Submit_00050801() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50801
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "郭子仪求助";
		elseif task:HasAcceptedTask(50801) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50801
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "郭子仪求助";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050801_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，如果不介意，可以问问你，你是魔族的吗？";
	action.m_ActionMsg = "是啊，什么事情？";
	return action;
end

function Task_00050801_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是魔族的就好，我正犹豫不知道该找谁帮忙，都说魔族的勇士很值得人信任，如果你愿意来帮我，我也不会让你空手而归的。";
	action.m_ActionMsg = "既然不是白帮忙，我倒是可以试试看。";
	return action;
end

function Task_00050801_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "别提了，最近真是太背了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050801_step_table = {
		[1] = Task_00050801_step_01,
		[2] = Task_00050801_step_02,
		[10] = Task_00050801_step_10,
		};

function Task_00050801_step(step)
	if Task_00050801_step_table[step] ~= nil then
		return Task_00050801_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050801_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050801() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50801) then
		return false;
	end
	task:AddTaskStep(50801);
	return true;
end



--�ύ����
function Task_00050801_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50801) then
		return false;
	end


	player:AddExp(3000);
	player:getCoin(4000);
	return true;
end

--��������
function Task_00050801_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50801);
end
