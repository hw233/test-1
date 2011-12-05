--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000528()
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(528) or task:HasCompletedTask(528) or task:HasSubmitedTask(528) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000528()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 0 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(528) or task:HasCompletedTask(528) or task:HasSubmitedTask(528) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000528()
	if GetPlayer():GetTaskMgr():HasCompletedTask(528) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000528(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(528) == npcId and Task_Accept_00000528 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 528
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ…ˆäº‘å¯ºå¦–åƒ§";
	elseif task:GetTaskSubmitNpc(528) == npcId then
		if Task_Submit_00000528() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 528
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ…ˆäº‘å¯ºå¦–åƒ§";
		elseif task:HasAcceptedTask(528) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 528
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ…ˆäº‘å¯ºå¦–åƒ§";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000528_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æˆéƒ½é™„è¿‘æœ‰åº§æ…ˆäº‘ç¦…å¸ˆï¼Œå¯ºå†…ä¸»æŒæ™ºé€šæœ¬å®˜è¿˜è§è¿‡ï¼Œä¸€ç›´éƒ½å®‰åˆ†å®ˆå·²ï¼Œå¯æ˜¯æœ€è¿‘å¯ºå†…æ¥äº†ä¸å°‘äº”æ¹–å››æµ·çš„å¼ºäººï¼Œåœ¨æˆéƒ½ä¸€å¸¦ä¸ºæ¶ï¼Œç”šè‡³è¿å¯ºå†…çš„åƒ§äººéƒ½å¼€å§‹ä½œå¥¸çŠ¯ç§‘ï¼Œ"..GetPlayerName(GetPlayer()).."ä½ è¿™å»å°†è¿™äº›ä¸ªå¦–åƒ§å‰¿ç­ã€‚";
	action.m_ActionMsg = "ä¸ºæ°‘é™¤å®³æ˜¯æˆ‘è¾ˆæœ¬è‰²ï¼Œå¼Ÿå­å»å»å°±å›ã€‚";
	return action;
end

function Task_00000528_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™äº›ä¸å®ˆæ¸…è§„æˆ’å¾‹çš„åƒ§äººçœŸæ˜¯ä½›é—¨ä¹‹å®³ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000528_step_table = {
		[1] = Task_00000528_step_01,
		[10] = Task_00000528_step_10,
		};

function Task_00000528_step(step)
	if Task_00000528_step_table[step] ~= nil then
		return Task_00000528_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000528_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000528() then
		return false;
	end
	if not task:AcceptTask(528) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000528_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(528) then
		return false;
	end


	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000528_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(528);
end
