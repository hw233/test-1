--����Ľ�������
function Task_Accept_00020106()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 5 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20106) or task:HasCompletedTask(20106) or task:HasSubmitedTask(20106) then
		return false;
	end
	if not task:HasSubmitedTask(20105) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020106()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 5 then
		return false;
	end
	if task:HasAcceptedTask(20106) or task:HasCompletedTask(20106) or task:HasSubmitedTask(20106) then
		return false;
	end
	if not task:HasSubmitedTask(20105) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020106()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20106) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020106(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20106) == npcId and Task_Accept_00020106 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20106
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "成长的脚步";
	elseif task:GetTaskSubmitNpc(20106) == npcId then
		if Task_Submit_00020106() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20106
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "成长的脚步";
		elseif task:HasAcceptedTask(20106) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20106
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "成长的脚步";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020106_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "不要忘记你是我们魔族的勇士，我们无畏无惧，我们背负对天族的仇恨，对力量的追求，对邪恶的憎恨。";
	action.m_ActionMsg = "我知道魔族需要我。";
	return action;
end

function Task_00020106_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "现在你需要离开十宝山到灰烬谷去，在勇健宫找到我们的战神因达罗，他会给你的成长更好的指导。我已经很久没有见到他了，帮我带去问候吧。";
	action.m_ActionMsg = "终于可以见到战神了啊。";
	return action;
end

function Task_00020106_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "原来是火神毗摩质多罗派来的勇士。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020106_step_table = {
		[1] = Task_00020106_step_01,
		[2] = Task_00020106_step_02,
		[10] = Task_00020106_step_10,
		};

function Task_00020106_step(step)
	if Task_00020106_step_table[step] ~= nil then
		return Task_00020106_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020106_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020106() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20106) then
		return false;
	end
	task:AddTaskStep(20106);
	return true;
end



--�ύ����
function Task_00020106_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(6,1,1) + package:GetItemUsedGrids(7,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(20106) then
		return false;
	end

	if IsEquipTypeId(6) then
		for k = 1, 1 do
			package:AddEquip(6, 1);
		end
	else 
		package:AddItem(6,1,1);
	end
	if IsEquipTypeId(7) then
		for k = 1, 1 do
			package:AddEquip(7, 1);
		end
	else 
		package:AddItem(7,1,1);
	end

	player:AddExp(800);
	player:getCoin(750);
	player:getTael(5);
	return true;
end

--��������
function Task_00020106_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20106);
end
