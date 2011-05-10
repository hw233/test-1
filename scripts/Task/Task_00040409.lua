--����Ľ�������
function Task_Accept_00040409()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 40 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(40409) or task:HasCompletedTask(40409) or task:HasSubmitedTask(40409) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00040409()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 40 then
		return false;
	end
	if task:HasAcceptedTask(40409) or task:HasCompletedTask(40409) or task:HasSubmitedTask(40409) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00040409()
	if GetPlayer():GetTaskMgr():HasCompletedTask(40409) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00040409(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(40409) == npcId and Task_Accept_00040409 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 40409
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "收服名将";
	elseif task:GetTaskSubmitNpc(40409) == npcId then
		if Task_Submit_00040409() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40409
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "收服名将";
		elseif task:HasAcceptedTask(40409) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 40409
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "收服名将";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00040409_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."，辛苦了，历练的路程是艰辛的，你需要给自己增强武力，去酒馆寻找你的将领吧，当然，你得花点钱，才能找到好将领。";
	action.m_ActionMsg = "好的，我这就去。";
	return action;
end

function Task_00040409_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你是我们天族最强壮的勇士。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00040409_step_table = {
		[1] = Task_00040409_step_01,
		[10] = Task_00040409_step_10,
		};

function Task_00040409_step(step)
	if Task_00040409_step_table[step] ~= nil then
		return Task_00040409_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00040409_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00040409() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(40409) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00040409_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(40409) then
		return false;
	end


	player:AddExp(3000);
	player:getCoin(10000);
	player:getTael(120);
	return true;
end

--��������
function Task_00040409_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(40409);
end
