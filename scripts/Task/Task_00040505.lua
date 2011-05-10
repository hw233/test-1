--����Ľ�������
function Task_Accept_00040505()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40505) or task:HasCompletedTask(40505) or task:HasSubmitedTask(40505) then
		return false;
	end
	if not task:HasSubmitedTask(40504) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040505()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(40505) or task:HasCompletedTask(40505) or task:HasSubmitedTask(40505) then
		return false;
	end
	if not task:HasSubmitedTask(40504) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040505()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40505) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040505(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40505) == npcId and Task_Accept_00040505 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40505
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "提炼自我";
	elseif task:GetTaskSubmitNpc(40505) == npcId then
		if Task_Submit_00040505() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40505
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "提炼自我";
		elseif task:HasAcceptedTask(40505) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40505
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "提炼自我";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040505_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，我等你很久了。";
	action.m_ActionMsg = "大师，你能帮我强化我的装备吗？我想变得更强大。 ";
	return action;
end

function Task_00040505_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "当然可以，首先你可以分解身上的一件装备，记得要用上分解保护符，它能帮助你分解成功。然后强化身上的一件装备，记得要用上强化保护符，它能防止强化回落。";
	action.m_ActionMsg = "那我们现在就开始吧，我已经等不及了。";
	return action;
end

function Task_00040505_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你找到了提升自己的好方法。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040505_step_table = {
		[1] = Task_00040505_step_01,
		[2] = Task_00040505_step_02,
		[10] = Task_00040505_step_10,
		};

function Task_00040505_step(step)
	if Task_00040505_step_table[step] ~= nil then
		return Task_00040505_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040505_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040505() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40505) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00040505_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40505) then
		return false;
	end


	player:AddExp(2000);
	player:getCoin(10000);
	return true;
end

--��������
function Task_00040505_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40505);
end
