--����Ľ�������
function Task_Accept_00020206()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20206) or task:HasCompletedTask(20206) or task:HasSubmitedTask(20206) then
		return false;
	end
	if not task:HasSubmitedTask(20205) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020206()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(20206) or task:HasCompletedTask(20206) or task:HasSubmitedTask(20206) then
		return false;
	end
	if not task:HasSubmitedTask(20205) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020206()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20206) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020206(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20206) == npcId and Task_Accept_00020206 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20206
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "护送密使";
	elseif task:GetTaskAcceptNpc(20206) == npcId and task:HasAcceptedTask(20206) then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20206
		action.m_ActionToken = 3;
		action.m_ActionStep = 11;
		action.m_ActionMsg = "护送密使";
	elseif task:GetTaskSubmitNpc(20206) == npcId then
		if Task_Submit_00020206() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20206
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "护送密使";
		elseif task:HasAcceptedTask(20206) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20206
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "护送密使";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020206_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "是因达罗让你这么匆忙地把信送来？看来是有很要紧的事情要通知公主了。";
	action.m_ActionMsg = "我想是吧，他让我最快速度送来。";
	return action;
end

function Task_00020206_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那我要赶紧动身去天庭了，路上又不知道要遇上什么危险了，勇士你能送我一程吗，这附近不知道什么时候就会冒出来一些怪物和强盗。";
	action.m_ActionMsg = "那走吧，我会在路上护送你的。";
	return action;
end

function Task_00020206_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "一路上还顺利吧。";
	action.m_ActionMsg = "";
	return action;
end

function Task_00020206_step_11()
	RunConveyAction(GetPlayer(),20206);
end

local Task_00020206_step_table = {
		[1] = Task_00020206_step_01,
		[2] = Task_00020206_step_02,
		[10] = Task_00020206_step_10,
		[11] = Task_00020206_step_11,
		};

function Task_00020206_step(step)
	if Task_00020206_step_table[step] ~= nil then
		return Task_00020206_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020206_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020206() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20206) then
		return false;
	end
	RunConveyAction(player,20206);
	return true;
end



--�ύ����
function Task_00020206_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8925,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(20206) then
		return false;
	end

	if IsEquipTypeId(8925) then
		for k = 1, 1 do
			package:AddEquip(8925, 1);
		end
	else 
		package:AddItem(8925,1,1);
	end

	player:AddExp(1400);
	player:getCoin(1500);
	player:getTael(10);
	return true;
end

--��������
function Task_00020206_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20206);
end
