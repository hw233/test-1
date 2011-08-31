--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000565()
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	local player = GetPlayer();
	if player:GetLev() < 30 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(565) or task:HasCompletedTask(565) or task:HasSubmitedTask(565) then
		return false;
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000565()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if GetPlayerData(6) ~= 1 then
		return false;
	end
	if player:GetLev() < 30 then
		return false;
	end
	if task:HasAcceptedTask(565) or task:HasCompletedTask(565) or task:HasSubmitedTask(565) then
		return false;
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000565()
	if GetPlayer():GetTaskMgr():HasCompletedTask(565) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000565(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(565) == npcId and Task_Accept_00000565 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 565
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "ç‰æ¸…å¤§å¸ˆ";
	elseif task:GetTaskSubmitNpc(565) == npcId then
		if Task_Submit_00000565() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 565
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "ç‰æ¸…å¤§å¸ˆ";
		elseif task:HasAcceptedTask(565) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 565
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "ç‰æ¸…å¤§å¸ˆ";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000565_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ç‰æ¸…å¤§å¸ˆæœ¬æ˜¯é‚ªé“æ¯’é¾™å°Šè€…çš„åŒé—¨ï¼Œåæ¥é‡è§ç¥å°¼ä¼˜æ˜™ç‚¹åŒ–å¼ƒæš—æŠ•æ˜ï¼Œæ‹œå…¥ç¥å°¼é—¨ä¸‹ï¼Œå¦‚ä»Šæ­£åœ¨æˆéƒ½éƒŠå¤–è¾Ÿé‚ªæ‘éšå±…ï¼Œä½ å»æ‹œè®¿ä¸€ä¸‹å¥¹ï¼Œè±¡å¥¹è¯·æ•™ä¸€ä¸‹å‰‘æœ¯ä¸Šçš„ç–‘é—®ï¼Œå¯¹ä½ å¤§æœ‰å¥½å¤„å•Šã€‚";
	action.m_ActionMsg = "å¼Ÿå­æ­£æœ‰äº›ç–‘é—®å‘¢ã€‚";
	return action;
end

function Task_00000565_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "æœ‰åŠ³å°å‹å¸¦æ¥é½æŒæ•™çš„é—®å€™ã€‚";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000565_step_table = {
		[1] = Task_00000565_step_01,
		[10] = Task_00000565_step_10,
		};

function Task_00000565_step(step)
	if Task_00000565_step_table[step] ~= nil then
		return Task_00000565_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000565_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000565() then
		return false;
	end
	if not task:AcceptTask(565) then
		return false;
	end
	task:AddTaskStep(565);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000565_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(565) then
		return false;
	end


	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000565_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(565);
end
