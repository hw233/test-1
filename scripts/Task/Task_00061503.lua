--����Ľ�������
function Task_Accept_00061503()
	local player = GetPlayer();
	if player:GetLev() < 58 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61503) or task:HasCompletedTask(61503) or task:HasSubmitedTask(61503) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61502) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61502) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061503()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 58 then
		return false;
	end
	if task:HasAcceptedTask(61503) or task:HasCompletedTask(61503) or task:HasSubmitedTask(61503) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(61502) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(61502) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061503()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61503) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061503(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61503) == npcId and Task_Accept_00061503 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61503
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "收拾庭院";
	elseif task:GetTaskSubmitNpc(61503) == npcId then
		if Task_Submit_00061503() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61503
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "收拾庭院";
		elseif task:HasAcceptedTask(61503) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61503
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "收拾庭院";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061503_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士多亏了你啊，要不我在这冰凌城，不是被怪物打死就是被冻死了。";
	action.m_ActionMsg = "你还真是挺不容易的。";
	return action;
end

function Task_00061503_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嗯别提了，再麻烦你去告诉我旁边的如烟的随从，跟他说门板已经弄回来了，请他把这乱糟糟的渡冰园收拾整理好吧。";
	action.m_ActionMsg = "好的，没问题。";
	return action;
end

function Task_00061503_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "好的好的，知道了，我这就开始收拾。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061503_step_table = {
		[1] = Task_00061503_step_01,
		[2] = Task_00061503_step_02,
		[10] = Task_00061503_step_10,
		};

function Task_00061503_step(step)
	if Task_00061503_step_table[step] ~= nil then
		return Task_00061503_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061503_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061503() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61503) then
		return false;
	end
	task:AddTaskStep(61503);
	return true;
end



--�ύ����
function Task_00061503_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8913,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(61503) then
		return false;
	end

	if IsEquipTypeId(8913) then
		for k = 1, 1 do
			package:AddEquip(8913, 1);
		end
	else 
		package:AddItem(8913,1,1);
	end

	player:AddExp(88000);
	player:getCoin(90000);
	return true;
end

--��������
function Task_00061503_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61503);
end
