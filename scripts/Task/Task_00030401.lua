--����Ľ�������
function Task_Accept_00030401()
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30401) or task:HasCompletedTask(30401) or task:HasSubmitedTask(30401) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30304) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30304) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030401()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(30401) or task:HasCompletedTask(30401) or task:HasSubmitedTask(30401) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30304) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30304) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030401()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30401) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030401(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30401) == npcId and Task_Accept_00030401 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30401
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "拜见帮主";
	elseif task:GetTaskSubmitNpc(30401) == npcId then
		if Task_Submit_00030401() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30401
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "拜见帮主";
		elseif task:HasAcceptedTask(30401) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30401
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "拜见帮主";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030401_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这位勇士，你是从哪里来？";
	action.m_ActionMsg = "万人墓的守墓人介绍我来见你们的帮主如影。";
	return action;
end

function Task_00030401_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "既然这样，你可以进城了，不过还是要先遵守我们的规矩，敢于与我们进行战斗才可以。山海大陆上狡猾的夜摩太多了，但是他们的大多数都惧怕我们东山派，不敢正面与我们战斗。";
	action.m_ActionMsg = "好的，没问题。";
	return action;
end

function Task_00030401_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "刚刚守卫已经向我汇报了，很高兴你来到东山城勇士。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030401_step_table = {
		[1] = Task_00030401_step_01,
		[2] = Task_00030401_step_02,
		[10] = Task_00030401_step_10,
		};

function Task_00030401_step(step)
	if Task_00030401_step_table[step] ~= nil then
		return Task_00030401_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030401_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030401() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30401) then
		return false;
	end
	RunAutoBattleAction(player,4204,100);
	task:AddTaskStep(30401);

	return true;
end



--�ύ����
function Task_00030401_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8905,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30401) then
		return false;
	end

	if IsEquipTypeId(8905) then
		for k = 1, 1 do
			package:AddEquip(8905, 1);
		end
	else 
		package:AddItem(8905,1,1);
	end

	player:AddExp(5000);
	player:getCoin(7500);
	player:getTael(10);
	return true;
end

--��������
function Task_00030401_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30401);
end
