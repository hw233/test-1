--����Ľ�������
function Task_Accept_00000052()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(52) or task:HasCompletedTask(52) or task:HasSubmitedTask(52) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(77) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(77) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(77) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000052()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(52) or task:HasCompletedTask(52) or task:HasSubmitedTask(52) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(77) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(77) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(77) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000052()
	if GetPlayer():GetTaskMgr():HasCompletedTask(52) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000052(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(52) == npcId and Task_Accept_00000052 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 52
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "返回戴家场";
	elseif task:GetTaskSubmitNpc(52) == npcId then
		if Task_Submit_00000052() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 52
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "返回戴家场";
		elseif task:HasAcceptedTask(52) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 52
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "返回戴家场";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000052_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我本来只是想帮助大哥，所以来吕村探探虚实，谁知却被那个妖道撞见，可怜我一身武艺没有施展之处，被他的妖术擒住，若不是少侠搭救，恐怕我凶多吉少。少侠为了救我不知道有没耽搁为大哥送信之事？还是请少侠回戴家场和大家商量对付吕村恶徒之事吧，我休息片刻就好。";
	action.m_ActionMsg = "那姑娘保重，我就先走了。";
	return action;
end

function Task_00000052_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "听到湘英安全无恙，我就放心了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000052_step_table = {
		[1] = Task_00000052_step_01,
		[10] = Task_00000052_step_10,
		};

function Task_00000052_step(step)
	if Task_00000052_step_table[step] ~= nil then
		return Task_00000052_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000052_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000052() then
		return false;
	end
	if not task:AcceptTask(52) then
		return false;
	end
	task:AddTaskStep(52);
	return true;
end



--�ύ����
function Task_00000052_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(52) then
		return false;
	end


	player:AddExp(18000);
	return true;
end

--��������
function Task_00000052_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(52);
end
