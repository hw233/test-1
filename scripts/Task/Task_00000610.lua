--����Ľ�������
function Task_Accept_00000610()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(610) or task:HasCompletedTask(610) or task:HasSubmitedTask(610) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000610()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(610) or task:HasCompletedTask(610) or task:HasSubmitedTask(610) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000610()
	if GetPlayer():GetTaskMgr():HasCompletedTask(610) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000610(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(610) == npcId and Task_Accept_00000610 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 610
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "慈云寺地牢";
	elseif task:GetTaskSubmitNpc(610) == npcId then
		if Task_Submit_00000610() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 610
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "慈云寺地牢";
		elseif task:HasAcceptedTask(610) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 610
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "慈云寺地牢";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000610_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "成都附近有座慈云禅师，寺内主持智通本官还见过，一直都安分守已，可是最近寺内来了不少五湖四海的强人，在成都一带为恶，甚至连寺内的僧人都开始作奸犯科，"..GetPlayerName(GetPlayer()).."你这去将这些个妖僧剿灭。";
	action.m_ActionMsg = "为民除害是我辈本色，弟子去去就回。";
	return action;
end

function Task_00000610_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."你真是身手不凡啊。 ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000610_step_table = {
		[1] = Task_00000610_step_01,
		[10] = Task_00000610_step_10,
		};

function Task_00000610_step(step)
	if Task_00000610_step_table[step] ~= nil then
		return Task_00000610_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000610_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000610() then
		return false;
	end
	if not task:AcceptTask(610) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000610_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(610) then
		return false;
	end


	player:AddExp(2000);
	player:getTael(1000);
	return true;
end

--��������
function Task_00000610_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(610);
end
