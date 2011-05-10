--����Ľ�������
function Task_Accept_00080061()
	local player = GetPlayer();
	if player:GetLev() < 61 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80061) or task:HasCompletedTask(80061) or task:HasSubmitedTask(80061) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080061()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 61 then
		return false;
	end
	if task:HasAcceptedTask(80061) or task:HasCompletedTask(80061) or task:HasSubmitedTask(80061) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080061()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80061) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080061(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80061) == npcId and Task_Accept_00080061 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80061
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "冰天雪地";
	elseif task:GetTaskSubmitNpc(80061) == npcId then
		if Task_Submit_00080061() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80061
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "冰天雪地";
		elseif task:HasAcceptedTask(80061) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80061
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "冰天雪地";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080061_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，修炼的历程是艰辛而曲折的，山海大陆的想必你已经有所了解。这摩加湖附近变得越来越不安全了，边湖窟的血浸骷髅，真是给我造成了太多困扰了，我正想拜托你去杀掉他们，收集40个他们的血浸碎石，来平息这样的状况，而且这同时能给你的修炼带来收获。";
	action.m_ActionMsg = "的确是一举两得的事情。";
	return action;
end

function Task_00080061_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这么困难的任务，居然就被你做完了。恭喜你进入更高级的修炼阶段。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080061_step_table = {
		[1] = Task_00080061_step_01,
		[10] = Task_00080061_step_10,
		};

function Task_00080061_step(step)
	if Task_00080061_step_table[step] ~= nil then
		return Task_00080061_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080061_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080061() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80061) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15094,1);
	if itemNum ~= 0 then
		if itemNum > 40 then
			itemNum = 40;
			package:SetItem(15094, itemNum, 1);
		end
		task:AddTaskStep2(80061, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00080061_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15094,1) < 40 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8925,2,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(80061) then
		return false;
	end

	if IsEquipTypeId(8925) then
		for k = 1, 2 do
			package:AddEquip(8925, 1);
		end
	else 
		package:AddItem(8925,2,1);
	end
	package:DelItemAll(15094,1);

	player:AddExp(50000);
	return true;
end

--��������
function Task_00080061_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15094,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80061);
end
