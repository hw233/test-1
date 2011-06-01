--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000008()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(8) or task:HasCompletedTask(8) or task:HasSubmitedTask(8) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(7) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(7) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000008()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(8) or task:HasCompletedTask(8) or task:HasSubmitedTask(8) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(7) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(7) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000008()
	if GetPlayer():GetTaskMgr():HasCompletedTask(8) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000008(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(8) == npcId and Task_Accept_00000008 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 8
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¯ç¨‹";
	elseif task:GetTaskSubmitNpc(8) == npcId then
		if Task_Submit_00000008() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 8
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¯ç¨‹";
		elseif task:HasAcceptedTask(8) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 8
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¯ç¨‹";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000008_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æ–¹æ‰æå®çˆ¶å¥³å·²ç»å‡†å¤‡å¥½è‡ªå·±çš„è¡ŒæåŒ…è£¹ï¼Œæ‰“ç®—ä¸æˆ‘å³åˆ»å¯ç¨‹å‰å¾€å³¨åµ‹å±±éšå±…ï¼Œä»–ä»¬å·²ç»åœ¨èˆèº«å´–å¤„ç­‰å€™ä½ æˆ‘ï¼Œå°‘ä¾ ä¸å¦‚å’Œæˆ‘ä»¬ä¸€èµ·å»å³¨çœ‰å±±ï¼Ÿ";
	action.m_ActionMsg = "èƒ½ä¸å‰è¾ˆåŒè¡Œï¼Œæˆ‘æ·±æ„Ÿè£å¹¸ã€‚";
	return action;
end

function Task_00000008_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "<name>ï¼Œæˆ‘çœ‹ä½ ç¥é‡‡å¥•å¥•ï¼Œè«éå¾—äº†ä»€ä¹ˆå¥‡é‡ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000008_step_table = {
		[1] = Task_00000008_step_01,
		[10] = Task_00000008_step_10,
		};

function Task_00000008_step(step)
	if Task_00000008_step_table[step] ~= nil then
		return Task_00000008_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000008_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000008() then
		return false;
	end
	if not task:AcceptTask(8) then
		return false;
	end
	task:AddTaskStep(8);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000008_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(8) then
		return false;
	end


	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000008_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(8);
end
