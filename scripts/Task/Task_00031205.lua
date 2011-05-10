--����Ľ�������
function Task_Accept_00031205()
	local player = GetPlayer();
	if player:GetLev() < 59 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31205) or task:HasCompletedTask(31205) or task:HasSubmitedTask(31205) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31204) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31204) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00031205()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 59 then
		return false;
	end
	if task:HasAcceptedTask(31205) or task:HasCompletedTask(31205) or task:HasSubmitedTask(31205) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(31204) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(31204) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00031205()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31205) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00031205(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31205) == npcId and Task_Accept_00031205 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31205
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "打击摩睺";
	elseif task:GetTaskSubmitNpc(31205) == npcId then
		if Task_Submit_00031205() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31205
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "打击摩睺";
		elseif task:HasAcceptedTask(31205) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31205
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "打击摩睺";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00031205_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "勇士你真的是运气非常好，这难得一见的醒神雪莲都能被你找到，现在有了这醒神雪莲的保护，就不会再怕摩睺的黑暗魔法，去雪崖峰杀掉那里的绝尘冰妖，给摩睺沉重的打击吧。";
	action.m_ActionMsg = "终有一天，我们会消灭摩睺，让这天地间恢复和平。";
	return action;
end

function Task_00031205_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "终于打败了那摩睺，勇士你对抗夜摩盟的事迹会传遍整个山海大陆的。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00031205_step_table = {
		[1] = Task_00031205_step_01,
		[10] = Task_00031205_step_10,
		};

function Task_00031205_step(step)
	if Task_00031205_step_table[step] ~= nil then
		return Task_00031205_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00031205_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00031205() then
		return false;
	end
	local package = player:GetPackage();
	if package:AddItem(15067, 1, 1) == nil then
		return false;
	end
	if not task:AcceptTask(31205) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00031205_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();
	if package:GetItemNum(15067,1) < 1 then
		return false;
	end

	if not player:GetTaskMgr():SubmitTask(31205) then
		return false;
	end

	package:DelItemAll(15067,1);

	player:AddExp(100000);
	player:getCoin(100000);
	player:getTael(30);
	return true;
end

--��������
function Task_00031205_abandon()
	local package = GetPlayer():GetPackage();
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(31205);
end
