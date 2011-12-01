--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000113()
	local player = GetPlayer();
	if player:GetLev() < 60 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(113) or task:HasCompletedTask(113) or task:HasSubmitedTask(113) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000113()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 60 then
		return false;
	end
	if task:HasAcceptedTask(113) or task:HasCompletedTask(113) or task:HasSubmitedTask(113) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000113()
	if GetPlayer():GetTaskMgr():HasCompletedTask(113) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000113(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(113) == npcId and Task_Accept_00000113 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 113
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "è½è‹å±±";
	elseif task:GetTaskSubmitNpc(113) == npcId then
		if Task_Submit_00000113() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 113
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "è½è‹å±±";
		elseif task:HasAcceptedTask(113) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 113
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "è½è‹å±±";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000113_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¦‚ä»Šç»¿è¢ä¼è¯›ï¼Œæˆ‘å³æ—¥å°±ä¸ä¸œæµ·è€å‹ä¼šå±±ç‚¼åˆ¶ä¸€å®—æ³•å®ï¼Œ"..GetPlayerName(GetPlayer()).."ä½ ä¸å¦‚å»è½è‹å±±ä¸€è¶Ÿï¼Œæè‹±ç¼çš„é‚£åªçˆ±å® è¢æ˜Ÿå‰ä¸ä¹…åœ¨è½è‹å±±èµ°å¤±ï¼Œä¸è¿‡æˆ‘ç”¨ç´«è–‡å¦æœ¯æ¨æµ‹ï¼Œç›®å‰è¢æ˜Ÿæ­£åœ¨é©¬ç†Šæ´ä¸­ï¼Œä¸å¦‚ä½ å»å°†å®ƒå¯»ä¼šï¼Œ";
	action.m_ActionMsg = "å¥½çš„ï¼Œå¼Ÿå­è¿™å°±å»ã€‚";
	return action;
end

function Task_00000113_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ å°å­è¿˜çœŸæ˜¯é˜´é­‚ä¸æ•£ï¼Œèµ°åˆ°å“ªéƒ½çœ‹å¾—åˆ°ä½ ï¼";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000113_step_table = {
		[1] = Task_00000113_step_01,
		[10] = Task_00000113_step_10,
		};

function Task_00000113_step(step)
	if Task_00000113_step_table[step] ~= nil then
		return Task_00000113_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000113_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000113() then
		return false;
	end
	if not task:AcceptTask(113) then
		return false;
	end
	task:AddTaskStep(113);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000113_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(113) then
		return false;
	end


	player:AddExp(30000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000113_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(113);
end
