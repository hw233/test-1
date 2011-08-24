--����Ľ�������
function Task_Accept_00000098()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(98) or task:HasCompletedTask(98) or task:HasSubmitedTask(98) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(97) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(97) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(97) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000098()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(98) or task:HasCompletedTask(98) or task:HasSubmitedTask(98) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(97) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(97) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(97) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000098()
	if GetPlayer():GetTaskMgr():HasCompletedTask(98) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000098(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(98) == npcId and Task_Accept_00000098 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 98
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "赤发寨主";
	elseif task:GetTaskSubmitNpc(98) == npcId then
		if Task_Submit_00000098() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 98
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "赤发寨主";
		elseif task:HasAcceptedTask(98) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 98
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "赤发寨主";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000098_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这赤发寨主虽是左派中人，但之前也未作恶太多，我听说他不知从哪里学会一种魔功，修炼的心性大变，行事也越发邪恶。如今之计只有将他尽早铲除，若待他魔功有成，那就大大的不妙了。";
	action.m_ActionMsg = "师你说的有理。";
	return action;
end

function Task_00000098_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "拔除了姑婆岭的这个毒瘤，我也可以安心潜修了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000098_step_table = {
		[1] = Task_00000098_step_01,
		[10] = Task_00000098_step_10,
		};

function Task_00000098_step(step)
	if Task_00000098_step_table[step] ~= nil then
		return Task_00000098_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000098_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000098() then
		return false;
	end
	if not task:AcceptTask(98) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000098_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(98) then
		return false;
	end


	player:AddExp(20000);
	return true;
end

--��������
function Task_00000098_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(98);
end
