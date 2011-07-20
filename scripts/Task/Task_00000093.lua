--����Ľ�������
function Task_Accept_00000093()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(93) or task:HasCompletedTask(93) or task:HasSubmitedTask(93) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(92) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(92) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(92) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000093()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(93) or task:HasCompletedTask(93) or task:HasSubmitedTask(93) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(92) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(92) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(92) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000093()
	if GetPlayer():GetTaskMgr():HasCompletedTask(93) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000093(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(93) == npcId and Task_Accept_00000093 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 93
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "凶相毕露";
	elseif task:GetTaskSubmitNpc(93) == npcId then
		if Task_Submit_00000093() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 93
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "凶相毕露";
		elseif task:HasAcceptedTask(93) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 93
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "凶相毕露";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000093_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "桀桀桀，你这个小辈，拜师是假，暗中谋算我是真吧，刚刚拔了九根子针我看你眼睛骨碌乱转是在找这根母针？老祖我若不是在慈云寺被极乐老贼暗算，如今只剩半截身躯，岂能让你们这些小辈得意！";
	action.m_ActionMsg = "没了下半身的老贼，你就乖乖认命吧……";
	return action;
end

function Task_00000093_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 3;
	action.m_NpcMsg = "桀桀桀，小辈你这口气还真大，老祖虽躯体残缺对付你还不是手到擒来，待我强占了你躯体，与老祖我合而为一，这么好的躯壳在你身上实在太浪费了，乖乖纳命来吧……桀桀桀。";
	action.m_ActionMsg = "上半截这么老，下半截这么年青，明显不匹配吧！";
	return action;
end

function Task_00000093_step_03()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你……你你……你个牙尖嘴滑的小辈……气煞我也！";
	action.m_ActionMsg = "你个贪心不足的老怪物……";
	return action;
end

function Task_00000093_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "小辈休要得意，老祖我有玄牝珠在手，元神分化，待我回到百蛮山重新找来新的躯体……到时候小辈再让我见到，定将你剥皮抽骨，碎尸万段。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000093_step_table = {
		[1] = Task_00000093_step_01,
		[2] = Task_00000093_step_02,
		[3] = Task_00000093_step_03,
		[10] = Task_00000093_step_10,
		};

function Task_00000093_step(step)
	if Task_00000093_step_table[step] ~= nil then
		return Task_00000093_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000093_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000093() then
		return false;
	end
	if not task:AcceptTask(93) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000093_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(93) then
		return false;
	end


	player:AddExp(30000);
	return true;
end

--��������
function Task_00000093_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(93);
end
