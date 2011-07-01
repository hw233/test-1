--����Ľ�������
function Task_Accept_00000062()
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(62) or task:HasCompletedTask(62) or task:HasSubmitedTask(62) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60) then
			return false;
		end
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000062()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(62) or task:HasCompletedTask(62) or task:HasSubmitedTask(62) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(60) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(60) then
			return false;
		end
	end
	return true;
end


--�����������
function Task_Submit_00000062()
	if GetPlayer():GetTaskMgr():HasCompletedTask(62) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000062(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(62) == npcId and Task_Accept_00000062 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 62
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "阴魔大法";
	elseif task:GetTaskSubmitNpc(62) == npcId then
		if Task_Submit_00000062() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "阴魔大法";
		elseif task:HasAcceptedTask(62) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 62
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "阴魔大法";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000062_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 2;
	action.m_NpcMsg = "啊……啊！这位少侠快帮忙救我出来，我和主人见这妖道恶毒，与他争斗，却被他这个什么九天都篆阴魔妖法所困，昏昏欲睡，若破不了这个妖术，恐怕我们要遭了妖道的毒手！少侠救命。";
	action.m_ActionMsg = "大猩猩，真是有趣。";
	return action;
end

function Task_00000062_step_02()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "少侠再不出手，会说话的大猩猩就变成死猩猩了。";
	action.m_ActionMsg = "大猩猩莫慌，看我神功无敌，救你出来。";
	return action;
end

function Task_00000062_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "你才是大猩猩，你全家都是大猩猩！";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000062_step_table = {
		[1] = Task_00000062_step_01,
		[2] = Task_00000062_step_02,
		[10] = Task_00000062_step_10,
		};

function Task_00000062_step(step)
	if Task_00000062_step_table[step] ~= nil then
		return Task_00000062_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000062_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000062() then
		return false;
	end
	if not task:AcceptTask(62) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000062_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(62) then
		return false;
	end


	player:AddExp(5555);
	return true;
end

--��������
function Task_00000062_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(62);
end
