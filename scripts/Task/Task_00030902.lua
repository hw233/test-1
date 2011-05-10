--����Ľ�������
function Task_Accept_00030902()
	local player = GetPlayer();
	if player:GetLev() < 54 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30902) or task:HasCompletedTask(30902) or task:HasSubmitedTask(30902) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30901) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30901) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030902()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 54 then
		return false;
	end
	if task:HasAcceptedTask(30902) or task:HasCompletedTask(30902) or task:HasSubmitedTask(30902) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30901) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30901) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030902()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30902) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030902(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30902) == npcId and Task_Accept_00030902 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30902
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "正面对决";
	elseif task:GetTaskSubmitNpc(30902) == npcId then
		if Task_Submit_00030902() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30902
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "正面对决";
		elseif task:HasAcceptedTask(30902) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30902
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "正面对决";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030902_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你在那里探查到什么情况了？";
	action.m_ActionMsg = "在冰封窟里聚集了许多石冰龙，旁边还有个叫摩伽的。";
	return action;
end

function Task_00030902_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "他们就是摩可拿的手下势力，石冰龙似乎是最近被夜摩盟收服了的怪物势力，应该不是非常厉害，你去将他们打败吧。";
	action.m_ActionMsg = "终于可以出手了，我忍了很久了.";
	return action;
end

function Task_00030902_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "虽然我不否认有一点担心，但是我还是相信你的实力的，能看到你回来，已经证明了我的想法。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030902_step_table = {
		[1] = Task_00030902_step_01,
		[2] = Task_00030902_step_02,
		[10] = Task_00030902_step_10,
		};

function Task_00030902_step(step)
	if Task_00030902_step_table[step] ~= nil then
		return Task_00030902_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030902_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030902() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30902) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00030902_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8902,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30902) then
		return false;
	end

	if IsEquipTypeId(8902) then
		for k = 1, 1 do
			package:AddEquip(8902, 1);
		end
	else 
		package:AddItem(8902,1,1);
	end

	player:AddExp(72000);
	player:getCoin(64000);
	player:getTael(20);
	return true;
end

--��������
function Task_00030902_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30902);
end
