--����Ľ�������
function Task_Accept_00080044()
	local player = GetPlayer();
	if player:GetLev() < 44 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80044) or task:HasCompletedTask(80044) or task:HasSubmitedTask(80044) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080044()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 44 then
		return false;
	end
	if task:HasAcceptedTask(80044) or task:HasCompletedTask(80044) or task:HasSubmitedTask(80044) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080044()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80044) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080044(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80044) == npcId and Task_Accept_00080044 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80044
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "力劈狼骑";
	elseif task:GetTaskSubmitNpc(80044) == npcId then
		if Task_Submit_00080044() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80044
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "力劈狼骑";
		elseif task:HasAcceptedTask(80044) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80044
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "力劈狼骑";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080044_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，修炼的历程是艰辛而曲折的，山海大陆的想必你已经有所了解。黑烟谷附近真是越来越不太平了。那里的疯狂狼骑对我造成了很大的困扰，我现在不得不借助你的力量，来平息这样的状况，而且这同时能给你的修炼带来收获。";
	action.m_ActionMsg = "的确是一举两得的事情。";
	return action;
end

function Task_00080044_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这么困难的任务，居然就被你做完了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080044_step_table = {
		[1] = Task_00080044_step_01,
		[10] = Task_00080044_step_10,
		};

function Task_00080044_step(step)
	if Task_00080044_step_table[step] ~= nil then
		return Task_00080044_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080044_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080044() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80044) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00080044_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8918,2,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(80044) then
		return false;
	end

	if IsEquipTypeId(8918) then
		for k = 1, 2 do
			package:AddEquip(8918, 1);
		end
	else 
		package:AddItem(8918,2,1);
	end

	player:AddExp(10000);
	return true;
end

--��������
function Task_00080044_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80044);
end
