--����Ľ�������
function Task_Accept_00010502()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 26 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10502) or task:HasCompletedTask(10502) or task:HasSubmitedTask(10502) then
		return false;
	end
	if not task:HasSubmitedTask(10501) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010502()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 26 then
		return false;
	end
	if task:HasAcceptedTask(10502) or task:HasCompletedTask(10502) or task:HasSubmitedTask(10502) then
		return false;
	end
	if not task:HasSubmitedTask(10501) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010502()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10502) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010502(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10502) == npcId and Task_Accept_00010502 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10502
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "情况汇报";
	elseif task:GetTaskSubmitNpc(10502) == npcId then
		if Task_Submit_00010502() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10502
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "情况汇报";
		elseif task:HasAcceptedTask(10502) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10502
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "情况汇报";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010502_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这些邪恶的夜摩盟，已经穿过我们跟山海大陆之间的连接处，来到我们这里了，他们的阴谋越来越明显了。";
	action.m_ActionMsg = "太可恶了，他们到底想得到什么呢？";
	return action;
end

function Task_00010502_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "成立夜摩盟的夜叉王和摩可拿，都曾经被天族和魔族驱逐了出去，可是他们内心邪恶，不肯罢休，贪念促使他们在山海大陆上结成夜摩盟，继续他们的邪恶计划，并一直觊觎我们天族在天庭的影响。我们不能任由他们的阴谋得逞，现在你去旁边找我们的军师鹤勒那天吧，他会帮助你。";
	action.m_ActionMsg = "他们还真是邪恶啊，竟敢觊觎我们的影响。";
	return action;
end

function Task_00010502_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "可恨的夜摩盟啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010502_step_table = {
		[1] = Task_00010502_step_01,
		[2] = Task_00010502_step_02,
		[10] = Task_00010502_step_10,
		};

function Task_00010502_step(step)
	if Task_00010502_step_table[step] ~= nil then
		return Task_00010502_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010502_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010502() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10502) then
		return false;
	end
	task:AddTaskStep(10502);
	return true;
end



--�ύ����
function Task_00010502_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(10502) then
		return false;
	end


	player:AddExp(3500);
	player:getCoin(4700);
	return true;
end

--��������
function Task_00010502_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10502);
end
