--����Ľ�������
function Task_Accept_00030603()
	local player = GetPlayer();
	if player:GetLev() < 44 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30603) or task:HasCompletedTask(30603) or task:HasSubmitedTask(30603) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(3060201) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(3060202) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030603()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 44 then
		return false;
	end
	if task:HasAcceptedTask(30603) or task:HasCompletedTask(30603) or task:HasSubmitedTask(30603) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(3060201) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(3060202) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030603()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30603) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030603(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30603) == npcId and Task_Accept_00030603 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30603
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "小鬼的感谢";
	elseif task:GetTaskSubmitNpc(30603) == npcId then
		if Task_Submit_00030603() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30603
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "小鬼的感谢";
		elseif task:HasAcceptedTask(30603) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30603
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "小鬼的感谢";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030603_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "真是太感谢您了，您的善心救活了我的兄弟，我一定会让更多的人知道你的事迹。";
	action.m_ActionMsg = "这是我应该做的，不必客气。";
	return action;
end

function Task_00030603_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "真是不知道该怎么感谢您才好，在山海大陆的旅途辛劳，您一定要留下来喝杯茶。";
	action.m_ActionMsg = "既然这样，只好盛情难却了。（将茶喝下）";
	return action;
end

function Task_00030603_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "呵呵哈哈，不要防备心这么强嘛。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030603_step_table = {
		[1] = Task_00030603_step_01,
		[2] = Task_00030603_step_02,
		[10] = Task_00030603_step_10,
		};

function Task_00030603_step(step)
	if Task_00030603_step_table[step] ~= nil then
		return Task_00030603_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030603_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030603() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30603) then
		return false;
	end
	task:AddTaskStep(30603);
	return true;
end



--�ύ����
function Task_00030603_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8927,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30603) then
		return false;
	end

	if IsEquipTypeId(8927) then
		for k = 1, 1 do
			package:AddEquip(8927, 1);
		end
	else 
		package:AddItem(8927,1,1);
	end

	player:AddExp(32000);
	player:getCoin(22000);
	return true;
end

--��������
function Task_00030603_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30603);
end
