--����Ľ�������
function Task_Accept_00060803()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 46 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60803) or task:HasCompletedTask(60803) or task:HasSubmitedTask(60803) then
		return false;
	end
	if not task:HasSubmitedTask(60802) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060803()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 46 then
		return false;
	end
	if task:HasAcceptedTask(60803) or task:HasCompletedTask(60803) or task:HasSubmitedTask(60803) then
		return false;
	end
	if not task:HasSubmitedTask(60802) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00060803()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60803) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060803(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60803) == npcId and Task_Accept_00060803 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60803
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "神药奇效";
	elseif task:GetTaskSubmitNpc(60803) == npcId then
		if Task_Submit_00060803() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60803
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "神药奇效";
		elseif task:HasAcceptedTask(60803) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60803
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "神药奇效";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060803_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "？？？";
	action.m_ActionMsg = "我知道你不会说话，神指大师让你服下这棵草，你的失语症就好了。";
	return action;
end

function Task_00060803_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "（服下草药）";
	action.m_ActionMsg = "现在感觉怎么样？";
	return action;
end

function Task_00060803_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我的整个喉咙肺腑完全通了，终于可以说话了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060803_step_table = {
		[1] = Task_00060803_step_01,
		[2] = Task_00060803_step_02,
		[10] = Task_00060803_step_10,
		};

function Task_00060803_step(step)
	if Task_00060803_step_table[step] ~= nil then
		return Task_00060803_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060803_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060803() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60803) then
		return false;
	end
	task:AddTaskStep(60803);
	return true;
end



--�ύ����
function Task_00060803_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8934,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(60803) then
		return false;
	end

	if IsEquipTypeId(8934) then
		for k = 1, 1 do
			package:AddEquip(8934, 1);
		end
	else 
		package:AddItem(8934,1,1);
	end

	player:AddExp(40000);
	player:getCoin(27000);
	return true;
end

--��������
function Task_00060803_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60803);
end
