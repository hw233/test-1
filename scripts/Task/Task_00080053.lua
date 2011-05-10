--����Ľ�������
function Task_Accept_00080053()
	local player = GetPlayer();
	if player:GetLev() < 53 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80053) or task:HasCompletedTask(80053) or task:HasSubmitedTask(80053) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080053()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 53 then
		return false;
	end
	if task:HasAcceptedTask(80053) or task:HasCompletedTask(80053) or task:HasSubmitedTask(80053) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080053()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80053) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080053(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80053) == npcId and Task_Accept_00080053 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80053
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "奋力一击";
	elseif task:GetTaskSubmitNpc(80053) == npcId then
		if Task_Submit_00080053() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80053
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "奋力一击";
		elseif task:HasAcceptedTask(80053) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80053
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "奋力一击";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080053_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，修炼的历程是艰辛而曲折的，山海大陆的想必你已经有所了解。我急需你的帮助，去杀掉那些在朝雪郡冰封窟的石冰龙，收集20个他们的冰龙骨，我现在不得不借助你的力量，来平息这样的状况，而且这同时能给你的修炼带来收获。";
	action.m_ActionMsg = "的确是一举两得的事情。";
	return action;
end

function Task_00080053_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这么困难的任务，居然就被你做完了。恭喜你进入更高级的修炼阶段。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080053_step_table = {
		[1] = Task_00080053_step_01,
		[10] = Task_00080053_step_10,
		};

function Task_00080053_step(step)
	if Task_00080053_step_table[step] ~= nil then
		return Task_00080053_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080053_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080053() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80053) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15091,1);
	if itemNum ~= 0 then
		if itemNum > 20 then
			itemNum = 20;
			package:SetItem(15091, itemNum, 1);
		end
		task:AddTaskStep2(80053, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00080053_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15091,1) < 20 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8915,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(80053) then
		return false;
	end

	if IsEquipTypeId(8915) then
		for k = 1, 1 do
			package:AddEquip(8915, 1);
		end
	else 
		package:AddItem(8915,1,1);
	end
	package:DelItemAll(15091,1);

	player:AddExp(22000);
	return true;
end

--��������
function Task_00080053_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15091,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80053);
end
