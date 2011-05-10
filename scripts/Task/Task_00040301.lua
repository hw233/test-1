--����Ľ�������
function Task_Accept_00040301()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 10 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40301) or task:HasCompletedTask(40301) or task:HasSubmitedTask(40301) then
		return false;
	end
	if not task:HasSubmitedTask(40105) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040301()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 10 then
		return false;
	end
	if task:HasAcceptedTask(40301) or task:HasCompletedTask(40301) or task:HasSubmitedTask(40301) then
		return false;
	end
	if not task:HasSubmitedTask(40105) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040301()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40301) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040301(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40301) == npcId and Task_Accept_00040301 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40301
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "决斗三回合";
	elseif task:GetTaskSubmitNpc(40301) == npcId then
		if Task_Submit_00040301() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40301
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "决斗三回合";
		elseif task:HasAcceptedTask(40301) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40301
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "决斗三回合";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040301_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士看你这意气风发的样子，想必对自己的实力很有自信了，有没有胆量跟我打一场？只要你能挺过我3个回合，就算你胜利。";
	action.m_ActionMsg = "这有什么问题，我从来就不知道怕的。";
	return action;
end

function Task_00040301_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "孺子可教啊，看来我没有看错。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040301_step_table = {
		[1] = Task_00040301_step_01,
		[10] = Task_00040301_step_10,
		};

function Task_00040301_step(step)
	if Task_00040301_step_table[step] ~= nil then
		return Task_00040301_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040301_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040301() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40301) then
		return false;
	end
	RunAutoBattleAction(player,4103,100);
	task:AddTaskStep(40301);

	return true;
end



--�ύ����
function Task_00040301_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40301) then
		return false;
	end


	player:AddExp(1000);
	player:getCoin(850);
	return true;
end

--��������
function Task_00040301_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40301);
end
