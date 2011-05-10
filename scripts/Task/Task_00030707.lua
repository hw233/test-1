--����Ľ�������
function Task_Accept_00030707()
	local player = GetPlayer();
	if player:GetLev() < 49 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30707) or task:HasCompletedTask(30707) or task:HasSubmitedTask(30707) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30706) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30706) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030707()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 49 then
		return false;
	end
	if task:HasAcceptedTask(30707) or task:HasCompletedTask(30707) or task:HasSubmitedTask(30707) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30706) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30706) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030707()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30707) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030707(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30707) == npcId and Task_Accept_00030707 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30707
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "击败暴龙";
	elseif task:GetTaskSubmitNpc(30707) == npcId then
		if Task_Submit_00030707() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30707
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "击败暴龙";
		elseif task:HasAcceptedTask(30707) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30707
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "击败暴龙";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030707_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "说了这么多，我想你已经大概了解我们镇邪派的情况了，我们愿意结识所有与夜摩盟为敌的有识之士，我很希望你能跟我们站在同一阵线。";
	action.m_ActionMsg = "我当然愿意。";
	return action;
end

function Task_00030707_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我们现在还处在可能会受到夜摩盟袭击的危险之中，希望你能帮我们解决那些在尘封岗的强化暴龙，他们不是那么容易对付的，这让我们很是头疼。";
	action.m_ActionMsg = "没问题，包在我身上。";
	return action;
end

function Task_00030707_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你太厉害了，真是帮了我们一个大忙。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030707_step_table = {
		[1] = Task_00030707_step_01,
		[2] = Task_00030707_step_02,
		[10] = Task_00030707_step_10,
		};

function Task_00030707_step(step)
	if Task_00030707_step_table[step] ~= nil then
		return Task_00030707_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030707_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030707() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30707) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00030707_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30707) then
		return false;
	end


	player:AddExp(52000);
	player:getCoin(40000);
	player:getTael(40);
	return true;
end

--��������
function Task_00030707_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30707);
end
