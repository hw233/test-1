--����Ľ�������
function Task_Accept_00020404()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 22 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(20404) or task:HasCompletedTask(20404) or task:HasSubmitedTask(20404) then
		return false;
	end
	if not task:HasSubmitedTask(20403) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00020404()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 22 then
		return false;
	end
	if task:HasAcceptedTask(20404) or task:HasCompletedTask(20404) or task:HasSubmitedTask(20404) then
		return false;
	end
	if not task:HasSubmitedTask(20403) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00020404()
	if GetPlayer():GetTaskMgr():HasCompletedTask(20404) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00020404(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(20404) == npcId and Task_Accept_00020404 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 20404
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "取回甘露";
	elseif task:GetTaskSubmitNpc(20404) == npcId then
		if Task_Submit_00020404() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20404
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "取回甘露";
		elseif task:HasAcceptedTask(20404) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 20404
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "取回甘露";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00020404_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "辛苦你了我的勇士，你是把库房里的甘露全部取过来的吗？怎么只有这些？似乎只有之前的一半啊，这是怎么回事？";
	action.m_ActionMsg = "我也不知道，我的确把全部的甘露都取过来了。";
	return action;
end

function Task_00020404_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这之中一定有问题，你现在马上去敦煌城找负责运输甘露的魔族使节问清楚情况。";
	action.m_ActionMsg = "我这就去找他问问是怎么回事。";
	return action;
end

function Task_00020404_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你来找我有什么事情吗？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00020404_step_table = {
		[1] = Task_00020404_step_01,
		[2] = Task_00020404_step_02,
		[10] = Task_00020404_step_10,
		};

function Task_00020404_step(step)
	if Task_00020404_step_table[step] ~= nil then
		return Task_00020404_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00020404_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00020404() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(20404) then
		return false;
	end
	task:AddTaskStep(20404);
	return true;
end



--�ύ����
function Task_00020404_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(20404) then
		return false;
	end


	player:AddExp(2200);
	player:getCoin(3500);
	return true;
end

--��������
function Task_00020404_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(20404);
end
