--����Ľ�������
function Task_Accept_00080701()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80701) or task:HasCompletedTask(80701) or task:HasSubmitedTask(80701) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080701()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80701) or task:HasCompletedTask(80701) or task:HasSubmitedTask(80701) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080701()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80701) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080701(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80701) == npcId and Task_Accept_00080701 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80701
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "玄幻草";
	elseif task:GetTaskSubmitNpc(80701) == npcId then
		if Task_Submit_00080701() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80701
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "玄幻草";
		elseif task:HasAcceptedTask(80701) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80701
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "玄幻草";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080701_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "让那些怪物见鬼去吧，我已经受够了他们。";
	action.m_ActionMsg = "我们不能心浮气躁，得想想办法。";
	return action;
end

function Task_00080701_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你现在去玄幻坡采集一株玄幻草来，我想我们该行动了。";
	action.m_ActionMsg = "玄幻草？那能管什么用？";
	return action;
end

function Task_00080701_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这就是传说中神奇的玄幻草吗？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080701_step_table = {
		[1] = Task_00080701_step_01,
		[2] = Task_00080701_step_02,
		[10] = Task_00080701_step_10,
		};

function Task_00080701_step(step)
	if Task_00080701_step_table[step] ~= nil then
		return Task_00080701_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080701_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080701() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80701) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15103,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15103, itemNum, 1);
		end
		task:AddTaskStep2(80701, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00080701_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15103,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(80701) then
		return false;
	end

	package:DelItemAll(15103,1);

	player:AddExp(675000);
	player:getCoin(314000);
	return true;
end

--��������
function Task_00080701_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15103,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80701);
end
