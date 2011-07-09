--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000057()
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(57) or task:HasCompletedTask(57) or task:HasSubmitedTask(57) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(56) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(56) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(56) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000057()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(57) or task:HasCompletedTask(57) or task:HasSubmitedTask(57) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(56) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(56) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(56) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000057()
	if GetPlayer():GetTaskMgr():HasCompletedTask(57) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000057(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(57) == npcId and Task_Accept_00000057 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 57
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "è¯›æ€ç¥é³„";
	elseif task:GetTaskSubmitNpc(57) == npcId then
		if Task_Submit_00000057() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 57
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "è¯›æ€ç¥é³„";
		elseif task:HasAcceptedTask(57) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 57
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "è¯›æ€ç¥é³„";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000057_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "åœ¨è¿™ç¦ä»™æ½­é‡Œæœ‰ä¸€åªæˆé“å¤šå¹´çš„ç¥é³„ï¼Œä¸é‚£æ¯’çŸ³ç›¸ä¼´ï¼Œå‡¶æ¶å¼‚å¸¸ï¼Œå¿…é¡»å…ˆå°†å…¶æ€ç­ï¼Œä¸ç„¶åœ¨ä½ ä»¬ä¸“å¿ƒé‡‡è¯æ—¶çªœå‡ºæ¥å–·åæ¯’é›¾ï¼Œå°±éå¸¸æ£˜æ‰‹äº†ã€‚";
	action.m_ActionMsg = "å§¥å§¥æ”¾å¿ƒï¼Œæˆ‘å¯¹ä»˜ä¸€åªå°å°çˆ¬è™«ä¸åœ¨è¯ä¸‹ã€‚";
	return action;
end

function Task_00000057_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ çœ‹æ¥å‰‘æœ¯ä¸é”™å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000057_step_table = {
		[1] = Task_00000057_step_01,
		[10] = Task_00000057_step_10,
		};

function Task_00000057_step(step)
	if Task_00000057_step_table[step] ~= nil then
		return Task_00000057_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000057_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000057() then
		return false;
	end
	if not task:AcceptTask(57) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000057_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(57) then
		return false;
	end


	player:AddExp(1111);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000057_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(57);
end
