--����Ľ�������
function Task_Accept_00000165()
	local player = GetPlayer();
	if player:GetLev() < 80 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(165) or task:HasCompletedTask(165) or task:HasSubmitedTask(165) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(164) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(164) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(164) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000165()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 80 then
		return false;
	end
	if task:HasAcceptedTask(165) or task:HasCompletedTask(165) or task:HasSubmitedTask(165) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(164) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(164) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(164) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000165()
	if GetPlayer():GetTaskMgr():HasCompletedTask(165) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000165(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(165) == npcId and Task_Accept_00000165 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 165
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "元磁菁英";
	elseif task:GetTaskSubmitNpc(165) == npcId then
		if Task_Submit_00000165() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 165
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "元磁菁英";
		elseif task:HasAcceptedTask(165) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 165
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "元磁菁英";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000165_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这里地处极北，天寒地冻终年难见人迹，小友来此真是稀客呀，那易周昔年见我修行不易，特地引北极的元磁真力来助我修炼，化我横骨以便我早日修成人形。只是如今横骨已化的七七八八，但却在禁制里凝成元磁菁英，借那元磁真力对我有莫大的伤害，还希望小友能帮忙消灭掉这些菁英。";
	action.m_ActionMsg = "是吗，我这就去除掉元磁菁英。";
	return action;
end

function Task_00000165_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "老朽修炼多年，终于可以化身成人了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000165_step_table = {
		[1] = Task_00000165_step_01,
		[10] = Task_00000165_step_10,
		};

function Task_00000165_step(step)
	if Task_00000165_step_table[step] ~= nil then
		return Task_00000165_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000165_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000165() then
		return false;
	end
	if not task:AcceptTask(165) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000165_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(165) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--��������
function Task_00000165_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(165);
end
