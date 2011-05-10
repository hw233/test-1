--����Ľ�������
function Task_Accept_00080504()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80504) or task:HasCompletedTask(80504) or task:HasSubmitedTask(80504) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80503) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80503) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080504()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80504) or task:HasCompletedTask(80504) or task:HasSubmitedTask(80504) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80503) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80503) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00080504()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80504) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080504(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80504) == npcId and Task_Accept_00080504 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80504
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "捷报";
	elseif task:GetTaskSubmitNpc(80504) == npcId then
		if Task_Submit_00080504() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80504
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "捷报";
		elseif task:HasAcceptedTask(80504) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80504
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "捷报";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080504_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这就是杀鸡儆猴，看那些冰妖和其他小怪还敢不敢勾结夜摩盟。";
	action.m_ActionMsg = "想不到夜摩盟的爪牙已遍布各处。";
	return action;
end

function Task_00080504_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "都是些虾兵蟹将不足为患，但我们也不能太过于轻敌。";
	action.m_ActionMsg = "我们现在得步步小心了。";
	return action;
end

function Task_00080504_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "形势不容乐观。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080504_step_table = {
		[1] = Task_00080504_step_01,
		[2] = Task_00080504_step_02,
		[10] = Task_00080504_step_10,
		};

function Task_00080504_step(step)
	if Task_00080504_step_table[step] ~= nil then
		return Task_00080504_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080504_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080504() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80504) then
		return false;
	end
	task:AddTaskStep(80504);
	return true;
end



--�ύ����
function Task_00080504_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80504) then
		return false;
	end


	player:AddExp(525000);
	player:getCoin(302000);
	return true;
end

--��������
function Task_00080504_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80504);
end
