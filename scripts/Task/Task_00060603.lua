--����Ľ�������
function Task_Accept_00060603()
	local player = GetPlayer();
	if player:GetLev() < 41 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(60603) or task:HasCompletedTask(60603) or task:HasSubmitedTask(60603) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60602) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60602) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00060603()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 41 then
		return false;
	end
	if task:HasAcceptedTask(60603) or task:HasCompletedTask(60603) or task:HasSubmitedTask(60603) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60602) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60602) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00060603()
	if GetPlayer():GetTaskMgr():HasCompletedTask(60603) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00060603(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(60603) == npcId and Task_Accept_00060603 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60603
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "押镖回城";
	elseif task:GetTaskAcceptNpc(60603) == npcId and task:HasAcceptedTask(60603) then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 60603
		action.m_ActionToken = 3;
		action.m_ActionStep = 11;
		action.m_ActionMsg = "押镖回城";
	elseif task:GetTaskSubmitNpc(60603) == npcId then
		if Task_Submit_00060603() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60603
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "押镖回城";
		elseif task:HasAcceptedTask(60603) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 60603
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "押镖回城";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00060603_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "那么勇士，现在我要把这个押镖的任务交给你了，我会让我的助手推着一箱宝物，从这里走出黑森林，然后去山河城送给天武。你的任务就是一定要保证我的助手和这箱宝物毫发无损的送到天武那里。";
	action.m_ActionMsg = "明白了，交给我你就放一百个心吧。";
	return action;
end

function Task_00060603_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我可是这整个山海大陆最有名气的镖师，建立起这样的招牌不容易，你如果毁了我的信誉，可别怪我不客气。";
	action.m_ActionMsg = "哪儿有那么严重，那我现在出发了。";
	return action;
end

function Task_00060603_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士我就知道，找你来做这件事情肯定没错。";
	action.m_ActionMsg = "";
	return action;
end

function Task_00060603_step_11()
	RunConveyAction(GetPlayer(),60603);
end

local Task_00060603_step_table = {
		[1] = Task_00060603_step_01,
		[2] = Task_00060603_step_02,
		[10] = Task_00060603_step_10,
		[11] = Task_00060603_step_11,
		};

function Task_00060603_step(step)
	if Task_00060603_step_table[step] ~= nil then
		return Task_00060603_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00060603_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00060603() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(60603) then
		return false;
	end
	RunConveyAction(player,60603);
	return true;
end



--�ύ����
function Task_00060603_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(60603) then
		return false;
	end


	player:AddExp(12000);
	player:getCoin(15000);
	return true;
end

--��������
function Task_00060603_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(60603);
end
