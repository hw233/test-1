--����Ľ�������
function Task_Accept_00050501()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50501) or task:HasCompletedTask(50501) or task:HasSubmitedTask(50501) then
		return false;
	end
	if not task:HasSubmitedTask(50407) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050501()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(50501) or task:HasCompletedTask(50501) or task:HasSubmitedTask(50501) then
		return false;
	end
	if not task:HasSubmitedTask(50407) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050501()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50501) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050501(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50501) == npcId and Task_Accept_00050501 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50501
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "风生水起";
	elseif task:GetTaskSubmitNpc(50501) == npcId then
		if Task_Submit_00050501() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50501
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "风生水起";
		elseif task:HasAcceptedTask(50501) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50501
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
function Task_00050501_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你好，我正发愁找不到人帮忙呢，你就过来了，不管你认不认识我，我只是想说我本来不是你现在看到的这个样子的，我本来可是战无不胜的勇士，可是不知道怎么了，现在突然一点力气也使不出来了。";
	action.m_ActionMsg = "我要怎么帮你呢？";
	return action;
end

function Task_00050501_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我想你应该认识十宝山的十宝寺的炼丹大师显无意，把我的状况告诉他，问问他是否有什么处方能够改善我现在的情况。";
	action.m_ActionMsg = "显无意大师我认识，我现在就去问问看。";
	return action;
end

function Task_00050501_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士很高兴又见到你了，是什么风又将你吹到我这里来的？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050501_step_table = {
		[1] = Task_00050501_step_01,
		[2] = Task_00050501_step_02,
		[10] = Task_00050501_step_10,
		};

function Task_00050501_step(step)
	if Task_00050501_step_table[step] ~= nil then
		return Task_00050501_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050501_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050501() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50501) then
		return false;
	end
	task:AddTaskStep(50501);
	return true;
end



--�ύ����
function Task_00050501_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50501) then
		return false;
	end


	player:AddExp(1400);
	player:getCoin(1600);
	return true;
end

--��������
function Task_00050501_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50501);
end
