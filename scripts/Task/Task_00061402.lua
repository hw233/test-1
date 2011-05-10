--����Ľ�������
function Task_Accept_00061402()
	local player = GetPlayer();
	if player:GetLev() < 56 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61402) or task:HasCompletedTask(61402) or task:HasSubmitedTask(61402) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61401) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61401) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061402()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 56 then
		return false;
	end
	if task:HasAcceptedTask(61402) or task:HasCompletedTask(61402) or task:HasSubmitedTask(61402) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61401) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61401) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061402()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61402) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061402(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61402) == npcId and Task_Accept_00061402 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61402
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "通知多闻";
	elseif task:GetTaskSubmitNpc(61402) == npcId then
		if Task_Submit_00061402() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61402
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "通知多闻";
		elseif task:HasAcceptedTask(61402) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61402
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "通知多闻";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061402_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士在你的帮助下，我们通过使用获得的那些魔法卷轴，弥补了我们部队在魔法上的缺点，我们的军备力量得到了很大的强化，这都是我们盼望已久的提升。";
	action.m_ActionMsg = "真是不错的消息。";
	return action;
end

function Task_00061402_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "如果你在将来某个时刻经过朝雪城，麻烦将我写给东山主将多闻的信送给他吧，让他了解我们现在的军备情况。";
	action.m_ActionMsg = "好的，我现在就去，我想他听到了这个消息会很高兴的。";
	return action;
end

function Task_00061402_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你每次带来的都是好消息啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061402_step_table = {
		[1] = Task_00061402_step_01,
		[2] = Task_00061402_step_02,
		[10] = Task_00061402_step_10,
		};

function Task_00061402_step(step)
	if Task_00061402_step_table[step] ~= nil then
		return Task_00061402_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061402_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061402() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15084, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(61402) then
		return false;
	end
	task:AddTaskStep(61402);
	return true;
end



--�ύ����
function Task_00061402_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15084,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(61402) then
		return false;
	end

	package:DelItemAll(15084,1);

	player:AddExp(80000);
	player:getCoin(75000);
	return true;
end

--��������
function Task_00061402_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15084,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61402);
end
