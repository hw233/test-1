--����Ľ�������
function Task_Accept_00000017()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(17) or task:HasCompletedTask(17) or task:HasSubmitedTask(17) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(16) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(16) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000017()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(17) or task:HasCompletedTask(17) or task:HasSubmitedTask(17) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(16) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(16) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000017()
	if GetPlayer():GetTaskMgr():HasCompletedTask(17) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000017(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(17) == npcId and Task_Accept_00000017 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 17
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "救命灵药";
	elseif task:GetTaskSubmitNpc(17) == npcId then
		if Task_Submit_00000017() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 17
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "救命灵药";
		elseif task:HasAcceptedTask(17) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 17
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "救命灵药";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000017_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这附近有一座云灵山，山中有一只蛇妖为祸，我和大师正着手准备除掉蛇咬，脱不开身。我那徒儿施林方才探查蛇妖洞穴误中毒雾，正好少侠你可以为我送解毒药给他，就在郊外的农家茅屋处。";
	action.m_ActionMsg = "李前辈，我这就赶去。";
	return action;
end

function Task_00000017_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "<name>真是太感谢你了，这蛇妖的毒雾好生厉害。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000017_step_table = {
		[1] = Task_00000017_step_01,
		[10] = Task_00000017_step_10,
		};

function Task_00000017_step(step)
	if Task_00000017_step_table[step] ~= nil then
		return Task_00000017_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000017_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000017() then
		return false;
	end
	if not task:AcceptTask(17) then
		return false;
	end
	task:AddTaskStep(17);
	return true;
end



--�ύ����
function Task_00000017_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(17) then
		return false;
	end


	player:AddExp(1111);
	return true;
end

--��������
function Task_00000017_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(17);
end
