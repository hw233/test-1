--����Ľ�������
function Task_Accept_00080033()
	local player = GetPlayer();
	if player:GetLev() < 33 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80033) or task:HasCompletedTask(80033) or task:HasSubmitedTask(80033) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080033()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 33 then
		return false;
	end
	if task:HasAcceptedTask(80033) or task:HasCompletedTask(80033) or task:HasSubmitedTask(80033) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080033()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80033) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080033(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80033) == npcId and Task_Accept_00080033 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80033
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "安抚游魂";
	elseif task:GetTaskSubmitNpc(80033) == npcId then
		if Task_Submit_00080033() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80033
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "安抚游魂";
		elseif task:HasAcceptedTask(80033) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80033
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "安抚游魂";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080033_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，修炼的历程是艰辛而曲折的，山海大陆的想必你已经有所了解。战争废墟那里这里，最近真是越来越不太平了，居然出现了一群游魂刺杀者，使得那些战争亡魂始终不得安宁。现在也只有你，能平息这样的状况，而且这同时能给你的修炼带来收获。\n";
	action.m_ActionMsg = "的确是一举两得的事情。";
	return action;
end

function Task_00080033_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这么困难的任务，居然就被你做完了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080033_step_table = {
		[1] = Task_00080033_step_01,
		[10] = Task_00080033_step_10,
		};

function Task_00080033_step(step)
	if Task_00080033_step_table[step] ~= nil then
		return Task_00080033_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080033_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080033() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80033) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00080033_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80033) then
		return false;
	end


	player:AddExp(2000);
	player:getTael(50);
	return true;
end

--��������
function Task_00080033_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80033);
end
