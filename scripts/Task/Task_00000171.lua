--����Ľ�������
function Task_Accept_00000171()
	local player = GetPlayer();
	if player:GetLev() < 80 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(171) or task:HasCompletedTask(171) or task:HasSubmitedTask(171) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(170) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(170) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(170) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000171()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 80 then
		return false;
	end
	if task:HasAcceptedTask(171) or task:HasCompletedTask(171) or task:HasSubmitedTask(171) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(170) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(170) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(170) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000171()
	if GetPlayer():GetTaskMgr():HasCompletedTask(171) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000171(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(171) == npcId and Task_Accept_00000171 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 171
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "天涔海";
	elseif task:GetTaskSubmitNpc(171) == npcId then
		if Task_Submit_00000171() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 171
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "天涔海";
		elseif task:HasAcceptedTask(171) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 171
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "天涔海";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000171_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "多谢少侠通知，我这就收拾一下回去面见师傅，不过这天涔海里仍然有一些夜叉未除，请少侠帮助剿灭这些余孽吧。";
	action.m_ActionMsg = "嗯，我懂的。";
	return action;
end

function Task_00000171_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "多谢少侠帮助。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000171_step_table = {
		[1] = Task_00000171_step_01,
		[10] = Task_00000171_step_10,
		};

function Task_00000171_step(step)
	if Task_00000171_step_table[step] ~= nil then
		return Task_00000171_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000171_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000171() then
		return false;
	end
	if not task:AcceptTask(171) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000171_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(171) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--��������
function Task_00000171_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(171);
end
