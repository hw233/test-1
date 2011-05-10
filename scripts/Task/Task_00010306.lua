--����Ľ�������
function Task_Accept_00010306()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 18 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10306) or task:HasCompletedTask(10306) or task:HasSubmitedTask(10306) then
		return false;
	end
	if not task:HasSubmitedTask(10305) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010306()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 18 then
		return false;
	end
	if task:HasAcceptedTask(10306) or task:HasCompletedTask(10306) or task:HasSubmitedTask(10306) then
		return false;
	end
	if not task:HasSubmitedTask(10305) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010306()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10306) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010306(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10306) == npcId and Task_Accept_00010306 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10306
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "回复神秘人";
	elseif task:GetTaskSubmitNpc(10306) == npcId then
		if Task_Submit_00010306() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10306
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "回复神秘人";
		elseif task:HasAcceptedTask(10306) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10306
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "回复神秘人";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010306_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "新来的勇士<name>，你带来了帝释天的密令？那么我先看一下，请你暂时回避一下。";
	action.m_ActionMsg = "这有什么关系，帝释天王都已经告诉过我了。";
	return action;
end

function Task_00010306_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "好了，我已经看完了，知道了帝释天的旨意了，但是王还要我叮嘱你，王妃的信你还是要马上给她送出去，并且装作什么也没有发生过。现在信在这里，快去吧，给你5分钟时间。";
	action.m_ActionMsg = "这么紧急啊，那我马上去了。";
	return action;
end

function Task_00010306_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士啊你终于带来了回信。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00010306_step_table = {
		[1] = Task_00010306_step_01,
		[2] = Task_00010306_step_02,
		[10] = Task_00010306_step_10,
		};

function Task_00010306_step(step)
	if Task_00010306_step_table[step] ~= nil then
		return Task_00010306_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010306_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010306() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15024, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(10306) then
		return false;
	end
	task:AddTaskStep(10306);
	return true;
end



--�ύ����
function Task_00010306_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15024,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(10306) then
		return false;
	end

	package:DelItemAll(15024,1);

	player:AddExp(1800);
	player:getCoin(3500);
	player:getTael(10);
	return true;
end

--��������
function Task_00010306_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15024,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10306);
end
