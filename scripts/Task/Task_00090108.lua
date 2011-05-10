--����Ľ�������
function Task_Accept_00090108()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(90108) or task:HasCompletedTask(90108) or task:HasSubmitedTask(90108) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90107) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90107) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00090108()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(90108) or task:HasCompletedTask(90108) or task:HasSubmitedTask(90108) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(90107) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(90107) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00090108()
	if GetPlayer():GetTaskMgr():HasCompletedTask(90108) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00090108(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(90108) == npcId and Task_Accept_00090108 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 90108
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "流血的骷髅";
	elseif task:GetTaskSubmitNpc(90108) == npcId then
		if Task_Submit_00090108() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90108
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "流血的骷髅";
		elseif task:HasAcceptedTask(90108) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 90108
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "流血的骷髅";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00090108_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "真的是那些怪物在害我吗？";
	action.m_ActionMsg = "是的，不过是些小妖怪，你无需害怕的。";
	return action;
end

function Task_00090108_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你能帮我去杀了那些骷髅吗？那些动静总是让我无法安睡。";
	action.m_ActionMsg = "看来那些怪物着实影响到你了，那我去帮你解决吧。";
	return action;
end

function Task_00090108_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我终于可以清静的睡上一觉了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00090108_step_table = {
		[1] = Task_00090108_step_01,
		[2] = Task_00090108_step_02,
		[10] = Task_00090108_step_10,
		};

function Task_00090108_step(step)
	if Task_00090108_step_table[step] ~= nil then
		return Task_00090108_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00090108_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00090108() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(90108) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00090108_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(90108) then
		return false;
	end


	player:AddExp(575000);
	player:getCoin(306000);
	return true;
end

--��������
function Task_00090108_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(90108);
end
