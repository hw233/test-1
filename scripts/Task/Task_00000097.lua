--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000097()
	local player = GetPlayer();
	if player:GetLev() < 50 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(97) or task:HasCompletedTask(97) or task:HasSubmitedTask(97) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(96) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(96) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(96) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000097()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 50 then
		return false;
	end
	if task:HasAcceptedTask(97) or task:HasCompletedTask(97) or task:HasSubmitedTask(97) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(96) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(96) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(96) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000097()
	if GetPlayer():GetTaskMgr():HasCompletedTask(97) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000097(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(97) == npcId and Task_Accept_00000097 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 97
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "æ¶ˆç­å¦–äºº";
	elseif task:GetTaskSubmitNpc(97) == npcId then
		if Task_Submit_00000097() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 97
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "æ¶ˆç­å¦–äºº";
		elseif task:HasAcceptedTask(97) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 97
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "æ¶ˆç­å¦–äºº";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000097_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å§‘å©†å²­æ­£é‚ªäººå£«æ··æ‚ï¼Œæˆ‘åœ¨æ­¤æ½œä¿®å¤šå¹´æœªä¸ä»–ä»¬æœ‰è¿‡å†²çªã€‚ä¸è¿‡è¿‘äº›æ—¶æ—¥æ¥ï¼Œé‚£èµ¤å‘å¯¨ä¸»çºµå®¹æ‰‹ä¸‹ä¸ºæ¶ï¼Œ"..GetPlayerName(GetPlayer()).."å¸Œæœ›ä½ å¯ä»¥æ¶ˆç­è¿™äº›é‚ªé“å¦–äººï¼Œä¹Ÿè¿˜è¿™å‘¨é­å±±å²­çš„æ¸…é™ã€‚";
	action.m_ActionMsg = "å¤§å¸ˆï¼Œæˆ‘å°±å»æ¶ˆç­è¿™äº›å¦–äººã€‚";
	return action;
end

function Task_00000097_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¤šæ—¥ä¸è§ï¼Œ"..GetPlayerName(GetPlayer()).."ä½ ä¿®ä¸ºæ—¥æ·±ï¼Œå‰‘æœ¯å¤§æœ‰é•¿è¿›å•Šã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000097_step_table = {
		[1] = Task_00000097_step_01,
		[10] = Task_00000097_step_10,
		};

function Task_00000097_step(step)
	if Task_00000097_step_table[step] ~= nil then
		return Task_00000097_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000097_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000097() then
		return false;
	end
	if not task:AcceptTask(97) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000097_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(97) then
		return false;
	end


	player:AddExp(30000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000097_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(97);
end
