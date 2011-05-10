--����Ľ�������
function Task_Accept_00090121()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90121) or task:HasCompletedTask(90121) or task:HasSubmitedTask(90121) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090121()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90121) or task:HasCompletedTask(90121) or task:HasSubmitedTask(90121) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00090121()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90121) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090121(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90121) == npcId and Task_Accept_00090121 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90121
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "狼狈为奸";
	elseif task:GetTaskSubmitNpc(90121) == npcId then
		if Task_Submit_00090121() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90121
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "狼狈为奸";
		elseif task:HasAcceptedTask(90121) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90121
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "狼狈为奸";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090121_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，据探子回报，黑风怪居然勾结了枯木狼人，并且互传情报。我需要你去断掉他们的情报来源。";
	action.m_ActionMsg = "这个简单，我将那狼人杀死，让黑风怪们死了这条心。";
	return action;
end

function Task_00090121_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "都是些无耻之徒。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090121_step_table = {
		[1] = Task_00090121_step_01,
		[10] = Task_00090121_step_10,
		};

function Task_00090121_step(step)
	if Task_00090121_step_table[step] ~= nil then
		return Task_00090121_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090121_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090121() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90121) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00090121_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90121) then
		return false;
	end


	player:AddExp(700000);
	player:getCoin(316000);
	return true;
end

--��������
function Task_00090121_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90121);
end
