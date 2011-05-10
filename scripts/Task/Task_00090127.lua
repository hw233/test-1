--����Ľ�������
function Task_Accept_00090127()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90127) or task:HasCompletedTask(90127) or task:HasSubmitedTask(90127) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90126) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90126) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090127()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90127) or task:HasCompletedTask(90127) or task:HasSubmitedTask(90127) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90126) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90126) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00090127()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90127) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090127(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90127) == npcId and Task_Accept_00090127 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90127
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "枯木计划";
	elseif task:GetTaskSubmitNpc(90127) == npcId then
		if Task_Submit_00090127() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90127
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "枯木计划";
		elseif task:HasAcceptedTask(90127) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90127
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "枯木计划";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090127_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "果然不出我所料。";
	action.m_ActionMsg = "那我们现在该怎么办？";
	return action;
end

function Task_00090127_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "枯木蝶已气急败坏，势必会进攻指挥营，你赶紧去通知勒勤，让他做好防范措施。";
	action.m_ActionMsg = "可恶的枯木蝶，我这就去。";
	return action;
end

function Task_00090127_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "看来我们要准备决一死战了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090127_step_table = {
		[1] = Task_00090127_step_01,
		[2] = Task_00090127_step_02,
		[10] = Task_00090127_step_10,
		};

function Task_00090127_step(step)
	if Task_00090127_step_table[step] ~= nil then
		return Task_00090127_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090127_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090127() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90127) then
		return false;
	end
	task:AddTaskStep(90127);
	return true;
end



--�ύ����
function Task_00090127_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90127) then
		return false;
	end


	player:AddExp(725000);
	player:getCoin(318000);
	return true;
end

--��������
function Task_00090127_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90127);
end
