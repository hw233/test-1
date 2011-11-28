--����Ľ�������
function Task_Accept_00000069()
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(69) or task:HasCompletedTask(69) or task:HasSubmitedTask(69) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(68) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(68) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(68) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000069()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(69) or task:HasCompletedTask(69) or task:HasSubmitedTask(69) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(68) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(68) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(68) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000069()
	if GetPlayer():GetTaskMgr():HasCompletedTask(69) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000069(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(69) == npcId and Task_Accept_00000069 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 69
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "戴家场之难";
	elseif task:GetTaskSubmitNpc(69) == npcId then
		if Task_Submit_00000069() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 69
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "戴家场之难";
		elseif task:HasAcceptedTask(69) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 69
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "戴家场之难";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000069_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我们这里叫葵花裕，那恶徒罗九和陈圩及吕村的妖人勾结为害我们戴家场，虽然他们人多势众，其实我们也不惧。只是最近他们请了一些妖人来助阵确让我们很是头痛。";
	action.m_ActionMsg = "我们是学剑之人，自然会帮助你们对付那些邪道妖人。";
	return action;
end

function Task_00000069_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这就太好了，戴大哥是我们戴家场的场主，有什么事你去和他商量吧。";
	action.m_ActionMsg = "好的。";
	return action;
end

function Task_00000069_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "有"..GetPlayerName(GetPlayer()).."你的帮助，对付这些恶人我们就大有把握了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000069_step_table = {
		[1] = Task_00000069_step_01,
		[2] = Task_00000069_step_02,
		[10] = Task_00000069_step_10,
		};

function Task_00000069_step(step)
	if Task_00000069_step_table[step] ~= nil then
		return Task_00000069_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000069_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000069() then
		return false;
	end
	if not task:AcceptTask(69) then
		return false;
	end
	task:AddTaskStep(69);
	return true;
end



--�ύ����
function Task_00000069_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(69) then
		return false;
	end


	player:AddExp(12000);
	return true;
end

--��������
function Task_00000069_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(69);
end
