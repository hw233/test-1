--����Ľ�������
function Task_Accept_00050401()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50401) or task:HasCompletedTask(50401) or task:HasSubmitedTask(50401) then
		return false;
	end
	if not task:HasSubmitedTask(50303) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050401()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(50401) or task:HasCompletedTask(50401) or task:HasSubmitedTask(50401) then
		return false;
	end
	if not task:HasSubmitedTask(50303) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050401()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50401) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050401(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50401) == npcId and Task_Accept_00050401 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50401
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "永生的试炼";
	elseif task:GetTaskSubmitNpc(50401) == npcId then
		if Task_Submit_00050401() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50401
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "永生的试炼";
		elseif task:HasAcceptedTask(50401) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50401
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "永生的试炼";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050401_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "天地无极，山海茫茫，勇士你也是相信永生的吧。会有这样的想法倒是一点都不奇怪，人人都会有这样的想法的，重要的是如何通过自己的经历真正了解到永生的真谛。\n";
	action.m_ActionMsg = "不是说有长生不老药这样的东西吗？";
	return action;
end

function Task_00050401_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "假如说永生可以通过长生不老的丹药实现，你可以去十宝山的十宝寺，找那位神奇的炼丹师显无意，天下没有他炼不出的丹药。";
	action.m_ActionMsg = "哦，显无意大师啊。";
	return action;
end

function Task_00050401_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "又是一位追求永生的勇士来找我了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050401_step_table = {
		[1] = Task_00050401_step_01,
		[2] = Task_00050401_step_02,
		[10] = Task_00050401_step_10,
		};

function Task_00050401_step(step)
	if Task_00050401_step_table[step] ~= nil then
		return Task_00050401_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050401_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050401() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50401) then
		return false;
	end
	task:AddTaskStep(50401);
	return true;
end



--�ύ����
function Task_00050401_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50401) then
		return false;
	end


	player:AddExp(1200);
	player:getCoin(1000);
	return true;
end

--��������
function Task_00050401_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50401);
end
