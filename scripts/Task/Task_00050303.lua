--����Ľ�������
function Task_Accept_00050303()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 10 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50303) or task:HasCompletedTask(50303) or task:HasSubmitedTask(50303) then
		return false;
	end
	if not task:HasSubmitedTask(50302) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050303()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 10 then
		return false;
	end
	if task:HasAcceptedTask(50303) or task:HasCompletedTask(50303) or task:HasSubmitedTask(50303) then
		return false;
	end
	if not task:HasSubmitedTask(50302) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050303()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50303) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050303(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50303) == npcId and Task_Accept_00050303 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50303
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "告诫";
	elseif task:GetTaskSubmitNpc(50303) == npcId then
		if Task_Submit_00050303() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50303
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "告诫";
		elseif task:HasAcceptedTask(50303) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50303
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "告诫";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050303_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，你不会还相信庞涓的话吧。";
	action.m_ActionMsg = "我现在是不信了，我回去找他。";
	return action;
end

function Task_00050303_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我就不信了，我一定会找出他居心叵测的证据来的，你现在看到的都是表面的。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050303_step_table = {
		[1] = Task_00050303_step_01,
		[10] = Task_00050303_step_10,
		};

function Task_00050303_step(step)
	if Task_00050303_step_table[step] ~= nil then
		return Task_00050303_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050303_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050303() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50303) then
		return false;
	end
	task:AddTaskStep(50303);
	return true;
end



--�ύ����
function Task_00050303_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8911,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(50303) then
		return false;
	end

	if IsEquipTypeId(8911) then
		for k = 1, 1 do
			package:AddEquip(8911, 1);
		end
	else 
		package:AddItem(8911,1,1);
	end

	player:AddExp(1000);
	player:getCoin(950);
	return true;
end

--��������
function Task_00050303_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50303);
end
