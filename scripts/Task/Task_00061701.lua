--����Ľ�������
function Task_Accept_00061701()
	local player = GetPlayer();
	if player:GetLev() < 63 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61701) or task:HasCompletedTask(61701) or task:HasSubmitedTask(61701) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061701()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 63 then
		return false;
	end
	if task:HasAcceptedTask(61701) or task:HasCompletedTask(61701) or task:HasSubmitedTask(61701) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00061701()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61701) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061701(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61701) == npcId and Task_Accept_00061701 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61701
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "醉鬼要债";
	elseif task:GetTaskSubmitNpc(61701) == npcId then
		if Task_Submit_00061701() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61701
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "醉鬼要债";
		elseif task:HasAcceptedTask(61701) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61701
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "醉鬼要债";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061701_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士快过来一下，麻烦你个事情，……额……，麻烦你个事情。";
	action.m_ActionMsg = "什么事啊，你这一身酒气的醉鬼？";
	return action;
end

function Task_00061701_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "没……没钱买酒了，我的钱被人借走了，你帮我去……去河西冰道的挪拉寺找梅犹前把我的钱要回来吧。";
	action.m_ActionMsg = "真是……只好帮你了。";
	return action;
end

function Task_00061701_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你这……你是谁啊？不要找我，我没钱。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061701_step_table = {
		[1] = Task_00061701_step_01,
		[2] = Task_00061701_step_02,
		[10] = Task_00061701_step_10,
		};

function Task_00061701_step(step)
	if Task_00061701_step_table[step] ~= nil then
		return Task_00061701_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061701_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061701() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61701) then
		return false;
	end
	task:AddTaskStep(61701);
	return true;
end



--�ύ����
function Task_00061701_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(61701) then
		return false;
	end


	player:AddExp(160000);
	player:getCoin(130000);
	return true;
end

--��������
function Task_00061701_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61701);
end
