--����Ľ�������
function Task_Accept_00060502()
	local player = GetPlayer();
	if player:GetLev() < 38 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60502) or task:HasCompletedTask(60502) or task:HasSubmitedTask(60502) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60501) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60501) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060502()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 38 then
		return false;
	end
	if task:HasAcceptedTask(60502) or task:HasCompletedTask(60502) or task:HasSubmitedTask(60502) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60501) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60501) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00060502()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60502) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060502(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60502) == npcId and Task_Accept_00060502 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60502
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "看望帮主父亲";
	elseif task:GetTaskSubmitNpc(60502) == npcId then
		if Task_Submit_00060502() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60502
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "看望帮主父亲";
		elseif task:HasAcceptedTask(60502) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60502
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "看望帮主父亲";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060502_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你还记得曾经介绍你，从阿兰若山谷来到东山城找我的那个守墓老人吗？";
	action.m_ActionMsg = "嗯，我记得他，是个奇怪的老头。";
	return action;
end

function Task_00060502_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "其实他也不是那么的奇怪，那个守墓人是我的父亲，可是许多年前的那场天魔之战实在太惨烈了，甚至惹伤了我的父亲，他认为生命不该承受这样的毁灭，所以这些年来，他就一直独自一人守着那万人墓。我一直想去看看他老人家，不过一时也忙不过来，我做了这兽皮垫，希望你能帮我送给他。";
	action.m_ActionMsg = "天哪，原来那守墓人是帮主的父亲啊。";
	return action;
end

function Task_00060502_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这可是上等的好皮毛做的垫子啊，以后可以坐的舒服一些了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00060502_step_table = {
		[1] = Task_00060502_step_01,
		[2] = Task_00060502_step_02,
		[10] = Task_00060502_step_10,
		};

function Task_00060502_step(step)
	if Task_00060502_step_table[step] ~= nil then
		return Task_00060502_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060502_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060502() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15007, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(60502) then
		return false;
	end
	task:AddTaskStep(60502);
	return true;
end



--�ύ����
function Task_00060502_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15007,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(60502) then
		return false;
	end

	package:DelItemAll(15007,1);

	player:AddExp(7000);
	player:getCoin(10000);
	return true;
end

--��������
function Task_00060502_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15007,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60502);
end
