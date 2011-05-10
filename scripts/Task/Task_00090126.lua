--����Ľ�������
function Task_Accept_00090126()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90126) or task:HasCompletedTask(90126) or task:HasSubmitedTask(90126) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90125) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90125) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090126()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90126) or task:HasCompletedTask(90126) or task:HasSubmitedTask(90126) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90125) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90125) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00090126()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90126) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090126(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90126) == npcId and Task_Accept_00090126 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90126
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "夜摩的成长";
	elseif task:GetTaskSubmitNpc(90126) == npcId then
		if Task_Submit_00090126() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90126
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "夜摩的成长";
		elseif task:HasAcceptedTask(90126) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90126
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "夜摩的成长";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090126_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，你去侦查下枯松镇，我们杀了这么多狼人，枯木蝶肯定会有下一步行动。";
	action.m_ActionMsg = "那些怪物已然不成气候。";
	return action;
end

function Task_00090126_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "还是不能太轻敌，现在夜摩势力与日俱增，同盟越来越多，不能小视。";
	action.m_ActionMsg = "那好，我这就去看看。";
	return action;
end

function Task_00090126_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "果然不出我所料。	";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090126_step_table = {
		[1] = Task_00090126_step_01,
		[2] = Task_00090126_step_02,
		[10] = Task_00090126_step_10,
		};

function Task_00090126_step(step)
	if Task_00090126_step_table[step] ~= nil then
		return Task_00090126_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090126_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090126() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90126) then
		return false;
	end
	task:AddTaskStep(90126);
	return true;
end



--�ύ����
function Task_00090126_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90126) then
		return false;
	end


	player:AddExp(725000);
	player:getCoin(318000);
	return true;
end

--��������
function Task_00090126_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90126);
end
