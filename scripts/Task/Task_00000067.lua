--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000067()
	local player = GetPlayer();
	if player:GetLev() < 1 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(67) or task:HasCompletedTask(67) or task:HasSubmitedTask(67) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(66) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(66) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(66) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000067()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 1 then
		return false;
	end
	if task:HasAcceptedTask(67) or task:HasCompletedTask(67) or task:HasSubmitedTask(67) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(66) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(66) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(66) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000067()
	if GetPlayer():GetTaskMgr():HasCompletedTask(67) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000067(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(67) == npcId and Task_Accept_00000067 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 67
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å‰‘ä¸‹ç•™äºº";
	elseif task:GetTaskSubmitNpc(67) == npcId then
		if Task_Submit_00000067() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 67
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å‰‘ä¸‹ç•™äºº";
		elseif task:HasAcceptedTask(67) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 67
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å‰‘ä¸‹ç•™äºº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000067_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è‹¥è®ºä¸–ä¿—æ­¦è‰ºï¼Œæˆ‘å…¶å®ä¹Ÿä¸è¾“ç»™ä½™è¹å§‘ï¼Œåªæ˜¯å¥¹è·Ÿéšå‰‘ä»™å­¦ä¹ äº†é‚£é£å‰‘ä¹‹æœ¯ï¼Œæˆ‘ä¸€ä»‹å‡¡äººå¯ä¸æ˜¯å¯¹æ‰‹â€¦â€¦å“å‘€ï¼Œå¥¹ç«Ÿç„¶æ”¾é£å‰‘æ¥æ‰“æˆ‘ï¼Œæˆ‘å‘½ä¼‘çŸ£ï¼ï¼";
	action.m_ActionMsg = "è«æ…Œâ€¦â€¦å…¶å®æˆ‘ä¹Ÿä¼šé£å‰‘ã€‚";
	return action;
end

function Task_00000067_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¤šè°¢æ•‘å‘½ä¹‹æ©ï¼Œä¸çŸ¥é“"..GetPlayerName(GetPlayer()).."ä½ èƒ½ä¸èƒ½æ•™æˆ‘é£å‰‘ä¹‹æœ¯ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000067_step_table = {
		[1] = Task_00000067_step_01,
		[10] = Task_00000067_step_10,
		};

function Task_00000067_step(step)
	if Task_00000067_step_table[step] ~= nil then
		return Task_00000067_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000067_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000067() then
		return false;
	end
	if not task:AcceptTask(67) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000067_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(67) then
		return false;
	end


	player:AddExp(18000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000067_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(67);
end
