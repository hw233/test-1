--����Ľ�������
function Task_Accept_00062102()
	local player = GetPlayer();
	if player:GetLev() < 73 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62102) or task:HasCompletedTask(62102) or task:HasSubmitedTask(62102) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62101) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62101) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00062102()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 73 then
		return false;
	end
	if task:HasAcceptedTask(62102) or task:HasCompletedTask(62102) or task:HasSubmitedTask(62102) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(62101) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(62101) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00062102()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62102) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00062102(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62102) == npcId and Task_Accept_00062102 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62102
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "活佛遗珠";
	elseif task:GetTaskSubmitNpc(62102) == npcId then
		if Task_Submit_00062102() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62102
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "活佛遗珠";
		elseif task:HasAcceptedTask(62102) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62102
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "活佛遗珠";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00062102_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "依鄙人所见，这串佛珠应该是一个成佛的人奉春华的，我听说他在成佛之前被一群沙化骷髅抢走了修行的佛珠，但是他佛心笃定，专心修炼，无暇追究，连佛祖也为之所动，赐他为活佛。";
	action.m_ActionMsg = "原来是活佛的佛珠啊。";
	return action;
end

function Task_00062102_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "可能这些怪物抢了他的佛珠却遗失在滑沙古城了，所以会被貂蝉发现。如果你愿意行善积德，就去腾格里荒漠的无边沙漠杀掉那些作恶的沙化骷髅。";
	action.m_ActionMsg = "这种事情对我来说很简单。";
	return action;
end

function Task_00062102_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这些怪物也是罪有应得。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00062102_step_table = {
		[1] = Task_00062102_step_01,
		[2] = Task_00062102_step_02,
		[10] = Task_00062102_step_10,
		};

function Task_00062102_step(step)
	if Task_00062102_step_table[step] ~= nil then
		return Task_00062102_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00062102_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00062102() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(62102) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00062102_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(62102) then
		return false;
	end


	player:AddExp(280000);
	player:getCoin(259000);
	return true;
end

--��������
function Task_00062102_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(62102);
end
