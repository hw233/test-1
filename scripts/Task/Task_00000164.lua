--����Ľ�������
function Task_Accept_00000164()
	local player = GetPlayer();
	if player:GetLev() < 80 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(164) or task:HasCompletedTask(164) or task:HasSubmitedTask(164) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000164()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 80 then
		return false;
	end
	if task:HasAcceptedTask(164) or task:HasCompletedTask(164) or task:HasSubmitedTask(164) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000164()
	if GetPlayer():GetTaskMgr():HasCompletedTask(164) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000164(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(164) == npcId and Task_Accept_00000164 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 164
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "九首神鳖";
	elseif task:GetTaskSubmitNpc(164) == npcId then
		if Task_Submit_00000164() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 164
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "九首神鳖";
		elseif task:HasAcceptedTask(164) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 164
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "九首神鳖";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000164_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我昔年曾云游四方，在那极北之处有一座奇怪的岛屿，名曰陷空岛。你如亲自去了就可以见识到其中玄妙之处，在陷空岛周遭有一座鳖极岛，其中住着一位修炼千年的九首神鳖，你不如去见见它。要知道它修炼多年见识非凡，可以在修道之路上指点你一二，就受用不尽了。";
	action.m_ActionMsg = "是吗，我这就去陷空岛转转。";
	return action;
end

function Task_00000164_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "老朽修炼多年，终于可以化身成人了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000164_step_table = {
		[1] = Task_00000164_step_01,
		[10] = Task_00000164_step_10,
		};

function Task_00000164_step(step)
	if Task_00000164_step_table[step] ~= nil then
		return Task_00000164_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000164_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000164() then
		return false;
	end
	if not task:AcceptTask(164) then
		return false;
	end
	task:AddTaskStep(164);
	return true;
end



--�ύ����
function Task_00000164_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(164) then
		return false;
	end


	player:AddExp(100000);
	return true;
end

--��������
function Task_00000164_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(164);
end
