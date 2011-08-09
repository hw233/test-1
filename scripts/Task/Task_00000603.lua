--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000603()
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(603) or task:HasCompletedTask(603) or task:HasSubmitedTask(603) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000603()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(603) or task:HasCompletedTask(603) or task:HasSubmitedTask(603) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000603()
	if GetPlayer():GetTaskMgr():HasCompletedTask(603) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000603(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(603) == npcId and Task_Accept_00000603 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 603
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ…ˆäº‘å¯ºæ¶åƒ§";
	elseif task:GetTaskSubmitNpc(603) == npcId then
		if Task_Submit_00000603() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 603
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ…ˆäº‘å¯ºæ¶åƒ§";
		elseif task:HasAcceptedTask(603) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 603
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ…ˆäº‘å¯ºæ¶åƒ§";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000603_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆéƒ½é™„è¿‘æœ‰åº§æ…ˆäº‘ç¦…å¸ˆï¼Œå¯ºå†…ä¸»æŒæ™ºé€šæœ¬å®˜è¿˜è§è¿‡ï¼Œä¸€ç›´éƒ½å®‰åˆ†å®ˆå·²ï¼Œå¯æ˜¯æœ€è¿‘å¯ºå†…æ¥äº†ä¸å°‘äº”æ¹–å››æµ·çš„å¼ºäººï¼Œåœ¨æˆéƒ½ä¸€å¸¦ä¸ºæ¶ï¼Œç”šè‡³è¿å¯ºå†…çš„å’Œå°šéƒ½å¼€å§‹ä½œå¥¸çŠ¯ç§‘ï¼Œ"..GetPlayerName(GetPlayer()).."ä½ è¿™å»å°†è¿™äº›ä¸ªå¦–åƒ§å‰¿ç­ã€‚";
	action.m_ActionMsg = "ä¸ºæ°‘é™¤å®³æ˜¯æˆ‘è¾ˆæœ¬è‰²ï¼Œå¼Ÿå­å»å»å°±å›ã€‚";
	return action;
end

function Task_00000603_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = GetPlayerName(GetPlayer()).."ä½ çœŸæ˜¯èº«æ‰‹ä¸å‡¡å•Šã€‚ ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000603_step_table = {
		[1] = Task_00000603_step_01,
		[10] = Task_00000603_step_10,
		};

function Task_00000603_step(step)
	if Task_00000603_step_table[step] ~= nil then
		return Task_00000603_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000603_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000603() then
		return false;
	end
	if not task:AcceptTask(603) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000603_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(603) then
		if DayTaskAward(0) then
			task:DayTaskSubmit(603);
			return true;
		end
	end

	return false;
end

--·ÅÆúÈÎÎñ
function Task_00000603_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(603);
end
