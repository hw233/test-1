--����Ľ�������
function Task_Accept_00062204()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62204) or task:HasCompletedTask(62204) or task:HasSubmitedTask(62204) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00062204()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(62204) or task:HasCompletedTask(62204) or task:HasSubmitedTask(62204) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00062204()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62204) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00062204(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62204) == npcId and Task_Accept_00062204 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62204
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "侦查焚骨窟";
	elseif task:GetTaskSubmitNpc(62204) == npcId then
		if Task_Submit_00062204() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62204
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "侦查焚骨窟";
		elseif task:HasAcceptedTask(62204) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62204
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "侦查焚骨窟";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00062204_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."，你听说过焚骨窟这个地方吗？太恐怖了，我们很多兄弟去了就没再回来。你能帮我去侦查一下那里吗？";
	action.m_ActionMsg = "这个没问题，你等我的消息。";
	return action;
end

function Task_00062204_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你一定要小心那里的怪物。\n";
	action.m_ActionMsg = "放心，那些家伙不是我的对手。";
	return action;
end

function Task_00062204_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我十分佩服你的勇气！";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062204_step_table = {
		[1] = Task_00062204_step_01,
		[2] = Task_00062204_step_02,
		[10] = Task_00062204_step_10,
		};

function Task_00062204_step(step)
	if Task_00062204_step_table[step] ~= nil then
		return Task_00062204_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00062204_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062204() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(62204) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00062204_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(62204) then
		return false;
	end


	player:AddExp(10000);
	player:getCoin(20000);
	player:getTael(200);
	return true;
end

--��������
function Task_00062204_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(62204);
end
