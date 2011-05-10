--����Ľ�������
function Task_Accept_00020503()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 26 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20503) or task:HasCompletedTask(20503) or task:HasSubmitedTask(20503) then
		return false;
	end
	if not task:HasSubmitedTask(20502) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020503()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 26 then
		return false;
	end
	if task:HasAcceptedTask(20503) or task:HasCompletedTask(20503) or task:HasSubmitedTask(20503) then
		return false;
	end
	if not task:HasSubmitedTask(20502) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020503()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20503) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020503(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20503) == npcId and Task_Accept_00020503 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20503
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "力量的测试";
	elseif task:GetTaskSubmitNpc(20503) == npcId then
		if Task_Submit_00020503() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20503
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "力量的测试";
		elseif task:HasAcceptedTask(20503) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20503
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "力量的测试";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020503_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士"..GetPlayerName(GetPlayer()).."，你的精神我很受感动，魔族的确需要更多像你这样勇敢又乐于奉献的勇士，来摧毁夜摩盟的阴谋。不过现在我要测试一下，你究竟有多少实力，夜摩盟可不是那么好对付的。";
	action.m_ActionMsg = "那就开始对我实力的测试吧。";
	return action;
end

function Task_00020503_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "看来你还没有我想的那么强力。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020503_step_table = {
		[1] = Task_00020503_step_01,
		[10] = Task_00020503_step_10,
		};

function Task_00020503_step(step)
	if Task_00020503_step_table[step] ~= nil then
		return Task_00020503_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020503_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020503() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20503) then
		return false;
	end
	RunAutoBattleAction(player,4183,100);
	task:AddTaskStep(20503);

	return true;
end



--�ύ����
function Task_00020503_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8905,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(20503) then
		return false;
	end

	if IsEquipTypeId(8905) then
		for k = 1, 1 do
			package:AddEquip(8905, 1);
		end
	else 
		package:AddItem(8905,1,1);
	end

	player:AddExp(3200);
	player:getCoin(4900);
	player:getTael(10);
	return true;
end

--��������
function Task_00020503_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20503);
end
