--����Ľ�������
function Task_Accept_00061401()
	local player = GetPlayer();
	if player:GetLev() < 56 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61401) or task:HasCompletedTask(61401) or task:HasSubmitedTask(61401) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061401()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 56 then
		return false;
	end
	if task:HasAcceptedTask(61401) or task:HasCompletedTask(61401) or task:HasSubmitedTask(61401) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00061401()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61401) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061401(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61401) == npcId and Task_Accept_00061401 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61401
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "魔法力量";
	elseif task:GetTaskSubmitNpc(61401) == npcId then
		if Task_Submit_00061401() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61401
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "魔法力量";
		elseif task:HasAcceptedTask(61401) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61401
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "魔法力量";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061401_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士雪山峻岭的朝雪郡迎接你的到来，你的修行练道之路一切顺利吧，有没有什么需要我帮忙的呢？";
	action.m_ActionMsg = "我还好，一切都算顺利，不过我觉得是你需要什么帮助吧。";
	return action;
end

function Task_00061401_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你说对了，我最近了解到，碎冰祭坛那里，出现了一些邪恶祭师，他们掌握了邪恶的魔法，如果能打败他们收集到他们的魔法卷轴，附加到我们东山士兵的装备上，能增强很大的战斗力。可是我们的人都不擅长使用魔法，一直拿那些祭师没有办法。";
	action.m_ActionMsg = "这个问题对我来说不在话下。";
	return action;
end

function Task_00061401_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这些魔法卷轴我可是计划了很久想要得到的呢.";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061401_step_table = {
		[1] = Task_00061401_step_01,
		[2] = Task_00061401_step_02,
		[10] = Task_00061401_step_10,
		};

function Task_00061401_step(step)
	if Task_00061401_step_table[step] ~= nil then
		return Task_00061401_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061401_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061401() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61401) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15013,1);
	if itemNum ~= 0 then
		if itemNum > 10 then
			itemNum = 10;
			package:SetItem(15013, itemNum, 1);
		end
		task:AddTaskStep2(61401, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00061401_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15013,1) < 10 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(61401) then
		return false;
	end

	package:DelItemAll(15013,1);

	player:AddExp(80000);
	player:getCoin(75000);
	return true;
end

--��������
function Task_00061401_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15013,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61401);
end
