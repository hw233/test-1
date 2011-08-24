--����Ľ�������
function Task_Accept_00000086()
	local player = GetPlayer();
	if player:GetLev() < 45 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(86) or task:HasCompletedTask(86) or task:HasSubmitedTask(86) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(85) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(85) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(85) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000086()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 45 then
		return false;
	end
	if task:HasAcceptedTask(86) or task:HasCompletedTask(86) or task:HasSubmitedTask(86) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(85) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(85) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(85) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000086()
	if GetPlayer():GetTaskMgr():HasCompletedTask(86) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000086(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(86) == npcId and Task_Accept_00000086 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 86
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "绝顶妖禽";
	elseif task:GetTaskSubmitNpc(86) == npcId then
		if Task_Submit_00000086() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 86
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "绝顶妖禽";
		elseif task:HasAcceptedTask(86) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 86
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "绝顶妖禽";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000086_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "哎，我虽早年改邪归正，可是旁门之法毕竟不是正道，修炼过程中不慎走火入魔，如今半身不遂所以在此参枯禅。这里地窍中有一个雪魂珠乃万年冰雪精英所化，可以助我成道。";
	action.m_ActionMsg = "这个雪魂珠看来很神奇啊。";
	return action;
end

function Task_00000086_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这个珠子生在地窍之中，须得打通此山主峰玉京潭绝顶，直下七千三百丈才可取得。可是玉京潭绝顶上有只妖禽总是妨碍我做法，老身如今半身不遂，奈何不得这只扁毛畜生，还希望小友你帮我赶走它。";
	action.m_ActionMsg = "前辈，我这就去教训这只大鸟。";
	return action;
end

function Task_00000086_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "老身行动不便，多谢小友仗义出手。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000086_step_table = {
		[1] = Task_00000086_step_01,
		[2] = Task_00000086_step_02,
		[10] = Task_00000086_step_10,
		};

function Task_00000086_step(step)
	if Task_00000086_step_table[step] ~= nil then
		return Task_00000086_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000086_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000086() then
		return false;
	end
	if not task:AcceptTask(86) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000086_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(86) then
		return false;
	end


	player:AddExp(26000);
	return true;
end

--��������
function Task_00000086_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(86);
end
