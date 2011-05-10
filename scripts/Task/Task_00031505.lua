--����Ľ�������
function Task_Accept_00031505()
	local player = GetPlayer();
	if player:GetLev() < 65 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31505) or task:HasCompletedTask(31505) or task:HasSubmitedTask(31505) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31504) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31504) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031505()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 65 then
		return false;
	end
	if task:HasAcceptedTask(31505) or task:HasCompletedTask(31505) or task:HasSubmitedTask(31505) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31504) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31504) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031505()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31505) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031505(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31505) == npcId and Task_Accept_00031505 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31505
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "吕布之死";
	elseif task:GetTaskSubmitNpc(31505) == npcId then
		if Task_Submit_00031505() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31505
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "吕布之死";
		elseif task:HasAcceptedTask(31505) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31505
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "吕布之死";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031505_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士我将夜摩的太师摩柯俱的藏身之处告密于你，又协助你将他打败，我已经成了一位彻头彻尾的叛徒，无论如何这是违背我做人的准则，但是继续帮助作恶多端的夜摩盟也是我不能容忍自己的，我会以我的方式解决这个事情，希望你能回去告诉善觉长者，我已经悔过了。";
	action.m_ActionMsg = "你这是要干什么……？";
	return action;
end

function Task_00031505_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "也许他是希望我能为他超度吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031505_step_table = {
		[1] = Task_00031505_step_01,
		[10] = Task_00031505_step_10,
		};

function Task_00031505_step(step)
	if Task_00031505_step_table[step] ~= nil then
		return Task_00031505_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031505_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031505() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(31505) then
		return false;
	end
	RunAutoBattleAction(player,4287,100);
	task:AddTaskStep(31505);

	return true;
end



--�ύ����
function Task_00031505_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	local fixReqGrid = package:GetItemUsedGrids(8930,1,1);
	if fixReqGrid > player:GetFreePackageSize() then
		player:sendMsgCode(2, 2013, 0);
		return false;
	end
	if not player:GetTaskMgr():SubmitTask(31505) then
		return false;
	end

	if IsEquipTypeId(8930) then
		for k = 1, 1 do
			package:AddEquip(8930, 1);
		end
	else 
		package:AddItem(8930,1,1);
	end

	player:AddExp(180000);
	player:getCoin(156000);
	player:getTael(50);
	return true;
end

--��������
function Task_00031505_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31505);
end
