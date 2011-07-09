--����Ľ�������
function Task_Accept_00000060()
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60) or task:HasCompletedTask(60) or task:HasSubmitedTask(60) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(59) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(59) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(59) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000060()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(60) or task:HasCompletedTask(60) or task:HasSubmitedTask(60) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(59) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(59) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(59) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000060()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000060(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60) == npcId and Task_Accept_00000060 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "解救姥姥";
	elseif task:GetTaskSubmitNpc(60) == npcId then
		if Task_Submit_00000060() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "解救姥姥";
		elseif task:HasAcceptedTask(60) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "解救姥姥";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000060_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "方才我去五色云雾外祭拜师傅，见那金驼三人的师傅飞龙师太飞剑毁了师父法体，又飞出剑光攻击师父已出窍的元婴。如今师父危在旦夕，我一人不是那贼婆的对手，<name>还请你帮忙一起救我师父，赶走飞龙师太。";
	action.m_ActionMsg = "若兰师妹莫急，我这就去救令师。";
	return action;
end

function Task_00000060_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "<name>多谢你出手赶走了飞龙师太，如今吾师飞升应已无忧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000060_step_table = {
		[1] = Task_00000060_step_01,
		[10] = Task_00000060_step_10,
		};

function Task_00000060_step(step)
	if Task_00000060_step_table[step] ~= nil then
		return Task_00000060_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000060_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000060() then
		return false;
	end
	if not task:AcceptTask(60) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000060_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(60) then
		return false;
	end


	player:AddExp(5555);
	return true;
end

--��������
function Task_00000060_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(60);
end
