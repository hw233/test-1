--����Ľ�������
function Task_Accept_00000079()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(79) or task:HasCompletedTask(79) or task:HasSubmitedTask(79) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(78) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(78) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(78) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000079()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(79) or task:HasCompletedTask(79) or task:HasSubmitedTask(79) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(78) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(78) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(78) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000079()
	if GetPlayer():GetTaskMgr():HasCompletedTask(79) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000079(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(79) == npcId and Task_Accept_00000079 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 79
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "妖化村民";
	elseif task:GetTaskSubmitNpc(79) == npcId then
		if Task_Submit_00000079() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 79
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "妖化村民";
		elseif task:HasAcceptedTask(79) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 79
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "妖化村民";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000079_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "那陈圩请来的妖人姚开江用妖术控制部分村民要打通鱼神洞旧道，这些被妖法控制的人变得力大无穷，全没人性，戴家场的村民肯定不是他们的对手。";
	action.m_ActionMsg = "这种妖术听起来似乎很厉害。";
	return action;
end

function Task_00000079_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那处旧道就在离鱼神洞不远的一处山坳，你速速赶去将这些妖化的村民们消灭，迟恐有变。";
	action.m_ActionMsg = "我这就去将他们消灭。";
	return action;
end

function Task_00000079_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那姚开江的妖法厉害，要想个办法对付才好。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000079_step_table = {
		[1] = Task_00000079_step_01,
		[2] = Task_00000079_step_02,
		[10] = Task_00000079_step_10,
		};

function Task_00000079_step(step)
	if Task_00000079_step_table[step] ~= nil then
		return Task_00000079_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000079_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000079() then
		return false;
	end
	if not task:AcceptTask(79) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000079_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(79) then
		return false;
	end


	player:AddExp(12000);
	return true;
end

--��������
function Task_00000079_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(79);
end
