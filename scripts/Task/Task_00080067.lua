--����Ľ�������
function Task_Accept_00080067()
	local player = GetPlayer();
	if player:GetLev() < 67 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80067) or task:HasCompletedTask(80067) or task:HasSubmitedTask(80067) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080067()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 67 then
		return false;
	end
	if task:HasAcceptedTask(80067) or task:HasCompletedTask(80067) or task:HasSubmitedTask(80067) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080067()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80067) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080067(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80067) == npcId and Task_Accept_00080067 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80067
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "取其精华";
	elseif task:GetTaskSubmitNpc(80067) == npcId then
		if Task_Submit_00080067() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80067
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "取其精华";
		elseif task:HasAcceptedTask(80067) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80067
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "取其精华";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080067_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，修炼的历程是艰辛而曲折的，山海大陆的想必你已经有所了解。我急需你的帮助，去杀掉那些在浦夷林的原始雷龙，收集50个他们的龙珠，我现在不得不借助你的力量，来平息这样的状况，而且这同时能给你的修炼带来收获。";
	action.m_ActionMsg = "的确是一举两得的事情。";
	return action;
end

function Task_00080067_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这么困难的任务，居然就被你做完了。恭喜你进入更高级的修炼阶段。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080067_step_table = {
		[1] = Task_00080067_step_01,
		[10] = Task_00080067_step_10,
		};

function Task_00080067_step(step)
	if Task_00080067_step_table[step] ~= nil then
		return Task_00080067_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080067_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080067() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80067) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15096,1);
	if itemNum ~= 0 then
		if itemNum > 50 then
			itemNum = 50;
			package:SetItem(15096, itemNum, 1);
		end
		task:AddTaskStep2(80067, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00080067_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15096,1) < 50 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8927,2,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(80067) then
		return false;
	end

	if IsEquipTypeId(8927) then
		for k = 1, 2 do
			package:AddEquip(8927, 1);
		end
	else 
		package:AddItem(8927,2,1);
	end
	package:DelItemAll(15096,1);

	player:AddExp(260000);
	return true;
end

--��������
function Task_00080067_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15096,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80067);
end
