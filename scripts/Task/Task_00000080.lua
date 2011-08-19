--����Ľ�������
function Task_Accept_00000080()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80) or task:HasCompletedTask(80) or task:HasSubmitedTask(80) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(79) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(79) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(79) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000080()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(80) or task:HasCompletedTask(80) or task:HasSubmitedTask(80) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(79) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(79) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(79) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000080()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000080(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80) == npcId and Task_Accept_00000080 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "破解妖法";
	elseif task:GetTaskSubmitNpc(80) == npcId then
		if Task_Submit_00000080() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "破解妖法";
		elseif task:HasAcceptedTask(80) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "破解妖法";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000080_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "姚开江的妖术颇为棘手，不过方才怪叫花凌浑前辈来过，他告诉我说姚开江的妖术全在他用元神练成的一条巨蛇，只要将这条巨蛇杀死，他的妖术自然可以破掉。";
	action.m_ActionMsg = "想不到他的妖术竟然有这样的弱点。";
	return action;
end

function Task_00000080_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嗯，凌浑前辈告诉我说姚开江常常在鱼神洞旧道那里练他的元神，你这就去守在那里，若见那毒蛇出来，迅速出手将它消灭。";
	action.m_ActionMsg = "我这就去。";
	return action;
end

function Task_00000080_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "山人姚开江的妖术一破，余者就不足为虑了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000080_step_table = {
		[1] = Task_00000080_step_01,
		[2] = Task_00000080_step_02,
		[10] = Task_00000080_step_10,
		};

function Task_00000080_step(step)
	if Task_00000080_step_table[step] ~= nil then
		return Task_00000080_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000080_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000080() then
		return false;
	end
	if not task:AcceptTask(80) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000080_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80) then
		return false;
	end


	player:AddExp(15000);
	return true;
end

--��������
function Task_00000080_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(80);
end
