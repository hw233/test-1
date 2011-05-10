--����Ľ�������
function Task_Accept_00050101()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 8 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50101) or task:HasCompletedTask(50101) or task:HasSubmitedTask(50101) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050101()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 8 then
		return false;
	end
	if task:HasAcceptedTask(50101) or task:HasCompletedTask(50101) or task:HasSubmitedTask(50101) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050101()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50101) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050101(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50101) == npcId and Task_Accept_00050101 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50101
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "王后的思念";
	elseif task:GetTaskSubmitNpc(50101) == npcId then
		if Task_Submit_00050101() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50101
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "王后的思念";
		elseif task:HasAcceptedTask(50101) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50101
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "王后的思念";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050101_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，你现在能否帮我个忙去找一下阿修罗？";
	action.m_ActionMsg = "王后的吩咐，定然没有问题。";
	return action;
end

function Task_00050101_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "能麻烦你帮我去找一下阿修罗，我最近非常想念我们的儿子文隣，问他愿不愿意陪我去一下文隣曾经修行学道的龙王庙。";
	action.m_ActionMsg = "王后，这样的事情其实你也可以自己去问的嘛。";
	return action;
end

function Task_00050101_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "为什么是王后让你来的？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050101_step_table = {
		[1] = Task_00050101_step_01,
		[2] = Task_00050101_step_02,
		[10] = Task_00050101_step_10,
		};

function Task_00050101_step(step)
	if Task_00050101_step_table[step] ~= nil then
		return Task_00050101_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050101_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050101() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50101) then
		return false;
	end
	task:AddTaskStep(50101);
	return true;
end



--�ύ����
function Task_00050101_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50101) then
		return false;
	end


	player:AddExp(850);
	player:getCoin(500);
	return true;
end

--��������
function Task_00050101_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50101);
end
