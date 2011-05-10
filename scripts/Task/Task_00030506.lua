--����Ľ�������
function Task_Accept_00030506()
	local player = GetPlayer();
	if player:GetLev() < 42 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30506) or task:HasCompletedTask(30506) or task:HasSubmitedTask(30506) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30505) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30505) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030506()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 42 then
		return false;
	end
	if task:HasAcceptedTask(30506) or task:HasCompletedTask(30506) or task:HasSubmitedTask(30506) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30505) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30505) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030506()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30506) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030506(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30506) == npcId and Task_Accept_00030506 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30506
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "随形的感激";
	elseif task:GetTaskSubmitNpc(30506) == npcId then
		if Task_Submit_00030506() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30506
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "随形的感激";
		elseif task:HasAcceptedTask(30506) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30506
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "随形的感激";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030506_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "怪不得如影一心要打败夜摩盟，他们真的是越来越险恶了，居然都偷袭到我这里来了，幸好有人及时来救我。";
	action.m_ActionMsg = "要不要我去帮你问问那茅屋的隐士？";
	return action;
end

function Task_00030506_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "也好，那就麻烦勇士你跑一趟吧，如果是的话，还麻烦你代我表示感激，并送他一坛美酒吧，我真是欠他太多了。";
	action.m_ActionMsg = "你不用太担心，我们总有一天会将这帮恶魔打败的。";
	return action;
end

function Task_00030506_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "难道我给你打的武器有问题？勇士怎么又回来了啊？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030506_step_table = {
		[1] = Task_00030506_step_01,
		[2] = Task_00030506_step_02,
		[10] = Task_00030506_step_10,
		};

function Task_00030506_step(step)
	if Task_00030506_step_table[step] ~= nil then
		return Task_00030506_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030506_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030506() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15042, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(30506) then
		return false;
	end
	task:AddTaskStep(30506);
	return true;
end



--�ύ����
function Task_00030506_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15042,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(30506) then
		return false;
	end

	package:DelItemAll(15042,1);

	player:AddExp(20000);
	player:getCoin(17500);
	player:getTael(30);
	return true;
end

--��������
function Task_00030506_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15042,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30506);
end
