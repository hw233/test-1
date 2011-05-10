--����Ľ�������
function Task_Accept_00040603()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40603) or task:HasCompletedTask(40603) or task:HasSubmitedTask(40603) then
		return false;
	end
	if not task:HasSubmitedTask(40602) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040603()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(40603) or task:HasCompletedTask(40603) or task:HasSubmitedTask(40603) then
		return false;
	end
	if not task:HasSubmitedTask(40602) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040603()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40603) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040603(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40603) == npcId and Task_Accept_00040603 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40603
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "采摘山茶";
	elseif task:GetTaskSubmitNpc(40603) == npcId then
		if Task_Submit_00040603() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40603
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "采摘山茶";
		elseif task:HasAcceptedTask(40603) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40603
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "采摘山茶";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040603_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "原来那些奇怪的怪物是些无头骑手啊，我从来没有见过，怪不得看起来那么奇怪，我都不敢出手，因为一直不了解这无头怪物究竟会有些什么可怕的能力呢，这次有了你的帮助，终于让我松了一口气。";
	action.m_ActionMsg = "这些无头怪物虽然看起来可怕，不过根本就不是我的对手。";
	return action;
end

function Task_00040603_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "现在我要告诉你我最近的一个发现，在奇沙阵那里，生长了一棵非常罕见的山茶，我想拜托你去采一束送给跟我一起来到天庭的刘邦，你可以在牛霍洲找到他，他喜欢那里的风景，也喜欢山茶。";
	action.m_ActionMsg = "刘邦喜欢山茶呀？那这山茶我喝了会变得力大无比吗？呵呵。";
	return action;
end

function Task_00040603_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嗯我已经闻到了山茶的清香了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040603_step_table = {
		[1] = Task_00040603_step_01,
		[2] = Task_00040603_step_02,
		[10] = Task_00040603_step_10,
		};

function Task_00040603_step(step)
	if Task_00040603_step_table[step] ~= nil then
		return Task_00040603_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040603_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040603() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40603) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15052,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15052, itemNum, 1);
		end
		task:AddTaskStep2(40603, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00040603_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15052,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8921,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(40603) then
		return false;
	end

	if IsEquipTypeId(8921) then
		for k = 1, 1 do
			package:AddEquip(8921, 1);
		end
	else 
		package:AddItem(8921,1,1);
	end
	package:DelItemAll(15052,1);

	player:AddExp(1500);
	player:getCoin(2000);
	return true;
end

--��������
function Task_00040603_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15052,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40603);
end
