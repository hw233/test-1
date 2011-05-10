--����Ľ�������
function Task_Accept_00031804()
	local player = GetPlayer();
	if player:GetLev() < 69 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31804) or task:HasCompletedTask(31804) or task:HasSubmitedTask(31804) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31803) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31803) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031804()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 69 then
		return false;
	end
	if task:HasAcceptedTask(31804) or task:HasCompletedTask(31804) or task:HasSubmitedTask(31804) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31803) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31803) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031804()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31804) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031804(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31804) == npcId and Task_Accept_00031804 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31804
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "护送蓦然";
	elseif task:GetTaskAcceptNpc(31804) == npcId and task:HasAcceptedTask(31804) then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31804
		action.m_ActionToken = 3;
		action.m_ActionStep = 11;
		action.m_ActionMsg = "护送蓦然";
	elseif task:GetTaskSubmitNpc(31804) == npcId then
		if Task_Submit_00031804() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31804
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "护送蓦然";
		elseif task:HasAcceptedTask(31804) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31804
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "护送蓦然";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031804_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士感谢你为我做的这一切，经过你的努力，额提终于肯放过我，让我回家了，真希望马上回到父亲那里，不过我还是希望你能送我一下。";
	action.m_ActionMsg = "没问题。";
	return action;
end

function Task_00031804_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你只要送我离开这片幽暗的山谷就好，我实在不认识这里的路，只要能离开这里，我会自己找到回猎户山的路的。不过，我走之前额提让我告诉你，送我走之后再回去找他。";
	action.m_ActionMsg = "好的，那我们走吧。";
	return action;
end

function Task_00031804_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "蓦然被安全送回她父亲那里了吗？";
	action.m_ActionMsg = "";
	return action;
end

function Task_00031804_step_11()
	RunConveyAction(GetPlayer(),31804);
end

local Task_00031804_step_table = {
		[1] = Task_00031804_step_01,
		[2] = Task_00031804_step_02,
		[10] = Task_00031804_step_10,
		[11] = Task_00031804_step_11,
		};

function Task_00031804_step(step)
	if Task_00031804_step_table[step] ~= nil then
		return Task_00031804_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031804_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031804() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31804) then
		return false;
	end
	RunConveyAction(player,31804);
	return true;
end



--�ύ����
function Task_00031804_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8919,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31804) then
		return false;
	end

	if IsEquipTypeId(8919) then
		for k = 1, 1 do
			package:AddEquip(8919, 1);
		end
	else 
		package:AddItem(8919,1,1);
	end

	player:AddExp(220000);
	player:getCoin(204000);
	player:getTael(40);
	return true;
end

--��������
function Task_00031804_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31804);
end
