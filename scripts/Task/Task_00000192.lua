--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000192()
	local player = GetPlayer();
	if player:GetLev() < 95 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(192) or task:HasCompletedTask(192) or task:HasSubmitedTask(192) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(191) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(191) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(191) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000192()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 95 then
		return false;
	end
	if task:HasAcceptedTask(192) or task:HasCompletedTask(192) or task:HasSubmitedTask(192) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(191) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(191) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(191) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000192()
	if GetPlayer():GetTaskMgr():HasCompletedTask(192) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000192(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(192) == npcId and Task_Accept_00000192 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 192
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¼‚å…½é¬¼è½¦";
	elseif task:GetTaskSubmitNpc(192) == npcId then
		if Task_Submit_00000192() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 192
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¼‚å…½é¬¼è½¦";
		elseif task:HasAcceptedTask(192) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 192
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¼‚å…½é¬¼è½¦";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000192_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç å³°ä¹ƒæ˜¯ä¸–ç•Œä¹‹å·…ï¼Œå€’æ˜¯æœ‰ä¸å°‘ç¥å¥‡çš„ç”Ÿç‰©ï¼Œä½ çœ‹é‚£é›ªå±±é‚£ä¸€ä¾§çš„ç»å£å¤„ï¼Œæœ‰ä¸€äº›ä¸Šå¤çš„å¼‚å…½åè£”ï¼Œæ˜¯åé¬¼è½¦ï¼Œé¢‡ä¸ºä¸å‡¡ï¼Œå°‘ä¾ ä½ ä¸å»å»è§è¯†è§è¯†ï¼Œå½“ç„¶æ‰“ä¸€ä»—æ˜¯éš¾å…çš„ï¼Œå“ˆå“ˆã€‚";
	action.m_ActionMsg = "æˆ‘è¿™å°±å»è§è¯†è§è¯†é¬¼è½¦çš„å¥‡å¦™ä¹‹å¤„ã€‚";
	return action;
end

function Task_00000192_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ çœŸæ˜¯ä¿®ä¸ºæ·±åšï¼Œå¤©èµ‹å¼‚ç¦€å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000192_step_table = {
		[1] = Task_00000192_step_01,
		[10] = Task_00000192_step_10,
		};

function Task_00000192_step(step)
	if Task_00000192_step_table[step] ~= nil then
		return Task_00000192_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000192_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000192() then
		return false;
	end
	if not task:AcceptTask(192) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000192_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(192) then
		return false;
	end


	player:AddExp(180000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000192_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(192);
end
