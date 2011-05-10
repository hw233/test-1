--����Ľ�������
function Task_Accept_00060304()
	local player = GetPlayer();
	if player:GetLev() < 33 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60304) or task:HasCompletedTask(60304) or task:HasSubmitedTask(60304) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60303) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60303) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060304()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 33 then
		return false;
	end
	if task:HasAcceptedTask(60304) or task:HasCompletedTask(60304) or task:HasSubmitedTask(60304) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60303) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60303) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00060304()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60304) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060304(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60304) == npcId and Task_Accept_00060304 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60304
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "寂寞无边";
	elseif task:GetTaskSubmitNpc(60304) == npcId then
		if Task_Submit_00060304() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60304
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "寂寞无边";
		elseif task:HasAcceptedTask(60304) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60304
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "寂寞无边";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060304_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "天哪，寂寞无边，偌大一个山谷，唯一的活人还是个不吃饭的高人，这日子可怎么过啊。";
	action.m_ActionMsg = "同情你。";
	return action;
end

function Task_00060304_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士不得不拜托你，如果以后遇到什么人，也像我一样，不妨把我介绍给他，在这寂寞的地方继续下去的话我都要疯了。";
	action.m_ActionMsg = "看来你吃的不是饭而是寂寞，我会时常来看你的。";
	return action;
end

function Task_00060304_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你这就要走了吗？你这一走，我又看不到活人了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060304_step_table = {
		[1] = Task_00060304_step_01,
		[2] = Task_00060304_step_02,
		[10] = Task_00060304_step_10,
		};

function Task_00060304_step(step)
	if Task_00060304_step_table[step] ~= nil then
		return Task_00060304_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060304_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060304() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60304) then
		return false;
	end
	task:AddTaskStep(60304);
	return true;
end



--�ύ����
function Task_00060304_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8909,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(60304) then
		return false;
	end

	if IsEquipTypeId(8909) then
		for k = 1, 1 do
			package:AddEquip(8909, 1);
		end
	else 
		package:AddItem(8909,1,1);
	end

	player:AddExp(4500);
	player:getCoin(6150);
	return true;
end

--��������
function Task_00060304_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60304);
end
