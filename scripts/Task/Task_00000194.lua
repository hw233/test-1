--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000194()
	local player = GetPlayer();
	if player:GetLev() < 95 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(194) or task:HasCompletedTask(194) or task:HasSubmitedTask(194) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(193) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(193) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(193) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000194()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 95 then
		return false;
	end
	if task:HasAcceptedTask(194) or task:HasCompletedTask(194) or task:HasSubmitedTask(194) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(193) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(193) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(193) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000194()
	if GetPlayer():GetTaskMgr():HasCompletedTask(194) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000194(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(194) == npcId and Task_Accept_00000194 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 194
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "çº¢é¬¼è°·";
	elseif task:GetTaskSubmitNpc(194) == npcId then
		if Task_Submit_00000194() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 194
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "çº¢é¬¼è°·";
		elseif task:HasAcceptedTask(194) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 194
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "çº¢é¬¼è°·";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000194_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è´«å°¼åœ¨æ­¤éšå±…å·²ä¹…ï¼Œä¹Ÿäºé‚£çº¢é¬¼è°·çš„æ¯’é¾™å°Šè€…äº•æ°´ä¸çŠ¯æ²³æ°´ï¼Œä¸è¿‡æœ€è¿‘æ¯’é¾™å¤–å‡ºï¼Œä»–æ‰‹ä¸‹ä¸å°‘å¼Ÿå­åœ¨å–œé©¬æ‹‰é›…å±±ä¸ºæ‰€æ¬²ä¸ºå¾ˆæ˜¯ä¸å ªï¼Œå°‘ä¾ ä½ å‡ºå»å·¡è§†ä¸€ä¸‹ï¼Œé“²é™¤é‚£äº›ä¸ºéä½œæ­¹çš„æ¯’é¾™å¼Ÿå­ã€‚";
	action.m_ActionMsg = "æˆ‘è¿™å°±å»å‘¨é­å·¡é€»ä¸€ä¸‹ã€‚";
	return action;
end

function Task_00000194_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ çœŸæ˜¯ä¿®ä¸ºæ·±åšï¼Œå¤©èµ‹å¼‚ç¦€å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000194_step_table = {
		[1] = Task_00000194_step_01,
		[10] = Task_00000194_step_10,
		};

function Task_00000194_step(step)
	if Task_00000194_step_table[step] ~= nil then
		return Task_00000194_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000194_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000194() then
		return false;
	end
	if not task:AcceptTask(194) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000194_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(194) then
		return false;
	end


	player:AddExp(180000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000194_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(194);
end
