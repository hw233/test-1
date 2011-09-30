--����Ľ�������
function Task_Accept_00000124()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(124) or task:HasCompletedTask(124) or task:HasSubmitedTask(124) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000124()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(124) or task:HasCompletedTask(124) or task:HasSubmitedTask(124) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000124()
	if GetPlayer():GetTaskMgr():HasCompletedTask(124) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000124(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(124) == npcId and Task_Accept_00000124 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 124
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "自动挂机";
	elseif task:GetTaskSubmitNpc(124) == npcId then
		if Task_Submit_00000124() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 124
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "自动挂机";
		elseif task:HasAcceptedTask(124) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 124
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "自动挂机";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000124_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠，修道之路漫长，你也不急一时，闲来可以挂机练级，当然了挂机怪物的等级不能超过您自身的等级，挂机还可以使用加速功能，获得大量的经验，当然了每天的加速次数是有限的，并且每加速一次要消耗10礼券，所以"..GetPlayerName(GetPlayer()).."你可要合理的安排自己的时间。";
	action.m_ActionMsg = "还有这种事，我一定要去试试。";
	return action;
end

function Task_00000124_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."若你闲来无事，就可以挂机练级，也方便你在挂机的时候做点其它的事情。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000124_step_table = {
		[1] = Task_00000124_step_01,
		[10] = Task_00000124_step_10,
		};

function Task_00000124_step(step)
	if Task_00000124_step_table[step] ~= nil then
		return Task_00000124_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000124_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000124() then
		return false;
	end
	if not task:AcceptTask(124) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000124_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(124) then
		return false;
	end


	player:AddExp(10000);
	player:getCoupon(20);
	player:getTael(500);
	return true;
end

--��������
function Task_00000124_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(124);
end
