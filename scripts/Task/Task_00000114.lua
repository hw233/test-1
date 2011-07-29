--����Ľ�������
function Task_Accept_00000114()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(114) or task:HasCompletedTask(114) or task:HasSubmitedTask(114) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(113) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(113) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(113) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000114()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(114) or task:HasCompletedTask(114) or task:HasSubmitedTask(114) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(113) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(113) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(113) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000114()
	if GetPlayer():GetTaskMgr():HasCompletedTask(114) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000114(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(114) == npcId and Task_Accept_00000114 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 114
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "山魈之祸";
	elseif task:GetTaskSubmitNpc(114) == npcId then
		if Task_Submit_00000114() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 114
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "山魈之祸";
		elseif task:HasAcceptedTask(114) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 114
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "山魈之祸";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000114_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "我们一干兄弟被附近山魈祸害的好惨，这两只山魈专喜生啖动物脑髓，被他们害死的兄弟不知其数，"..GetPlayerName(GetPlayer()).."我看你也学过一些三脚猫的剑术，对付这两只山魈应该不成问题吧。";
	action.m_ActionMsg = "三脚猫的剑术恐怕对付不了这两只凶兽吧。";
	return action;
end

function Task_00000114_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "嘿嘿，"..GetPlayerName(GetPlayer()).."你大人有大量，别跟我一般见识。";
	action.m_ActionMsg = "大猩猩，你虽然是个废材，不过嘴皮的能耐还是不差得。";
	return action;
end

function Task_00000114_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."看不出你还有两把刷子吗。";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000114_step_table = {
		[1] = Task_00000114_step_01,
		[2] = Task_00000114_step_02,
		[10] = Task_00000114_step_10,
		};

function Task_00000114_step(step)
	if Task_00000114_step_table[step] ~= nil then
		return Task_00000114_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000114_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000114() then
		return false;
	end
	if not task:AcceptTask(114) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000114_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(114) then
		return false;
	end


	player:AddExp(4444);
	return true;
end

--��������
function Task_00000114_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(114);
end
