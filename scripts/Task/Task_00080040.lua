--����Ľ�������
function Task_Accept_00080040()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80040) or task:HasCompletedTask(80040) or task:HasSubmitedTask(80040) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080040()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(80040) or task:HasCompletedTask(80040) or task:HasSubmitedTask(80040) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080040()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80040) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080040(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80040) == npcId and Task_Accept_00080040 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80040
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "正义不灭";
	elseif task:GetTaskSubmitNpc(80040) == npcId then
		if Task_Submit_00080040() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80040
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "正义不灭";
		elseif task:HasAcceptedTask(80040) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80040
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "正义不灭";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080040_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，修炼的历程是艰辛而曲折的，山海大陆的想必你已经有所了解。王舍城旧殿这里，最近真是越来越不太平了，黑风岗的那些狼魔首领已经猖狂的无法无天了，我们现在不得不借助你的力量，来平息这样的状况，而且这同时能给你的修炼带来收获。";
	action.m_ActionMsg = "的确是一举两得的事情。";
	return action;
end

function Task_00080040_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这么困难的任务，居然就被你做完了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080040_step_table = {
		[1] = Task_00080040_step_01,
		[10] = Task_00080040_step_10,
		};

function Task_00080040_step(step)
	if Task_00080040_step_table[step] ~= nil then
		return Task_00080040_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080040_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080040() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80040) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00080040_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80040) then
		return false;
	end


	player:AddExp(2000);
	player:getTael(60);
	return true;
end

--��������
function Task_00080040_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80040);
end
