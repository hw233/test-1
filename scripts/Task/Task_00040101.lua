--����Ľ�������
function Task_Accept_00040101()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 8 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40101) or task:HasCompletedTask(40101) or task:HasSubmitedTask(40101) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040101()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 8 then
		return false;
	end
	if task:HasAcceptedTask(40101) or task:HasCompletedTask(40101) or task:HasSubmitedTask(40101) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040101()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40101) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040101(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40101) == npcId and Task_Accept_00040101 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40101
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "忧郁的王妃";
	elseif task:GetTaskSubmitNpc(40101) == npcId then
		if Task_Submit_00040101() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40101
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "忧郁的王妃";
		elseif task:HasAcceptedTask(40101) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40101
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "忧郁的王妃";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040101_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，如果你现在有时间能帮我个忙吗？";
	action.m_ActionMsg = "您说吧。";
	return action;
end

function Task_00040101_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你能帮我去问一下我们的帝释天，今晚是月圆之夜，他愿不愿意来陪我去赏月呢？";
	action.m_ActionMsg = "王妃，不是我不愿意，这点小事您为什么不能自己去问呢？";
	return action;
end

function Task_00040101_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，你怎么是从王妃那里过来的？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040101_step_table = {
		[1] = Task_00040101_step_01,
		[2] = Task_00040101_step_02,
		[10] = Task_00040101_step_10,
		};

function Task_00040101_step(step)
	if Task_00040101_step_table[step] ~= nil then
		return Task_00040101_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040101_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040101() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40101) then
		return false;
	end
	task:AddTaskStep(40101);
	return true;
end



--�ύ����
function Task_00040101_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40101) then
		return false;
	end


	player:AddExp(800);
	player:getCoin(500);
	return true;
end

--��������
function Task_00040101_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40101);
end
