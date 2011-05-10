--����Ľ�������
function Task_Accept_00030304()
	local player = GetPlayer();
	if player:GetLev() < 34 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30304) or task:HasCompletedTask(30304) or task:HasSubmitedTask(30304) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30303) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30303) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030304()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 34 then
		return false;
	end
	if task:HasAcceptedTask(30304) or task:HasCompletedTask(30304) or task:HasSubmitedTask(30304) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30303) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30303) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030304()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30304) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030304(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30304) == npcId and Task_Accept_00030304 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30304
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "结识东山派";
	elseif task:GetTaskSubmitNpc(30304) == npcId then
		if Task_Submit_00030304() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30304
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "结识东山派";
		elseif task:HasAcceptedTask(30304) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30304
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "结识东山派";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030304_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "年轻的勇士，虽然我们的接触不多，不过我觉得你待老朽还不错，是个有态度又有实力的后辈，我很希望你能结识东山派，虽然我一直觉得他们过于偏执了，但是他们都是很正直的人，不知道你愿不愿意啊。";
	action.m_ActionMsg = "我愿意结识更多的有识之士。";
	return action;
end

function Task_00030304_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那么你就离开这里，去王舍城废墟的东山城找他们的帮主如影，他如果知道你是从我们这里过去的，会很高兴接待你。";
	action.m_ActionMsg = "我相信能和他们成为战斗中的兄弟，后会有期了。";
	return action;
end

function Task_00030304_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士止步，这里不是随便什么人就可以进的。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030304_step_table = {
		[1] = Task_00030304_step_01,
		[2] = Task_00030304_step_02,
		[10] = Task_00030304_step_10,
		};

function Task_00030304_step(step)
	if Task_00030304_step_table[step] ~= nil then
		return Task_00030304_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030304_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030304() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30304) then
		return false;
	end
	task:AddTaskStep(30304);
	return true;
end



--�ύ����
function Task_00030304_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30304) then
		return false;
	end


	player:AddExp(5000);
	player:getCoin(6800);
	player:getTael(20);
	return true;
end

--��������
function Task_00030304_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30304);
end
