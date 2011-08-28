--����Ľ�������
function Task_Accept_00000050()
	local player = GetPlayer();
	if player:GetLev() < 32 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50) or task:HasCompletedTask(50) or task:HasSubmitedTask(50) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000050()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 32 then
		return false;
	end
	if task:HasAcceptedTask(50) or task:HasCompletedTask(50) or task:HasSubmitedTask(50) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(70) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000050()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000050(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50) == npcId and Task_Accept_00000050 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "决战之地";
	elseif task:GetTaskSubmitNpc(50) == npcId then
		if Task_Submit_00000050() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "决战之地";
		elseif task:HasAcceptedTask(50) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "决战之地";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000050_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "方才醉道人前辈去了慈云寺，与众妖人约定在魏家场展开决战，届时成都周遭的正道剑仙都回去魏家场，少侠你正好可以去助大家一臂之力，多诛杀几个邪派的妖人。";
	action.m_ActionMsg = "除恶扬善，我自然是义不容辞。";
	return action;
end

function Task_00000050_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "魏家场一战，邪派妖人被打得是丢盔弃甲啊，哈哈。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000050_step_table = {
		[1] = Task_00000050_step_01,
		[10] = Task_00000050_step_10,
		};

function Task_00000050_step(step)
	if Task_00000050_step_table[step] ~= nil then
		return Task_00000050_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000050_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000050() then
		return false;
	end
	if not task:AcceptTask(50) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000050_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50) then
		return false;
	end


	player:AddExp(13000);
	return true;
end

--��������
function Task_00000050_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(50);
end
