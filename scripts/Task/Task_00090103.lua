--����Ľ�������
function Task_Accept_00090103()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90103) or task:HasCompletedTask(90103) or task:HasSubmitedTask(90103) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090103()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90103) or task:HasCompletedTask(90103) or task:HasSubmitedTask(90103) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00090103()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90103) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090103(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90103) == npcId and Task_Accept_00090103 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90103
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "冰冷的骨头";
	elseif task:GetTaskSubmitNpc(90103) == npcId then
		if Task_Submit_00090103() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90103
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "冰冷的骨头";
		elseif task:HasAcceptedTask(90103) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90103
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "冰冷的骨头";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090103_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你知道永左塔的那些冰妖吗？别看那些妖怪长相丑恶。他们身上的骨头可是极好的魔法药材。";
	action.m_ActionMsg = "我每次碰到那些冷冰冰的家伙都会有种汗毛竖起的感觉。";
	return action;
end

function Task_00090103_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你真是我的好帮手。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090103_step_table = {
		[1] = Task_00090103_step_01,
		[10] = Task_00090103_step_10,
		};

function Task_00090103_step(step)
	if Task_00090103_step_table[step] ~= nil then
		return Task_00090103_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090103_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090103() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15104, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(90103) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00090103_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15104,1) < 20 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(90103) then
		return false;
	end

	package:DelItemAll(15104,1);

	player:AddExp(550000);
	player:getCoin(304000);
	return true;
end

--��������
function Task_00090103_abandon()
	local package = GetPlayer():GetPackage();
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90103);
end
