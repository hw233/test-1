--����Ľ�������
function Task_Accept_00000048()
	local player = GetPlayer();
	if player:GetLev() < 28 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(48) or task:HasCompletedTask(48) or task:HasSubmitedTask(48) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(47) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(47) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(47) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000048()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 28 then
		return false;
	end
	if task:HasAcceptedTask(48) or task:HasCompletedTask(48) or task:HasSubmitedTask(48) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(47) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(47) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(47) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000048()
	if GetPlayer():GetTaskMgr():HasCompletedTask(48) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000048(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(48) == npcId and Task_Accept_00000048 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 48
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "避难";
	elseif task:GetTaskSubmitNpc(48) == npcId then
		if Task_Submit_00000048() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 48
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "避难";
		elseif task:HasAcceptedTask(48) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 48
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "避难";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000048_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哎，我在此地开辟个菜园已有多年，慈云寺方丈智通虽然不是好人，但对我还算照顾。如今收留了你们，我也会被寺内的恶僧们责难……还是早点离去吧，先去不远处的豆腐房避一避。";
	action.m_ActionMsg = "老张说的有道理，我们这就走吧。";
	return action;
end

function Task_00000048_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "诸位到此就不必惊慌了，其实我也是正道中人，在这里开了个豆腐房监视慈云寺很久了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000048_step_table = {
		[1] = Task_00000048_step_01,
		[10] = Task_00000048_step_10,
		};

function Task_00000048_step(step)
	if Task_00000048_step_table[step] ~= nil then
		return Task_00000048_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000048_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000048() then
		return false;
	end
	if not task:AcceptTask(48) then
		return false;
	end
	task:AddTaskStep(48);
	return true;
end



--�ύ����
function Task_00000048_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(4999,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(48) then
		return false;
	end

	if IsEquipTypeId(4999) then
		for k = 1, 1 do
			package:AddEquip(4999, 1);
		end
	else 
		package:AddItem(4999,1,1);
	end

	player:AddExp(5000);
	return true;
end

--��������
function Task_00000048_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(48);
end
