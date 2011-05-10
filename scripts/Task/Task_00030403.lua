--����Ľ�������
function Task_Accept_00030403()
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30403) or task:HasCompletedTask(30403) or task:HasSubmitedTask(30403) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30402) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30402) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030403()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(30403) or task:HasCompletedTask(30403) or task:HasSubmitedTask(30403) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30402) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30402) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030403()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30403) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030403(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30403) == npcId and Task_Accept_00030403 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30403
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "备战夜摩";
	elseif task:GetTaskSubmitNpc(30403) == npcId then
		if Task_Submit_00030403() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30403
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "备战夜摩";
		elseif task:HasAcceptedTask(30403) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30403
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "备战夜摩";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030403_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "很高兴你愿意为对付夜摩盟贡献你的力量，可是你也应该知道，夜摩盟在夜叉王和摩可拿邪恶阴谋的指引下，正在疯狂的扩张自己的势力，他们并不是那么好对付的。";
	action.m_ActionMsg = "我明白。";
	return action;
end

function Task_00030403_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我相信你是具有很强的天赋和战斗能力的，不过不要忽视装备对你的战斗和生存能力的提高，如果你不嫌弃，去城外的黑风岗，杀掉那里的硬甲狼，收集一些他们的皮毛，他们坚硬的皮毛是做装备的最好的材料。";
	action.m_ActionMsg = "我这就去了。";
	return action;
end

function Task_00030403_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "有没有发现这些狼皮真的很坚硬啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030403_step_table = {
		[1] = Task_00030403_step_01,
		[2] = Task_00030403_step_02,
		[10] = Task_00030403_step_10,
		};

function Task_00030403_step(step)
	if Task_00030403_step_table[step] ~= nil then
		return Task_00030403_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030403_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030403() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30403) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15036,1);
	if itemNum ~= 0 then
		if itemNum > 12 then
			itemNum = 12;
			package:SetItem(15036, itemNum, 1);
		end
		task:AddTaskStep2(30403, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00030403_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15036,1) < 12 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8906,1,1) + package:GetItemUsedGrids(8914,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(30403) then
		return false;
	end

	if IsEquipTypeId(8906) then
		for k = 1, 1 do
			package:AddEquip(8906, 1);
		end
	else 
		package:AddItem(8906,1,1);
	end
	if IsEquipTypeId(8914) then
		for k = 1, 1 do
			package:AddEquip(8914, 1);
		end
	else 
		package:AddItem(8914,1,1);
	end
	package:DelItemAll(15036,1);

	player:AddExp(5000);
	player:getCoin(7500);
	player:getTael(10);
	return true;
end

--��������
function Task_00030403_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15036,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30403);
end
