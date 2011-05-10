--����Ľ�������
function Task_Accept_00040801()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 25 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40801) or task:HasCompletedTask(40801) or task:HasSubmitedTask(40801) then
		return false;
	end
	if not task:HasSubmitedTask(40703) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040801()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 25 then
		return false;
	end
	if task:HasAcceptedTask(40801) or task:HasCompletedTask(40801) or task:HasSubmitedTask(40801) then
		return false;
	end
	if not task:HasSubmitedTask(40703) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040801()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40801) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040801(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40801) == npcId and Task_Accept_00040801 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40801
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "庞统求助";
	elseif task:GetTaskSubmitNpc(40801) == npcId then
		if Task_Submit_00040801() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40801
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "庞统求助";
		elseif task:HasAcceptedTask(40801) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40801
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "庞统求助";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040801_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，如果不介意，可以问问你，你是天族的吗？";
	action.m_ActionMsg = "是啊，什么事情？";
	return action;
end

function Task_00040801_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是天族的就好，我正犹豫不知道该找谁帮忙，都说天族的勇士很值得人信任，如果你愿意来帮我，我也不会让你空手而归的。";
	action.m_ActionMsg = "既然不是白帮忙，我倒是可以试试看。";
	return action;
end

function Task_00040801_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我等了这些天，找到个人帮忙真不容易啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040801_step_table = {
		[1] = Task_00040801_step_01,
		[2] = Task_00040801_step_02,
		[10] = Task_00040801_step_10,
		};

function Task_00040801_step(step)
	if Task_00040801_step_table[step] ~= nil then
		return Task_00040801_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040801_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040801() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40801) then
		return false;
	end
	task:AddTaskStep(40801);
	return true;
end



--�ύ����
function Task_00040801_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40801) then
		return false;
	end


	player:AddExp(3000);
	player:getCoin(3800);
	return true;
end

--��������
function Task_00040801_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40801);
end
