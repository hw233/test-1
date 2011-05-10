--����Ľ�������
function Task_Accept_00050102()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 8 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(50102) or task:HasCompletedTask(50102) or task:HasSubmitedTask(50102) then
		return false;
	end
	if not task:HasSubmitedTask(50101) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00050102()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 8 then
		return false;
	end
	if task:HasAcceptedTask(50102) or task:HasCompletedTask(50102) or task:HasSubmitedTask(50102) then
		return false;
	end
	if not task:HasSubmitedTask(50101) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00050102()
	if GetPlayer():GetTaskMgr():HasCompletedTask(50102) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00050102(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(50102) == npcId and Task_Accept_00050102 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 50102
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "不可开交";
	elseif task:GetTaskSubmitNpc(50102) == npcId then
		if Task_Submit_00050102() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50102
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "不可开交";
		elseif task:HasAcceptedTask(50102) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 50102
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "不可开交";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00050102_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "勇士，什么事情让你从王后那里来找我？";
	action.m_ActionMsg = "王后说想念儿子文隣，想去龙王庙看看，问你能不能陪她去。";
	return action;
end

function Task_00050102_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "唉没看我正忙着吗，文隣也是我的儿子，我也很想念他，不过总不能一有时间就跑去威海岛，现在形势那么危险，我们任何时候都不能放松警惕，龙王庙里原来文隣所有的东西都原封不动的保存，你去劝一下王后让她别去了。";
	action.m_ActionMsg = "思子心切是可以理解的，我现在去劝劝她吧。";
	return action;
end

function Task_00050102_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "王也太冷酷了，这都不陪我去。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00050102_step_table = {
		[1] = Task_00050102_step_01,
		[2] = Task_00050102_step_02,
		[10] = Task_00050102_step_10,
		};

function Task_00050102_step(step)
	if Task_00050102_step_table[step] ~= nil then
		return Task_00050102_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00050102_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00050102() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(50102) then
		return false;
	end
	task:AddTaskStep(50102);
	return true;
end



--�ύ����
function Task_00050102_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(50102) then
		return false;
	end


	player:AddExp(850);
	player:getCoin(550);
	return true;
end

--��������
function Task_00050102_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(50102);
end
