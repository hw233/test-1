--����Ľ�������
function Task_Accept_00000101()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(101) or task:HasCompletedTask(101) or task:HasSubmitedTask(101) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000101()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(101) or task:HasCompletedTask(101) or task:HasSubmitedTask(101) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000101()
	if GetPlayer():GetTaskMgr():HasCompletedTask(101) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000101(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(101) == npcId and Task_Accept_00000101 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 101
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "教诲";
	elseif task:GetTaskSubmitNpc(101) == npcId then
		if Task_Submit_00000101() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 101
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "教诲";
		elseif task:HasAcceptedTask(101) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 101
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "教诲";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000101_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "<name>我们峨眉开府在即，也是天下正道的领袖，作为峨眉弟子一定要时刻自省其身。玄真子师伯早在我入门前就跟随你师祖长眉真人，受训颇严，今日你就去师伯处聆听教诲吧。";
	action.m_ActionMsg = "谨遵掌教之令。";
	return action;
end

function Task_00000101_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我看你心存正气，将来必然会将我派发扬光大。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000101_step_table = {
		[1] = Task_00000101_step_01,
		[10] = Task_00000101_step_10,
		};

function Task_00000101_step(step)
	if Task_00000101_step_table[step] ~= nil then
		return Task_00000101_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000101_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000101() then
		return false;
	end
	if not task:AcceptTask(101) then
		return false;
	end
	task:AddTaskStep(101);
	return true;
end



--�ύ����
function Task_00000101_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(101) then
		if DayTaskAward(0) then
			task:DayTaskSubmit(101);
			return true;
		end
	end

	return false;
end

--��������
function Task_00000101_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(101);
end
