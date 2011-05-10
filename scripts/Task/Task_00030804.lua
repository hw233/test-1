--����Ľ�������
function Task_Accept_00030804()
	local player = GetPlayer();
	if player:GetLev() < 52 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30804) or task:HasCompletedTask(30804) or task:HasSubmitedTask(30804) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30803) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30803) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030804()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 52 then
		return false;
	end
	if task:HasAcceptedTask(30804) or task:HasCompletedTask(30804) or task:HasSubmitedTask(30804) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30803) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30803) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030804()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30804) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030804(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30804) == npcId and Task_Accept_00030804 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30804
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "锻造装备";
	elseif task:GetTaskSubmitNpc(30804) == npcId then
		if Task_Submit_00030804() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30804
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "锻造装备";
		elseif task:HasAcceptedTask(30804) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30804
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "锻造装备";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030804_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，你带来了的都是祖言帮主为我准备的打造装备的材料啊，就凭祖言帮主这份诚意，我也要用我的实力帮助镇邪派。";
	action.m_ActionMsg = "是啊，都是祖言帮主让我带给你的。";
	return action;
end

function Task_00030804_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我愿意投靠祖言帮主，也是因为佩服他的人品和作为，不过我自己可不会造装备啊，我们只能求助别人了，你将所有的材料带去朝雪郡的朝山门，那里有一位东山军备师，他叫飞鸣，是著名的军备师，告诉他我们的需求，他会愿意帮忙的。\n";
	action.m_ActionMsg = "好的，那我过去了。";
	return action;
end

function Task_00030804_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "很荣幸接到祖言帮主交付的任务。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030804_step_table = {
		[1] = Task_00030804_step_01,
		[2] = Task_00030804_step_02,
		[10] = Task_00030804_step_10,
		};

function Task_00030804_step(step)
	if Task_00030804_step_table[step] ~= nil then
		return Task_00030804_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030804_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030804() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15060, 1, 1) == nil then
		return false;
	end
	if package:AddItem(15062, 1, 1) == nil then
		return false;
	end
	if package:AddItem(15063, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(30804) then
		return false;
	end
	task:AddTaskStep(30804);
	return true;
end



--�ύ����
function Task_00030804_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15060,1) < 1 or package:GetItemNum(15062,1) < 1 or package:GetItemNum(15063,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(503,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30804) then
		return false;
	end

	if IsEquipTypeId(503) then
		for k = 1, 1 do
			package:AddEquip(503, 1);
		end
	else 
		package:AddItem(503,1,1);
	end
	package:DelItemAll(15060,1);
	package:DelItemAll(15062,1);
	package:DelItemAll(15063,1);

	player:AddExp(64000);
	player:getCoin(53000);
	player:getTael(20);
	return true;
end

--��������
function Task_00030804_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15060,1);
	package:DelItemAll(15062,1);
	package:DelItemAll(15063,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30804);
end
