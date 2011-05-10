--����Ľ�������
function Task_Accept_00031403()
	local player = GetPlayer();
	if player:GetLev() < 62 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31403) or task:HasCompletedTask(31403) or task:HasSubmitedTask(31403) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31402) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31402) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031403()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 62 then
		return false;
	end
	if task:HasAcceptedTask(31403) or task:HasCompletedTask(31403) or task:HasSubmitedTask(31403) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31402) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31402) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031403()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31403) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031403(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31403) == npcId and Task_Accept_00031403 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31403
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "毒气弥漫";
	elseif task:GetTaskSubmitNpc(31403) == npcId then
		if Task_Submit_00031403() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31403
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "毒气弥漫";
		elseif task:HasAcceptedTask(31403) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31403
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "毒气弥漫";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031403_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士请你现在马上再过去一趟，刚刚的毒气只能维持狼族守卫5分钟的昏睡时间，一定要在这之前将碎心塔的钥匙偷出来，不然他们就会醒过来。";
	action.m_ActionMsg = "那我现在就过去。";
	return action;
end

function Task_00031403_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你太英勇了，居然帮我们弄到了碎心塔的钥匙。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031403_step_table = {
		[1] = Task_00031403_step_01,
		[10] = Task_00031403_step_10,
		};

function Task_00031403_step(step)
	if Task_00031403_step_table[step] ~= nil then
		return Task_00031403_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031403_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031403() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31403) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15073,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15073, itemNum, 1);
		end
		task:AddTaskStep2(31403, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00031403_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15073,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8912,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31403) then
		return false;
	end

	if IsEquipTypeId(8912) then
		for k = 1, 1 do
			package:AddEquip(8912, 1);
		end
	else 
		package:AddItem(8912,1,1);
	end
	package:DelItemAll(15073,1);

	player:AddExp(150000);
	player:getCoin(125000);
	player:getTael(30);
	return true;
end

--��������
function Task_00031403_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15073,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31403);
end
