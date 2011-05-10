--����Ľ�������
function Task_Accept_00030105()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 32 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30105) or task:HasCompletedTask(30105) or task:HasSubmitedTask(30105) then
		return false;
	end
	if not task:HasSubmitedTask(30104) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030105()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 32 then
		return false;
	end
	if task:HasAcceptedTask(30105) or task:HasCompletedTask(30105) or task:HasSubmitedTask(30105) then
		return false;
	end
	if not task:HasSubmitedTask(30104) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00030105()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30105) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030105(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30105) == npcId and Task_Accept_00030105 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30105
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "边防情况";
	elseif task:GetTaskSubmitNpc(30105) == npcId then
		if Task_Submit_00030105() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30105
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "边防情况";
		elseif task:HasAcceptedTask(30105) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30105
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "边防情况";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030105_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "非常感谢您，天族的勇士，我们孤独的驻守在这里很久没有人来慰问我们了，您不仅带来了我们需要的物资，还带来了击退魔族复仇者的好消息，很让我们感动，更加增强了我们守卫边防的信心。";
	action.m_ActionMsg = "不必这么客气。";
	return action;
end

function Task_00030105_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "我们这些驻守边防的士兵，很多都是曾经在天魔之战中家族受到伤害的人，我们背负家族的仇恨来到这里，就是为了带给家人安定的生活。边防的生活很寂寞和单调，不过您的到来，给我们带动了很大的士气。";
	action.m_ActionMsg = "我很高兴能帮到你们";
	return action;
end

function Task_00030105_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "放心吧勇士，防守这里就是我们的责任。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030105_step_table = {
		[1] = Task_00030105_step_01,
		[2] = Task_00030105_step_02,
		[10] = Task_00030105_step_10,
		};

function Task_00030105_step(step)
	if Task_00030105_step_table[step] ~= nil then
		return Task_00030105_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030105_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030105() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(30105) then
		return false;
	end
	task:AddTaskStep(30105);
	return true;
end



--�ύ����
function Task_00030105_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(30105) then
		return false;
	end


	player:AddExp(4500);
	player:getCoin(5900);
	return true;
end

--��������
function Task_00030105_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30105);
end
