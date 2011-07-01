--ÈÎÎñµÄ½ÓÊÜÌõ¼ş
function Task_Accept_00000031()
	local player = GetPlayer();
	if player:GetLev() < 25 then
		return false;
	end
	local task =  player:GetTaskMgr();
	if task:HasAcceptedTask(31) or task:HasCompletedTask(31) or task:HasSubmitedTask(31) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30) then
			return false;
		end
	end
	return true;
end




-----¿É½ÓÈÎÎñÌõ¼ş
function Task_Can_Accept_00000031()
	local player = GetPlayer();
	local task =  player:GetTaskMgr();
	if player:GetLev() < 25 then
		return false;
	end
	if task:HasAcceptedTask(31) or task:HasCompletedTask(31) or task:HasSubmitedTask(31) then
		return false;
	end
	local state = GetPlayerData(6);
	if state == 0 then
		if not task:HasSubmitedTask(30) then
			return false;
		end
	end
	if state == 1 then
		if not task:HasSubmitedTask(30) then
			return false;
		end
	end
	return true;
end


--ÈÎÎñÍê³ÉÌõ¼ş
function Task_Submit_00000031()
	if GetPlayer():GetTaskMgr():HasCompletedTask(31) then
		return true;
	end
	return false;
end


---------------------------------------
------NPC½»»¥µÄÈÎÎñ½Å±¾
---------------------------------------
function Task_00000031(npcId)
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	local action = ActionTable:Instance();

	if task:GetTaskAcceptNpc(31) == npcId and Task_Accept_00000031 () then
		action.m_ActionType = 0x0001;
		action.m_ActionID = 31
		action.m_ActionToken = 1;
		action.m_ActionStep = 01;
		action.m_ActionMsg = "å¤ªä¹™çœŸé‡‘";
	elseif task:GetTaskSubmitNpc(31) == npcId then
		if Task_Submit_00000031() then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31
			action.m_ActionToken = 2;
			action.m_ActionStep = 10;
			action.m_ActionMsg = "å¤ªä¹™çœŸé‡‘";
		elseif task:HasAcceptedTask(31) then
			action.m_ActionType = 0x0001;
			action.m_ActionID = 31
			action.m_ActionToken = 0;
			action.m_ActionStep = 0;
			action.m_ActionMsg = "å¤ªä¹™çœŸé‡‘";
		end
	end
	return action;
end

-------------------------------------------------
--------ÈÎÎñ½»»¥²½Öè
-------------------------------------------------
function Task_00000031_step_01()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "çœ‹èµ·æ¥ï¼Œ<name>ä½ ç»è¿‡å¤šæ¬¡å†ç»ƒï¼Œå·²ç»æˆé•¿äº†ä¸å°‘ã€‚æˆ‘è¿™é‡Œæœ‰ä¸€å—åœ¨ä¸œæµ·å¶å¾—çš„å¤ªä¹™çœŸé‡‘ï¼Œæ˜¯å¼ºåŒ–ç¥å…µçš„æœ€ä½³ææ–™ï¼Œä½ æ‹¿å»æ‰¾æ­¦å™¨åº—çš„æ²ˆæµªï¼Œä»–æ˜¯ä¸€ä½éšå±…å¤šå¹´çš„å‰‘ä»™ï¼Œçœ‹åˆ°è¿™å—å¤ªä¹™çœŸé‡‘è‡ªä¼šå‘Šè¯‰ä½ ç”¨æ³•ã€‚";
	action.m_ActionMsg = "å¤šè°¢å‰è¾ˆæŒ‡ç‚¹ï¼Œæˆ‘è¿™å°±å»ã€‚";
	return action;
end

function Task_00000031_step_10()
	local action = ActionTable:Instance();
	action.m_ActionType = 0x0001;
	action.m_ActionToken = 3;
	action.m_ActionStep = 0;
	action.m_NpcMsg = "ä½ æ‰‹ä¸Šå‘å…‰çš„æ˜¯ä»€ä¹ˆä¸œè¥¿ï¼Ÿ";
	action.m_ActionMsg = "";
	return action;
end

local Task_00000031_step_table = {
		[1] = Task_00000031_step_01,
		[10] = Task_00000031_step_10,
		};

function Task_00000031_step(step)
	if Task_00000031_step_table[step] ~= nil then
		return Task_00000031_step_table[step]();
	end
	return ActionTable:Instance();
end

--½ÓÊÜÈÎÎñ
function Task_00000031_accept()
	local player = GetPlayer();
	local task = player:GetTaskMgr();
	if not Task_Accept_00000031() then
		return false;
	end
	if not task:AcceptTask(31) then
		return false;
	end
	task:AddTaskStep(31);
	return true;
end



--Ìá½»ÈÎÎñ
function Task_00000031_submit(itemId, itemNum)
	local player = GetPlayer();

	local package = player:GetPackage();

	if not player:GetTaskMgr():SubmitTask(31) then
		return false;
	end


	player:AddExp(2222);
	return true;
end

--·ÅÆúÈÎÎñ
function Task_00000031_abandon()
	local package = GetPlayer():GetPackage();
	return GetPlayer():GetTaskMgr():AbandonTask(31);
end
