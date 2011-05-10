--����Ľ�������
function Task_Accept_00030505()
	local player = GetPlayer();
	if player:GetLev() < 42 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30505) or task:HasCompletedTask(30505) or task:HasSubmitedTask(30505) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30504) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30504) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030505()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 42 then
		return false;
	end
	if task:HasAcceptedTask(30505) or task:HasCompletedTask(30505) or task:HasSubmitedTask(30505) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30504) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30504) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030505()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30505) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030505(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30505) == npcId and Task_Accept_00030505 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30505
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "随形被救";
	elseif task:GetTaskSubmitNpc(30505) == npcId then
		if Task_Submit_00030505() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30505
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "随形被救";
		elseif task:HasAcceptedTask(30505) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30505
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "随形被救";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030505_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，很高兴再见到你，如影那里一切都还好吗？";
	action.m_ActionMsg = "他很好，而且其实一直惦记着你。帮主说有僵尸刺客可能会偷袭你。";
	return action;
end

function Task_00030505_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "是啊，所以我一直都没有放弃我们的感情。其实在你回来之前，已经有刺客潜过来了，可是就在我刚刚发现之时，有一个蒙面忍者神速的冲过来解决了他们，然后瞬间消失，我都没有看清楚是谁。";
	action.m_ActionMsg = "我才就是茅屋的那个隐士吧。";
	return action;
end

function Task_00030505_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "有的时候，人生还真是矛盾啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030505_step_table = {
		[1] = Task_00030505_step_01,
		[2] = Task_00030505_step_02,
		[10] = Task_00030505_step_10,
		};

function Task_00030505_step(step)
	if Task_00030505_step_table[step] ~= nil then
		return Task_00030505_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030505_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030505() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30505) then
		return false;
	end
	task:AddTaskStep(30505);
	return true;
end



--�ύ����
function Task_00030505_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30505) then
		return false;
	end


	player:AddExp(20000);
	player:getCoin(17500);
	return true;
end

--��������
function Task_00030505_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30505);
end
