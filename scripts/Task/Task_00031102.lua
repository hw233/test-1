--����Ľ�������
function Task_Accept_00031102()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 57 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31102) or task:HasCompletedTask(31102) or task:HasSubmitedTask(31102) then
		return false;
	end
	if not task:HasSubmitedTask(31101) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031102()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 57 then
		return false;
	end
	if task:HasAcceptedTask(31102) or task:HasCompletedTask(31102) or task:HasSubmitedTask(31102) then
		return false;
	end
	if not task:HasSubmitedTask(31101) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00031102()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31102) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031102(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31102) == npcId and Task_Accept_00031102 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31102
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "护送蒙不凡";
	elseif task:GetTaskAcceptNpc(31102) == npcId and task:HasAcceptedTask(31102) then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31102
		action.m_ActionToken = 3;
		action.m_ActionStep = 11;
		action.m_ActionMsg = "护送蒙不凡";
	elseif task:GetTaskSubmitNpc(31102) == npcId then
		if Task_Submit_00031102() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31102
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "护送蒙不凡";
		elseif task:HasAcceptedTask(31102) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31102
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "护送蒙不凡";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031102_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你好勇士"..GetPlayerName(GetPlayer()).."，你是来帮助我们的吗？";
	action.m_ActionMsg = "是的，我能帮点什么忙？";
	return action;
end

function Task_00031102_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那太好了，你知道我是文官，不擅长战斗，但是我也很想找回如烟，我希望你能协助我前往裂冰残寺，我知道那里有魔族的远征部队，我想他们或许会知道些情况。";
	action.m_ActionMsg = "好的，这个我肯定能帮你。";
	return action;
end

function Task_00031102_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你是什么人？对如烟做了什么？";
	action.m_ActionMsg = "";
	return action;
end

function Task_00031102_step_11()
	RunConveyAction(GetPlayer(),31102);
end

local Task_00031102_step_table = {
		[1] = Task_00031102_step_01,
		[2] = Task_00031102_step_02,
		[10] = Task_00031102_step_10,
		[11] = Task_00031102_step_11,
		};

function Task_00031102_step(step)
	if Task_00031102_step_table[step] ~= nil then
		return Task_00031102_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031102_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031102() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31102) then
		return false;
	end
	RunConveyAction(player,31102);
	return true;
end



--�ύ����
function Task_00031102_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(505,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31102) then
		return false;
	end

	if IsEquipTypeId(505) then
		for k = 1, 1 do
			package:AddEquip(505, 1);
		end
	else 
		package:AddItem(505,1,1);
	end

	player:AddExp(84000);
	player:getCoin(84000);
	player:getTael(40);
	return true;
end

--��������
function Task_00031102_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31102);
end
