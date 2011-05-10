--����Ľ�������
function Task_Accept_00050105()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 8 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50105) or task:HasCompletedTask(50105) or task:HasSubmitedTask(50105) then
		return false;
	end
	if not task:HasSubmitedTask(50104) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050105()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 8 then
		return false;
	end
	if task:HasAcceptedTask(50105) or task:HasCompletedTask(50105) or task:HasSubmitedTask(50105) then
		return false;
	end
	if not task:HasSubmitedTask(50104) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050105()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50105) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050105(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50105) == npcId and Task_Accept_00050105 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50105
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "深藏不露";
	elseif task:GetTaskSubmitNpc(50105) == npcId then
		if Task_Submit_00050105() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50105
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "深藏不露";
		elseif task:HasAcceptedTask(50105) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50105
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "深藏不露";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050105_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "施主所为何事？";
	action.m_ActionMsg = "香音王后因为想念文隣，想把这个回音贝放在龙王庙中。";
	return action;
end

function Task_00050105_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "贫僧了解。";
	action.m_ActionMsg = "多谢。（扫地僧，莫非是深藏不露的高手？）";
	return action;
end

function Task_00050105_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "后会有期。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050105_step_table = {
		[1] = Task_00050105_step_01,
		[2] = Task_00050105_step_02,
		[10] = Task_00050105_step_10,
		};

function Task_00050105_step(step)
	if Task_00050105_step_table[step] ~= nil then
		return Task_00050105_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050105_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050105() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50105) then
		return false;
	end
	task:AddTaskStep(50105);
	return true;
end



--�ύ����
function Task_00050105_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8900,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(50105) then
		return false;
	end

	if IsEquipTypeId(8900) then
		for k = 1, 1 do
			package:AddEquip(8900, 1);
		end
	else 
		package:AddItem(8900,1,1);
	end

	player:AddExp(900);
	player:getCoin(700);
	return true;
end

--��������
function Task_00050105_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50105);
end
