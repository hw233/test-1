--����Ľ�������
function Task_Accept_00090124()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90124) or task:HasCompletedTask(90124) or task:HasSubmitedTask(90124) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90123) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90123) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090124()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90124) or task:HasCompletedTask(90124) or task:HasSubmitedTask(90124) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90123) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90123) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00090124()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90124) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090124(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90124) == npcId and Task_Accept_00090124 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90124
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "可恶的狼人";
	elseif task:GetTaskSubmitNpc(90124) == npcId then
		if Task_Submit_00090124() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90124
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "可恶的狼人";
		elseif task:HasAcceptedTask(90124) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90124
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "可恶的狼人";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090124_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我们必须教训那些怪物。";
	action.m_ActionMsg = "先消消气，我们必须从长计议。";
	return action;
end

function Task_00090124_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "不用想那么远，你去帮我杀枯木狼人，方能解我心头之恨。";
	action.m_ActionMsg = "这个没问题，我乐意铲奸除恶。";
	return action;
end

function Task_00090124_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "看他们还敢造次。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090124_step_table = {
		[1] = Task_00090124_step_01,
		[2] = Task_00090124_step_02,
		[10] = Task_00090124_step_10,
		};

function Task_00090124_step(step)
	if Task_00090124_step_table[step] ~= nil then
		return Task_00090124_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090124_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090124() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90124) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00090124_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90124) then
		return false;
	end


	player:AddExp(700000);
	player:getCoin(316000);
	return true;
end

--��������
function Task_00090124_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90124);
end
