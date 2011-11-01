--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000055()
	local player = GetPlayer();
	if player:GetLev() < 35 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(55) or task:HasCompletedTask(55) or task:HasSubmitedTask(55) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(54) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(54) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(54) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000055()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 35 then
		return false;
	end
	if task:HasAcceptedTask(55) or task:HasCompletedTask(55) or task:HasSubmitedTask(55) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(54) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(54) then
			return false;
		end
	end
	if state == 2 then
		if not task:HasSubmitedTask(54) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000055()
	if GetPlayer():GetTaskMgr():HasCompletedTask(55) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000055(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(55) == npcId and Task_Accept_00000055 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 55
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "çº¢èŠ±å§¥å§¥";
	elseif task:GetTaskSubmitNpc(55) == npcId then
		if Task_Submit_00000055() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 55
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "çº¢èŠ±å§¥å§¥";
		elseif task:HasAcceptedTask(55) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 55
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "çº¢èŠ±å§¥å§¥";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000055_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ çœ‹ç¦ä»™æ½­å››å‘¨å›´ç»•çš„äº”è‰²äº‘é›¾å°±æ˜¯å®¶å¸ˆè®¾ä¸‹çš„ç¦åˆ¶ï¼Œå¤©æåœ°å®å”¯æœ‰æœ‰ç¼˜è€…å¾—ä¹‹ã€‚ä¸è¿‡æ½­åº•æœ‰åªç¥é³„å¾ˆæ˜¯å‰å®³ï¼Œè¦æ€ä¹ˆå–è¯æˆ‘ä¹Ÿä¸æ¸…æ¥šï¼Œè¿˜æ˜¯å»é—®å®¶å¸ˆå§ã€‚";
	action.m_ActionMsg = "å¥½ï¼Œæˆ‘è¿™å°±å»è§çº¢èŠ±å§¥å§¥";
	return action;
end

function Task_00000055_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "è€èº«åœ¨æ­¤ä¿®ç‚¼å¤šå¹´ï¼Œè¦å¤šå¤šæ„Ÿè°¢å³¨çœ‰ç¥–å¸ˆé•¿çœ‰çœŸäººçš„æ•™è¯²ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000055_step_table = {
		[1] = Task_00000055_step_01,
		[10] = Task_00000055_step_10,
		};

function Task_00000055_step(step)
	if Task_00000055_step_table[step] ~= nil then
		return Task_00000055_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000055_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000055() then
		return false;
	end
	if not task:AcceptTask(55) then
		return false;
	end
	task:AddTaskStep(55);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000055_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(55) then
		return false;
	end


	player:AddExp(20000);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000055_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(55);
end
