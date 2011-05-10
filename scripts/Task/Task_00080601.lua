--����Ľ�������
function Task_Accept_00080601()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80601) or task:HasCompletedTask(80601) or task:HasSubmitedTask(80601) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080601()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80601) or task:HasCompletedTask(80601) or task:HasSubmitedTask(80601) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080601()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80601) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080601(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80601) == npcId and Task_Accept_00080601 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80601
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "侦察石桥";
	elseif task:GetTaskSubmitNpc(80601) == npcId then
		if Task_Submit_00080601() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80601
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "侦察石桥";
		elseif task:HasAcceptedTask(80601) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80601
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "侦察石桥";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080601_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "现在的形势不容乐观。";
	action.m_ActionMsg = "想不到夜摩盟的爪牙已遍布各处。";
	return action;
end

function Task_00080601_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "为了保险起见，你再去侦查下石桥，看那边的情况如何。";
	action.m_ActionMsg = "但愿不会引来更大的危机。";
	return action;
end

function Task_00080601_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "果然在我预料之内。	";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080601_step_table = {
		[1] = Task_00080601_step_01,
		[2] = Task_00080601_step_02,
		[10] = Task_00080601_step_10,
		};

function Task_00080601_step(step)
	if Task_00080601_step_table[step] ~= nil then
		return Task_00080601_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080601_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080601() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80601) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00080601_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80601) then
		return false;
	end


	player:AddExp(600000);
	player:getCoin(308000);
	return true;
end

--��������
function Task_00080601_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80601);
end
