--����Ľ�������
function Task_Accept_00080502()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80502) or task:HasCompletedTask(80502) or task:HasSubmitedTask(80502) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80501) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80501) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080502()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80502) or task:HasCompletedTask(80502) or task:HasSubmitedTask(80502) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80501) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80501) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00080502()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80502) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080502(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80502) == npcId and Task_Accept_00080502 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80502
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "下马威";
	elseif task:GetTaskSubmitNpc(80502) == npcId then
		if Task_Submit_00080502() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80502
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "下马威";
		elseif task:HasAcceptedTask(80502) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80502
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "下马威";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080502_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "你是来帮我的吗？";
	action.m_ActionMsg = "是寒尧让我来的，你这有什么需要我帮忙的尽管说。";
	return action;
end

function Task_00080502_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你来得正好，帮我去侦查黑风寨，听说他们勾结夜摩盟，不知是真是假，顺便杀几个，给他们一个下马威。";
	action.m_ActionMsg = "打击夜摩盟我义不容辞。";
	return action;
end

function Task_00080502_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "黑风寨情况如何？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080502_step_table = {
		[1] = Task_00080502_step_01,
		[2] = Task_00080502_step_02,
		[10] = Task_00080502_step_10,
		};

function Task_00080502_step(step)
	if Task_00080502_step_table[step] ~= nil then
		return Task_00080502_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080502_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080502() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80502) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00080502_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80502) then
		return false;
	end


	player:AddExp(525000);
	player:getCoin(302000);
	return true;
end

--��������
function Task_00080502_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80502);
end
