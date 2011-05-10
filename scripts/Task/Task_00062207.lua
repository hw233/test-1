--����Ľ�������
function Task_Accept_00062207()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62207) or task:HasCompletedTask(62207) or task:HasSubmitedTask(62207) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00062207()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(62207) or task:HasCompletedTask(62207) or task:HasSubmitedTask(62207) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00062207()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62207) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00062207(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62207) == npcId and Task_Accept_00062207 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62207
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "捐献战功";
	elseif task:GetTaskSubmitNpc(62207) == npcId then
		if Task_Submit_00062207() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62207
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "捐献战功";
		elseif task:HasAcceptedTask(62207) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62207
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "捐献战功";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00062207_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "齐心协力才能使宗族变的强大，变强大的第一步就是提升宗族等级，为自己的宗族等级去捐献一次战功吧！";
	action.m_ActionMsg = "这还不容易！我这就去捐献一次。";
	return action;
end

function Task_00062207_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你的宗族因你而强大！";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062207_step_table = {
		[1] = Task_00062207_step_01,
		[10] = Task_00062207_step_10,
		};

function Task_00062207_step(step)
	if Task_00062207_step_table[step] ~= nil then
		return Task_00062207_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00062207_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062207() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(62207) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00062207_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(62207) then
		return false;
	end


	player:AddExp(20000);
	return true;
end

--��������
function Task_00062207_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(62207);
end
