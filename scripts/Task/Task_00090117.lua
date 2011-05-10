--����Ľ�������
function Task_Accept_00090117()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90117) or task:HasCompletedTask(90117) or task:HasSubmitedTask(90117) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090117()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90117) or task:HasCompletedTask(90117) or task:HasSubmitedTask(90117) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00090117()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90117) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090117(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90117) == npcId and Task_Accept_00090117 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90117
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "回绝勒勤";
	elseif task:GetTaskSubmitNpc(90117) == npcId then
		if Task_Submit_00090117() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90117
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "回绝勒勤";
		elseif task:HasAcceptedTask(90117) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90117
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "回绝勒勤";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090117_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我知道你来这里劝吕布很久了，我希望你能去夜摩城回绝勒勤，吕布是不会去那里的。";
	action.m_ActionMsg = "难道吕先生和勒将军不合？";
	return action;
end

function Task_00090117_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "旧事莫提，常泉坡确实需要吕布在这镇守。";
	action.m_ActionMsg = "那好吧，我想勒将军要失望了。";
	return action;
end

function Task_00090117_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "看来他还是忘不了旧事。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090117_step_table = {
		[1] = Task_00090117_step_01,
		[2] = Task_00090117_step_02,
		[10] = Task_00090117_step_10,
		};

function Task_00090117_step(step)
	if Task_00090117_step_table[step] ~= nil then
		return Task_00090117_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090117_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090117() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90117) then
		return false;
	end
	task:AddTaskStep(90117);
	return true;
end



--�ύ����
function Task_00090117_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90117) then
		return false;
	end


	player:AddExp(650000);
	player:getCoin(312000);
	return true;
end

--��������
function Task_00090117_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90117);
end
