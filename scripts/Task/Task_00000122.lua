--����Ľ�������
function Task_Accept_00000122()
	local player = GetPlayer();
	if player:GetLev() < 46 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(122) or task:HasCompletedTask(122) or task:HasSubmitedTask(122) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000122()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 46 then
		return false;
	end
	if task:HasAcceptedTask(122) or task:HasCompletedTask(122) or task:HasSubmitedTask(122) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000122()
	if GetPlayer():GetTaskMgr():HasCompletedTask(122) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000122(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(122) == npcId and Task_Accept_00000122 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 122
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "青螺四魔";
	elseif task:GetTaskSubmitNpc(122) == npcId then
		if Task_Submit_00000122() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 122
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "青螺四魔";
		elseif task:HasAcceptedTask(122) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 122
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "青螺四魔";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000122_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "青螺魔宫的几个魔头奴役我们已久，尤其是那四魔伊红樱常常下山作恶，每次路过我们清远寺，寺内僧众轻则被打骂，重则丢失性命……我们是敢怒不敢言，希望少侠可以为我们除掉这个恶人。";
	action.m_ActionMsg = "你放心，我一定帮你报这个仇。";
	return action;
end

function Task_00000122_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "想不到终于得报此仇，真是谢谢"..GetPlayerName(GetPlayer()).."你了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000122_step_table = {
		[1] = Task_00000122_step_01,
		[10] = Task_00000122_step_10,
		};

function Task_00000122_step(step)
	if Task_00000122_step_table[step] ~= nil then
		return Task_00000122_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000122_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000122() then
		return false;
	end
	if not task:AcceptTask(122) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000122_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(122) then
		return false;
	end


	player:AddExp(35000);
	return true;
end

--��������
function Task_00000122_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(122);
end
