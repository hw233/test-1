--����Ľ�������
function Task_Accept_00040601()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 14 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40601) or task:HasCompletedTask(40601) or task:HasSubmitedTask(40601) then
		return false;
	end
	if not task:HasSubmitedTask(40503) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040601()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 14 then
		return false;
	end
	if task:HasAcceptedTask(40601) or task:HasCompletedTask(40601) or task:HasSubmitedTask(40601) then
		return false;
	end
	if not task:HasSubmitedTask(40503) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040601()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40601) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040601(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40601) == npcId and Task_Accept_00040601 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40601
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "找寻伍子胥";
	elseif task:GetTaskSubmitNpc(40601) == npcId then
		if Task_Submit_00040601() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40601
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "找寻伍子胥";
		elseif task:HasAcceptedTask(40601) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40601
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "找寻伍子胥";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040601_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，你可曾听说过一位叫伍子胥的人士啊？";
	action.m_ActionMsg = "似乎听说过。";
	return action;
end

function Task_00040601_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "听说他也在瞻部峰，我曾多次给他写信，却总是没有回音，你是否愿意帮我去找找他，我最近听说他遇到麻烦了，你能去看一下能否帮上什么忙。把我的信带去，他就会信任你的。";
	action.m_ActionMsg = "这是举手之劳，我这就替你去找他，但愿他是平安的。";
	return action;
end

function Task_00040601_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "终于在这寂静的地方见到人了。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040601_step_table = {
		[1] = Task_00040601_step_01,
		[2] = Task_00040601_step_02,
		[10] = Task_00040601_step_10,
		};

function Task_00040601_step(step)
	if Task_00040601_step_table[step] ~= nil then
		return Task_00040601_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040601_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040601() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15057, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(40601) then
		return false;
	end
	task:AddTaskStep(40601);
	return true;
end



--�ύ����
function Task_00040601_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15057,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(40601) then
		return false;
	end

	package:DelItemAll(15057,1);

	player:AddExp(1500);
	player:getCoin(1800);
	return true;
end

--��������
function Task_00040601_abandon()
	local package = GetPlayer():GetPackage();
	package:DelItemAll(15057,1);
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40601);
end
