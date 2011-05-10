--����Ľ�������
function Task_Accept_00040102()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 8 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40102) or task:HasCompletedTask(40102) or task:HasSubmitedTask(40102) then
		return false;
	end
	if not task:HasSubmitedTask(40101) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040102()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 8 then
		return false;
	end
	if task:HasAcceptedTask(40102) or task:HasCompletedTask(40102) or task:HasSubmitedTask(40102) then
		return false;
	end
	if not task:HasSubmitedTask(40101) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040102()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40102) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040102(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40102) == npcId and Task_Accept_00040102 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40102
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "目中无人";
	elseif task:GetTaskSubmitNpc(40102) == npcId then
		if Task_Submit_00040102() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40102
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "目中无人";
		elseif task:HasAcceptedTask(40102) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40102
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "目中无人";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040102_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，何事惊慌啊？";
	action.m_ActionMsg = "是悦意王妃让我来的，她想问你今晚能陪她赏月去吗？";
	return action;
end

function Task_00040102_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "哦，是悦意啊，她还真是麻烦，你看我现在正忙着呢。昨天刚有人禀报，有几个貌美如花的姑娘新来天庭，我打算今晚去看看呢。";
	action.m_ActionMsg = "哇，又有美女来到天庭啊。不过这么冷落悦意王妃，不好吧。";
	return action;
end

function Task_00040102_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "虽然我觉得没什么，不过既然你这么说了……";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040102_step_table = {
		[1] = Task_00040102_step_01,
		[2] = Task_00040102_step_02,
		[10] = Task_00040102_step_10,
		};

function Task_00040102_step(step)
	if Task_00040102_step_table[step] ~= nil then
		return Task_00040102_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040102_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040102() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40102) then
		return false;
	end
	task:AddTaskStep(40102);
	return true;
end



--�ύ����
function Task_00040102_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40102) then
		return false;
	end


	player:AddExp(800);
	player:getCoin(550);
	return true;
end

--��������
function Task_00040102_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40102);
end
