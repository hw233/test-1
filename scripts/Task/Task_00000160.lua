--����Ľ�������
function Task_Accept_00000160()
	local player = GetPlayer();
	if player:GetLev() < 75 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(160) or task:HasCompletedTask(160) or task:HasSubmitedTask(160) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(159) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(159) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(159) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000160()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 75 then
		return false;
	end
	if task:HasAcceptedTask(160) or task:HasCompletedTask(160) or task:HasSubmitedTask(160) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(159) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(159) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(159) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000160()
	if GetPlayer():GetTaskMgr():HasCompletedTask(160) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000160(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(160) == npcId and Task_Accept_00000160 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 160
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "崇明岛恶人";
	elseif task:GetTaskSubmitNpc(160) == npcId then
		if Task_Submit_00000160() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 160
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "崇明岛恶人";
		elseif task:HasAcceptedTask(160) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 160
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "崇明岛恶人";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000160_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "峨眉派的几位少侠方才助我渡劫，如今天劫已渡，他们也纷纷离去，不过方才见李英琼神色焦急，似乎是她的爱宠佛奴遇敌，往崇明岛方向去了。那座岛上有一个妖妇唤作蒲妙妙，不如你赶去与英琼联手除掉此恶。";
	action.m_ActionMsg = "嗯，我知道了。";
	return action;
end

function Task_00000160_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这等邪派人物定要早早消灭。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000160_step_table = {
		[1] = Task_00000160_step_01,
		[10] = Task_00000160_step_10,
		};

function Task_00000160_step(step)
	if Task_00000160_step_table[step] ~= nil then
		return Task_00000160_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000160_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000160() then
		return false;
	end
	if not task:AcceptTask(160) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000160_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(160) then
		return false;
	end


	player:AddExp(100000);
	return true;
end

--��������
function Task_00000160_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(160);
end
