--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00070307()
	local player = GetPlayer();
	if player:GetLev() < 50 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(70307) or task:HasCompletedTask(70307) or task:HasSubmitedTask(70307) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00070307()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 50 then
		return false;
	end
	if task:HasAcceptedTask(70307) or task:HasCompletedTask(70307) or task:HasSubmitedTask(70307) then
		return false;
	end
	if task:GetDayTaskCompletedCnt(1003) >= 100 then
		return false;
	end
	if task:GetDayTaskCurrTaskId(1003) ~= 70307 then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00070307()
	if GetPlayer():GetTaskMgr():HasCompletedTask(70307) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00070307(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(70307) == npcId and Task_Accept_00070307 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 70307
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ä¸è’¸é¦’å¤´äº‰å£æ°”";
	elseif task:GetTaskSubmitNpc(70307) == npcId then
		if Task_Submit_00070307() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70307
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ä¸è’¸é¦’å¤´äº‰å£æ°”";
		elseif task:HasAcceptedTask(70307) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 70307
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ä¸è’¸é¦’å¤´äº‰å£æ°”";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00070307_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "å¤©æ± ç å¤´åºŸå¢Ÿçš„æ¸”äººå¤´ç›®ï¼Œä»—ç€è‡ªå·±èº«å¼ºåŠ›å£®ï¼Œä¸”è€³ç›®ä¼—å¤šï¼Œå‡ ä¹åœ¨å¤©æ± ç§°éœ¸ä¸€æ–¹äº†ï¼Œæˆ‘ä»¬æ€ä¹ˆå¯ä»¥å®¹å¿ä¸€ä¸ªæ¸”äººè¿™æ ·åšï¼Œå»æŠŠä»–æ€æ‰å§ï¼Œå°±ç®—ä¸è’¸é¦’å¤´ä¹Ÿè¦äº‰å£æ°”ã€‚";
	action.m_ActionMsg = "åˆæ˜¯é‚£äº›åŠäººåŠé±¼çš„å®¶ä¼™ï¼Œè¿˜çœŸæ˜¯ä¸æ­»å¿ƒï¼Œæˆ‘è¿™å°±å»ã€‚";
	return action;
end

function Task_00070307_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "äººäº‰ä¸€å£æ°”ï¼Œä½›å—ä¸€æŸ±é¦™ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00070307_step_table = {
		[1] = Task_00070307_step_01,
		[10] = Task_00070307_step_10,
		};

function Task_00070307_step(step)
	if Task_00070307_step_table[step] ~= nil then
		return Task_00070307_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00070307_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00070307() then
		return false;
	end
	local package = player:GetPackage();
	if not task:AcceptTask(70307) then
		return false;
	end
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00070307_submit(itemId, itemNum)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local package = player:GetPackage();

	if task:CanDayTaskSubmit(70307) then
		if DayTaskAward(1003) then
			task:DayTaskSubmit(70307);
			return true;
		end
	end

	return false;
end

--·ÅÆúÈÎÎñ
function Task_00070307_abandon()
	local task = GetPlayer():GetTaskMgr();
	return task:AbandonTask(70307);
end
