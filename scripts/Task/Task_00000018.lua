--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000018()
	local player = GetPlayer();
	if player:GetLev() < 15 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(18) or task:HasCompletedTask(18) or task:HasSubmitedTask(18) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(17) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(17) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(17) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000018()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 15 then
		return false;
	end
	if task:HasAcceptedTask(18) or task:HasCompletedTask(18) or task:HasSubmitedTask(18) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(17) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(17) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(17) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000018()
	if GetPlayer():GetTaskMgr():HasCompletedTask(18) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000018(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(18) == npcId and Task_Accept_00000018 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 18
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "è›‡å¦–ä¹‹å®³";
	elseif task:GetTaskSubmitNpc(18) == npcId then
		if Task_Submit_00000018() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 18
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "è›‡å¦–ä¹‹å®³";
		elseif task:HasAcceptedTask(18) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 18
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "è›‡å¦–ä¹‹å®³";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000018_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¤šè°¢å°‘ä¾ é€è¯ä¹‹æ©ï¼Œé‚£è›‡å¦–ä¿®ç‚¼å¤šå¹´å·²ç»å¿«æˆæ°”å€™ï¼Œè¦å°½æ—©é“²é™¤ï¼Œå¦åˆ™ææ€•éš¾ä»¥å¯¹ä»˜ï¼Œæˆ‘æ¯’ä¼¤æœªæ„ˆï¼Œè¿˜è¯·å°‘ä¾ å»é€šçŸ¥ç™½äº‘å¤§å¸ˆã€‚";
	action.m_ActionMsg = "å¥½ï¼Œæˆ‘è¿™å°±å»";
	return action;
end

function Task_00000018_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ å¤é“çƒ­è‚ ï¼ŒçœŸæ˜¯å¤šè°¢äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000018_step_table = {
		[1] = Task_00000018_step_01,
		[10] = Task_00000018_step_10,
		};

function Task_00000018_step(step)
	if Task_00000018_step_table[step] ~= nil then
		return Task_00000018_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000018_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000018() then
		return false;
	end
	if not task:AcceptTask(18) then
		return false;
	end
	task:AddTaskStep(18);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000018_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(18) then
		return false;
	end


	player:AddExp(2222);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000018_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(18);
end
