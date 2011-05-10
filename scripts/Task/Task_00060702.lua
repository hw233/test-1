--����Ľ�������
function Task_Accept_00060702()
	local player = GetPlayer();
	if player:GetLev() < 43 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60702) or task:HasCompletedTask(60702) or task:HasSubmitedTask(60702) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60701) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60701) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060702()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 43 then
		return false;
	end
	if task:HasAcceptedTask(60702) or task:HasCompletedTask(60702) or task:HasSubmitedTask(60702) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60701) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60701) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00060702()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60702) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060702(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60702) == npcId and Task_Accept_00060702 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60702
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "情为何物";
	elseif task:GetTaskSubmitNpc(60702) == npcId then
		if Task_Submit_00060702() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60702
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "情为何物";
		elseif task:HasAcceptedTask(60702) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60702
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "情为何物";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060702_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，天武是不是没有跟你说我们的故事？那我来告诉你，天武年轻的时候紫衣姑娘曾经陪伴在他身边，可是他一心只知道忙着做生意，忽视了身边人的感受，后来紫衣跟我在一起了，他却一直耿耿于怀，给紫衣施加压力，以至于使紫衣患了失语症。";
	action.m_ActionMsg = "这个他没有直接告诉我，原来是三角恋的故事。";
	return action;
end

function Task_00060702_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "那天武会派你来，肯定是让你帮他将紫衣姑娘从我这里抢回去，我一定不会让你们得手的，除非你先打败我。";
	action.m_ActionMsg = "这可不是我要求的啊。";
	return action;
end

function Task_00060702_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我盛一刀居然被你打败了，难道紫衣就要这么离开了吗？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060702_step_table = {
		[1] = Task_00060702_step_01,
		[2] = Task_00060702_step_02,
		[10] = Task_00060702_step_10,
		};

function Task_00060702_step(step)
	if Task_00060702_step_table[step] ~= nil then
		return Task_00060702_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060702_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060702() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60702) then
		return false;
	end
	RunAutoBattleAction(player,4224,100);
	task:AddTaskStep(60702);

	return true;
end



--�ύ����
function Task_00060702_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(60702) then
		return false;
	end


	player:AddExp(25000);
	player:getCoin(19000);
	return true;
end

--��������
function Task_00060702_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60702);
end
