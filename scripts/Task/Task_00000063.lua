--����Ľ�������
function Task_Accept_00000063()
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(63) or task:HasCompletedTask(63) or task:HasSubmitedTask(63) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(62) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000063()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(63) or task:HasCompletedTask(63) or task:HasSubmitedTask(63) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(62) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000063()
	if GetPlayer():GetTaskMgr():HasCompletedTask(63) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000063(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(63) == npcId and Task_Accept_00000063 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 63
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "修炼邪功";
	elseif task:GetTaskSubmitNpc(63) == npcId then
		if Task_Submit_00000063() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 63
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "修炼邪功";
		elseif task:HasAcceptedTask(63) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 63
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "修炼邪功";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000063_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这个鬼道人奸猾得很，见我来了帮手就逃进了后洞丹室，他劫掠了不少的少年男女在极阴洞里修炼邪功，少侠可一定不要放过他。";
	action.m_ActionMsg = "真是聪明的大猩猩。";
	return action;
end

function Task_00000063_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "小样，看你得瑟的，会玩飞剑了不起啊……等你拿到紫青双剑再来显摆吧。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000063_step_table = {
		[1] = Task_00000063_step_01,
		[10] = Task_00000063_step_10,
		};

function Task_00000063_step(step)
	if Task_00000063_step_table[step] ~= nil then
		return Task_00000063_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000063_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000063() then
		return false;
	end
	if not task:AcceptTask(63) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000063_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(63) then
		return false;
	end


	player:AddExp(18000);
	return true;
end

--��������
function Task_00000063_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(63);
end
