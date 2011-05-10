--����Ľ�������
function Task_Accept_00040903()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 28 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40903) or task:HasCompletedTask(40903) or task:HasSubmitedTask(40903) then
		return false;
	end
	if not task:HasSubmitedTask(40902) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040903()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 28 then
		return false;
	end
	if task:HasAcceptedTask(40903) or task:HasCompletedTask(40903) or task:HasSubmitedTask(40903) then
		return false;
	end
	if not task:HasSubmitedTask(40902) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040903()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40903) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040903(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40903) == npcId and Task_Accept_00040903 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40903
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "心无杂念";
	elseif task:GetTaskSubmitNpc(40903) == npcId then
		if Task_Submit_00040903() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40903
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "心无杂念";
		elseif task:HasAcceptedTask(40903) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40903
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "心无杂念";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040903_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "为了检验你是否真的已经心无杂念，无视凡尘，一心追求力量的提升，我现在需要你去西来山谷，打败那里的僵尸射手，既为证明你自己，也为恢复观音山的平静安定。";
	action.m_ActionMsg = "我四处征战多年，那些僵尸根本不是我的对手。";
	return action;
end

function Task_00040903_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你做事情真是干净漂亮，这一点很让人欣赏。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040903_step_table = {
		[1] = Task_00040903_step_01,
		[10] = Task_00040903_step_10,
		};

function Task_00040903_step(step)
	if Task_00040903_step_table[step] ~= nil then
		return Task_00040903_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040903_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040903() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40903) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00040903_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40903) then
		return false;
	end


	player:AddExp(4500);
	player:getCoin(4900);
	return true;
end

--��������
function Task_00040903_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40903);
end
