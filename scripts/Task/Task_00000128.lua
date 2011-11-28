--����Ľ�������
function Task_Accept_00000128()
	local player = GetPlayer();
	if player:GetLev() < 38 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(128) or task:HasCompletedTask(128) or task:HasSubmitedTask(128) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(60) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000128()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 38 then
		return false;
	end
	if task:HasAcceptedTask(128) or task:HasCompletedTask(128) or task:HasSubmitedTask(128) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(60) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000128()
	if GetPlayer():GetTaskMgr():HasCompletedTask(128) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000128(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(128) == npcId and Task_Accept_00000128 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 128
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "每日挂机";
	elseif task:GetTaskSubmitNpc(128) == npcId then
		if Task_Submit_00000128() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 128
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "每日挂机";
		elseif task:HasAcceptedTask(128) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 128
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "每日挂机";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000128_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "每天除了正常的任务流程和副本、阵图的锻炼之外，闲暇时间"..GetPlayerName(GetPlayer()).."你可以尝试一下挂机打怪，这种方式比较省力，每天每个人都有一定的加速次数，当然要消耗一定数量的礼券。";
	action.m_ActionMsg = "这种方式听起来不错！";
	return action;
end

function Task_00000128_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."你真是好身手。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000128_step_table = {
		[1] = Task_00000128_step_01,
		[10] = Task_00000128_step_10,
		};

function Task_00000128_step(step)
	if Task_00000128_step_table[step] ~= nil then
		return Task_00000128_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000128_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000128() then
		return false;
	end
	if not task:AcceptTask(128) then
		return false;
	end
	task:AddTaskStep(128);
	return true;
end



--�ύ����
function Task_00000128_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(128) then
		return false;
	end


	player:AddExp(5000);
	player:getCoupon(40);
	return true;
end

--��������
function Task_00000128_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(128);
end
