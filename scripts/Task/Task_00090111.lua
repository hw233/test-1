--����Ľ�������
function Task_Accept_00090111()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90111) or task:HasCompletedTask(90111) or task:HasSubmitedTask(90111) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90110) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90110) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090111()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90111) or task:HasCompletedTask(90111) or task:HasSubmitedTask(90111) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90110) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90110) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00090111()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90111) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090111(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90111) == npcId and Task_Accept_00090111 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90111
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "劝将";
	elseif task:GetTaskSubmitNpc(90111) == npcId then
		if Task_Submit_00090111() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90111
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "劝将";
		elseif task:HasAcceptedTask(90111) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90111
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "劝将";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090111_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "是勒勤将军让你来探望妾身的？";
	action.m_ActionMsg = "是的，他十分想念姑娘，特遣在下来探望。";
	return action;
end

function Task_00090111_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "呵呵，我看他不是想念我，是想念吕布。我知道他想叫吕布去夜摩城，但是吕布是何等的心高气傲之人，哪能听他调遣。不过你可以去试试。";
	action.m_ActionMsg = "我是奉命而来，不能空手而归，那我去试试吧。";
	return action;
end

function Task_00090111_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "原来你是来劝将的。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090111_step_table = {
		[1] = Task_00090111_step_01,
		[2] = Task_00090111_step_02,
		[10] = Task_00090111_step_10,
		};

function Task_00090111_step(step)
	if Task_00090111_step_table[step] ~= nil then
		return Task_00090111_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090111_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090111() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90111) then
		return false;
	end
	task:AddTaskStep(90111);
	return true;
end



--�ύ����
function Task_00090111_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90111) then
		return false;
	end


	player:AddExp(600000);
	player:getCoin(308000);
	return true;
end

--��������
function Task_00090111_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90111);
end
