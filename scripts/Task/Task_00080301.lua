--����Ľ�������
function Task_Accept_00080301()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80301) or task:HasCompletedTask(80301) or (task:HasSubmitedTask(80301) and (GetSharpDay(task:GetTaskEndTime(80301)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080301()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(80301) or task:HasCompletedTask(80301) or (task:HasSubmitedTask(80301) and (GetSharpDay(task:GetTaskEndTime(80301)) == GetSharpDay(os.time()))) then 
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00080301()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80301) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080301(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80301) == npcId and Task_Accept_00080301 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80301
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "匹夫之责";
	elseif task:GetTaskSubmitNpc(80301) == npcId then
		if Task_Submit_00080301() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80301
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "匹夫之责";
		elseif task:HasAcceptedTask(80301) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80301
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "匹夫之责";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080301_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "战争即将开始，我命你出战。用你的力量去打败敌人，魔族将以你为荣。";
	action.m_ActionMsg = "生是魔族的人，死是魔族的魂。请王放心，此去必胜！";
	return action;
end

function Task_00080301_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "魔族以你为荣。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080301_step_table = {
		[1] = Task_00080301_step_01,
		[10] = Task_00080301_step_10,
		};

function Task_00080301_step(step)
	if Task_00080301_step_table[step] ~= nil then
		return Task_00080301_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080301_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080301() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80301) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00080301_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80301) then
		return false;
	end


	player:AddExp(25000);
	player:getTael(50);
	return true;
end

--��������
function Task_00080301_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80301);
end
