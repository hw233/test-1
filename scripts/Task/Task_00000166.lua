--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000166()
	local player = GetPlayer();
	if player:GetLev() < 80 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(166) or task:HasCompletedTask(166) or task:HasSubmitedTask(166) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(165) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(165) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(165) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000166()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 80 then
		return false;
	end
	if task:HasAcceptedTask(166) or task:HasCompletedTask(166) or task:HasSubmitedTask(166) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(165) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(165) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(165) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000166()
	if GetPlayer():GetTaskMgr():HasCompletedTask(166) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000166(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(166) == npcId and Task_Accept_00000166 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 166
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç„å†¥ç•Œ";
	elseif task:GetTaskSubmitNpc(166) == npcId then
		if Task_Submit_00000166() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 166
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç„å†¥ç•Œ";
		elseif task:HasAcceptedTask(166) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 166
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç„å†¥ç•Œ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000166_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è¿™å…ƒç£çœŸåŠ›ç„å¦™å¼‚å¸¸ï¼Œä¸è¿‡å› ä¸ºåœ°å¤„åŒ—æï¼Œå…ƒç£çœŸåŠ›ç»“åˆé˜´å¯’ä¹‹åŠ›ç«Ÿç„¶æ²Ÿé€šåˆ°ç„å†¥å¼‚ç•Œï¼Œå¼•å¾—ä¸å°‘ç„å†¥å¼‚æ°‘é™ä¸´ï¼Œæ­¤ç±»å¼‚ç•Œç”Ÿçµæ˜¯æˆ‘ç­‰ä¹‹å¤§æ•Œï¼Œåªæ˜¯æˆ‘é¦–èè‹±éªšæ‰°åœ¨å…ˆï¼Œè¿˜æ— æ³•å®Œå…¨åŒ–ä¸ºäººå½¢ï¼Œå¸Œæœ›å°å‹ä½ å¯ä»¥å‡ºæ‰‹é™¤æ‰æ­¤å®³ã€‚";
	action.m_ActionMsg = "æ˜¯å—ï¼Œæˆ‘è¿™å°±å»é™¤æ‰ç„å†¥å¼‚æ°‘ã€‚";
	return action;
end

function Task_00000166_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è€æœ½ä¿®ç‚¼å¤šå¹´ï¼Œç»ˆäºå¯ä»¥åŒ–èº«æˆäººäº†ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000166_step_table = {
		[1] = Task_00000166_step_01,
		[10] = Task_00000166_step_10,
		};

function Task_00000166_step(step)
	if Task_00000166_step_table[step] ~= nil then
		return Task_00000166_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000166_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000166() then
		return false;
	end
	if not task:AcceptTask(166) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000166_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(166) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000166_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(166);
end
