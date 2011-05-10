--����Ľ�������
function Task_Accept_00050503()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50503) or task:HasCompletedTask(50503) or task:HasSubmitedTask(50503) then
		return false;
	end
	if not task:HasSubmitedTask(50502) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050503()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(50503) or task:HasCompletedTask(50503) or task:HasSubmitedTask(50503) then
		return false;
	end
	if not task:HasSubmitedTask(50502) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050503()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50503) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050503(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50503) == npcId and Task_Accept_00050503 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50503
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "周身轻松";
	elseif task:GetTaskSubmitNpc(50503) == npcId then
		if Task_Submit_00050503() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50503
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "周身轻松";
		elseif task:HasAcceptedTask(50503) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50503
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "周身轻松";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050503_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "就在你回来之前的一刹那，我突然感到周身前所未有的轻松，我的力气也逐渐恢复了，能告诉我你做了什么了吗？";
	action.m_ActionMsg = "显无意大师说，你是因为受到了乱石堆的电闪骷髅的影响。";
	return action;
end

function Task_00050503_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哦原来是这样，真是非常感谢你勇士，现在我的感觉正在越来越好，想来我的力气也可以很快恢复了，你的帮助应该得到嘉奖。";
	action.m_ActionMsg = "你太客气了。";
	return action;
end

function Task_00050503_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士咱们后会有期。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050503_step_table = {
		[1] = Task_00050503_step_01,
		[2] = Task_00050503_step_02,
		[10] = Task_00050503_step_10,
		};

function Task_00050503_step(step)
	if Task_00050503_step_table[step] ~= nil then
		return Task_00050503_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050503_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050503() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50503) then
		return false;
	end
	task:AddTaskStep(50503);
	return true;
end



--�ύ����
function Task_00050503_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8917,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(50503) then
		return false;
	end

	if IsEquipTypeId(8917) then
		for k = 1, 1 do
			package:AddEquip(8917, 1);
		end
	else 
		package:AddItem(8917,1,1);
	end

	player:AddExp(1400);
	player:getCoin(1800);
	return true;
end

--��������
function Task_00050503_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50503);
end
