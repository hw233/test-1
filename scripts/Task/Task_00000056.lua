--����Ľ�������
function Task_Accept_00000056()
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(56) or task:HasCompletedTask(56) or task:HasSubmitedTask(56) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(55) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(55) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000056()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(56) or task:HasCompletedTask(56) or task:HasSubmitedTask(56) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(55) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(55) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000056()
	if GetPlayer():GetTaskMgr():HasCompletedTask(56) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000056(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(56) == npcId and Task_Accept_00000056 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 56
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "辟毒药草";
	elseif task:GetTaskSubmitNpc(56) == npcId then
		if Task_Submit_00000056() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 56
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "辟毒药草";
		elseif task:HasAcceptedTask(56) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 56
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "辟毒药草";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000056_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "乌风草生长在福仙潭底得一块毒石之上，那毒石周围十丈以内，发出一种黑氛毒雾，非常厉害。需要瘴疠石边的茎素草才可辟毒，你们去采一点回来，不过千万要小心石畔的毒蛙。";
	action.m_ActionMsg = "姥姥放心，我会小心的。";
	return action;
end

function Task_00000056_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "有了这些草辟毒，取乌风草就多几分把握了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000056_step_table = {
		[1] = Task_00000056_step_01,
		[10] = Task_00000056_step_10,
		};

function Task_00000056_step(step)
	if Task_00000056_step_table[step] ~= nil then
		return Task_00000056_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000056_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000056() then
		return false;
	end
	if not task:AcceptTask(56) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000056_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(56) then
		return false;
	end


	player:AddExp(11111);
	return true;
end

--��������
function Task_00000056_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(56);
end
