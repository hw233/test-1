--����Ľ�������
function Task_Accept_00020407()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 22 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20407) or task:HasCompletedTask(20407) or task:HasSubmitedTask(20407) then
		return false;
	end
	if not task:HasSubmitedTask(20406) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020407()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 22 then
		return false;
	end
	if task:HasAcceptedTask(20407) or task:HasCompletedTask(20407) or task:HasSubmitedTask(20407) then
		return false;
	end
	if not task:HasSubmitedTask(20406) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020407()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20407) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020407(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20407) == npcId and Task_Accept_00020407 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20407
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "刺客的秘密";
	elseif task:GetTaskSubmitNpc(20407) == npcId then
		if Task_Submit_00020407() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20407
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "刺客的秘密";
		elseif task:HasAcceptedTask(20407) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20407
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "刺客的秘密";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020407_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你来这里做什么？就凭你一个小小的魔族勇士，凭什么让我说我就说，况且我根本就什么都不知道，你问我也没有用。";
	action.m_ActionMsg = "帝释天一定是有什么阴谋，不然不会这样。";
	return action;
end

function Task_00020407_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "不管怎样，你从我这里是问不出什么来的，有本事你就去打败其他的天族刺客，看他们肯不肯说了。\n";
	action.m_ActionMsg = "好的，你等着瞧。";
	return action;
end

function Task_00020407_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我想你一定是了解到情况了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020407_step_table = {
		[1] = Task_00020407_step_01,
		[2] = Task_00020407_step_02,
		[10] = Task_00020407_step_10,
		};

function Task_00020407_step(step)
	if Task_00020407_step_table[step] ~= nil then
		return Task_00020407_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020407_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020407() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20407) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15030,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15030, itemNum, 1);
		end
		task:AddTaskStep2(20407, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00020407_submit(itemId, itemNum)
	local player = GetPlayer();

	--���ѡ������Ʒ
	local select = false;
	if itemId == 130 and itemNum == 1 then
		select = true;
	elseif itemId == 132 and itemNum == 1 then
		select = true;
	elseif itemId == 134 and itemNum == 1 then
		select = true;
	end

	if not select then return false; end
	local package = player:GetPackage();
	if package:GetItemNum(15030,1) < 1 then
		return false;
	end

	local selReqGrid = package:GetItemUsedGrids(itemId, itemNum, 1);
	if selReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(20407) then
		return false;
	end

	if IsEquipTypeId(itemId) then 
		for j = 1, itemNum do
			package:AddEquip(itemId, 1);
		end
	else
		package:AddItem(itemId, itemNum, 1);
	end
	package:DelItemAll(15030,1);

	player:AddExp(2200);
	player:getCoin(4100);
	player:getTael(10);
	return true;
end

--��������
function Task_00020407_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15030,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20407);
end
