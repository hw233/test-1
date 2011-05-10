--����Ľ�������
function Task_Accept_00060503()
	local player = GetPlayer();
	if player:GetLev() < 38 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60503) or task:HasCompletedTask(60503) or task:HasSubmitedTask(60503) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60502) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60502) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060503()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 38 then
		return false;
	end
	if task:HasAcceptedTask(60503) or task:HasCompletedTask(60503) or task:HasSubmitedTask(60503) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60502) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60502) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00060503()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60503) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060503(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60503) == npcId and Task_Accept_00060503 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60503
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "守墓高人";
	elseif task:GetTaskSubmitNpc(60503) == npcId then
		if Task_Submit_00060503() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60503
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "守墓高人";
		elseif task:HasAcceptedTask(60503) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60503
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "守墓高人";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060503_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你也看到我的生活了，虽然你可能觉得太孤寂艰苦，但是对于活着的人来说，有生命已经比那些牺牲的人幸福太多了，况且经过这么多年的 修炼，我已经完全不需要吃饭睡觉。";
	action.m_ActionMsg = "这是高级的法力吗？";
	return action;
end

function Task_00060503_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "……如影的好意我也已经接受了，你回去告诉他吧，他的任务还重着呢，以后也别再来见我了。";
	action.m_ActionMsg = "嗯好吧，我去见如影帮主。";
	return action;
end

function Task_00060503_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我的父亲就是这么的固执。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060503_step_table = {
		[1] = Task_00060503_step_01,
		[2] = Task_00060503_step_02,
		[10] = Task_00060503_step_10,
		};

function Task_00060503_step(step)
	if Task_00060503_step_table[step] ~= nil then
		return Task_00060503_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060503_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060503() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60503) then
		return false;
	end
	task:AddTaskStep(60503);
	return true;
end



--�ύ����
function Task_00060503_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8912,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(60503) then
		return false;
	end

	if IsEquipTypeId(8912) then
		for k = 1, 1 do
			package:AddEquip(8912, 1);
		end
	else 
		package:AddItem(8912,1,1);
	end

	player:AddExp(7000);
	player:getCoin(10000);
	return true;
end

--��������
function Task_00060503_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60503);
end
