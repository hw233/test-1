--����Ľ�������
function Task_Accept_00080704()
	local player = GetPlayer();
	if player:GetLev() < 200 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(80704) or task:HasCompletedTask(80704) or task:HasSubmitedTask(80704) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80703) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80703) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00080704()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 200 then
		return false;
	end
	if task:HasAcceptedTask(80704) or task:HasCompletedTask(80704) or task:HasSubmitedTask(80704) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(80703) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(80703) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00080704()
	if GetPlayer():GetTaskMgr():HasCompletedTask(80704) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00080704(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(80704) == npcId and Task_Accept_00080704 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 80704
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "夜叉督军";
	elseif task:GetTaskSubmitNpc(80704) == npcId then
		if Task_Submit_00080704() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80704
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "夜叉督军";
		elseif task:HasAcceptedTask(80704) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 80704
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "夜叉督军";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00080704_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "可惜了那一大片的玄幻草。";
	action.m_ActionMsg = "是啊，我使用了巫师给我的那株玄幻草后整个草坡都被毁坏了。我想巫师是为了防止夜摩盟势力使用玄幻草来迷惑我们的将士，不得已才出此下策的。";
	return action;
end

function Task_00080704_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "现在勾搭夜摩盟的那些势力没了玄幻草，你便可以去杀死夜叉督军了。";
	action.m_ActionMsg = "终于可以下手了。";
	return action;
end

function Task_00080704_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "夜叉督军只是夜摩盟势力的一个小兵。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00080704_step_table = {
		[1] = Task_00080704_step_01,
		[2] = Task_00080704_step_02,
		[10] = Task_00080704_step_10,
		};

function Task_00080704_step(step)
	if Task_00080704_step_table[step] ~= nil then
		return Task_00080704_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00080704_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00080704() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(80704) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00080704_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(80704) then
		return false;
	end


	player:AddExp(675000);
	player:getCoin(314000);
	return true;
end

--��������
function Task_00080704_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(80704);
end
