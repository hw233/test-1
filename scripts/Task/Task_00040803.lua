--����Ľ�������
function Task_Accept_00040803()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 25 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40803) or task:HasCompletedTask(40803) or task:HasSubmitedTask(40803) then
		return false;
	end
	if not task:HasSubmitedTask(40802) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040803()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 25 then
		return false;
	end
	if task:HasAcceptedTask(40803) or task:HasCompletedTask(40803) or task:HasSubmitedTask(40803) then
		return false;
	end
	if not task:HasSubmitedTask(40802) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040803()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40803) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040803(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40803) == npcId and Task_Accept_00040803 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40803
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "护送战马";
	elseif task:GetTaskAcceptNpc(40803) == npcId and task:HasAcceptedTask(40803) then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40803
		action.m_ActionToken = 3;
		action.m_ActionStep = 11;
		action.m_ActionMsg = "护送战马";
	elseif task:GetTaskSubmitNpc(40803) == npcId then
		if Task_Submit_00040803() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40803
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "护送战马";
		elseif task:HasAcceptedTask(40803) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40803
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "护送战马";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040803_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "请将我带回到我的主人庞统那里。";
	action.m_ActionMsg = "奇怪了，马居然会说话。";
	return action;
end

function Task_00040803_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我的马回来了，太好了，真是感谢你勇士。";
	action.m_ActionMsg = "";
	return action;
end

function Task_00040803_step_11()
	RunConveyAction(GetPlayer(),40803);
end

local Task_00040803_step_table = {
		[1] = Task_00040803_step_01,
		[10] = Task_00040803_step_10,
		[11] = Task_00040803_step_11,
		};

function Task_00040803_step(step)
	if Task_00040803_step_table[step] ~= nil then
		return Task_00040803_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040803_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040803() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40803) then
		return false;
	end
	RunConveyAction(player,40803);
	return true;
end



--�ύ����
function Task_00040803_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8933,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(40803) then
		return false;
	end

	if IsEquipTypeId(8933) then
		for k = 1, 1 do
			package:AddEquip(8933, 1);
		end
	else 
		package:AddItem(8933,1,1);
	end

	player:AddExp(3000);
	player:getCoin(4200);
	return true;
end

--��������
function Task_00040803_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40803);
end
