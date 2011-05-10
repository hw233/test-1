--����Ľ�������
function Task_Accept_00030104()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30104) or task:HasCompletedTask(30104) or task:HasSubmitedTask(30104) then
		return false;
	end
	if not task:HasSubmitedTask(30103) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030104()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(30104) or task:HasCompletedTask(30104) or task:HasSubmitedTask(30104) then
		return false;
	end
	if not task:HasSubmitedTask(30103) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00030104()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30104) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030104(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30104) == npcId and Task_Accept_00030104 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30104
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "慰问边防";
	elseif task:GetTaskSubmitNpc(30104) == npcId then
		if Task_Submit_00030104() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30104
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "慰问边防";
		elseif task:HasAcceptedTask(30104) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30104
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "慰问边防";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030104_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我们击退了那些疯狂的魔族复仇者，也算是个好消息，你去边防那里告知士兵们这个消息吧，顺便再带去一些他们需要的物资，帮助他们更好的防守我们的势力。";
	action.m_ActionMsg = "好的，我现在去。";
	return action;
end

function Task_00030104_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "感谢您带来的消息和物资。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030104_step_table = {
		[1] = Task_00030104_step_01,
		[10] = Task_00030104_step_10,
		};

function Task_00030104_step(step)
	if Task_00030104_step_table[step] ~= nil then
		return Task_00030104_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030104_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030104() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15032, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(30104) then
		return false;
	end
	task:AddTaskStep(30104);
	return true;
end



--�ύ����
function Task_00030104_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15032,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(30104) then
		return false;
	end

	package:DelItemAll(15032,1);

	player:AddExp(4000);
	player:getCoin(5500);
	player:getTael(10);
	return true;
end

--��������
function Task_00030104_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15032,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30104);
end
