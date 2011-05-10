--����Ľ�������
function Task_Accept_00030903()
	local player = GetPlayer();
	if player:GetLev() < 54 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(30903) or task:HasCompletedTask(30903) or task:HasSubmitedTask(30903) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30902) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30902) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00030903()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 54 then
		return false;
	end
	if task:HasAcceptedTask(30903) or task:HasCompletedTask(30903) or task:HasSubmitedTask(30903) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30902) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30902) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00030903()
	if GetPlayer():GetTaskMgr():HasCompletedTask(30903) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00030903(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(30903) == npcId and Task_Accept_00030903 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 30903
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "恐吓";
	elseif task:GetTaskSubmitNpc(30903) == npcId then
		if Task_Submit_00030903() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30903
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "恐吓";
		elseif task:HasAcceptedTask(30903) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 30903
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "恐吓";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00030903_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士你在与石冰龙的战斗中，有没有注意到那个叫摩伽的？";
	action.m_ActionMsg = "那个叫摩伽的并没有帮助石冰龙战斗。";
	return action;
end

function Task_00030903_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "即使这样，我们还是不能放松警惕，我认为他就是摩可拿用来掩人耳目的替身，这里有一面巫毒镜，你现在带去在石冰窟的门口使用它，看是否有摩可拿的踪影，然后回来向我汇报。";
	action.m_ActionMsg = "我这就去让他现出原形。";
	return action;
end

function Task_00030903_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "在那边使用了巫毒镜，发现摩可拿的踪迹了吗？";
	action.m_ActionMsg = "";
	return action;
end

local Task_00030903_step_table = {
		[1] = Task_00030903_step_01,
		[2] = Task_00030903_step_02,
		[10] = Task_00030903_step_10,
		};

function Task_00030903_step(step)
	if Task_00030903_step_table[step] ~= nil then
		return Task_00030903_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00030903_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00030903() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15065, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(30903) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00030903_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15065,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(30903) then
		return false;
	end

	package:DelItemAll(15065,1);

	player:AddExp(72000);
	player:getCoin(64000);
	player:getTael(20);
	return true;
end

--��������
function Task_00030903_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15065,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(30903);
end
