--����Ľ�������
function Task_Accept_00040501()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40501) or task:HasCompletedTask(40501) or task:HasSubmitedTask(40501) then
		return false;
	end
	if not task:HasSubmitedTask(40407) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040501()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(40501) or task:HasCompletedTask(40501) or task:HasSubmitedTask(40501) then
		return false;
	end
	if not task:HasSubmitedTask(40407) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040501()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40501) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040501(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40501) == npcId and Task_Accept_00040501 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40501
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "风生水起";
	elseif task:GetTaskSubmitNpc(40501) == npcId then
		if Task_Submit_00040501() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40501
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "风生水起";
		elseif task:HasAcceptedTask(40501) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40501
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "风生水起";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040501_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你好，我正发愁找不到人帮忙呢，你就过来了，不管你认不认识我，我只是想说我本来不是现在你看到的这个样子的，我本来可是战无不胜的勇士，可是不知道怎么了，现在突然一点力气也使不出来了。";
	action.m_ActionMsg = "那我要怎么帮你呢？";
	return action;
end

function Task_00040501_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我想你应该认识胜神洲的汉山城的炼丹大师神指，把我的状况告诉他，问问他是否有什么处方能够改善我现在的情况。";
	action.m_ActionMsg = "神指大师我认识，我现在就去问问看。";
	return action;
end

function Task_00040501_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士很高兴又见到你了，是什么风又将你吹到汉山城来的？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040501_step_table = {
		[1] = Task_00040501_step_01,
		[2] = Task_00040501_step_02,
		[10] = Task_00040501_step_10,
		};

function Task_00040501_step(step)
	if Task_00040501_step_table[step] ~= nil then
		return Task_00040501_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040501_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040501() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40501) then
		return false;
	end
	task:AddTaskStep(40501);
	return true;
end



--�ύ����
function Task_00040501_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40501) then
		return false;
	end


	player:AddExp(1400);
	player:getCoin(1500);
	return true;
end

--��������
function Task_00040501_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40501);
end
