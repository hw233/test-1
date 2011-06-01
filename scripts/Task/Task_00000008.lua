--����Ľ�������
function Task_Accept_00000008()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(8) or task:HasCompletedTask(8) or task:HasSubmitedTask(8) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(7) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(7) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000008()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(8) or task:HasCompletedTask(8) or task:HasSubmitedTask(8) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(7) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(7) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000008()
	if GetPlayer():GetTaskMgr():HasCompletedTask(8) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000008(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(8) == npcId and Task_Accept_00000008 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 8
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "启程";
	elseif task:GetTaskSubmitNpc(8) == npcId then
		if Task_Submit_00000008() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 8
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "启程";
		elseif task:HasAcceptedTask(8) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 8
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "启程";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000008_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "方才李宁父女已经准备好自己的行李包裹，打算与我即刻启程前往峨嵋山隐居，他们已经在舍身崖处等候你我，少侠不如和我们一起去峨眉山？";
	action.m_ActionMsg = "能与前辈同行，我深感荣幸。";
	return action;
end

function Task_00000008_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "<name>，我看你神采奕奕，莫非得了什么奇遇？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000008_step_table = {
		[1] = Task_00000008_step_01,
		[10] = Task_00000008_step_10,
		};

function Task_00000008_step(step)
	if Task_00000008_step_table[step] ~= nil then
		return Task_00000008_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000008_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000008() then
		return false;
	end
	if not task:AcceptTask(8) then
		return false;
	end
	task:AddTaskStep(8);
	return true;
end



--�ύ����
function Task_00000008_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(8) then
		return false;
	end


	return true;
end

--��������
function Task_00000008_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(8);
end
