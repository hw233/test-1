--����Ľ�������
function Task_Accept_00000004()
	local player = GetPlayer();
	if player:GetLev() < 4 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(4) or task:HasCompletedTask(4) or task:HasSubmitedTask(4) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(3) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(3) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(3) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000004()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 4 then
		return false;
	end
	if task:HasAcceptedTask(4) or task:HasCompletedTask(4) or task:HasSubmitedTask(4) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(3) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(3) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(3) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000004()
	if GetPlayer():GetTaskMgr():HasCompletedTask(4) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000004(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(4) == npcId and Task_Accept_00000004 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 4
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "文昌阁";
	elseif task:GetTaskSubmitNpc(4) == npcId then
		if Task_Submit_00000004() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 4
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "文昌阁";
		elseif task:HasAcceptedTask(4) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 4
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "文昌阁";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000004_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "唉，大好河山落入异族之手，令我心如死灰，现在只想与李宁大哥一起隐居峨嵋，不问世事。只是我先前教授的一群小童，略有天资，不想耽误了他们的前程，希望少侠去文昌阁找一下马湘，让他代我教导这些学生。";
	action.m_ActionMsg = "没问题，我这就去。";
	return action;
end

function Task_00000004_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嗯，我也挺喜欢这些乖巧聪明的童子，这件事就包在我身上了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000004_step_table = {
		[1] = Task_00000004_step_01,
		[10] = Task_00000004_step_10,
		};

function Task_00000004_step(step)
	if Task_00000004_step_table[step] ~= nil then
		return Task_00000004_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000004_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000004() then
		return false;
	end
	if not task:AcceptTask(4) then
		return false;
	end
	task:AddTaskStep(4);
	return true;
end



--�ύ����
function Task_00000004_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(4) then
		return false;
	end


	player:AddExp(1111);
	return true;
end

--��������
function Task_00000004_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(4);
end
