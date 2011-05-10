--����Ľ�������
function Task_Accept_00050202()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50202) or task:HasCompletedTask(50202) or task:HasSubmitedTask(50202) then
		return false;
	end
	if not task:HasSubmitedTask(50201) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050202()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(50202) or task:HasCompletedTask(50202) or task:HasSubmitedTask(50202) then
		return false;
	end
	if not task:HasSubmitedTask(50201) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050202()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50202) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050202(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50202) == npcId and Task_Accept_00050202 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50202
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "不堪回首";
	elseif task:GetTaskSubmitNpc(50202) == npcId then
		if Task_Submit_00050202() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50202
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "不堪回首";
		elseif task:HasAcceptedTask(50202) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50202
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "不堪回首";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050202_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "还真的是为悦意的事情，我实话我已经不想再提这个事情了，我宁愿将所有的痛苦都藏在心底，因为不会有人比我更难过了。";
	action.m_ActionMsg = "很抱歉，我想香音王后也是出于好意。";
	return action;
end

function Task_00050202_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我明白，不过你还是回复王后说，我只能尽量与悦意取得联系，但是其他的我不想提了，希望王后可以理解。";
	action.m_ActionMsg = "这都是帝释天的错，我想悦意现在也很痛苦.";
	return action;
end

function Task_00050202_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "因达罗这么说，我也理解，那就不再跟他提这个事情了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050202_step_table = {
		[1] = Task_00050202_step_01,
		[2] = Task_00050202_step_02,
		[10] = Task_00050202_step_10,
		};

function Task_00050202_step(step)
	if Task_00050202_step_table[step] ~= nil then
		return Task_00050202_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050202_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050202() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50202) then
		return false;
	end
	task:AddTaskStep(50202);
	return true;
end



--�ύ����
function Task_00050202_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8900,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(50202) then
		return false;
	end

	if IsEquipTypeId(8900) then
		for k = 1, 1 do
			package:AddEquip(8900, 1);
		end
	else 
		package:AddItem(8900,1,1);
	end

	player:AddExp(1100);
	player:getCoin(800);
	return true;
end

--��������
function Task_00050202_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50202);
end
