--����Ľ�������
function Task_Accept_00020304()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 19 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20304) or task:HasCompletedTask(20304) or task:HasSubmitedTask(20304) then
		return false;
	end
	if not task:HasSubmitedTask(20303) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020304()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 19 then
		return false;
	end
	if task:HasAcceptedTask(20304) or task:HasCompletedTask(20304) or task:HasSubmitedTask(20304) then
		return false;
	end
	if not task:HasSubmitedTask(20303) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020304()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20304) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020304(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20304) == npcId and Task_Accept_00020304 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20304
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "山花留念";
	elseif task:GetTaskSubmitNpc(20304) == npcId then
		if Task_Submit_00020304() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20304
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "山花留念";
		elseif task:HasAcceptedTask(20304) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20304
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "山花留念";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020304_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "最近又该是祭奠文隣的日子了，文隣生前最喜欢的就是蒲昌海的山花，可是那里的山花都被荒郊的那些采花怪兽抢走了，野外再也采不到了，你能帮我去打败他们然后抢一束山花回来吗？不过希望你小心，那里的怪兽还是非常厉害的。";
	action.m_ActionMsg = "当然可以，我现在就去收拾那些采花怪兽。";
	return action;
end

function Task_00020304_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你真是太厉害了，这么快就带回了文隣最爱的山花。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020304_step_table = {
		[1] = Task_00020304_step_01,
		[10] = Task_00020304_step_10,
		};

function Task_00020304_step(step)
	if Task_00020304_step_table[step] ~= nil then
		return Task_00020304_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020304_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020304() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20304) then
		return false;
	end
	local package = player:GetPackage();
	local itemNum = package:GetItemNum(15029,1);
	if itemNum ~= 0 then
		if itemNum > 1 then
			itemNum = 1;
			package:SetItem(15029, itemNum, 1);
		end
		task:AddTaskStep2(20304, 1, itemNum);
	end
	return true;
end



--�ύ����
function Task_00020304_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15029,1) < 1 then
		return false;
	end

	local fixReqGrid = package:GetItemUsedGrids(8904,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(20304) then
		return false;
	end

	if IsEquipTypeId(8904) then
		for k = 1, 1 do
			package:AddEquip(8904, 1);
		end
	else 
		package:AddItem(8904,1,1);
	end
	package:DelItemAll(15029,1);

	player:AddExp(1800);
	player:getCoin(2300);
	player:getTael(10);
	return true;
end

--��������
function Task_00020304_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15029,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20304);
end
