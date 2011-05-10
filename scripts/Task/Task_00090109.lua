--����Ľ�������
function Task_Accept_00090109()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90109) or task:HasCompletedTask(90109) or task:HasSubmitedTask(90109) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90108) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90108) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090109()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90109) or task:HasCompletedTask(90109) or task:HasSubmitedTask(90109) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90108) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90108) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00090109()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90109) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090109(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90109) == npcId and Task_Accept_00090109 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90109
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "感激之情";
	elseif task:GetTaskSubmitNpc(90109) == npcId then
		if Task_Submit_00090109() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90109
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "感激之情";
		elseif task:HasAcceptedTask(90109) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90109
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "感激之情";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090109_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我终于可以清静的睡上一觉了。";
	action.m_ActionMsg = "那就好，我可以顺利的回去复命了。";
	return action;
end

function Task_00090109_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嗯，多谢你，勇士。也请你帮我向巫师表达谢意。";
	action.m_ActionMsg = "嗯,祝你有个好梦。";
	return action;
end

function Task_00090109_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "多谢你帮她。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090109_step_table = {
		[1] = Task_00090109_step_01,
		[2] = Task_00090109_step_02,
		[10] = Task_00090109_step_10,
		};

function Task_00090109_step(step)
	if Task_00090109_step_table[step] ~= nil then
		return Task_00090109_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090109_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090109() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90109) then
		return false;
	end
	task:AddTaskStep(90109);
	return true;
end



--�ύ����
function Task_00090109_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90109) then
		return false;
	end


	player:AddExp(575000);
	player:getCoin(306000);
	return true;
end

--��������
function Task_00090109_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90109);
end
