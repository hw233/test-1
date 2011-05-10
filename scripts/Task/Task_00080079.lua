--����Ľ�������
function Task_Accept_00080079()
	local player = GetPlayer();
	if player:GetLev() < 79 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80079) or task:HasCompletedTask(80079) or task:HasSubmitedTask(80079) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080079()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 79 then
		return false;
	end
	if task:HasAcceptedTask(80079) or task:HasCompletedTask(80079) or task:HasSubmitedTask(80079) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080079()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80079) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080079(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80079) == npcId and Task_Accept_00080079 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80079
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "绝地逢生";
	elseif task:GetTaskSubmitNpc(80079) == npcId then
		if Task_Submit_00080079() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80079
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "绝地逢生";
		elseif task:HasAcceptedTask(80079) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80079
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "绝地逢生";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080079_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，修炼的历程是艰辛而曲折的，山海大陆的想必你已经有所了解。我急需你的帮助，去杀掉那些在海市桥悬冥塔的绝地霸王龙，收集80个他们的霸王龙珠，我现在不得不借助你的力量，来平息这样的状况，而且这同时能给你的修炼带来收获。";
	action.m_ActionMsg = "是个不错的主意，我愿意接受。";
	return action;
end

function Task_00080079_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这么困难的任务，居然就被你做完了。恭喜你进入更高级的修炼阶段。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080079_step_table = {
		[1] = Task_00080079_step_01,
		[10] = Task_00080079_step_10,
		};

function Task_00080079_step(step)
	if Task_00080079_step_table[step] ~= nil then
		return Task_00080079_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080079_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080079() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80079) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15100,1);
	if itemNum ~= 0 then
		if itemNum > 80 then
			itemNum = 80;
			package:SetItem(15100, itemNum, 1);
		end
		task:AddTaskStep2(80079, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00080079_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15100,1) < 80 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8930,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(80079) then
		return false;
	end

	if IsEquipTypeId(8930) then
		for k = 1, 1 do
			package:AddEquip(8930, 1);
		end
	else 
		package:AddItem(8930,1,1);
	end
	package:DelItemAll(15100,1);

	player:AddExp(480000);
	return true;
end

--��������
function Task_00080079_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15100,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80079);
end
