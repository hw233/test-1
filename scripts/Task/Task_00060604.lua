--����Ľ�������
function Task_Accept_00060604()
	local player = GetPlayer();
	if player:GetLev() < 41 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60604) or task:HasCompletedTask(60604) or task:HasSubmitedTask(60604) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60603) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60603) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060604()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 41 then
		return false;
	end
	if task:HasAcceptedTask(60604) or task:HasCompletedTask(60604) or task:HasSubmitedTask(60604) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60603) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60603) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00060604()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60604) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060604(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60604) == npcId and Task_Accept_00060604 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60604
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "丰厚奖赏";
	elseif task:GetTaskSubmitNpc(60604) == npcId then
		if Task_Submit_00060604() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60604
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "丰厚奖赏";
		elseif task:HasAcceptedTask(60604) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60604
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "丰厚奖赏";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060604_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士这一路真是辛苦你了，你也知道我是个生意人，财物对我很重要，所以我对这次押镖很在意，可是我也不是重财不重义的人，你帮了我这么大的忙，我一定得给你很丰厚的奖励。";
	action.m_ActionMsg = "好吧，那我就接受了。";
	return action;
end

function Task_00060604_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，在山河郡有任何需要我帮助的地方尽管跟我开口。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060604_step_table = {
		[1] = Task_00060604_step_01,
		[10] = Task_00060604_step_10,
		};

function Task_00060604_step(step)
	if Task_00060604_step_table[step] ~= nil then
		return Task_00060604_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060604_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060604() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60604) then
		return false;
	end
	task:AddTaskStep(60604);
	return true;
end



--�ύ����
function Task_00060604_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8934,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(60604) then
		return false;
	end

	if IsEquipTypeId(8934) then
		for k = 1, 1 do
			package:AddEquip(8934, 1);
		end
	else 
		package:AddItem(8934,1,1);
	end

	player:AddExp(12000);
	player:getCoin(15000);
	return true;
end

--��������
function Task_00060604_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60604);
end
