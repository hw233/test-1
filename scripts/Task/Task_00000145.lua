--����Ľ�������
function Task_Accept_00000145()
	local player = GetPlayer();
	if player:GetLev() < 65 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(145) or task:HasCompletedTask(145) or task:HasSubmitedTask(145) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000145()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 65 then
		return false;
	end
	if task:HasAcceptedTask(145) or task:HasCompletedTask(145) or task:HasSubmitedTask(145) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(144) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000145()
	if GetPlayer():GetTaskMgr():HasCompletedTask(145) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000145(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(145) == npcId and Task_Accept_00000145 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 145
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "妖道神鸟";
	elseif task:GetTaskSubmitNpc(145) == npcId then
		if Task_Submit_00000145() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 145
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "妖道神鸟";
		elseif task:HasAcceptedTask(145) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 145
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "妖道神鸟";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000145_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这三尸墓早在黄帝时代就已存在，墓中的两个僵尸妖术非常厉害，后来他们偷盗了轩辕神墓中的法宝，并勾结了一个妖道叫钟敢在庐山为恶，那妖道钟敢手下一直神鸟天生凶禽，少侠你要小心对付才是。";
	action.m_ActionMsg = "一只扁毛畜生而已，待我去消灭它。";
	return action;
end

function Task_00000145_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."，少侠真是剑术高强，一身正气啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000145_step_table = {
		[1] = Task_00000145_step_01,
		[10] = Task_00000145_step_10,
		};

function Task_00000145_step(step)
	if Task_00000145_step_table[step] ~= nil then
		return Task_00000145_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000145_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000145() then
		return false;
	end
	if not task:AcceptTask(145) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000145_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(145) then
		return false;
	end


	player:AddExp(80000);
	return true;
end

--��������
function Task_00000145_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(145);
end
