--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000173()
	local player = GetPlayer();
	if player:GetLev() < 80 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(173) or task:HasCompletedTask(173) or task:HasSubmitedTask(173) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000173()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 80 then
		return false;
	end
	if task:HasAcceptedTask(173) or task:HasCompletedTask(173) or task:HasSubmitedTask(173) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000173()
	if GetPlayer():GetTaskMgr():HasCompletedTask(173) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000173(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(173) == npcId and Task_Accept_00000173 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 173
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "çƒˆç„°ä¾è€…";
	elseif task:GetTaskSubmitNpc(173) == npcId then
		if Task_Submit_00000173() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 173
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "çƒˆç„°ä¾è€…";
		elseif task:HasAcceptedTask(173) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 173
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "çƒˆç„°ä¾è€…";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000173_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "åœ¨è¿™ç‰‡æµ·åŸŸé™„è¿‘ï¼Œæœ‰ä¸€å¤„å¥‡å¼‚çš„ç«å±±å«åšæœˆå„¿å²›ï¼Œä¹ƒæ˜¯æ˜”å¹´è¿å±±å¤§å¸ˆååŒ–ä¹‹æ‰€ï¼Œå¬è¯´æœ‰ä¸å°‘å¥‡çé—ç•™ï¼Œä½ ä¸å¦‚å»æ¢å¯»ä¸€ä¸‹ï¼Œè¯´ä¸å®šæœ‰æ‰€æ”¶è·ã€‚ä¸è¿‡æœˆå„¿å²›çš„å®ˆå«ä»åœ¨ï¼Œä½ è¦å°å¿ƒåº”ä»˜æ‰å¯¹ã€‚";
	action.m_ActionMsg = "æ˜¯å—ï¼Œæˆ‘è¿™å°±å»çœ‹çœ‹ã€‚";
	return action;
end

function Task_00000173_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å°‘ä¾ åœ¨æœˆå„¿å²›æœ‰ä½•æ”¶è·ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000173_step_table = {
		[1] = Task_00000173_step_01,
		[10] = Task_00000173_step_10,
		};

function Task_00000173_step(step)
	if Task_00000173_step_table[step] ~= nil then
		return Task_00000173_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000173_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000173() then
		return false;
	end
	if not task:AcceptTask(173) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000173_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(173) then
		return false;
	end


	player:AddExp(150000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000173_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(173);
end
