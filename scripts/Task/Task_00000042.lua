--����Ľ�������
function Task_Accept_00000042()
	local player = GetPlayer();
	if player:GetLev() < 11 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(42) or task:HasCompletedTask(42) or task:HasSubmitedTask(42) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(41) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(41) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(41) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000042()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 11 then
		return false;
	end
	if task:HasAcceptedTask(42) or task:HasCompletedTask(42) or task:HasSubmitedTask(42) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(41) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(41) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(41) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000042()
	if GetPlayer():GetTaskMgr():HasCompletedTask(42) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000042(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(42) == npcId and Task_Accept_00000042 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 42
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "书生之祸";
	elseif task:GetTaskSubmitNpc(42) == npcId then
		if Task_Submit_00000042() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 42
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "书生之祸";
		elseif task:HasAcceptedTask(42) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 42
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "书生之祸";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000042_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "几天前我曾在望江楼遇见一个仙根和资质都非常好的书生叫周云从，但看他们随行的书生印堂晦暗，面带死气，几乎都有杀身之祸。我因为有事耽搁迟了些天回来，回来发现他们一行去了慈云寺被那些邪派妖人抓住，恐怕有杀身之祸，你得赶快去慈云寺救周云从。";
	action.m_ActionMsg = "我这就去救出这些书生。";
	return action;
end

function Task_00000042_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "慢着，慈云寺妖人众多，你一个人去势孤力单肯定不是他们对手，不过据我这几天暗中观察所见，寺内的知客僧了一有向善之心，你可以去找他帮忙救出这些书生。";
	action.m_ActionMsg = "好的，我一定小心从事。";
	return action;
end

function Task_00000042_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我看少侠一脸正气，一定是正道中人。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000042_step_table = {
		[1] = Task_00000042_step_01,
		[2] = Task_00000042_step_02,
		[10] = Task_00000042_step_10,
		};

function Task_00000042_step(step)
	if Task_00000042_step_table[step] ~= nil then
		return Task_00000042_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000042_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000042() then
		return false;
	end
	if not task:AcceptTask(42) then
		return false;
	end
	task:AddTaskStep(42);
	return true;
end



--�ύ����
function Task_00000042_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(42) then
		return false;
	end


	player:AddExp(5000);
	return true;
end

--��������
function Task_00000042_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(42);
end
