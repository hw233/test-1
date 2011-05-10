--����Ľ�������
function Task_Accept_00040305()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40305) or task:HasCompletedTask(40305) or task:HasSubmitedTask(40305) then
		return false;
	end
	if not task:HasSubmitedTask(40304) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040305()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(40305) or task:HasCompletedTask(40305) or task:HasSubmitedTask(40305) then
		return false;
	end
	if not task:HasSubmitedTask(40304) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040305()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40305) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040305(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40305) == npcId and Task_Accept_00040305 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40305
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "实力的较量";
	elseif task:GetTaskSubmitNpc(40305) == npcId then
		if Task_Submit_00040305() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40305
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "实力的较量";
		elseif task:HasAcceptedTask(40305) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40305
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "实力的较量";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040305_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "现在，你看到我们天族的力量有多么强大了吧。";
	action.m_ActionMsg = "是的，我还需要努力锻炼自己。";
	return action;
end

function Task_00040305_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "恩，不过经过这一次，我发现你进步很大。所以你接下来将再次去到竞技场，完成3次挑战，你将会成长更快的。";
	action.m_ActionMsg = "我会谨遵大人的教诲。";
	return action;
end

function Task_00040305_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "终有一天，你会成为我们天族最强大的勇士。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040305_step_table = {
		[1] = Task_00040305_step_01,
		[2] = Task_00040305_step_02,
		[10] = Task_00040305_step_10,
		};

function Task_00040305_step(step)
	if Task_00040305_step_table[step] ~= nil then
		return Task_00040305_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040305_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040305() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40305) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00040305_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40305) then
		return false;
	end


	player:AddExp(2000);
	player:getCoin(10000);
	player:getTael(120);
	return true;
end

--��������
function Task_00040305_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40305);
end
