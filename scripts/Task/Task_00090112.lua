--����Ľ�������
function Task_Accept_00090112()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90112) or task:HasCompletedTask(90112) or task:HasSubmitedTask(90112) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90111) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90111) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090112()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90112) or task:HasCompletedTask(90112) or task:HasSubmitedTask(90112) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90111) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90111) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00090112()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90112) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090112(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90112) == npcId and Task_Accept_00090112 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90112
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "职守阵营";
	elseif task:GetTaskSubmitNpc(90112) == npcId then
		if Task_Submit_00090112() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90112
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "职守阵营";
		elseif task:HasAcceptedTask(90112) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90112
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "职守阵营";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090112_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "原来你是来劝将的。";
	action.m_ActionMsg = "正是，在下奉勒勤将军命，特来邀请吕先生协助作战。";
	return action;
end

function Task_00090112_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 3;
	action.m_NpcMsg = "呵呵，他还挺会想的。很抱歉，我必须镇守此处，哪儿也不能去，我也不想去。";
	action.m_ActionMsg = "这叫在下如何是好。";
	return action;
end

function Task_00090112_step_03()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你替我去回了那勒勤吧。";
	action.m_ActionMsg = "看来勒将军要失望了。";
	return action;
end

function Task_00090112_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "他果真是不愿来。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090112_step_table = {
		[1] = Task_00090112_step_01,
		[2] = Task_00090112_step_02,
		[3] = Task_00090112_step_03,
		[10] = Task_00090112_step_10,
		};

function Task_00090112_step(step)
	if Task_00090112_step_table[step] ~= nil then
		return Task_00090112_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090112_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090112() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90112) then
		return false;
	end
	task:AddTaskStep(90112);
	return true;
end



--�ύ����
function Task_00090112_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90112) then
		return false;
	end


	player:AddExp(600000);
	player:getCoin(308000);
	return true;
end

--��������
function Task_00090112_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90112);
end
