--����Ľ�������
function Task_Accept_00032005()
	local player = GetPlayer();
	if player:GetLev() < 75 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(32005) or task:HasCompletedTask(32005) or task:HasSubmitedTask(32005) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(32004) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(32004) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00032005()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 75 then
		return false;
	end
	if task:HasAcceptedTask(32005) or task:HasCompletedTask(32005) or task:HasSubmitedTask(32005) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(32004) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(32004) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00032005()
	if GetPlayer():GetTaskMgr():HasCompletedTask(32005) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00032005(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(32005) == npcId and Task_Accept_00032005 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 32005
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "终极战役";
	elseif task:GetTaskSubmitNpc(32005) == npcId then
		if Task_Submit_00032005() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 32005
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "终极战役";
		elseif task:HasAcceptedTask(32005) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 32005
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "终极战役";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00032005_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士我们与夜摩的决战就要开始了，你做好准备了吗？夜叉王和摩可拿以及他的部下，分别藏在灵岩浮岛的灰岩塔，沙波海底古窟的永左塔和海市桥的冥悬塔里面。";
	action.m_ActionMsg = "我已经做好准备了。";
	return action;
end

function Task_00032005_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "东山派和镇邪派也已经准备好了，并且派出了最精锐的部队，让我们联合起来，让夜摩盟彻底灭亡。";
	action.m_ActionMsg = "为了那些受伤牺牲的将士，我要狠狠的教训他们。";
	return action;
end

function Task_00032005_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哈哈，勇士在你与我们所有人的共同努力下，终于结束了夜摩盟的邪恶势力。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00032005_step_table = {
		[1] = Task_00032005_step_01,
		[2] = Task_00032005_step_02,
		[10] = Task_00032005_step_10,
		};

function Task_00032005_step(step)
	if Task_00032005_step_table[step] ~= nil then
		return Task_00032005_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00032005_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00032005() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(32005) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00032005_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8907,1,1) + package:GetItemUsedGrids(8930,1,1) + package:GetItemUsedGrids(1306,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(32005) then
		return false;
	end

	if IsEquipTypeId(8907) then
		for k = 1, 1 do
			package:AddEquip(8907, 1);
		end
	else 
		package:AddItem(8907,1,1);
	end
	if IsEquipTypeId(8930) then
		for k = 1, 1 do
			package:AddEquip(8930, 1);
		end
	else 
		package:AddItem(8930,1,1);
	end
	if IsEquipTypeId(1306) then
		for k = 1, 1 do
			package:AddEquip(1306, 1);
		end
	else 
		package:AddItem(1306,1,1);
	end

	player:AddExp(300000);
	player:getCoin(300000);
	player:getTael(60);
	return true;
end

--��������
function Task_00032005_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(32005);
end
