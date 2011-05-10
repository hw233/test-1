--����Ľ�������
function Task_Accept_00010202()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(10202) or task:HasCompletedTask(10202) or task:HasSubmitedTask(10202) then
		return false;
	end
	if not task:HasSubmitedTask(10201) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00010202()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(10202) or task:HasCompletedTask(10202) or task:HasSubmitedTask(10202) then
		return false;
	end
	if not task:HasSubmitedTask(10201) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00010202()
	if GetPlayer():GetTaskMgr():HasCompletedTask(10202) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00010202(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(10202) == npcId and Task_Accept_00010202 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10202
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "安全的护送";
	elseif task:GetTaskAcceptNpc(10202) == npcId and task:HasAcceptedTask(10202) then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 10202
		action.m_ActionToken = 3;
		action.m_ActionStep = 11;
		action.m_ActionMsg = "安全的护送";
	elseif task:GetTaskSubmitNpc(10202) == npcId then
		if Task_Submit_00010202() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10202
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "安全的护送";
		elseif task:HasAcceptedTask(10202) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 10202
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "安全的护送";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00010202_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士看着你面生啊，是第一次来帮忙送甘露的吧。我听说瞻部峰这周围最近不太平，我怕路上会遇到危险，怕是还要麻烦你护送我们安全出城啊。";
	action.m_ActionMsg = "没问题，我一定会尽力帮你，我们走吧。";
	return action;
end

function Task_00010202_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这么快就完成我吩咐的事情了啊，真是一个很有潜力的家伙。";
	action.m_ActionMsg = "";
	return action;
end

function Task_00010202_step_11()
	RunConveyAction(GetPlayer(),10202);
end

local Task_00010202_step_table = {
		[1] = Task_00010202_step_01,
		[10] = Task_00010202_step_10,
		[11] = Task_00010202_step_11,
		};

function Task_00010202_step(step)
	if Task_00010202_step_table[step] ~= nil then
		return Task_00010202_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00010202_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00010202() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(10202) then
		return false;
	end
	RunConveyAction(player,10202);
	return true;
end



--�ύ����
function Task_00010202_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8917,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(10202) then
		return false;
	end

	if IsEquipTypeId(8917) then
		for k = 1, 1 do
			package:AddEquip(8917, 1);
		end
	else 
		package:AddItem(8917,1,1);
	end

	player:AddExp(1400);
	player:getCoin(1100);
	player:getTael(10);
	return true;
end

--��������
function Task_00010202_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(10202);
end
