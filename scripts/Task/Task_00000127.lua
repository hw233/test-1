--����Ľ�������
function Task_Accept_00000127()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(127) or task:HasCompletedTask(127) or task:HasSubmitedTask(127) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(125) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(125) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(125) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000127()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(127) or task:HasCompletedTask(127) or task:HasSubmitedTask(127) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(125) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(125) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(125) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000127()
	if GetPlayer():GetTaskMgr():HasCompletedTask(127) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000127(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(127) == npcId and Task_Accept_00000127 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 127
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "日常活动";
	elseif task:GetTaskSubmitNpc(127) == npcId then
		if Task_Submit_00000127() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 127
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "日常活动";
		elseif task:HasAcceptedTask(127) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 127
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "日常活动";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000127_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."，我整日处理公文，繁忙异常后来叫师爷先将公文备案，让我批阅起来一目了然。这些日来我见少侠你奔波忙碌，是在过意不去，特地为你整理了一份日常活动案表，你可以仔细研究一番，我想一定可以为你节省不少时间和精力。";
	action.m_ActionMsg = "是吗，那多谢知府大人了。";
	return action;
end

function Task_00000127_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嗯，每天的活动都可以在日常里方便的查询，副本、阵图等等关卡也可以通过日常里的传送按钮方便快捷的到达。酒馆、书商的刷新时间也可以在日常里查看并且随时传送到地点。";
	action.m_ActionMsg = "好的，没事我会经常查看日常的。";
	return action;
end

function Task_00000127_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."，正是少年英杰啊，想当年我在你这么大的时候，还什么都不懂呢。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000127_step_table = {
		[1] = Task_00000127_step_01,
		[2] = Task_00000127_step_02,
		[10] = Task_00000127_step_10,
		};

function Task_00000127_step(step)
	if Task_00000127_step_table[step] ~= nil then
		return Task_00000127_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000127_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000127() then
		return false;
	end
	if not task:AcceptTask(127) then
		return false;
	end
	task:AddTaskStep(127);
	return true;
end



--�ύ����
function Task_00000127_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(127) then
		return false;
	end


	player:AddExp(5000);
	return true;
end

--��������
function Task_00000127_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(127);
end
