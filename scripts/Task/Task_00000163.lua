--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000163()
	local player = GetPlayer();
	if player:GetLev() < 75 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(163) or task:HasCompletedTask(163) or task:HasSubmitedTask(163) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(162) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(162) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(162) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000163()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 75 then
		return false;
	end
	if task:HasAcceptedTask(163) or task:HasCompletedTask(163) or task:HasSubmitedTask(163) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(162) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(162) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(162) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000163()
	if GetPlayer():GetTaskMgr():HasCompletedTask(163) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000163(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(163) == npcId and Task_Accept_00000163 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 163
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¤©ç—´ä¸Šäºº";
	elseif task:GetTaskSubmitNpc(163) == npcId then
		if Task_Submit_00000163() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 163
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¤©ç—´ä¸Šäºº";
		elseif task:HasAcceptedTask(163) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 163
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¤©ç—´ä¸Šäºº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000163_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™é“œæ¤°å²›çš„åœ°ç£çœŸæ°”å¿’è¿‡å‰å®³ï¼Œç‰¢ç‰¢å¸ä½çˆ·çˆ·çš„è¾Ÿé­”ç¥æ¢­ï¼Œè®©æˆ‘ä»¬æ— å¤„å¯éï¼Œæ‰€å¹¸å°‘ä¾ æ–¹æ‰å‡ºæ‰‹æ•™è®­é“œæ¤°å²›å¼Ÿå­ï¼Œå‡ºæ‰‹ä¸é‡ï¼Œæ²¡ä»€ä¹ˆä¼¤äº¡ã€‚å¦‚ä»Šä¹‹è®¡åªèƒ½å»å¤©ç—´è€å„¿å¤„ä¹–ä¹–è®¤é”™ï¼Œå¸Œæœ›ä»–èƒ½çœ‹åœ¨çˆ·çˆ·çš„é¢å­ä¸Šæ”¾è¿‡æˆ‘ä¿©â€¦â€¦";
	action.m_ActionMsg = "çŸ¥é“è®¤é”™å°±å¥½ã€‚";
	return action;
end

function Task_00000163_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç½¢äº†ç½¢äº†ï¼Œçœ‹åœ¨æ˜“å‘¨çš„é¢å­ä¸Šæˆ‘å°±ä¸è®¡è¾ƒæ­¤äº‹äº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000163_step_table = {
		[1] = Task_00000163_step_01,
		[10] = Task_00000163_step_10,
		};

function Task_00000163_step(step)
	if Task_00000163_step_table[step] ~= nil then
		return Task_00000163_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000163_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000163() then
		return false;
	end
	if not task:AcceptTask(163) then
		return false;
	end
	task:AddTaskStep(163);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000163_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(163) then
		return false;
	end


	player:AddExp(100000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000163_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(163);
end
