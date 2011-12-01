--����Ľ�������
function Task_Accept_00000631()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(631) or task:HasCompletedTask(631) or task:HasSubmitedTask(631) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000631()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(631) or task:HasCompletedTask(631) or task:HasSubmitedTask(631) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000631()
	if GetPlayer():GetTaskMgr():HasCompletedTask(631) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000631(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(631) == npcId and Task_Accept_00000631 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 631
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "寒泉恶人";
	elseif task:GetTaskSubmitNpc(631) == npcId then
		if Task_Submit_00000631() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 631
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "寒泉恶人";
		elseif task:HasAcceptedTask(631) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 631
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "寒泉恶人";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000631_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "成都不远处的桂花山是一处美景，不过山中有五色云雾，寻常人轻易不得入内，我手下捕快捉拿一个恶徒金驼到桂花山，见那厮驾飞剑进入寒泉就再也不见踪迹，听说那里是个世外高人的隐居之所，所以只有拜托少侠你去跑一趟了。";
	action.m_ActionMsg = "为民除害是我辈本色，我去去就回。";
	return action;
end

function Task_00000631_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."你真是身手不凡啊。 ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000631_step_table = {
		[1] = Task_00000631_step_01,
		[10] = Task_00000631_step_10,
		};

function Task_00000631_step(step)
	if Task_00000631_step_table[step] ~= nil then
		return Task_00000631_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000631_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000631() then
		return false;
	end
	if not task:AcceptTask(631) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000631_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(631) then
		return false;
	end


	player:AddExp(3000);
	return true;
end

--��������
function Task_00000631_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(631);
end
