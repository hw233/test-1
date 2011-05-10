--����Ľ�������
function Task_Accept_00061001()
	local player = GetPlayer();
	if player:GetLev() < 48 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(61001) or task:HasCompletedTask(61001) or task:HasSubmitedTask(61001) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60803) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60903) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00061001()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 48 then
		return false;
	end
	if task:HasAcceptedTask(61001) or task:HasCompletedTask(61001) or task:HasSubmitedTask(61001) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60803) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60903) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00061001()
	if GetPlayer():GetTaskMgr():HasCompletedTask(61001) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00061001(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(61001) == npcId and Task_Accept_00061001 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 61001
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "所爱之人";
	elseif task:GetTaskSubmitNpc(61001) == npcId then
		if Task_Submit_00061001() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61001
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "所爱之人";
		elseif task:HasAcceptedTask(61001) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 61001
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "所爱之人";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00061001_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，姑娘紫衣对你的帮助千恩万谢，因为得了失语症，我已经很久没有说话了，没有人知道我的心情。";
	action.m_ActionMsg = "既然这样，盛一刀的条件也达成了，现在你跟我去天武那里吧。";
	return action;
end

function Task_00061001_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "可是，我想告诉你的是，在我得失语症的这些年来，一直照顾我的是盛一刀，而不是天武，我喜欢的是盛一刀，而且他照顾了我这么久，我已经完全离不开他了，你能否去跟天武说一下，他是否真的想夺人所爱。";
	action.m_ActionMsg = "原来姑娘爱的是盛一刀啊，那没办法了，我去找天武说说吧。";
	return action;
end

function Task_00061001_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你怎么独自一人来的，紫衣姑娘的病还没有治好吗？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00061001_step_table = {
		[1] = Task_00061001_step_01,
		[2] = Task_00061001_step_02,
		[10] = Task_00061001_step_10,
		};

function Task_00061001_step(step)
	if Task_00061001_step_table[step] ~= nil then
		return Task_00061001_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00061001_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00061001() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(61001) then
		return false;
	end
	task:AddTaskStep(61001);
	return true;
end



--�ύ����
function Task_00061001_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(61001) then
		return false;
	end


	player:AddExp(48000);
	player:getCoin(34000);
	return true;
end

--��������
function Task_00061001_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(61001);
end
