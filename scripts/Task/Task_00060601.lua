--����Ľ�������
function Task_Accept_00060601()
	local player = GetPlayer();
	if player:GetLev() < 41 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60601) or task:HasCompletedTask(60601) or task:HasSubmitedTask(60601) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060601()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 41 then
		return false;
	end
	if task:HasAcceptedTask(60601) or task:HasCompletedTask(60601) or task:HasSubmitedTask(60601) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00060601()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60601) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060601(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60601) == npcId and Task_Accept_00060601 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60601
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "拜访镖师";
	elseif task:GetTaskSubmitNpc(60601) == npcId then
		if Task_Submit_00060601() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60601
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "拜访镖师";
		elseif task:HasAcceptedTask(60601) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60601
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "拜访镖师";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060601_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，我已经听说了你在这山海大陆的许多事迹，我的朋友如影也告诉我你是一个值得信任的勇士，我现在正需要一个值得信任的人的帮忙，你愿意抽出时间给我吗？";
	action.m_ActionMsg = "城主客气了，既然都这么说了，我当然是会帮忙的了。";
	return action;
end

function Task_00060601_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我曾经拜托了黑森林的山海镖师，帮我押运一些东西，可是我现在临时没有人去接镖，现在只能麻烦你去黑森林找一下山海镖师，他会告诉你要做什么的。";
	action.m_ActionMsg = "好的，那我现在去黑森林了。";
	return action;
end

function Task_00060601_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "天武城主终于派人来了啊。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060601_step_table = {
		[1] = Task_00060601_step_01,
		[2] = Task_00060601_step_02,
		[10] = Task_00060601_step_10,
		};

function Task_00060601_step(step)
	if Task_00060601_step_table[step] ~= nil then
		return Task_00060601_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060601_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060601() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60601) then
		return false;
	end
	task:AddTaskStep(60601);
	return true;
end



--�ύ����
function Task_00060601_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(60601) then
		return false;
	end


	player:AddExp(12000);
	player:getCoin(15000);
	return true;
end

--��������
function Task_00060601_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60601);
end
