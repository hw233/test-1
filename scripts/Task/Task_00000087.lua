--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000087()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(87) or task:HasCompletedTask(87) or task:HasSubmitedTask(87) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(86) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(86) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(86) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000087()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(87) or task:HasCompletedTask(87) or task:HasSubmitedTask(87) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(86) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(86) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(86) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000087()
	if GetPlayer():GetTaskMgr():HasCompletedTask(87) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000087(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(87) == npcId and Task_Accept_00000087 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 87
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é›ªé­‚ç ";
	elseif task:GetTaskSubmitNpc(87) == npcId then
		if Task_Submit_00000087() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 87
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é›ªé­‚ç ";
		elseif task:HasAcceptedTask(87) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 87
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é›ªé­‚ç ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000087_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ–¹æ‰æˆ‘å·²ç»ç”¨å½»åœ°ç¥é’ˆæ‰“é€šç‰äº¬æ½­ç»é¡¶ï¼Œåªä¸‹ä¸ƒåƒä¸‰ç™¾ä¸ˆåˆ°äº†åœ°çªï¼Œå°å‹åªéœ€å»åœ°çªä¸­å‡»è´¥ä¸‡å¹´å†°é›ªç²¾è‹±ï¼Œæˆ‘å³å¯åšæ³•å°†å®ƒæ‰€åŒ–å¾—é›ªé­‚ç å–æ¥ã€‚";
	action.m_ActionMsg = "å¬èµ·æ¥çœŸç„ã€‚";
	return action;
end

function Task_00000087_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æœ‰äº†è¿™ä¸ªé›ªé­‚ç ï¼Œæˆ‘éƒ‘å…«å§‘å‚æ¯ç¦…ç»ˆäºå¯ä»¥æˆåŠŸäº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000087_step_table = {
		[1] = Task_00000087_step_01,
		[10] = Task_00000087_step_10,
		};

function Task_00000087_step(step)
	if Task_00000087_step_table[step] ~= nil then
		return Task_00000087_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000087_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000087() then
		return false;
	end
	if not task:AcceptTask(87) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000087_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(87) then
		return false;
	end


	player:AddExp(2222);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000087_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(87);
end
