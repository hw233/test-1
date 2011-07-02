--����Ľ�������
function Task_Accept_00000019()
	local player = GetPlayer();
	if player:GetLev() < 17 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(19) or task:HasCompletedTask(19) or task:HasSubmitedTask(19) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(18) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(18) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000019()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 17 then
		return false;
	end
	if task:HasAcceptedTask(19) or task:HasCompletedTask(19) or task:HasSubmitedTask(19) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(18) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(18) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000019()
	if GetPlayer():GetTaskMgr():HasCompletedTask(19) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000019(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(19) == npcId and Task_Accept_00000019 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 19
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "云灵山除妖";
	elseif task:GetTaskSubmitNpc(19) == npcId then
		if Task_Submit_00000019() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 19
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "云灵山除妖";
		elseif task:HasAcceptedTask(19) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 19
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "云灵山除妖";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000019_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "蛇妖确实比较厉害，不过我早已准备了应付之法，特地从黄山的餐霞大师那里借来金鳞蜈蚣，它可是蛇妖的克星。消灭这只蛇妖不在话下，<name>你这就和我们一起去吧。";
	action.m_ActionMsg = "晚辈正好也想去见识一下蛇妖的厉害。";
	return action;
end

function Task_00000019_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "只是可惜这只金鳞蜈蚣了，竟然和蛇妖同归于尽。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000019_step_table = {
		[1] = Task_00000019_step_01,
		[10] = Task_00000019_step_10,
		};

function Task_00000019_step(step)
	if Task_00000019_step_table[step] ~= nil then
		return Task_00000019_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000019_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000019() then
		return false;
	end
	if not task:AcceptTask(19) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000019_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(19) then
		return false;
	end


	player:AddExp(3333);
	return true;
end

--��������
function Task_00000019_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(19);
end
