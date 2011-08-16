--����Ľ�������
function Task_Accept_00000072()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(72) or task:HasCompletedTask(72) or task:HasSubmitedTask(72) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000072()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(72) or task:HasCompletedTask(72) or task:HasSubmitedTask(72) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000072()
	if GetPlayer():GetTaskMgr():HasCompletedTask(72) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000072(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(72) == npcId and Task_Accept_00000072 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 72
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "约战陈圩";
	elseif task:GetTaskSubmitNpc(72) == npcId then
		if Task_Submit_00000072() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 72
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "约战陈圩";
		elseif task:HasAcceptedTask(72) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 72
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "约战陈圩";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000072_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这另一封信是交给陈圩的陈长泰，到时候恩怨一并了解，还请少侠务必将这两封信送到。";
	action.m_ActionMsg = "戴场主，这你就放心吧。";
	return action;
end

function Task_00000072_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "打擂台吗，这正合吾意。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000072_step_table = {
		[1] = Task_00000072_step_01,
		[10] = Task_00000072_step_10,
		};

function Task_00000072_step(step)
	if Task_00000072_step_table[step] ~= nil then
		return Task_00000072_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000072_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000072() then
		return false;
	end
	local package = player:GetPackage();
	local reqGrids = 0;
	reqGrids = reqGrids + package:GetItemUsedGrids(810, 1, 1);
	if reqGrids > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2012, 0);
		return false;
	end
	if not task:AcceptTask(72) then
		return false;
	end
	package:AddItem(810, 1, 1);
	task:AddTaskStep(72);
	return true;
end



--�ύ����
function Task_00000072_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(810,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(72) then
		return false;
	end

	package:DelItem(810,1,1);

	player:AddExp(12000);
	return true;
end

--��������
function Task_00000072_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItem(810, 1, 1);
	local itemNum = 0;
	return GetPlayer():GetTaskMgr():AbandonTask(72);
end
