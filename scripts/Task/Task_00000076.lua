--����Ľ�������
function Task_Accept_00000076()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(76) or task:HasCompletedTask(76) or task:HasSubmitedTask(76) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000076()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(76) or task:HasCompletedTask(76) or task:HasSubmitedTask(76) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(75) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000076()
	if GetPlayer():GetTaskMgr():HasCompletedTask(76) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000076(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(76) == npcId and Task_Accept_00000076 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 76
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "约战吕村";
	elseif task:GetTaskSubmitNpc(76) == npcId then
		if Task_Submit_00000076() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 76
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "约战吕村";
		elseif task:HasAcceptedTask(76) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 76
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "约战吕村";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000076_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "陈圩的恶徒欲借吕村的道打通鱼神洞，确实防不胜防，我看与少侠同来的人里有不少是剑仙一流，不如和他们约定光明正大的打一次擂台以了解恩怨。";
	action.m_ActionMsg = "这个办法不错。";
	return action;
end

function Task_00000076_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我这里有书信两封，这封信请少侠交给吕村的吕宪明，约定他们三日后来戴家场擂台一绝胜负。";
	action.m_ActionMsg = "我定会把信带到。";
	return action;
end

function Task_00000076_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "打擂台吗，难道我还怕了你们不成！";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000076_step_table = {
		[1] = Task_00000076_step_01,
		[2] = Task_00000076_step_02,
		[10] = Task_00000076_step_10,
		};

function Task_00000076_step(step)
	if Task_00000076_step_table[step] ~= nil then
		return Task_00000076_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000076_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000076() then
		return false;
	end
	if not task:AcceptTask(76) then
		return false;
	end
	task:AddTaskStep(76);
	return true;
end



--�ύ����
function Task_00000076_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(76) then
		return false;
	end


	player:AddExp(12000);
	return true;
end

--��������
function Task_00000076_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(76);
end
