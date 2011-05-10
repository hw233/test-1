--����Ľ�������
function Task_Accept_00030905()
	local player = GetPlayer();
	if player:GetLev() < 55 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30905) or task:HasCompletedTask(30905) or task:HasSubmitedTask(30905) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30904) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30904) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030905()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 55 then
		return false;
	end
	if task:HasAcceptedTask(30905) or task:HasCompletedTask(30905) or task:HasSubmitedTask(30905) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30904) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30904) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030905()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30905) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030905(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30905) == npcId and Task_Accept_00030905 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30905
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "逼供摩伽";
	elseif task:GetTaskSubmitNpc(30905) == npcId then
		if Task_Submit_00030905() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30905
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "逼供摩伽";
		elseif task:HasAcceptedTask(30905) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30905
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "逼供摩伽";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030905_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你是谁？到底想干什么？";
	action.m_ActionMsg = "我是谁不重要，我就是想让你告诉我，摩可拿逃去哪里了？";
	return action;
end

function Task_00030905_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "啊！别打了，我招，他在您过来之前就已经从我这里逃走了，似乎是往西边去了……";
	action.m_ActionMsg = "（举起手中的武器）算你识相……";
	return action;
end

function Task_00030905_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "原来摩可拿已经往西逃跑了啊，看来我暂时也做不了什么了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030905_step_table = {
		[1] = Task_00030905_step_01,
		[2] = Task_00030905_step_02,
		[10] = Task_00030905_step_10,
		};

function Task_00030905_step(step)
	if Task_00030905_step_table[step] ~= nil then
		return Task_00030905_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030905_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030905() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30905) then
		return false;
	end
	task:AddTaskStep(30905);
	return true;
end



--�ύ����
function Task_00030905_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30905) then
		return false;
	end


	player:AddExp(76000);
	player:getCoin(70000);
	player:getTael(20);
	return true;
end

--��������
function Task_00030905_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30905);
end
