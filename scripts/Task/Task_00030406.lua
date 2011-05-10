--����Ľ�������
function Task_Accept_00030406()
	local player = GetPlayer();
	if player:GetLev() < 37 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30406) or task:HasCompletedTask(30406) or task:HasSubmitedTask(30406) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30405) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30405) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030406()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 37 then
		return false;
	end
	if task:HasAcceptedTask(30406) or task:HasCompletedTask(30406) or task:HasSubmitedTask(30406) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30405) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30405) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030406()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30406) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030406(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30406) == npcId and Task_Accept_00030406 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30406
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "康国历史";
	elseif task:GetTaskSubmitNpc(30406) == npcId then
		if Task_Submit_00030406() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30406
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "康国历史";
		elseif task:HasAcceptedTask(30406) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30406
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "康国历史";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030406_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "阿弥陀佛，这位年轻的勇士，怎么会到这片斑驳的遗迹里来的？";
	action.m_ActionMsg = "有人介绍我到这里来，向您了解东山派的历史。";
	return action;
end

function Task_00030406_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "康国已去，东山再起，老衲已出家，康国成回忆。吾曾经是康国的王子，因为夜叉王和摩可拿的袭击，使我国破人亡，吾之弟子如影誓与夜摩为敌，吾老矣，靠你们了。但是人不能总活在过去，离开这里，去山河郡的山河城会一会他们的城主吧，你会有所收获。";
	action.m_ActionMsg = "原来是这样。谢谢你，告辞了。";
	return action;
end

function Task_00030406_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "禅静大师难道还在康国遗迹那里吗？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030406_step_table = {
		[1] = Task_00030406_step_01,
		[2] = Task_00030406_step_02,
		[10] = Task_00030406_step_10,
		};

function Task_00030406_step(step)
	if Task_00030406_step_table[step] ~= nil then
		return Task_00030406_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030406_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030406() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30406) then
		return false;
	end
	task:AddTaskStep(30406);
	return true;
end



--�ύ����
function Task_00030406_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30406) then
		return false;
	end


	player:AddExp(6000);
	player:getCoin(9500);
	return true;
end

--��������
function Task_00030406_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30406);
end
