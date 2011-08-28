--����Ľ�������
function Task_Accept_00000518()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(518) or task:HasCompletedTask(518) or task:HasSubmitedTask(518) then
		return false;
	end
	return true;
end




-----�ɽ���������
function Task_Can_Accept_00000518()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(518) or task:HasCompletedTask(518) or task:HasSubmitedTask(518) then
		return false;
	end
	return true;
end


--�����������
function Task_Submit_00000518()
	if GetPlayer():GetTaskMgr():HasCompletedTask(518) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC����������ű�
---------------------------------------
function Task_00000518(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(518) == npcId and Task_Accept_00000518 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 518
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "巨大鳄鱼";
	elseif task:GetTaskSubmitNpc(518) == npcId then
		if Task_Submit_00000518() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 518
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "巨大鳄鱼";
		elseif task:HasAcceptedTask(518) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 518
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "巨大鳄鱼";
		end
	end
	return action;
end

-------------------------------------------------
--------���񽻻�����
-------------------------------------------------
function Task_00000518_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "成都不远处的桂花山是一处美景，不过山中有五色云雾，寻常人轻易不得入内，听说有个室外高人隐居在山中，布下了五色云雾。在桂花山里有一处福仙潭，潭附近有一些巨大的鳄鱼出没，伤害了无数过往行人商旅，"..GetPlayerName(GetPlayer()).."你速速去除掉这一害。";
	action.m_ActionMsg = "对付这些四脚蜥蜴，那还不简单，我去去就回。";
	return action;
end

function Task_00000518_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "这些毒鳄凶猛无比，伤害了无数过往行人商旅。 ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000518_step_table = {
		[1] = Task_00000518_step_01,
		[10] = Task_00000518_step_10,
		};

function Task_00000518_step(step)
	if Task_00000518_step_table[step] ~= nil then
		return Task_00000518_step_table[step]();
	end
	return ActionTable:Instance();
end

--��������
function Task_00000518_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000518() then
		return false;
	end
	if not task:AcceptTask(518) then
		return false;
	end
	return true;
end



--�ύ����
function Task_00000518_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(518) then
		return false;
	end


	player:AddExp(10000);
	return true;
end

--��������
function Task_00000518_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(518);
end
