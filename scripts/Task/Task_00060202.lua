--����Ľ�������
function Task_Accept_00060202()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 31 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60202) or task:HasCompletedTask(60202) or task:HasSubmitedTask(60202) then
		return false;
	end
	if not task:HasSubmitedTask(60201) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060202()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 31 then
		return false;
	end
	if task:HasAcceptedTask(60202) or task:HasCompletedTask(60202) or task:HasSubmitedTask(60202) then
		return false;
	end
	if not task:HasSubmitedTask(60201) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00060202()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60202) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060202(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60202) == npcId and Task_Accept_00060202 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60202
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "进攻计划";
	elseif task:GetTaskSubmitNpc(60202) == npcId then
		if Task_Submit_00060202() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60202
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "进攻计划";
		elseif task:HasAcceptedTask(60202) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60202
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "进攻计划";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060202_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这些狼鬼族们果然盯上了我们的前线哨所，他们很清楚的查探到了，我们这里的传送门是通往地界的唯一道路，居然还在巨石堆安插了接应他们的巨型狼人，勇士我希望你能现在就帮我们把那些巨型狼人消灭掉，阻止他们的进攻计划。";
	action.m_ActionMsg = "魔界哪是他们那些低贱的怪物所能踏进来的。";
	return action;
end

function Task_00060202_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你做的太好了，相信这样可以给狼鬼族们沉重的打击。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060202_step_table = {
		[1] = Task_00060202_step_01,
		[10] = Task_00060202_step_10,
		};

function Task_00060202_step(step)
	if Task_00060202_step_table[step] ~= nil then
		return Task_00060202_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060202_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060202() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60202) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00060202_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(60202) then
		return false;
	end


	player:AddExp(4000);
	player:getCoin(5600);
	return true;
end

--��������
function Task_00060202_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60202);
end
