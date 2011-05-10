--����Ľ�������
function Task_Accept_00080055()
	local player = GetPlayer();
	if player:GetLev() < 55 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80055) or task:HasCompletedTask(80055) or task:HasSubmitedTask(80055) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080055()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 55 then
		return false;
	end
	if task:HasAcceptedTask(80055) or task:HasCompletedTask(80055) or task:HasSubmitedTask(80055) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080055()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80055) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080055(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80055) == npcId and Task_Accept_00080055 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80055
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "斩妖除魔";
	elseif task:GetTaskSubmitNpc(80055) == npcId then
		if Task_Submit_00080055() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80055
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "斩妖除魔";
		elseif task:HasAcceptedTask(80055) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80055
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "斩妖除魔";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080055_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，修炼的历程是艰辛而曲折的，山海大陆的想必你已经有所了解。我急需你的帮助，去杀掉那些在孤冷山的乱窟崖，收集30个他们的僵尸咒符，我现在不得不借助你的力量，来平息这样的状况，而且这同时能给你的修炼带来收获。";
	action.m_ActionMsg = "的确是一举两得的事情。";
	return action;
end

function Task_00080055_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这么困难的任务，居然就被你做完了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080055_step_table = {
		[1] = Task_00080055_step_01,
		[10] = Task_00080055_step_10,
		};

function Task_00080055_step(step)
	if Task_00080055_step_table[step] ~= nil then
		return Task_00080055_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080055_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080055() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80055) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15092,1);
	if itemNum ~= 0 then
		if itemNum > 30 then
			itemNum = 30;
			package:SetItem(15092, itemNum, 1);
		end
		task:AddTaskStep2(80055, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00080055_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15092,1) < 30 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(80055) then
		return false;
	end

	package:DelItemAll(15092,1);

	player:AddExp(25000);
	player:getTael(120);
	return true;
end

--��������
function Task_00080055_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15092,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80055);
end
