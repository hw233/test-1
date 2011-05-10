--����Ľ�������
function Task_Accept_00040401()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40401) or task:HasCompletedTask(40401) or task:HasSubmitedTask(40401) then
		return false;
	end
	if not task:HasSubmitedTask(40302) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040401()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(40401) or task:HasCompletedTask(40401) or task:HasSubmitedTask(40401) then
		return false;
	end
	if not task:HasSubmitedTask(40302) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040401()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40401) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040401(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40401) == npcId and Task_Accept_00040401 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40401
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "永生的试炼";
	elseif task:GetTaskSubmitNpc(40401) == npcId then
		if Task_Submit_00040401() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40401
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "永生的试炼";
		elseif task:HasAcceptedTask(40401) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40401
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
function Task_00040401_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "天地无极，山海茫茫，勇士你也是相信永生的吧。其实会有这样的想法倒是一点都不奇怪，人人都会有这样的想法的，重要的是如何通过自己的经历真正了解到永生的真谛。";
	action.m_ActionMsg = "不是说有长生不老药这样的东西吗？";
	return action;
end

function Task_00040401_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "假如说永生可以通过长生不老的丹药实现，你可以去胜神洲的汉山城，找那位神奇的炼丹师神指，天下没有他炼不出的丹药。";
	action.m_ActionMsg = "哦，神指大师啊。";
	return action;
end

function Task_00040401_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "又是一位追求永生的勇士来找我了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040401_step_table = {
		[1] = Task_00040401_step_01,
		[2] = Task_00040401_step_02,
		[10] = Task_00040401_step_10,
		};

function Task_00040401_step(step)
	if Task_00040401_step_table[step] ~= nil then
		return Task_00040401_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040401_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040401() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40401) then
		return false;
	end
	task:AddTaskStep(40401);
	return true;
end



--�ύ����
function Task_00040401_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40401) then
		return false;
	end


	player:AddExp(1200);
	player:getCoin(950);
	return true;
end

--��������
function Task_00040401_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40401);
end
