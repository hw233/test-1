--����Ľ�������
function Task_Accept_00000030()
	local player = GetPlayer();
	if player:GetLev() < 29 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30) or task:HasCompletedTask(30) or task:HasSubmitedTask(30) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(29) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(29) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000030()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 29 then
		return false;
	end
	if task:HasAcceptedTask(30) or task:HasCompletedTask(30) or task:HasSubmitedTask(30) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(29) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(29) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000030()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000030(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30) == npcId and Task_Accept_00000030 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "再会醉道人";
	elseif task:GetTaskSubmitNpc(30) == npcId then
		if Task_Submit_00000030() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "再会醉道人";
		elseif task:HasAcceptedTask(30) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "再会醉道人";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000030_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "虽然慈云寺大战在即，不过正邪双方都在邀请帮手，所以暂时是风平浪静的，你先去成都望江楼吧，醉道人师兄他一般都在那里饮酒。";
	action.m_ActionMsg = "多谢大师赠送的法宝，我这就去了。";
	return action;
end

function Task_00000030_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你小子运气不错嘛，玉清这件法宝虽然多年不用，不过也是一件降魔至宝呢。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000030_step_table = {
		[1] = Task_00000030_step_01,
		[10] = Task_00000030_step_10,
		};

function Task_00000030_step(step)
	if Task_00000030_step_table[step] ~= nil then
		return Task_00000030_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000030_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000030() then
		return false;
	end
	if not task:AcceptTask(30) then
		return false;
	end
	task:AddTaskStep(30);
	return true;
end



--�ύ����
function Task_00000030_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30) then
		return false;
	end


	player:AddExp(1111);
	return true;
end

--��������
function Task_00000030_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(30);
end
