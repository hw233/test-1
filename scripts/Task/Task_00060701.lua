--����Ľ�������
function Task_Accept_00060701()
	local player = GetPlayer();
	if player:GetLev() < 43 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60701) or task:HasCompletedTask(60701) or task:HasSubmitedTask(60701) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060701()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 43 then
		return false;
	end
	if task:HasAcceptedTask(60701) or task:HasCompletedTask(60701) or task:HasSubmitedTask(60701) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00060701()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60701) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060701(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60701) == npcId and Task_Accept_00060701 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60701
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "看望紫衣";
	elseif task:GetTaskSubmitNpc(60701) == npcId then
		if Task_Submit_00060701() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60701
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "看望紫衣";
		elseif task:HasAcceptedTask(60701) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60701
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "看望紫衣";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060701_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士，还有医官事情很不好意思要麻烦你了，当你经过回音山的回音殿的时候，能不能进去看一下那里的紫衣姑娘，现在是否一切都好，顺便跟哪里的盛一刀说说，问他能否将紫衣姑娘送到我的山河城来。";
	action.m_ActionMsg = "这是怎么回事？我不妨去帮你问问看。";
	return action;
end

function Task_00060701_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那天武居然还好意思让我把紫衣姑娘送去他那里。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060701_step_table = {
		[1] = Task_00060701_step_01,
		[10] = Task_00060701_step_10,
		};

function Task_00060701_step(step)
	if Task_00060701_step_table[step] ~= nil then
		return Task_00060701_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060701_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060701() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60701) then
		return false;
	end
	task:AddTaskStep(60701);
	return true;
end



--�ύ����
function Task_00060701_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(60701) then
		return false;
	end


	player:AddExp(25000);
	player:getCoin(19000);
	return true;
end

--��������
function Task_00060701_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60701);
end
