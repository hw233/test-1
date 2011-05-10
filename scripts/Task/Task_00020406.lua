--����Ľ�������
function Task_Accept_00020406()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 22 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20406) or task:HasCompletedTask(20406) or task:HasSubmitedTask(20406) then
		return false;
	end
	if not task:HasSubmitedTask(20405) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020406()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 22 then
		return false;
	end
	if task:HasAcceptedTask(20406) or task:HasCompletedTask(20406) or task:HasSubmitedTask(20406) then
		return false;
	end
	if not task:HasSubmitedTask(20405) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020406()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20406) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020406(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20406) == npcId and Task_Accept_00020406 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20406
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "审问刺客";
	elseif task:GetTaskSubmitNpc(20406) == npcId then
		if Task_Submit_00020406() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20406
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "审问刺客";
		elseif task:HasAcceptedTask(20406) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20406
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "审问刺客";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020406_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."，你已经问过负责运输甘露的使节了吗？他知道是怎么回事吗？";
	action.m_ActionMsg = "问过了，他也不知道是怎么回事。";
	return action;
end

function Task_00020406_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这样吧，我的手下刚刚抓回来几个天族的刺客，关在十宝山的地牢，我想他们一定是带着阴谋来的，你去十宝山找他们审问一下，一定要让他们交代出来，必要的时候可以动用武力。";
	action.m_ActionMsg = "我一定会逼他说出来的。";
	return action;
end

function Task_00020406_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你休想从我这里得到任何东西。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020406_step_table = {
		[1] = Task_00020406_step_01,
		[2] = Task_00020406_step_02,
		[10] = Task_00020406_step_10,
		};

function Task_00020406_step(step)
	if Task_00020406_step_table[step] ~= nil then
		return Task_00020406_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020406_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020406() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20406) then
		return false;
	end
	task:AddTaskStep(20406);
	return true;
end



--�ύ����
function Task_00020406_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8905,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(20406) then
		return false;
	end

	if IsEquipTypeId(8905) then
		for k = 1, 1 do
			package:AddEquip(8905, 1);
		end
	else 
		package:AddItem(8905,1,1);
	end

	player:AddExp(2200);
	player:getCoin(3900);
	player:getTael(10);
	return true;
end

--��������
function Task_00020406_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20406);
end
