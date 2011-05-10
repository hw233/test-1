--����Ľ�������
function Task_Accept_00060402()
	local player = GetPlayer();
	if player:GetLev() < 36 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60402) or task:HasCompletedTask(60402) or task:HasSubmitedTask(60402) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60401) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60401) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060402()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 36 then
		return false;
	end
	if task:HasAcceptedTask(60402) or task:HasCompletedTask(60402) or task:HasSubmitedTask(60402) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60401) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60401) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00060402()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60402) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060402(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60402) == npcId and Task_Accept_00060402 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60402
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "黑风残留";
	elseif task:GetTaskSubmitNpc(60402) == npcId then
		if Task_Submit_00060402() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60402
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "黑风残留";
		elseif task:HasAcceptedTask(60402) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60402
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "黑风残留";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060402_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你打掉那狼魔首领的事情，在山海大陆还真是引起了不小的风波呢，有你这样的勇士，希望康国的历史不会再重演了。";
	action.m_ActionMsg = "大师过奖。";
	return action;
end

function Task_00060402_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我给你这样的考验也是因为有很多事情需要你的力量，黑风岗那里，又出现了一群黑风兽，这些魔兽具有继续亡魂的残留，并将它们附在自己体内的能力。我需要你去帮我打败他们，收集他们的黑风残留。";
	action.m_ActionMsg = "好的，我现在就去。";
	return action;
end

function Task_00060402_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你居然这么快就完成了，看了这件事情好办了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060402_step_table = {
		[1] = Task_00060402_step_01,
		[2] = Task_00060402_step_02,
		[10] = Task_00060402_step_10,
		};

function Task_00060402_step(step)
	if Task_00060402_step_table[step] ~= nil then
		return Task_00060402_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060402_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060402() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60402) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15004,1);
	if itemNum ~= 0 then
		if itemNum > 12 then
			itemNum = 12;
			package:SetItem(15004, itemNum, 1);
		end
		task:AddTaskStep2(60402, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00060402_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15004,1) < 12 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(60402) then
		return false;
	end

	package:DelItemAll(15004,1);

	player:AddExp(5500);
	player:getCoin(8000);
	return true;
end

--��������
function Task_00060402_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15004,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60402);
end
