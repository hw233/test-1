--����Ľ�������
function Task_Accept_00000101()
	local player = GetPlayer();
	if player:GetLev() < 52 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(101) or task:HasCompletedTask(101) or task:HasSubmitedTask(101) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000101()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 52 then
		return false;
	end
	if task:HasAcceptedTask(101) or task:HasCompletedTask(101) or task:HasSubmitedTask(101) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000101()
	if GetPlayer():GetTaskMgr():HasCompletedTask(101) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000101(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(101) == npcId and Task_Accept_00000101 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 101
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "武当借剑";
	elseif task:GetTaskSubmitNpc(101) == npcId then
		if Task_Submit_00000101() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 101
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "武当借剑";
		elseif task:HasAcceptedTask(101) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 101
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "武当借剑";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000101_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "当年我炼制这九九八十一根玄女针，虽是犀利无比，可也有伤天和，如今劫难降至，只有兵解来免去大劫。可是这次兵解需要的飞剑只有武当半边大师的七口青牛剑才合适，不知道少侠可否帮我去武当山找半边借来这七口青牛剑？";
	action.m_ActionMsg = "武当半边大师性格怪异，我尽力而为吧。";
	return action;
end

function Task_00000101_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "贫尼已经知晓你的来意。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000101_step_table = {
		[1] = Task_00000101_step_01,
		[10] = Task_00000101_step_10,
		};

function Task_00000101_step(step)
	if Task_00000101_step_table[step] ~= nil then
		return Task_00000101_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000101_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000101() then
		return false;
	end
	if not task:AcceptTask(101) then
		return false;
	end
	task:AddTaskStep(101);
	return true;
end



--�ύ����
function Task_00000101_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(101) then
		return false;
	end


	player:AddExp(2222);
	return true;
end

--��������
function Task_00000101_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(101);
end
