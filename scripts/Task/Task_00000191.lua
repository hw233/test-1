--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000191()
	local player = GetPlayer();
	if player:GetLev() < 95 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(191) or task:HasCompletedTask(191) or task:HasSubmitedTask(191) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(190) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(190) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(190) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000191()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 95 then
		return false;
	end
	if task:HasAcceptedTask(191) or task:HasCompletedTask(191) or task:HasSubmitedTask(191) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(190) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(190) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(190) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000191()
	if GetPlayer():GetTaskMgr():HasCompletedTask(191) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000191(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(191) == npcId and Task_Accept_00000191 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 191
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "é›ªå±±èµ¤é¸ ";
	elseif task:GetTaskSubmitNpc(191) == npcId then
		if Task_Submit_00000191() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 191
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "é›ªå±±èµ¤é¸ ";
		elseif task:HasAcceptedTask(191) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 191
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "é›ªå±±èµ¤é¸ ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000191_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç¦»æ­¤ä¸è¿œçš„é¸ ç›˜é¡¶ä¸Šç›˜è¸äº†ä¸€äº›èµ¤é¸ ï¼Œè™½ç„¶ç”Ÿæ€§çˆ†çƒˆï¼Œä¸è¿‡èƒ½æˆé•¿åœ¨é›ªå±±ï¼Œä¹Ÿæ˜¯ä¸å‡¡æœ‰ç€ä¸Šå¤ç¥å…½æœ±é›€çš„è¡€è„‰ï¼Œå°‘ä¾ ä½ å»å¸®æˆ‘æŠ“å‡ åªè¿‡æ¥ã€‚";
	action.m_ActionMsg = "æˆ‘è¿™å°±å»å°†è¿™äº›èµ¤é¸ æŠ“æ¥ã€‚";
	return action;
end

function Task_00000191_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ çœŸæ˜¯ä¿®ä¸ºæ·±åšï¼Œå¤©èµ‹å¼‚ç¦€å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000191_step_table = {
		[1] = Task_00000191_step_01,
		[10] = Task_00000191_step_10,
		};

function Task_00000191_step(step)
	if Task_00000191_step_table[step] ~= nil then
		return Task_00000191_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000191_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000191() then
		return false;
	end
	if not task:AcceptTask(191) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000191_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(191) then
		return false;
	end


	player:AddExp(180000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000191_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(191);
end
