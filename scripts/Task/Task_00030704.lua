--����Ľ�������
function Task_Accept_00030704()
	local player = GetPlayer();
	if player:GetLev() < 47 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30704) or task:HasCompletedTask(30704) or task:HasSubmitedTask(30704) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30703) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30703) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030704()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 47 then
		return false;
	end
	if task:HasAcceptedTask(30704) or task:HasCompletedTask(30704) or task:HasSubmitedTask(30704) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30703) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30703) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030704()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30704) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030704(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30704) == npcId and Task_Accept_00030704 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30704
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "初识镇邪派";
	elseif task:GetTaskSubmitNpc(30704) == npcId then
		if Task_Submit_00030704() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30704
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "初识镇邪派";
		elseif task:HasAcceptedTask(30704) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30704
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "初识镇邪派";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030704_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "这位是东山派的勇士吗？";
	action.m_ActionMsg = "不是的，我只是结交了东山派，与他们一起对付夜摩盟。";
	return action;
end

function Task_00030704_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "很好，我们也是以夜摩盟为敌的，如果真想结识我们镇邪派，不妨拿出点诚意来，去击败碎石阵那些厉害的邪毒水妖吧。";
	action.m_ActionMsg = "我总是在经受不同的考验，高手见多了，这些水妖根本不算什么。";
	return action;
end

function Task_00030704_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "真的是你自己打败那些邪毒怪物的？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030704_step_table = {
		[1] = Task_00030704_step_01,
		[2] = Task_00030704_step_02,
		[10] = Task_00030704_step_10,
		};

function Task_00030704_step(step)
	if Task_00030704_step_table[step] ~= nil then
		return Task_00030704_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030704_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030704() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30704) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00030704_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30704) then
		return false;
	end


	player:AddExp(44000);
	player:getCoin(31500);
	return true;
end

--��������
function Task_00030704_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30704);
end
